// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/Bruteforce/BruteforceGenerator.h"
#include "Generators/ResultSearch.h"
#include "Generators/Constants.inl.h"
#include "Generators/Utils.h"

#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"
#include "Engine/PipelineCompiler/glsl/glsl_source_vfs.h"

namespace CodeGen
{

/*
=================================================
	constructor
=================================================
*/
	BruteforceGenerator::BruteforceGenerator () :
		_maxResults{ 10'000 }
	{
		_Clear();
	}
	
/*
=================================================
	_Gen
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_Gen (ArrayCRef<TestCase<T>> testCases)
	{
		_gpuThread = PlatformTools::GPUThreadHelper::FindComputeThread( GetMainSystemInstance()->GlobalSystems() );
		CHECK_ERR( _gpuThread );

		Message< GpuMsg::GetDeviceProperties >	req_props;
		_gpuThread->Send( req_props );
		_devProperties = *req_props->result;

		_testsCount	 = testCases.Count();

		CHECK_ERR( _Prepare( testCases ) );

		CHECK_ERR( _CreateSource<T>() );

		CHECK_ERR( _CreateResources() );

		CHECK_ERR( _InitBuffer( testCases ) );

		return true;
	}
		
/*
=================================================
	Initialize
=================================================
*/
	bool BruteforceGenerator::Initialize (VariantCRef testCases, const Config &cfg)
	{
		using TestsMF	= Array<TestCase<MixedF>>;
		using TestsMD	= Array<TestCase<MixedD>>;
		using TestsD	= Array<TestCase<double>>;
		using TestsF	= Array<TestCase<float>>;
		using TestsI	= Array<TestCase<int>>;
		using TestsU	= Array<TestCase<uint>>;
		
		_Clear();

		_config = cfg;
		_timer.Start();

		TypeId		id	= testCases.GetValueTypeId();

		if ( id == TypeIdOf< TestsMF >() )
		{
			_typeId = "mixedF"_ValueID;
			return _Gen<MixedF>( testCases.Get<TestsMF>() );
		}
		else
		if ( id == TypeIdOf< TestsMD >() )
		{
			_typeId = "mixedD"_ValueID;
			return _Gen<MixedD>( testCases.Get<TestsMD>() );
		}
		else
		if ( id == TypeIdOf< TestsD >() )
		{
			_typeId = "double"_ValueID;
			return _Gen<double>( testCases.Get<TestsD>() );
		}
		else
		if ( id == TypeIdOf< TestsF >() )
		{
			_typeId = "float"_ValueID;
			return _Gen<float>( testCases.Get<TestsF>() );
		}
		else
		if ( id == TypeIdOf< TestsI >() )
		{
			_typeId = "int"_ValueID;
			return _Gen<int>( testCases.Get<TestsI>() );
		}
		else
		if ( id == TypeIdOf< TestsU >() )
		{
			_typeId = "uint"_ValueID;
			return _Gen<uint>( testCases.Get<TestsU>() );
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
	bool BruteforceGenerator::_Prepare (ArrayCRef<TestCase<T>> testCases)
	{
		const auto		data			= Commands::GetCommands( _config.commandSetType );
		const BitsU		bits_per_cmd	= GetMinBitsForNumber( data.Count() );
		const uint		min_commands	= usize(IntLog2( _devProperties.maxComputeWorkGroupInvocations ) / bits_per_cmd) + 1;

		_config.maxCommands = Max( _config.maxCommands, min_commands );

		const BitsU		max_bits		= bits_per_cmd * _config.maxCommands;
		
		const BitsU		max_step		= Min( BitsU::SizeOf<uint>()-8, max_bits );
		const uint		num_threads		= 1u << usize(Min( max_step, max_bits ));
		const uint		local_threads	= FloorPowerOfTwo( Min( _devProperties.maxComputeWorkGroupSize.x, _devProperties.maxComputeWorkGroupInvocations ) );
		const uint		num_groups		= Min( _devProperties.maxComputeWorkGroupCount.x, (num_threads + local_threads - 1) / local_threads );

		CHECK_ERR( max_bits <= FunctionHash_t::MaxSize() );

		uint			max_inputs = 0;
		CHECK_ERR( _CheckTests( testCases, OUT max_inputs ) );

		_config.maxConstants = Max( _config.maxConstants, 1u );

		const Set<T>	constants			= Commands::GetConstants<T>( _config.constantType );
		const BitsU		bits_per_const		= GetMinBitsForNumber( constants.Count() );
		const BitsU		const_count_bits	= GetMinBitsForNumber( _config.maxConstants );
		const BitsU		const_max_bits		= const_count_bits + bits_per_const * _config.maxConstants;

		const usize		fun_bi_count		= usize(max_bits / CompileTime::SizeOf<uint>::bits) + 1;
		const usize		const_bi_count		= usize(const_max_bits / CompileTime::SizeOf<uint>::bits) + 1;

		_maxFuncArgs	= max_inputs;
		_bigintCount	= Max( fun_bi_count, const_bi_count );
		_maxBits		= max_bits;
		_maxConstBits	= const_max_bits;
		
		_testCaseSize	= SizeOf<T> * (_maxFuncArgs + /*output*/1);
		_bigIntSize		= SizeOf<uint> * (_bigintCount + /*lastbit*/1);
		_atomicsSize	= SizeOf<_Atomics>;
		_resultSize		= /*funHash*/_bigIntSize + /*constHash*/_bigIntSize + SizeOf<_Result>;
		
		_localThreads	= local_threads;
		_groupCount		= num_groups;
		
		_inputBufferSize	= /*initial hash*/_bigIntSize + /*constHash*/_bigIntSize +
							  /*maxAccuracy*/SizeOf<float> + /*numConstants*/SizeOf<int> +
							  /*constants*/SizeOf<T> * _config.maxConstants +
							  /*testCases*/_testCaseSize * _testsCount;

		_outputBufferSize	= _atomicsSize + _resultSize * _maxResults;

		// initial value: count = 0, indices - max, but unused
		_constHash.Fill( bits_per_const * _config.maxConstants );

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

		for (auto& test : testCases)
		{
			for (auto& arg : test.input)
			{
				CHECK_ERR( IsFinite( arg ) );
			}

			CHECK_ERR( IsFinite( test.output ) );

			CHECK_ERR( maxInputs == test.input.Count() );
		}

		CHECK_ERR( maxInputs > 0 );
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
	template <>	inline String _TypeToString <uint> ()		{ return "uint"; }
	template <>	inline String _TypeToString <MixedF> ()		{ return "MixedF"; }
	template <>	inline String _TypeToString <MixedD> ()		{ return "MixedD"; }

/*
=================================================
	_CreateSource
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_CreateSource ()
	{
		Array<StringCRef>	sources;
		const auto			data			= Commands::GetCommands( _config.commandSetType );
		const BitsU			bits_per_cmd	= GetMinBitsForNumber( data.Count() );

		
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
		StaticArray<String, 10>		_temp_src;
		{
			CHECK_ERR( glsl_vfs::LoadFile( "Common/Defines.glsl",	OUT _temp_src[0] ) );
			CHECK_ERR( glsl_vfs::LoadFile( "Common/Cmp.glsl",		OUT _temp_src[1] ) );
			CHECK_ERR( glsl_vfs::LoadFile( "Math/BitMath.glsl",		OUT _temp_src[2] ) );
			CHECK_ERR( glsl_vfs::LoadFile( "Math/MathDef.glsl",		OUT _temp_src[3] ) );
			CHECK_ERR( glsl_vfs::LoadFile( "Common/TypeInfo.glsl",	OUT _temp_src[4] ) );
		
			sources.PushBack( _temp_src[0].cstr() );
			sources.PushBack( _temp_src[1].cstr() );
			sources.PushBack( _temp_src[2].cstr() );
			sources.PushBack( _temp_src[3].cstr() );
			sources.PushBack( _temp_src[4].cstr() );
		}

		// settings
		String	_settings_src;
		{
			String&	src			= _settings_src;
			uint	max_args	= 0;
			
			FOR( i, data ) {
				max_args = Max( max_args, data[i].args );
			}

			src << "#define MAX_CMD_ARGS      " << max_args << "\n"
				<< "#define MAX_INPUTS        " << (_maxFuncArgs + _config.maxConstants) << "\n"
				<< "#define MAX_FN_ARGS       " << _maxFuncArgs << "\n"
				<< "#define BIGINT_SIZE       " << _bigintCount << "\n"
				<< "#define MAX_COMMANDS      " << _config.maxCommands << "\n"
				<< "#define BITS_PER_COMMAND  " << usize(bits_per_cmd) << "\n"
				<< "#define MAX_TESTS         " << _testsCount << "\n"
				<< "#define MAX_RESULTS       " << _maxResults << "\n"
				<< "#define MAX_CONSTANTS     " << _config.maxConstants << "\n"
				<< "#define T                 " << _TypeToString<T>() << "\n\n";

			src	<< "layout (local_size_x=" << _localThreads << ", local_size_y=1, local_size_z=1) in;\n\n";

			sources.PushBack( src.cstr() );
		}

		// fitness function
		String	_fitness_src;
		{
			_fitness_src = FitnessFunctions::GetSource( _config.fitnessFunc );

			sources.PushBack( _fitness_src.cstr() );
		}

		// functions to shader source
		String	_func_src;
		{
			String&	src = _func_src;

			src << "T CallFunction (const uint id, const T args[MAX_CMD_ARGS], const int argsCount)\n{\n"
				<< "\tswitch ( id & " << ToMask<uint>( bits_per_cmd ) << " )\n\t{\n";

			FOR( i, data )
			{
				src << "\t\tcase " << i << ": return ";
				data[i].toGPU( OUT src );
				src << ";\n";
			}
			src << "\t}\n\treturn MaxValue(T(0));\n}\n";


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
	bool BruteforceGenerator::_CreateResources ()
	{
		// create resources
		auto	factory	= GetMainSystemInstance()->GlobalSystems()->modulesFactory;
		
		_syncManager	= _gpuThread->GetModuleByMsg<CompileTime::TypeListFrom< Message<GpuMsg::CreateFence> >>();
		CHECK_ERR( _syncManager );

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		_gpuThread->Send( req_ids );
		ComputeModuleIDs	gpu_ids = *req_ids->compute;
		
		CHECK_ERR( factory->Create(
						gpu_ids.commandBuilder,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuCommandBuilder{},
						OUT _cmdBuilder )
		);

		CHECK_ERR( factory->Create(
						gpu_ids.buffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ _inputBufferSize, EBufferUsage::Storage | EBufferUsage::TransferDst },
							EGpuMemory::LocalInGPU,
							EMemoryAccess::GpuReadWrite },
						OUT _inBuffer ) );
		
		CHECK_ERR( factory->Create(
						gpu_ids.buffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ _inputBufferSize, EBufferUsage::Storage | EBufferUsage::TransferSrc },
							EGpuMemory::CoherentWithCPU },
						OUT _inStagingBuffer ) );
		
		CHECK_ERR( factory->Create(
						gpu_ids.buffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ _outputBufferSize, EBufferUsage::Storage | EBufferUsage::TransferDst | EBufferUsage::TransferSrc },
							EGpuMemory::LocalInGPU,
							EMemoryAccess::GpuReadWrite },
						OUT _outBuffer ) );
		
		CHECK_ERR( factory->Create(
						gpu_ids.buffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ _outputBufferSize, EBufferUsage::Storage | EBufferUsage::TransferDst },
							EGpuMemory::CoherentWithCPU },
						OUT _outStagingBuffer ) );


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

		ModuleUtils::Initialize({ _cmdBuilder, _inBuffer, _outBuffer, _inStagingBuffer, _outStagingBuffer, _pipeline, _resourceTable });

		for (auto& cb : _cmdBuffers)
		{
			ModulePtr	out_buf;
			CHECK_ERR( factory->Create(
						gpu_ids.buffer,
						_gpuThread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ _atomicsSize, EBufferUsage::TransferDst },
							EGpuMemory::CoherentWithCPU },
						OUT out_buf ) );

			ModulePtr	cmd_buf;
			CHECK_ERR( factory->Create(
							gpu_ids.commandBuffer,
							_gpuThread->GlobalSystems(),
							CreateInfo::GpuCommandBuffer{ CommandBufferDescriptor{ ECmdBufferCreate::ImplicitResetable } },
							OUT cmd_buf ) );

			_cmdBuilder->Send< ModuleMsg::AttachModule >({ cmd_buf });
			ModuleUtils::Initialize({ cmd_buf, out_buf });

			Message< GpuMsg::CreateFence >	fence_ctor;
			_syncManager->Send( fence_ctor );

			cb.fence		= *fence_ctor->result;
			cb.cmd			= cmd_buf;
			cb.outputBuf	= out_buf;
		}
		return true;
	}

/*
=================================================
	_InitBuffer
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_InitBuffer (ArrayCRef<TestCase<T>> testCases)
	{
		_inStagingBuffer->Send< GpuMsg::MapMemoryToCpu >({ GpuMsg::EMappingFlags::Write });
		
		BytesU	offset;

		// write const data
		{
			_BigIntBuf		buf;

			BinArrayRef		initial_hash = buf;
			_WriteBigInt( INOUT initial_hash, FunctionHash_t(), _bigintCount );
			_inStagingBuffer->Send< DSMsg::WriteRegion >({ offset, initial_hash });
			offset += initial_hash.Size();
			
			BinArrayRef		const_hash = buf;
			_WriteBigInt( INOUT const_hash, ConstantHash_t(), _bigintCount );
			_inStagingBuffer->Send< DSMsg::WriteRegion >({ offset, const_hash });
			offset += const_hash.Size();

			_inStagingBuffer->Send< DSMsg::WriteRegion >({ offset, BinArrayCRef::FromValue(_config.maxAccuracy) });
			offset += sizeof(_config.maxAccuracy);
			
			const int		num_const = 0;
			_inStagingBuffer->Send< DSMsg::WriteRegion >({ offset, BinArrayCRef::FromValue(num_const) });
			offset += sizeof(num_const);
			
			// skip constants
			offset += sizeof(T) * _config.maxConstants;
		}

		// write test cases
		{
			FixedSizeArray< T, 32 >		buf;
			buf.Resize( _maxFuncArgs + 1 );

			for (auto& tc : testCases)
			{
				FOR( i, tc.input ) {
					buf[i] = tc.input[i];
				}
				buf.Back() = tc.output;

				_inStagingBuffer->Send< DSMsg::WriteRegion >({ offset, BinArrayCRef::From(buf) });
				offset += buf.Size();
			}
		}

		_inStagingBuffer->Send< GpuMsg::UnmapMemory >({});
		
		CHECK( _inputBufferSize == offset );
		return true;
	}

/*
=================================================
	_Clear
=================================================
*/
	void BruteforceGenerator::_Clear ()
	{
		_gpuThread			= null;
		_syncManager		= null;
		
		_cmdBuilder			= null;
		_cmdBufferIndex		= 0;
		_cmdBuffers.Clear();

		_inBuffer			= null;
		_outBuffer			= null;
		_inStagingBuffer	= null;
		_outStagingBuffer	= null;

		_pipelineTempl		= null;
		_pipeline			= null;
		_resourceTable		= null;
		
		_testCaseSize		= BytesU();
		_bigIntSize			= BytesU();
		_atomicsSize		= BytesU();
		_resultSize			= BytesU();
		_inputBufferSize	= BytesU();
		_outputBufferSize	= BytesU();
		_maxBits			= BitsU();

		_maxFuncArgs		= 0;
		_bigintCount		= 0;
		_testsCount			= 0;
		_localThreads		= 0;
		_groupCount			= 0;

		_typeId				= Uninitialized;
		_funHash			= Uninitialized;
		_constHash			= Uninitialized;
		_config				= Uninitialized;

		_completed			= false;
		_firstCall			= true;
		_breakOnBestResult	= false;
		_bestResultFound	= false;
	}
	
/*
=================================================
	_WriteBigInt
=================================================
*/
	template <typename T, usize S>
	void BruteforceGenerator::_WriteBigInt (INOUT BinArrayRef &dst, const BigInteger<T,S> &hash, uint numParts)
	{
		BytesU	offset;

		// write hash data
		for (uint i = 0; i < numParts; ++i)
		{
			const uint	val = hash.ToArray()[i];
			const auto	src = BinArrayCRef::FromValue( val );

			MemCopy( OUT dst.SubArray( usize(offset) ), src );

			offset += src.Size();
		}

		// write hash size
		{
			const uint	last_bit = uint(hash.Count());
			const auto	src = BinArrayCRef::FromValue( last_bit );
		
			MemCopy( OUT dst.SubArray( usize(offset) ), src );

			offset += src.Size();
		}

		dst = dst.SubArray( 0, usize(offset) );
	}
	
/*
=================================================
	Update
=================================================
*/
	bool BruteforceGenerator::Update ()
	{
		if ( _completed )
			return false;
		
		switch ( _typeId )
		{
			case "float"_ValueID :		_ProcessBlockWithConst<float>();	break;
			case "double"_ValueID :		_ProcessBlockWithConst<double>();	break;
			case "int"_ValueID :		_ProcessBlockWithConst<int>();		break;
			case "uint"_ValueID :		_ProcessBlockWithConst<uint>();		break;
			case "mixedF"_ValueID :		_ProcessBlockWithConst<MixedF>();	break;
			case "mixedD"_ValueID :		_ProcessBlockWithConst<MixedD>();	break;
			default :					RETURN_ERR( "unknown type!" );
		}
		
		if ( _completed )
		{
			_CopyOutputToStagingBuffer();
			return false;
		}

		return true;
	}

/*
=================================================
	_ProcessBlock
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_ProcessBlock (ArrayCRef<T> constants)
	{
		auto&		curr_buf	= _cmdBuffers[ _cmdBufferIndex ];
		auto&		next_buf	= _cmdBuffers[ _cmdBufferIndex + 1 ];
		const bool	first_call	= _firstCall;

		_firstCall = false;

		_cmdBuilder->Send< GpuMsg::CmdBegin >({ curr_buf.cmd });

		// initialize in first update
		if ( first_call )
		{
			_cmdBuilder->Send< GpuMsg::CmdCopyBuffer >({ _inStagingBuffer, _inBuffer, 0_b, 0_b, _inputBufferSize });

			_cmdBuilder->Send< GpuMsg::CmdFillBuffer >({ _outBuffer, 0u });
			_cmdBuilder->Send< GpuMsg::CmdFillBuffer >({ curr_buf.outputBuf, 0u });
			_cmdBuilder->Send< GpuMsg::CmdFillBuffer >({ next_buf.outputBuf, 0u });
		}

		_cmdBuilder->Send< GpuMsg::CmdBindComputePipeline >({ _pipeline });
		_cmdBuilder->Send< GpuMsg::CmdBindComputeResourceTable >({ _resourceTable });

		// update initial hash
		{
			_BigIntBuf		buf;
			
			// TODO: use push constants
			BinArrayRef		initial_hash = buf;
			_WriteBigInt( INOUT initial_hash, _funHash, _bigintCount );
			_cmdBuilder->Send< GpuMsg::CmdUpdateBuffer >({ _inBuffer, initial_hash, 0_b });
			
			BinArrayRef		const_hash = buf;
			_WriteBigInt( INOUT const_hash, _constHash, _bigintCount );
			_cmdBuilder->Send< GpuMsg::CmdUpdateBuffer >({ _inBuffer, const_hash, initial_hash.Size() });
		}
		
		// update constants
		{
			const BytesU	offset		= _bigIntSize + _bigIntSize + SizeOf<float>;
			const int		num_const	= constants.Count();
			
			_cmdBuilder->Send< GpuMsg::CmdUpdateBuffer >({ _inBuffer, BinArrayCRef::FromValue(num_const), offset });

			if ( not constants.Empty() ) {
				_cmdBuilder->Send< GpuMsg::CmdUpdateBuffer >({ _inBuffer, BinArrayCRef::From(constants), offset + SizeOf<int> });
			}
		}
			
		// sync _inBuffer
		_cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({});

		_cmdBuilder->Send< GpuMsg::CmdDispatch >({ uint3( _groupCount, 1, 1 ) });
		
		// sync _outBuffer
		_cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({});

		_cmdBuilder->Send< GpuMsg::CmdCopyBuffer >({ _outBuffer, curr_buf.outputBuf, 0_b, 0_b, _atomicsSize });

		_cmdBuilder->Send< GpuMsg::CmdEnd >({});


		Message< GpuMsg::SubmitComputeQueueCommands >	submit;
		submit->commands.PushBack( curr_buf.cmd );
		submit->fence = curr_buf.fence;
		CHECK( _gpuThread->Send( submit ) );
		
		if ( not first_call ) {
			_syncManager->Send< GpuMsg::ClientWaitFence >({ next_buf.fence });
		}

		_funHash += (_groupCount * _localThreads);

		if ( _funHash.Count() <= _maxBits )
		{
			if ( _breakOnBestResult )
			{
				// read atomics from buffer
				_Atomics	atomics = {};
				next_buf.outputBuf->Send< GpuMsg::ReadFromGpuMemory >({ BinArrayRef::FromValue( atomics ) });

				if ( atomics.bestResults > 0 )
				{
					_completed			= true;
					_bestResultFound	= true;

					_statistic.commandCount = uint(_funHash.Count() * _config.maxCommands / _maxBits);
				}
			}
		}
		else
		{
			_completed = true;
			
			_statistic.commandCount = _config.maxCommands;
		}

		++_cmdBufferIndex;
		return true;
	}
	
/*
=================================================
	_ProcessBlockWithConst
=================================================
*/
	template <typename T>
	bool BruteforceGenerator::_ProcessBlockWithConst ()
	{
		const Set<T>	constants			= Commands::GetConstants<T>( _config.constantType );
		const BitsU		bits_per_const		= GetMinBitsForNumber( constants.Count() );
		const usize		max_constants		= (1u << usize(bits_per_const));
		const BitsU		const_count_bits	= GetMinBitsForNumber( _config.maxConstants );
		Array<T>		const_args;

		const auto		FillUnusedBits		= LAMBDA( this, &const_count_bits, &bits_per_const ) ()
		{{
			const uint	count		= Min( _constHash.Read( _maxConstBits - const_count_bits, const_count_bits ), _config.maxConstants );
			const BitsU	first_bit	= bits_per_const * count;
			const BitsU	bit_count	= (_config.maxConstants - count) * bits_per_const;

			_constHash.Fill( first_bit, bit_count );
		}};

		if ( not constants.Empty() )
		{
			const uint	count	= _constHash.Read( _maxConstBits - const_count_bits, const_count_bits );
			BitsU		pos;

			if ( count > _config.maxConstants )
			{
				_completed = true;
				return true;
			}

			for (usize i = 0; i < count; ++i)
			{
				uint	id = _constHash.Read( pos, bits_per_const );

				// skip unused values
				if ( id >= constants.Count() )
				{
					ConstantHash_t	inc;
					inc.Write( max_constants - id, pos, bits_per_const );

					_constHash += inc;
					FillUnusedBits();

					return true;
				}

				const_args << constants[id];

				pos += bits_per_const;
			}
		}

		_ProcessBlock<T>( const_args );

		// increase constant hash
		if ( not constants.Empty()					and
			 _constHash.Count() <= _maxConstBits	and
			 _completed								and
			 not _bestResultFound )
		{
			_completed	= false;
			_funHash	= Uninitialized;

			++_constHash;
			FillUnusedBits();
		}

		return true;
	}
	
/*
=================================================
	_CopyOutputToStagingBuffer
=================================================
*/
	bool BruteforceGenerator::_CopyOutputToStagingBuffer ()
	{
		auto&	curr_buf = _cmdBuffers[ _cmdBufferIndex ];

		_cmdBuilder->Send< GpuMsg::CmdBegin >({ curr_buf.cmd });
		_cmdBuilder->Send< GpuMsg::CmdCopyBuffer >({ _outBuffer, _outStagingBuffer, 0_b, 0_b, _outputBufferSize });
		_cmdBuilder->Send< GpuMsg::CmdEnd >({});

		Message< GpuMsg::SubmitComputeQueueCommands >	submit;
		submit->commands.PushBack( curr_buf.cmd );
		CHECK( _gpuThread->Send( submit ) );

		_syncManager->Send< GpuMsg::ClientWaitDeviceIdle >({});

		_statistic.duration = _timer.GetTimeDelta();
		return true;
	}

/*
=================================================
	GetResults
=================================================
*/
	bool BruteforceGenerator::GetResults (OUT Statistic &stat, OUT GenFunctions_t &functions, usize maxResults)
	{
		CHECK_ERR( _completed );

		CHECK_ERR( _ProcessResults( OUT functions, maxResults ) );

		stat = _statistic;
		return true;
	}

/*
=================================================
	_ProcessResults
=================================================
*/
	bool BruteforceGenerator::_ProcessResults (OUT GenFunctions_t &functions, usize maxOutputResults)
	{
		functions.Clear();

		_outStagingBuffer->Send< GpuMsg::MapMemoryToCpu >({ GpuMsg::EMappingFlags::Read });

		_Atomics	atomics;
		_outStagingBuffer->Send< DSMsg::ReadRegion >({ BinArrayRef::FromValue(atomics) });

		if ( atomics.results == 0 ) {
			LOG( "no results found, increase max commands count or max accuracy value", ELog::Warning );
		}

		if ( atomics.results >= _maxResults ) {
			LOG( "number of results is too big, decrease max accuracy value", ELog::Warning );
		}
		
		const usize	res_count = Min( atomics.results, _maxResults );
		BinaryArray	results;	results.Resize( usize(res_count * _resultSize) );

		_outStagingBuffer->Send< DSMsg::ReadRegion >({ SizeOf<_Atomics>, results });
		
		Parameters	params;
		params.commandSet	= _config.commandSetType;
		params.constants	= _config.constantType;
		params.fitnessFunc	= _config.fitnessFunc;
		params.typeId		= _typeId;

		CHECK_ERR( ResultSearch::SearchResults( results, res_count, _config.maxConstants, _maxFuncArgs, _resultSize,
											    _bigIntSize, params, maxOutputResults, OUT functions ) );

		_outStagingBuffer->Send< GpuMsg::UnmapMemory >({});
		return true;
	}

}	// CodeGen
