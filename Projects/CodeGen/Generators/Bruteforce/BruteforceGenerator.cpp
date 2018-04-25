// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/Bruteforce/BruteforceGenerator.h"
#include "Generators/ResultSearch.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"
#include "Engine/PipelineCompiler/glsl/glsl_source_vfs.h"

namespace CodeGen
{

/*
=================================================
	TypeToValueID
=================================================
*/
	template <typename T>	inline ValueID::type TypeToValueID ();

	template <>	inline ValueID::type TypeToValueID<float> ()	{ return "float"_ValueID; }
	template <>	inline ValueID::type TypeToValueID<double> ()	{ return "double"_ValueID; }
	template <>	inline ValueID::type TypeToValueID<int> ()		{ return "int"_ValueID; }
	template <>	inline ValueID::type TypeToValueID<ScalarF> ()	{ return "scalarf"_ValueID; }

/*
=================================================
	constructor
=================================================
*/
	BruteforceGenerator::BruteforceGenerator () :
		_localThreads{ 1024 }
	{}
	
/*
=================================================
	_Gen
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_Gen (ArrayCRef<TestCase<T>> testCases, ECommandSet::bits commandSetType, EConstantSet::bits constantType,
									float maxAccuracy, uint maxCommands, OUT GenFunctions_t &functions)
	{
		const usize		max_results	= 10000;

		_Clear();
		
		_maxCommands = maxCommands;

		CHECK_ERR( _Prepare( testCases, commandSetType ) );

		CHECK_ERR( _CreateSource<T>( commandSetType ) );

		CHECK_ERR( _CreateResources( testCases.Count(), max_results ) );

		CHECK_ERR( _InitBuffer( testCases, maxAccuracy, max_results ) );

		CHECK_ERR( _BuildCommandBuffer() );

		CHECK_ERR( _Execute() );

		CHECK_ERR( _ProcessResults( max_results, TypeToValueID<T>(), commandSetType, constantType, OUT functions ) );

		_Clear();
		return true;
	}
		
/*
=================================================
	Generate
=================================================
*/
	bool BruteforceGenerator::Generate (VariantCRef testCases,
										ECommandSet::bits commandSetType, EConstantSet::bits constantType,
										float maxAccuracy, uint maxCommands, OUT GenFunctions_t &functions)
	{
		using TestsSF	= Array<TestCase<ScalarF>>;
		using TestsD	= Array<TestCase<double>>;
		using TestsF	= Array<TestCase<float>>;
		using TestsI	= Array<TestCase<int>>;

		TypeId	id	= testCases.GetValueTypeId();

		if ( id == TypeIdOf< TestsSF >() )
		{
			return _Gen<ScalarF>( testCases.Get<TestsSF>(), commandSetType, constantType, maxAccuracy, maxCommands, OUT functions );
		}
		else
		if ( id == TypeIdOf< TestsD >() )
		{
			return _Gen<double>( testCases.Get<TestsD>(), commandSetType, constantType, maxAccuracy, maxCommands, OUT functions );
		}
		else
		if ( id == TypeIdOf< TestsF >() )
		{
			return _Gen<float>( testCases.Get<TestsF>(), commandSetType, constantType, maxAccuracy, maxCommands, OUT functions );
		}
		else
		if ( id == TypeIdOf< TestsI >() )
		{
			return _Gen<int>( testCases.Get<TestsI>(), commandSetType, constantType, maxAccuracy, maxCommands, OUT functions );
		}
		else
		{
			RETURN_ERR( "unsupported types!" );
		}
	}
	
/*
=================================================
	_Prepare
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_Prepare (ArrayCRef<TestCase<T>> testCases, ECommandSet::bits commandSetType)
	{
		const auto		data			= Commands::GetCommands( commandSetType );
		const BitsU		bits_per_cmd	= BitsU( IntLog2( data.Count() ) );
		const uint		min_commands	= usize(IntLog2( _localThreads ) / bits_per_cmd) + 1;

		_maxCommands = Max( _maxCommands, min_commands );

		// calculate number of threads and iterations
		const BitsU		max_bits		= bits_per_cmd * _maxCommands;
		const usize		bigint_count	= usize(max_bits / CompileTime::SizeOf<uint>::bits) + 1;
		const BitsU		max_block_size	= ( (BitsU::SizeOf<uint>() - 1) / bits_per_cmd ) * (bits_per_cmd - 1);
		const BitsU		block_size		= Min( max_bits, max_block_size );
		const uint		iterations		= 1u << usize(max_bits - block_size);
		const uint		num_threads		= 1u << usize(block_size);
		const uint		num_groups		= num_threads / _localThreads;
		uint			max_inputs		= 0;
		
		ASSERT( iterations == 1 );	// TODO

		CHECK_ERR( num_threads % _localThreads == 0 );
		CHECK_ERR( max_bits <= HashCode_t::MaxSize() );

		CHECK_ERR( _CheckTests( testCases, OUT max_inputs ) );
		CHECK_ERR( max_inputs > 0 );

		_maxInputs			= max_inputs;
		_bigintCount		= bigint_count;
		_iterationsCount	= iterations;
		_groupSize			= num_groups;
		
		_testCaseSize		= SizeOf<T> * (_maxInputs + 1);
		_bigIntSize			= SizeOf<uint> * (_bigintCount + 1);
		_constSize			= /*initHash*/_bigIntSize + SizeOf<_ConstData>;
		_atomicsSize		= SizeOf<_Atomics>;
		_resultSize			= _bigIntSize + SizeOf<_Result>;

		return true;
	}
	
/*
=================================================
	_CheckTests
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_CheckTests (ArrayCRef<TestCase<T>> testCases, OUT uint &maxInputs) const
	{
		CHECK_ERR( not testCases.Empty() );

		maxInputs = uint(testCases.Front().input.Count());

		for (auto& test : Range(testCases))
		{
			FOR( j, test.input )
			{
				CHECK_ERR( IsFinite( test.input[j] ) );
			}

			CHECK_ERR( IsFinite( test.output ) );

			CHECK_ERR( maxInputs == test.input.Count() );
		}

		return true;
	}
	
/*
=================================================
	_TypeToString
=================================================
*/
	template <typename T> inline String _TypeToString ();

	template <>	inline String _TypeToString <float> ()		{ return "float"; }
	template <>	inline String _TypeToString <double> ()		{ return "double"; }
	template <>	inline String _TypeToString <int> ()		{ return "int"; }
	template <>	inline String _TypeToString <ScalarF> ()	{ return "ScalarF"; }

/*
=================================================
	_CreateSource
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_CreateSource (ECommandSet::bits commandSetType)
	{
		Array<StringCRef>	sources;
		const auto			data			= Commands::GetCommands( commandSetType );
		const BitsU			bits_per_cmd	= BitsU( IntLog2( data.Count() ) );

		
		static const char	default_types[] = R"#(
#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4
#define int2		ivec2
#define int3		ivec3
#define int4		ivec4
#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4
#define float2		vec2
#define float3		vec3
#define float4		vec4
#define double2		dvec2
#define double3		dvec3
#define double4		dvec4
#extension GL_ARB_gpu_shader_int64 : require
		)#";
		sources.PushBack( default_types );


		// load standart library
		String		_defines_src;
		String		_cmp_src;
		String		_bitmath_src;
		{
			CHECK_ERR( glsl_vfs::LoadFile( "Common/Defines.glsl",	OUT _defines_src ) );
			CHECK_ERR( glsl_vfs::LoadFile( "Common/Cmp.glsl",		OUT _cmp_src ) );
			CHECK_ERR( glsl_vfs::LoadFile( "Math/BitMath.glsl",		OUT _bitmath_src ) );
		
			sources.PushBack( _defines_src.cstr() );
			sources.PushBack( _cmp_src.cstr() );
			sources.PushBack( _bitmath_src.cstr() );
		}

		// settings
		String	_settings_src;
		{
			String&	src			= _settings_src;
			uint	max_args	= 0;
			
			FOR( i, data ) {
				max_args = Max( max_args, data[i].args );
			}

			src << "#define MAX_ARGS         " << max_args << "\n"
				<< "#define MAX_INPUTS       " << _maxInputs << "\n"
				<< "#define BIGINT_SIZE      " << _bigintCount << "\n"
				<< "#define MAX_COMMANDS     " << _maxCommands << "\n"
				<< "#define BITS_PER_COMMAND " << usize(bits_per_cmd) << "\n"
				<< "#define T                " << _TypeToString<T>() << "\n\n"

				<< "layout (local_size_x=" << _localThreads << ", local_size_y=1, local_size_z=1) in;\n\n";

			sources.PushBack( src.cstr() );
		}


		// functions to shader source
		String	_func_src;
		{
			String&	src = _func_src;

			src << "T CallFunction (const uint id, const T args[MAX_ARGS], const int argsCount)\n{\n"
				<< "\tswitch ( id & " << ToMask<uint>( bits_per_cmd ) << " )\n\t{\n";

			FOR( i, data )
			{
				src << "\t\tcase " << i << ": return ";
				data[i].toGPU( OUT src );
				src << ";\n";
			}
			src << "\t}\n\treturn T(0);\n}\n";


			src << "int GetFunctionArgsCount (const uint id)\n{\n"
				<< "\tswitch ( id & " << ToMask<uint>( bits_per_cmd ) << " )\n\t{\n";
			
			FOR( i, data ) {
				src << "\t\tcase " << i << ": return " << data[i].args << ";\n";
			}
			src << "\t}\n\treturn 0;\n}\n";
			

			src << "uint GetFunctionTicks (const uint id)\n{\n"
				<< "\tswitch ( id & " << ToMask<uint>( bits_per_cmd ) << " )\n\t{\n";
			
			FOR( i, data ) {
				src << "\t\tcase " << i << ": return " << data[i].ticks << ";\n";
			}
			src << "\t}\n\treturn 0;\n}\n";

			sources.PushBack( src.cstr() );
		}


		sources.PushBack(
			#include "../BigInt.glsl"
		);
		sources.PushBack(
			#include "Bruteforce.glsl"
		);
		
		_gpuThread		= PlatformTools::GPUThreadHelper::FindComputeThread( GetMainSystemInstance()->GlobalSystems() );
		CHECK_ERR( _gpuThread );

		CreateInfo::PipelineTemplate	pt_ci;
		CHECK_ERR( _CompileProgram( sources, OUT pt_ci ) );
		
		CHECK_ERR( _gpuThread->GlobalSystems()->modulesFactory->Create(
						PipelineTemplateModuleID,
						_gpuThread->GlobalSystems(),
						pt_ci,
						OUT _pipelineTempl ) );

		ModuleUtils::Initialize({ _pipelineTempl });
		return true;
	}

/*
=================================================
	_CreateResources
=================================================
*/
	bool BruteforceGenerator::_CreateResources (const usize testsCount, const usize resultsCount)
	{
		// create resources
		auto	factory	= GetMainSystemInstance()->GlobalSystems()->modulesFactory;
		
		_syncManager	= _gpuThread->GetModuleByMsg<CompileTime::TypeListFrom< Message<GpuMsg::CreateFence> >>();
		CHECK_ERR( _syncManager );

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		_gpuThread->Send( req_ids );
		ComputeModuleIDs	gpu_ids = *req_ids->compute;
		
		Message< GpuMsg::CreateFence >	fence_ctor;
		_syncManager->Send( fence_ctor );
		_fence = *fence_ctor->result;
		
		CHECK_ERR( factory->Create(
						gpu_ids.commandBuilder,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuCommandBuilder{},
						OUT _cmdBuilder )
		);

		CHECK_ERR( factory->Create(
						gpu_ids.commandBuffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuCommandBuffer{},
						OUT _cmdBuffer ) );
		_cmdBuilder->Send< ModuleMsg::AttachModule >({ _cmdBuffer });


		const BytesU	inbuf_size		= _constSize + _testCaseSize * testsCount;
		const BytesU	outbuf_size		= _atomicsSize + _resultSize * resultsCount;

		CHECK_ERR( factory->Create(
						gpu_ids.buffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ inbuf_size, EBufferUsage::Storage },
							EGpuMemory::CoherentWithCPU,
							EMemoryAccess::All
						},
						OUT _inBuffer ) );
		
		CHECK_ERR( factory->Create(
						gpu_ids.buffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ outbuf_size, EBufferUsage::Storage | EBufferUsage::TransferDst },
							EGpuMemory::CoherentWithCPU,
							EMemoryAccess::All
						},
						OUT _outBuffer ) );

		// TODO: staging buffer
	

		Message< GpuMsg::CreateComputePipeline >	cppl_ctor{ gpu_ids.pipeline, _gpuThread };
		_pipelineTempl->Send( cppl_ctor );

		_pipeline = *cppl_ctor->result;

		CHECK_ERR( factory->Create(
						gpu_ids.resourceTable,
						_gpuThread->GlobalSystems(),
						CreateInfo::PipelineResourceTable{},
						OUT _resourceTable ) );
	
		_resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline",   _pipeline  });
		_resourceTable->Send< ModuleMsg::AttachModule >({ "ssb_input",  _inBuffer  });
		_resourceTable->Send< ModuleMsg::AttachModule >({ "ssb_output", _outBuffer });

		ModuleUtils::Initialize({ _cmdBuilder, _cmdBuffer, _inBuffer, _outBuffer, _pipeline, _resourceTable });
		return true;
	}
	
/*
=================================================
	_InitBuffer
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_InitBuffer (ArrayCRef<TestCase<T>> testCases, const float maxAccuracy, const usize resultsCount)
	{
		_inBuffer->Send< GpuMsg::MapMemoryToCpu >({ GpuMsg::MapMemoryToCpu::EMappingFlags::Write });
		
		BytesU	offset;

		// write const data
		{
			_BigIntBuf	init_hash;
			init_hash.Resize( _bigintCount+1 );

			_inBuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::From(init_hash), offset });
			offset += init_hash.Size();
			
			_ConstData	cdata;
			cdata.maxAccuracy		= maxAccuracy;
			cdata.resultsCapacity	= resultsCount;
			cdata.testCasesCount	= testCases.Count();
			
			_inBuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(cdata), offset });
			offset += SizeOf<_ConstData>;
		}

		// write test cases
		{
			if_constexpr( CompileTime::IsSameTypes< T, float > )
			{
				FixedSizeArray< float, 32 >		buf;
				buf.Resize( _maxInputs + 1 );

				for (auto& tc : Range(testCases))
				{
					FOR( i, tc.input ) {
						buf[i] = tc.input[i];
					}
					buf.Back() = tc.output;

					_inBuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::From(buf), offset });
					offset += buf.Size();
				}
			}
			else
			if_constexpr( CompileTime::IsSameTypes< T, double > )
			{
				FixedSizeArray< double, 32 >	buf;
				buf.Resize( _maxInputs + 1 );

				for (auto& tc : Range(testCases))
				{
					FOR( i, tc.input ) {
						buf[i] = tc.input[i];
					}
					buf.Back() = tc.output;

					_inBuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::From(buf), offset });
					offset += buf.Size();
				}
			}
			else
			if_constexpr( CompileTime::IsSameTypes< T, int > )
			{
				FixedSizeArray< int, 32 >	buf;
				buf.Resize( _maxInputs + 1 );

				for (auto& tc : Range(testCases))
				{
					FOR( i, tc.input ) {
						buf[i] = tc.input[i];
					}
					buf.Back() = tc.output;

					_inBuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::From(buf), offset });
					offset += buf.Size();
				}
			}
			else
			if_constexpr( CompileTime::IsSameTypes< T, ScalarF > )
			{
				FixedSizeArray< ScalarF, 32 >	buf;
				buf.Resize( _maxInputs + 1 );

				for (auto& tc : Range(testCases))
				{
					FOR( i, tc.input ) {
						buf[i] = tc.input[i];
					}
					buf.Back() = tc.output;

					_inBuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::From(buf), offset });
					offset += buf.Size();
				}
			}
			else
			{
				STATIC_WARNING( "unsupported type!" );
			}
		}

		_inBuffer->Send< GpuMsg::UnmapMemory >({});
		
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		_inBuffer->Send( req_descr );

		CHECK( req_descr->result->size == BytesUL(offset) );
		return true;
	}

/*
=================================================
	_BuildCommandBuffer
=================================================
*/
	bool BruteforceGenerator::_BuildCommandBuffer ()
	{
		_cmdBuilder->Send< GpuMsg::CmdBegin >({ _cmdBuffer });
		_cmdBuilder->Send< GpuMsg::CmdFillBuffer >({ _outBuffer, 0u });

		_cmdBuilder->Send< GpuMsg::CmdBindComputePipeline >({ _pipeline });
		_cmdBuilder->Send< GpuMsg::CmdBindComputeResourceTable >({ _resourceTable });

		_cmdBuilder->Send< GpuMsg::CmdDispatch >({ uint3( _groupSize, 1, 1 ) });

		Message< GpuMsg::CmdEnd >	cmd_end;
		_cmdBuilder->Send( cmd_end );

		return true;
	}
	
/*
=================================================
	_Execute
=================================================
*/
	bool BruteforceGenerator::_Execute ()
	{
		_gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ _cmdBuffer, _fence });

		_syncManager->Send< GpuMsg::ClientWaitFence >({ _fence });

		return true;
	}

/*
=================================================
	_ProcessResults
=================================================
*/
	bool BruteforceGenerator::_ProcessResults (const usize resultsCount, ValueID::type typeId,
											   ECommandSet::bits commandSetType, EConstantSet::bits constantType,
											   OUT GenFunctions_t &functions)
	{
		functions.Clear();

		_outBuffer->Send< GpuMsg::MapMemoryToCpu >({ GpuMsg::MapMemoryToCpu::EMappingFlags::Read });

		_Atomics	atomics;
		_outBuffer->Send< GpuMsg::ReadFromGpuMemory >({ BinArrayRef::FromValue(atomics) });
			
		// if too match results, decrease maxAccuracy or maxCommands.
		// if no results - increase maxAccuracy and/or maxCommands.
		ASSERT( atomics.results > 0 and atomics.total > 0 );

		Message< ModuleMsg::ReadFromStream >	results{ BytesU::SizeOf( atomics ) };
		_outBuffer->Send( results );

		const usize	res_count = Min( atomics.results, resultsCount );

		CHECK_ERR( ResultSearch::SearchResults( *results->result, res_count, _maxInputs, _resultSize,
											    _bigIntSize, typeId, commandSetType, constantType, OUT functions ) );

		_outBuffer->Send< GpuMsg::UnmapMemory >({});
		return true;
	}

/*
=================================================
	_Clear
=================================================
*/
	void BruteforceGenerator::_Clear ()
	{
		if ( _syncManager and _fence != GpuFenceId::Unknown ) {
			_syncManager->Send< GpuMsg::DestroyFence >({ _fence });
		}

		_gpuThread		= null;
		_syncManager	= null;
		
		_cmdBuilder		= null;
		_cmdBuffer		= null;
		_fence			= Uninitialized;

		_inBuffer		= null;
		_outBuffer		= null;

		_pipelineTempl	= null;
		_pipeline		= null;
		_resourceTable	= null;

		_maxCommands	= 0;
		_maxInputs		= 0;
		_bigintCount	= 0;
		_iterationsCount= 0;
		_groupSize		= 0;
	}
	
/*
=================================================
	ToSource
=================================================
*/
	bool BruteforceGenerator::ToSource (const GenFunction &func, OUT String &src) const
	{
		return ResultSearch::ToSource( func, OUT src );
	}

}	// CodeGen
