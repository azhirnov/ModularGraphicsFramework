// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PApp.h"
#include "Pipelines/all_pipelines.h"

bool PApp::_Test_GlobalToLocal ()
{
	const BytesU	buf_size = SizeOf< Pipelines::GlobalToLocal_SSBO >;

	decltype(Pipelines::GlobalToLocal_SSBO::data)	in_data = { 1, 20, 300, 4000 };


	// create resources
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	Message< GpuMsg::CreateFence >	fence_ctor;
	syncManager->Send( fence_ctor );

	ModulePtr	cmd_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.commandBuffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{},
					OUT cmd_buffer ) );
	cmdBuilder->Send< ModuleMsg::AttachModule >({ cmd_buffer });

	ModulePtr	buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ buf_size, EBufferUsage::Storage },
						EGpuMemory::CoherentWithCPU },
					OUT buffer ) );

	CreateInfo::PipelineTemplate	pt_ci;
	Pipelines::Create_globaltolocal( OUT pt_ci.descr );
	
	ModulePtr	pipeline_template;
	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					gpuThread->GlobalSystems(),
					pt_ci,
					OUT pipeline_template ) );
	ModuleUtils::Initialize({ pipeline_template });

	Message< GpuMsg::CreateComputePipeline >	cppl_ctor{ gpuIDs.pipeline, gpuThread };
	pipeline_template->Send( cppl_ctor );

	ModulePtr	pipeline	= *cppl_ctor->result;

	ModulePtr	resource_table;
	CHECK_ERR( factory->Create(
					gpuIDs.resourceTable,
					gpuThread->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resource_table ) );
	
	resource_table->Send< ModuleMsg::AttachModule >({ "pipeline", pipeline });
	resource_table->Send< GpuMsg::PipelineAttachBuffer >({ "ssb", buffer, buf_size, 0_b });

	ModuleUtils::Initialize({ cmd_buffer, buffer, pipeline, resource_table });
	

	// write data to buffer
	Message< GpuMsg::WriteToGpuMemory >	write_cmd{ BinArrayCRef::FromValue(in_data) };
	buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL::SizeOf(in_data) );


	// build command buffer
	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer });

	cmdBuilder->Send< GpuMsg::CmdBindComputePipeline >({ pipeline });
	cmdBuilder->Send< GpuMsg::CmdBindComputeResourceTable >({ resource_table });
	cmdBuilder->Send< GpuMsg::CmdDispatch >({ uint3(1) });
	
	Message< GpuMsg::CmdEnd >	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence_ctor->result });

	syncManager->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });


	// read from buffer
	BinaryArray	dst_data;	dst_data.Resize( usize(buf_size) );

	Message< GpuMsg::ReadFromGpuMemory >	read_cmd{ dst_data };
	buffer->Send( read_cmd );

	auto* st = Cast<const Pipelines::GlobalToLocal_SSBO *>( read_cmd->result->ptr() );

	CHECK_ERR( st->results[0] == 4321 );
	//CHECK_ERR( st->results[1] == 4322 );	// TODO

	LOG( "GlobalToLocal - OK", ELog::Info );
	return true;
}
