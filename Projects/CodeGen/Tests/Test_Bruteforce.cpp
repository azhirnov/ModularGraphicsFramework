// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Engine.Platforms.h"

#if 0

#ifdef GRAPHICS_API_SOFT
#include "Engine/Platforms/Soft/ShaderLang/SWLang.h"
#endif
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang
{
	#define INOUT
	#define IN
	#define OUT
	
	struct BigInt
	{
		UInt value[1];
		UInt lastBit;
	};
	
	struct CmdNode
	{
		UInt id;
		Int args[3];
		Int argsCount;
		Float result;
	};
	
	struct Bruteforce_Atomics
	{
		Atomic<UInt> total;
		Atomic<UInt> total2;
		Atomic<UInt> results;
	};
	
	struct Bruteforce_Result
	{
		BigInt hash;
		Float ticks;
		Float accuracy;
	};
	
	struct TestCase
	{
		Float inArgs[4];
		Float result;
	};
	
	struct Bruteforce_ConstData
	{
		BigInt initialHash;
		Float maxAccuracy;
		UInt resultsCapacity;
		UInt testCasesCount;
	};
	
	struct NodeArray
	{
		CmdNode nodes[18];
		Int count;
	};
	
	struct Bruteforce_SSBO
	{
		Bruteforce_ConstData constData;
		TestCase tests[];
	};
	
	struct Bruteforce_Results
	{
		Bruteforce_Atomics stat;
		Bruteforce_Result funcs[];
	};
	
	
	//---------------------------------
	
	UInt ToMask(const UInt x);
	Bool IsZero(const Float x);
	UInt BitScanReverse(const UInt x);
	UInt GetFunctionTicks(const UInt id);
	Float CallFunction(const UInt id, const Float args[3], const Int argsCount);
	Int GetFunctionArgsCount(const UInt id);
	void BigInt_Create(OUT BigInt &bi);
	void BigInt_Add(INOUT BigInt &bi, IN UInt right);
	Bool BigInt_IsZero(IN BigInt bi, const UInt pos);
	void BigInt_Add(INOUT BigInt &bi, IN BigInt right);
	UInt _BigInt_CalcBits(IN BigInt bi, const UInt i);
	UInt BigInt_Read(IN BigInt bi, const UInt pos, const UInt count);
	Float ValueDifference(IN Float a, IN Float b);
	void Node_Create(OUT CmdNode &n);
	Bool BuildCommandsList(const BigInt hash, OUT Float &outTicks, INOUT NodeArray &tmp, INOUT NodeArray &commands);
	void RunCommands(INOUT NodeArray &commands, const Float inArgs[4]);
	
	//---------------------------------
	
	Bool IsZero(const Float x)
	{
		return (abs( x ) < Float( 0.00001f ));
	}
	
	
	UInt ToMask(const UInt x)
	{
		return ( ((x < UInt( 0U ))) ? (UInt( 0U )) : (( ((x < UInt( 32U ))) ? (((UInt( 1U ) << x) - UInt( 1U ))) : (UInt( 4294967295U )) )) );
	}
	
	
	UInt BitScanReverse(const UInt x)
	{
		return UInt( findMSB( x ) );
	}
	
	
	Float CallFunction(const UInt id, const Float args[3], const Int argsCount)
	{
		switch( (id & UInt( 7U )) )
		{
		case Int( 0 ): return -( (args[Int( 0 )]) );
		;
		case Int( 1 ): return (args[Int( 0 )]);
		;
		case Int( 2 ): return ((args[Int( 0 )]) + (args[Int( 1 )]));
		;
		case Int( 3 ): return ((args[Int( 0 )]) * (args[Int( 1 )]));
		;
		case Int( 4 ): return ((args[Int( 0 )]) / (args[Int( 1 )]));
		;
		case Int( 5 ): return ( (((args[Int( 0 )]) < (args[Int( 1 )]))) ? (Float( 1.0f )) : (Float( -1.0f )) );
		;
		case Int( 6 ): return ( (((args[Int( 0 )]) > (args[Int( 1 )]))) ? (Float( 1.0f )) : (Float( -1.0f )) );
		;
		case Int( 7 ): return ( (((args[Int( 0 )]) >= Float( 0.0f ))) ? ((args[Int( 1 )])) : ((args[Int( 2 )])) );
		;
		}
		;
		return Float( 0.0f );
	}
	
	
	Int GetFunctionArgsCount(const UInt id)
	{
		switch( (id & UInt( 7U )) )
		{
		case Int( 0 ): return Int( 1 );
		;
		case Int( 1 ): return Int( 1 );
		;
		case Int( 2 ): return Int( 2 );
		;
		case Int( 3 ): return Int( 2 );
		;
		case Int( 4 ): return Int( 2 );
		;
		case Int( 5 ): return Int( 2 );
		;
		case Int( 6 ): return Int( 2 );
		;
		case Int( 7 ): return Int( 3 );
		;
		}
		;
		return Int( 0 );
	}
	
	
	UInt GetFunctionTicks(const UInt id)
	{
		switch( (id & UInt( 7U )) )
		{
		case Int( 0 ): return UInt( 2U );
		;
		case Int( 1 ): return UInt( 1U );
		;
		case Int( 2 ): return UInt( 2U );
		;
		case Int( 3 ): return UInt( 4U );
		;
		case Int( 4 ): return UInt( 10U );
		;
		case Int( 5 ): return UInt( 3U );
		;
		case Int( 6 ): return UInt( 3U );
		;
		case Int( 7 ): return UInt( 5U );
		;
		}
		;
		return UInt( 0U );
	}
	
	
	void BigInt_Create(OUT BigInt &bi)
	{
		for(UInt i = UInt( 0U ); (i < UInt( 1U )); ++( i ))
		{
			(bi.value[i]) = UInt( 0U );
		}
		;
		bi.lastBit = UInt( 0U );
	}
	
	
	void BigInt_Add(INOUT BigInt &bi, IN UInt right)
	{
		for(UInt i = UInt( 0U ); (i < UInt( 1U )); ++( i ))
		{
			const UInt prev = (bi.value[i]);
			;
			((bi.value[i]) += right);
			if (((bi.value[i]) >= prev))
			{
				bi.lastBit = max( bi.lastBit, _BigInt_CalcBits(bi, i) );
				break;
			;
			}
			;
			right = UInt( 1U );
		}
		;
	}
	
	
	void BigInt_Add(INOUT BigInt &bi, IN BigInt right)
	{
		for(UInt j = UInt( 0U ); (j < UInt( 1U )); ++( j ))
		{
			UInt val = (right.value[j]);
			;
			if ((val == UInt( 0U )))
			{
				continue;
			}
			;
			for(UInt i = j; (i < UInt( 1U )); ++( i ))
			{
				const UInt prev = (bi.value[i]);
				;
				((bi.value[i]) += val);
				if (((bi.value[i]) >= prev))
				{
					bi.lastBit = max( bi.lastBit, _BigInt_CalcBits(bi, i) );
					break;
				;
				}
				;
				val = UInt( 1U );
			}
			;
		}
		;
	}
	
	
	Bool BigInt_IsZero(IN BigInt bi, const UInt pos)
	{
		return (pos >= bi.lastBit);
	}
	
	
	UInt BigInt_Read(IN BigInt bi, const UInt pos, const UInt count)
	{
		const UInt i = clamp( (pos / UInt( 32U )), UInt( 0U ), UInt( 0U ) );
		;
		UInt result = (((bi.value[i]) >> pos) & ToMask(count));
		;
		UInt max_count = (UInt( 32U ) - pos);
		;
		if (((count > max_count) && ((i + UInt( 1U )) < UInt( 1U ))))
		{
			(result |= (((bi.value[(i + UInt( 1U ))]) & ToMask((count - max_count))) << max_count));
		;
		}
		;
		return result;
	}
	
	
	UInt _BigInt_CalcBits(IN BigInt bi, const UInt i)
	{
		return ((BitScanReverse((bi.value[i])) + UInt( 1U )) + (i * UInt( 32U )));
	}
	
	
	Float ValueDifference(IN Float a, IN Float b)
	{
		if (IsZero((a - b)))
		{
			return Float( 0.0f );
		}
		;
		Float diff = abs( Float( (a - b) ) );
		;
		Float div = abs( Float( a ) );
		;
		return ( (IsZero(div)) ? (Float( 9.99999968e+37f )) : ((diff / div)) );
	}
	
	
	void Node_Create(OUT CmdNode &n)
	{
		n.id = UInt( 4294967295U );
		n.argsCount = Int( 0 );
		n.result = Float( 0.0f );
		for(UInt i = UInt( 0U ); (i < UInt( 3U )); ++( i ))
		{
			(n.args[i]) = Int( 0 );
		}
		;
	}
	
	
	Bool BuildCommandsList(const BigInt hash, OUT Float &outTicks, INOUT NodeArray &tmp, INOUT NodeArray &commands)
	{
		Int pos = Int( 0 );
		;
		Int arg_idx = Int( 0 );
		;
		outTicks = Float( 0.0f );
		tmp.count = Int( 0 );
		do {
			const Int i = tmp.count;
			ASSERT( i >= 0 and i < GX_STL::GXTypes::CountOf(tmp.nodes) );

			Node_Create((tmp.nodes[i]));
			(tmp.nodes[i]).id = BigInt_Read(hash, UInt( pos ), UInt( 3U ));
			
			const Int cnt = GetFunctionArgsCount((tmp.nodes[i]).id);
			ASSERT( cnt >= 0 and cnt <= 3 );
			;
			(outTicks += Float( GetFunctionTicks((tmp.nodes[i]).id) ));
			(pos += Int( 3 ));
			arg_idx = max( arg_idx, i );
			for(Int j = (cnt - Int( 1 )); (j >= Int( 0 )); --( j ))
			{
				ASSERT( j >= 0 and j < 3 );
				((tmp.nodes[i]).args[j]) = ++( arg_idx );
			}
			;
			(tmp.nodes[i]).argsCount = cnt;
			++( tmp.count );
		} while( (!( BigInt_IsZero(hash, UInt( pos )) ) && (tmp.count < Int( 10 ))));
		;
		if ((tmp.count <= Int( 0 )))
		{
			return Bool( false );
		}
		;
		commands.count = Int( 4 );
		const Int cnt = (tmp.count - Int( 1 ));
		;
		for(Int i = cnt; (i >= Int( 0 )); --( i ))
		{
			ASSERT( i >= 0 and i < GX_STL::GXTypes::CountOf(tmp.nodes) );

			for(Int j = ((tmp.nodes[i]).argsCount - Int( 1 )); (j >= Int( 0 )); --( j ))
			{
				ASSERT( j >= 0 and j < 3 );

				const Int idx = ((tmp.nodes[i]).args[j]);
				;
				if ((idx <= cnt))
				{
					((tmp.nodes[i]).args[j]) = ((cnt - idx) + Int( 4 ));
				}
				else
				{
					((tmp.nodes[i]).args[j]) = ((arg_idx - idx) % Int( 4 ));
				}
				ASSERT( tmp.nodes[i].args[j] >= 0 and tmp.nodes[i].args[j] < commands.count );
			}

			ASSERT( commands.count >= 0 and commands.count < GX_STL::GXTypes::CountOf(commands.nodes) );
			ASSERT( i < tmp.count );

			(commands.nodes[commands.count]) = (tmp.nodes[i]);
			++( commands.count );
		}
		;
		return Bool( true );
	}
	
	
	void RunCommands(INOUT NodeArray &commands, const Float inArgs[4])
	{
		Float reg[3];
		(reg[Int( 0 )]) = Float( 0.0f );
		for(UInt i = UInt( 0U ); (i < UInt( 4U )); ++( i ))
		{
			ASSERT( i < GX_STL::GXTypes::CountOf(commands.nodes) );
			(commands.nodes[i]).result = (inArgs[i]);
		}
		;
		const Int cmd_cnt = commands.count;
		;
		for(Int i = Int( 4 ); (i < cmd_cnt); ++( i ))
		{
			ASSERT( i < GX_STL::GXTypes::CountOf(commands.nodes) );
			const Int args_cnt = (commands.nodes[i]).argsCount;
			;
			for(Int j = Int( 0 ); (j < args_cnt); ++( j ))
			{
				ASSERT( j < 3 );
				Int idx = ((commands.nodes[i]).args[j]);
				;
				(reg[j]) = (commands.nodes[idx]).result;
			}
			;
			(commands.nodes[i]).result = CallFunction((commands.nodes[i]).id, reg, args_cnt);
		}
		;
	}
	
	
	static void sw_bruteforce (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::StorageBuffer< Bruteforce_SSBO, Impl::EStorageAccess::ReadOnly > ssb_input;	_helper_.GetStorageBuffer( 0, ssb_input );
		Impl::StorageBuffer< Bruteforce_Results, Impl::EStorageAccess::Coherent > ssb_output;	_helper_.GetStorageBuffer( 1, ssb_output );
		auto& gl_GlobalInvocationID = _helper_.GetComputeShaderState().inGlobalInvocationID;
	
		// shader
	{
		if ((atomicAdd( ssb_output->stat.total, UInt( 1U ) ) == UInt( 4294967295U )))
		{
			atomicAdd( ssb_output->stat.total2, UInt( 1U ) );
		;
		}
		;
		BigInt initial_hash = ssb_input->constData.initialHash;
		;
		BigInt hash;
		BigInt_Create(hash);
		//BigInt_Add(hash, initial_hash);
		BigInt_Add(hash, gl_GlobalInvocationID.x + 0xFFFFFFF);
		Float ticks = Float( 0.0f );
		;
		NodeArray tmp_commands;
		NodeArray commands;
		if (!( BuildCommandsList(hash, ticks, tmp_commands, commands) ))
		{
			return;
		}
		;
		Float accuracy = Float( 0.0f );
		;
		for(UInt i = UInt( 0U ); (i < ssb_input->constData.testCasesCount); ++( i ))
		{
			RunCommands(commands, (ssb_input->tests[i]).inArgs);
			const UInt idx = UInt( (commands.count - Int( 1 )) );
			;
			const Float res = (commands.nodes[idx]).result;
			;
			if ((isinf( res ) || isnan( res )))
			{
				return;
			}
			;
			(accuracy += ValueDifference((ssb_input->tests[i]).result, res));
			if ((accuracy > (ssb_input->constData.maxAccuracy * Float( max( (i + UInt( 1U )), UInt( 2U ) ) ))))
			{
				return;
			}
			;
		}
		;
		(accuracy /= Float( ssb_input->constData.testCasesCount ));
		if ((accuracy > ssb_input->constData.maxAccuracy))
		{
			return;
		}
		;
		const UInt idx = atomicAdd( ssb_output->stat.results, UInt( 1U ) );
		;
		if ((idx >= ssb_input->constData.resultsCapacity))
		{
			atomicExchange( ssb_output->stat.results, ssb_input->constData.resultsCapacity );
			return;
		;
		}
		;
		(ssb_output->funcs[idx]).hash = hash;
		(ssb_output->funcs[idx]).ticks = ticks;
		(ssb_output->funcs[idx]).accuracy = accuracy;
	}
	
	
	}
}	// SWShaderLang
#endif	// GRAPHICS_API_SOFT




#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"
#include "Generators/BigInteger.h"

extern bool Test_Bruteforce ()
{
	using namespace Engine;
	using namespace Engine::Platforms;
	using namespace CodeGen;

	auto			ms			= GetMainSystemInstance();
	auto			factory		= ms->GlobalSystems()->modulesFactory;
	ModulePtr		gpu_thread	= PlatformTools::GPUThreadHelper::FindComputeThread( ms->GlobalSystems() );
	ModulePtr		sync_mngr	= gpu_thread->GetModuleByMsg<CompileTime::TypeListFrom< Message<GpuMsg::CreateFence> >>();
	
	const BytesU	testcase_size	= SizeOf< SWShaderLang::TestCase >;
	const BytesU	bigint_size		= SizeOf< SWShaderLang::BigInt >;
	const BytesU	const_size		= SizeOf< SWShaderLang::Bruteforce_ConstData >;
	const BytesU	atomics_size	= SizeOf< SWShaderLang::Bruteforce_Atomics >;
	const BytesU	result_size		= SizeOf< SWShaderLang::Bruteforce_Result >;

	const uint		max_results		= 10000;
	const uint		testcases_count	= 10;
	const BitsU		max_bits		= 32_bit;
	const uint		local_threads	= 8;

	const BytesU	inbuf_size		= const_size + testcase_size * testcases_count;
	const BytesU	outbuf_size		= atomics_size + result_size * max_results;

	CreateInfo::PipelineTemplate	pt_ci;
	pt_ci.descr.supportedShaders = EShader::Compute;

	pt_ci.descr.localGroupSize = uint3(local_threads, 1, 1);
	pt_ci.descr.layout = PipelineLayoutDescriptor::Builder()
							.AddStorageBuffer( "ssb_input", const_size, testcase_size, EShaderMemoryModel::ReadOnly, 0, 0, EShader::Compute )
							.AddStorageBuffer( "ssb_output", atomics_size, result_size, EShaderMemoryModel::Coherent, 1, 1, EShader::Compute )
							.Finish();

	pt_ci.descr.Compute().FuncSW( &SWShaderLang::sw_bruteforce );
	
	ModulePtr	pipeline_template;
	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pt_ci,
					OUT pipeline_template ) );

	ModuleUtils::Initialize({ pipeline_template });
	
	Message< GpuMsg::GetGraphicsModules >	req_ids;
	gpu_thread->Send( req_ids );
	ComputeModuleIDs	gpu_ids = *req_ids->compute;
	
	Message< GpuMsg::CreateFence >	fence_ctor;
	sync_mngr->Send( fence_ctor );
	
	ModulePtr	cmd_builder;
	CHECK_ERR( factory->Create(
					gpu_ids.commandBuilder,
					ms->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{},
					OUT cmd_builder )
	);

	ModulePtr	cmd_buffer;
	CHECK_ERR( factory->Create(
					gpu_ids.commandBuffer,
					ms->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{},
					OUT cmd_buffer ) );
	cmd_builder->Send< ModuleMsg::AttachModule >({ cmd_buffer });
	
	ModulePtr	in_buffer;
	CHECK_ERR( factory->Create(
					gpu_ids.buffer,
					ms->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ inbuf_size, EBufferUsage::Storage | EBufferUsage::TransferDst },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT in_buffer ) );
		
	ModulePtr	out_buffer;
	CHECK_ERR( factory->Create(
					gpu_ids.buffer,
					ms->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ outbuf_size, EBufferUsage::Storage | EBufferUsage::TransferDst },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT out_buffer ) );
	
	Message< GpuMsg::CreateComputePipeline >	cppl_ctor{ gpu_ids.pipeline, gpu_thread };
	pipeline_template->Send( cppl_ctor );

	ModulePtr	pipeline = *cppl_ctor->result;
	
	ModulePtr	resource_table;
	CHECK_ERR( factory->Create(
					gpu_ids.resourceTable,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resource_table ) );
	
	resource_table->Send< ModuleMsg::AttachModule >({ "pipeline",   pipeline  });
	resource_table->Send< ModuleMsg::AttachModule >({ "ssb_input",  in_buffer  });
	resource_table->Send< ModuleMsg::AttachModule >({ "ssb_output", out_buffer });
	
	ModuleUtils::Initialize({ cmd_builder, cmd_buffer, in_buffer, out_buffer, pipeline, resource_table });
	
	// write data to buffer
	{
		auto	rnd =			LAMBDA() () {
									return ImprovedRandom::FloatRange( -5.0f, 5.0f );
								};
		auto	expectedFunc =	LAMBDA() (ArrayCRef<SWShaderLang::Float> args) {
									return (args[0] * args[1] + args[2] / args[3]) * 8;
								};

		in_buffer->Send< GpuMsg::MapMemoryToCpu >({ GpuMsg::MapMemoryToCpu::EMappingFlags::Write });

		SWShaderLang::Bruteforce_ConstData	cdata;
		cdata.testCasesCount	= testcases_count;
		cdata.resultsCapacity	= max_results;
		cdata.maxAccuracy		= 0.001f;
		cdata.initialHash		= {};
		
		BytesU	offset;
		in_buffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(cdata), offset });
		offset += BytesU::SizeOf( cdata );

		for (uint i = 0; i < testcases_count; ++i)
		{
			SWShaderLang::TestCase	tc;
			tc.inArgs[0] = rnd();
			tc.inArgs[1] = rnd();
			tc.inArgs[2] = rnd();
			tc.inArgs[3] = rnd();
			tc.result = expectedFunc( tc.inArgs );

			in_buffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(tc), offset });
			offset += BytesU::SizeOf( tc );
		}

		in_buffer->Send< GpuMsg::UnmapMemory >({});
		CHECK( inbuf_size == offset );
	}

	// build command buffer
	{
		BigInteger<uint,4>	hash;		hash += MaxValue<uint>() >> 3;
		const BitsU			max_step	= Min( BitsU::SizeOf<uint>()-5, max_bits );
		const BitsU			step		= Min( max_step, max_bits );
		const uint			num_threads	= 1u << usize(step);
		const uint			num_groups	= (num_threads + local_threads - 1) / local_threads;

		cmd_builder->Send< GpuMsg::CmdBegin >({ cmd_buffer });
		cmd_builder->Send< GpuMsg::CmdFillBuffer >({ out_buffer, 0u });

		cmd_builder->Send< GpuMsg::CmdBindComputePipeline >({ pipeline });
		cmd_builder->Send< GpuMsg::CmdBindComputeResourceTable >({ resource_table });
		
		for (; hash.Count() <= max_bits;)
		{
			// update initial hash
			/*{
				SWShaderLang::BigInt	initial_hash;
				initial_hash.lastBit	= uint(hash.Count());

				FOR( i, initial_hash.value ) {
					initial_hash.value[i] = hash.ToArray()[i];
				}

				cmd_builder->Send< GpuMsg::CmdUpdateBuffer >({ in_buffer, BinArrayCRef::FromValue(initial_hash) });
			}*/

			cmd_builder->Send< GpuMsg::CmdDispatch >({ uint3( num_groups, 1, 1 ) });
			
			hash += (num_groups * local_threads);
		}
		cmd_builder->Send< GpuMsg::CmdEnd >({});
	}
	
	gpu_thread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_buffer, *fence_ctor->result });

	sync_mngr->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });

	{
		out_buffer->Send< GpuMsg::MapMemoryToCpu >({ GpuMsg::MapMemoryToCpu::EMappingFlags::Read });

		SWShaderLang::Bruteforce_Atomics	atomics;
		out_buffer->Send< GpuMsg::ReadFromGpuMemory >({ BinArrayRef::FromValue(atomics) });
		
		CHECK_FATAL( uint(atomics.results) > 0 and uint(atomics.results) < max_results );
	}
	return true;
}

#endif
