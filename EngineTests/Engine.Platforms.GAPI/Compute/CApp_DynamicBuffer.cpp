// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "Pipelines/all_pipelines.h"

bool CApp::_Test_DynamicBuffer ()
{
	const BytesU	buf_size = SizeOf<Pipelines::DynamicBuffer_SSBO> + SizeOf<Pipelines::DynamicBuffer_Struct> * 100;

	// create resources
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	GpuMsg::CreateFence		fence_ctor;
	syncManager->Send( fence_ctor );

	ModulePtr	cmd_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.commandBuffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{},
					OUT cmd_buffer ) );
	cmdBuilder->Send( ModuleMsg::AttachModule{ cmd_buffer });

	ModulePtr	buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescription{ buf_size, EBufferUsage::TransferDst | EBufferUsage::Storage },
						EGpuMemory::CoherentWithCPU },
					OUT buffer ) );

	CreateInfo::PipelineTemplate	pt_ci;
	Pipelines::Create_dynamicbuffer( OUT pt_ci.descr );
	
	ModulePtr	pipeline_template;
	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					gpuThread->GlobalSystems(),
					pt_ci,
					OUT pipeline_template ) );
	ModuleUtils::Initialize({ pipeline_template });

	GpuMsg::CreateComputePipeline	cppl_ctor{ gpuIDs.pipeline, gpuThread };
	pipeline_template->Send( cppl_ctor );

	ModulePtr	pipeline	= *cppl_ctor.result;
	ModulePtr	resource_table;
	CHECK_ERR( factory->Create(
					gpuIDs.resourceTable,
					gpuThread->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resource_table ) );
	
	resource_table->Send( ModuleMsg::AttachModule{ "pipeline", pipeline });
	resource_table->Send( ModuleMsg::AttachModule{ "ssb", buffer });

	ModuleUtils::Initialize({ cmd_buffer, buffer, pipeline, resource_table });


	// write data to buffer
	Pipelines::DynamicBuffer_SSBO	st_buf = {};
	st_buf.f2 = float2(54.654f, 21.7653f);
	st_buf.i4 = int4(76543, 234, 7563, 146543);

	GpuMsg::WriteToGpuMemory	write_cmd{ BinArrayCRef::FromValue(st_buf) };
	buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd.wasWritten == BytesUL::SizeOf(st_buf) );


	// build command buffer
	cmdBuilder->Send( GpuMsg::CmdBegin{ cmd_buffer });

	cmdBuilder->Send( GpuMsg::CmdFillBuffer{ buffer, 0, buf_size - BytesU::SizeOf(st_buf), BytesU::SizeOf(st_buf) });	// clear dynamic part

	cmdBuilder->Send( GpuMsg::CmdBindComputePipeline{ pipeline });
	cmdBuilder->Send( GpuMsg::CmdBindComputeResourceTable{ resource_table });
	cmdBuilder->Send( GpuMsg::CmdDispatch{ uint3(100, 1, 1) });
	
	GpuMsg::CmdEnd	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send( GpuMsg::SubmitComputeQueueCommands{ *cmd_end.result, *fence_ctor.result });

	syncManager->Send( GpuMsg::ClientWaitFence{ *fence_ctor.result });


	// read from buffer
	BinaryArray	dst_data;	dst_data.Resize( usize(buf_size) );

	GpuMsg::ReadFromGpuMemory	read_cmd{ dst_data };
	buffer->Send( read_cmd );

	Pipelines::DynamicBuffer_SSBO const&	res_stbuf  = *reinterpret_cast< Pipelines::DynamicBuffer_SSBO const *>( dst_data.ptr() );
	Pipelines::DynamicBuffer_Struct const*	res_dynbuf = reinterpret_cast< Pipelines::DynamicBuffer_Struct const *>( dst_data.ptr() + BytesU::SizeOf(st_buf) );

	CHECK_ERR(	All( res_stbuf.f2 == st_buf.f2 )	and
				All( res_stbuf.i4 == st_buf.i4 ) );

	for (uint i = 0; i < 100; ++i)
	{
		CHECK_ERR(All( res_dynbuf[i].i2 == int2( st_buf.i4.x, i ) ));
		CHECK_ERR(All( res_dynbuf[i].b1 == Bool32((i&1) == 0) ));
		CHECK_ERR(All( res_dynbuf[i].f2 == -st_buf.f2 ));
		CHECK_ERR(All( res_dynbuf[i].i3.xy() == int2(0) ));
		CHECK_ERR(All( res_dynbuf[i].i3.z == ~0 ));
		CHECK_ERR(All( res_dynbuf[i].b2 != Bool32_2(false) ));
	}

	LOG( "DynamicBuffer - OK", ELog::Info );
	return true;
}
