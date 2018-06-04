// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Projects/CodeGen/Common.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"
#include "all_pipelines.h"

namespace CodeGen
{

	extern bool Test_GpuBruteforce ()
	{
		// create resources
		auto		factory		= GetMainSystemInstance()->GlobalSystems()->modulesFactory;
		
		ModulePtr	gpu_thread	= PlatformTools::GPUThreadHelper::FindComputeThread( factory->GlobalSystems() );
		CHECK_ERR( gpu_thread );
		
		Message< GpuMsg::GetGraphicsModules >	req_ids;
		gpu_thread->Send( req_ids );
		ComputeModuleIDs	gpuIDs = *req_ids->compute;

		ModulePtr	sync_manager = gpu_thread->GetModuleByMsg<CompileTime::TypeListFrom< Message<GpuMsg::CreateFence> >>();
		CHECK_ERR( sync_manager );

		Message< GpuMsg::CreateFence >	fence_ctor;
		sync_manager->Send( fence_ctor );
		
		ModulePtr	cmd_builder;
		CHECK_ERR( factory->Create(
						gpuIDs.commandBuilder,
						gpu_thread->GlobalSystems(),
						CreateInfo::GpuCommandBuilder{},
						OUT cmd_builder ) );

		ModulePtr	cmd_buffer;
		CHECK_ERR( factory->Create(
						gpuIDs.commandBuffer,
						gpu_thread->GlobalSystems(),
						CreateInfo::GpuCommandBuffer{},
						OUT cmd_buffer ) );
		cmd_builder->Send< ModuleMsg::AttachModule >({ cmd_buffer });

		ModulePtr	ssb_input;
		CHECK_ERR( factory->Create(
						gpuIDs.buffer,
						gpu_thread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ SizeOf<Pipelines::Bruteforce_InputsSSBO>, EBufferUsage::Storage },
							EGpuMemory::CoherentWithCPU },
						OUT ssb_input ) );
		
		ModulePtr	ssb_output;
		CHECK_ERR( factory->Create(
						gpuIDs.buffer,
						gpu_thread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ SizeOf<Pipelines::Bruteforce_OutputsSSBO>, EBufferUsage::Storage },
							EGpuMemory::CoherentWithCPU },
						OUT ssb_output ) );

		ModulePtr	buffer;
		CHECK_ERR( factory->Create(
						gpuIDs.buffer,
						gpu_thread->GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{ 16_b, EBufferUsage::Storage },
							EGpuMemory::CoherentWithCPU },
						OUT buffer ) );

		CreateInfo::PipelineTemplate	pt_ci;
		Pipelines::Create_bruteforcetest( OUT pt_ci.descr );
	
		ModulePtr	pipeline_template;
		CHECK_ERR( factory->Create(
						PipelineTemplateModuleID,
						gpu_thread->GlobalSystems(),
						pt_ci,
						OUT pipeline_template ) );
		ModuleUtils::Initialize({ pipeline_template });

		Message< GpuMsg::CreateComputePipeline >	cppl_ctor{ gpuIDs.pipeline, gpu_thread };
		pipeline_template->Send( cppl_ctor );

		ModulePtr	pipeline	= *cppl_ctor->result;
		ModulePtr	resource_table;
		CHECK_ERR( factory->Create(
						gpuIDs.resourceTable,
						gpu_thread->GlobalSystems(),
						CreateInfo::PipelineResourceTable{},
						OUT resource_table ) );
	
		resource_table->Send< ModuleMsg::AttachModule >({ "pipeline",	pipeline });
		resource_table->Send< ModuleMsg::AttachModule >({ "ssb",		buffer });
		resource_table->Send< ModuleMsg::AttachModule >({ "ssb_input",	ssb_input });
		resource_table->Send< ModuleMsg::AttachModule >({ "ssb_output",	ssb_output });

		ModuleUtils::Initialize({ cmd_builder, cmd_buffer, buffer, ssb_input, ssb_output, pipeline, resource_table });


		// prepare data
		Pipelines::Bruteforce_InputsSSBO	data1;
		Pipelines::Bruteforce_OutputsSSBO	data2;
		
		data1.initialHash.value[0] = 0x11223344;
		data1.initialHash.value[1] = 0x55667788;
		data1.initialHash.lastBit  = 64;

		data1.maxAccuracy = 1.0f;

		data1.tests[0].inArgs[0] = 1;
		data1.tests[0].inArgs[1] = 2;
		data1.tests[0].result    = 3;
	
		data1.tests[1].inArgs[0] = 10;
		data1.tests[1].inArgs[1] = 20;
		data1.tests[1].result    = 30;
	
		data1.tests[2].inArgs[0] = 11;
		data1.tests[2].inArgs[1] = 21;
		data1.tests[2].result    = 31;
	
		data2.stat.results		= 1234;
		data2.stat.bestResults	= 543;
	
		data2.funcs[0].funHash.value[0] = 0x12121212;
		data2.funcs[0].funHash.value[1] = 0;
		data2.funcs[0].funHash.lastBit  = 32;
		data2.funcs[0].ticks			= 20.0f;
		data2.funcs[0].accuracy			= 1.0f;
	
		data2.funcs[1].funHash.value[0] = 0x21212121;
		data2.funcs[1].funHash.value[1] = 0x1;
		data2.funcs[1].funHash.lastBit  = 33;
		data2.funcs[1].ticks			= 22.0f;
		data2.funcs[1].accuracy			= 2.0f;
	
		data2.funcs[2].funHash.value[0] = 0x32323232;
		data2.funcs[2].funHash.value[1] = 0x2;
		data2.funcs[2].funHash.lastBit  = 34;
		data2.funcs[2].ticks			= 14.0f;
		data2.funcs[2].accuracy			= 1.5f;

		ssb_input->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(data1) });
		ssb_output->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(data2) });


		// build command buffer
		cmd_builder->Send< GpuMsg::CmdBegin >({ cmd_buffer });

		cmd_builder->Send< GpuMsg::CmdBindComputePipeline >({ pipeline });
		cmd_builder->Send< GpuMsg::CmdBindComputeResourceTable >({ resource_table });
		cmd_builder->Send< GpuMsg::CmdDispatch >({ uint3(1) });
	
		Message< GpuMsg::CmdEnd >	cmd_end;
		cmd_builder->Send( cmd_end );


		// submit and sync
		gpu_thread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence_ctor->result });

		sync_manager->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });


		// read from buffer
		uint	result = 0;

		Message< GpuMsg::ReadFromGpuMemory >	read_cmd{ BinArrayRef::FromValue( result ) };
		buffer->Send( read_cmd );

		CHECK_FATAL( result == 111 );

		LOG( "Test_GpuBruteforce - OK", ELog::Info );
		return true;
	}

}	// CodeGen
