// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "Pipelines/all_pipelines.h"

bool CApp::_Test_BufferRange ()
{
	// generate data
	Pipelines::BufferAlign_Struct	st1;
	st1.b1	= true;
	st1.b3	= Bool32_3( true );
	st1.f1	= 4.123f;
	st1.f2	= float2( 6.434f, 9.23f );
	st1.i1	= 753;
	st1.i4	= int4( 655, 123, 554, 264 );
	st1.u3	= uint3( 14342, 5643, 1234 );
	
	Pipelines::BufferAlign_Struct	st2;
	st2.i4 = int4( 1, -2, 3, -4 );
	st2.f2 = float2( 3.1f, 5.5f );
	st2.b1 = true;
	st2.u3 = uint3( 9, 8, 7 );
	st2.i1 = 0x123456;
	st2.b3 = Bool32_3( false, true, false );
	st2.f1 = 1.4335f;

	BytesU	buf_off		= SizeOf<Pipelines::BufferAlign_Struct> * 2;
	BytesU	buf_size	= buf_off + SizeOf<Pipelines::BufferAlign_Struct> * 3;


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
						BufferDescription{ buf_size, EBufferUsage::Storage },
						EGpuMemory::CoherentWithCPU },
					OUT buffer ) );

	CreateInfo::PipelineTemplate	pt_ci;
	Pipelines::Create_bufferalign( OUT pt_ci.descr );
	
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
	resource_table->Send( GpuMsg::PipelineAttachBuffer{ "ssb", buffer, (buf_size - buf_off), buf_off });

	ModuleUtils::Initialize({ cmd_buffer, buffer, pipeline, resource_table });


	// write data to buffer
	GpuMsg::WriteToGpuMemory	write_cmd{ buf_off, BinArrayCRef::FromValue(st1) };
	buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd.wasWritten == BytesUL::SizeOf(st1) );


	// build command buffer
	cmdBuilder->Send( GpuMsg::CmdBegin{ cmd_buffer });

	cmdBuilder->Send( GpuMsg::CmdBindComputePipeline{ pipeline });
	cmdBuilder->Send( GpuMsg::CmdBindComputeResourceTable{ resource_table });
	cmdBuilder->Send( GpuMsg::CmdDispatch{ uint3(1) });
	
	GpuMsg::CmdEnd	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send( GpuMsg::SubmitComputeQueueCommands{ *cmd_end.result }.SetFence( *fence_ctor.result ));

	syncManager->Send( GpuMsg::ClientWaitFence{ *fence_ctor.result });


	// read from buffer
	BinaryArray	dst_data;	dst_data.Resize( usize(buf_size) );

	GpuMsg::ReadFromGpuMemory	read_cmd{ dst_data };
	buffer->Send( read_cmd );

	Pipelines::BufferAlign_Struct const*	res_st = reinterpret_cast< Pipelines::BufferAlign_Struct const *>( dst_data.ptr() );
	
	++res_st;
	++res_st;
	CHECK_ERR(	All( st1.b1			== res_st->b1		)	and
				All( st1.b3.xyz()	== res_st->b3.xyz() )	and
				All( st1.f1			== res_st->f1		)	and
				All( st1.f2.xy()	== res_st->f2.xy()	)	and
				All( st1.i1			== res_st->i1		)	and
				All( st1.i4.xyzw()	== res_st->i4.xyzw())	and
				All( st1.u3.xyz()	== res_st->u3.xyz() )	);
	
	++res_st;
	CHECK_ERR(	All( st1.b1			== res_st->b1		)	and
				All( st1.b3.xyz()	== res_st->b3.xyz() )	and
				All( st1.f1			== res_st->f1		)	and
				All( st1.f2.xy()	== res_st->f2.xy()	)	and
				All( st1.i1			== res_st->i1		)	and
				All( st1.i4.xyzw()	== res_st->i4.xyzw())	and
				All( st1.u3.xyz()	== res_st->u3.xyz() )	);

	++res_st;
	CHECK_ERR(	All( st2.b1			== res_st->b1		)	and
				All( st2.b3.xyz()	== res_st->b3.xyz() )	and
				All( st2.f1			== res_st->f1		)	and
				All( st2.f2.xy()	== res_st->f2.xy()	)	and
				All( st2.i1			== res_st->i1		)	and
				All( st2.i4.xyzw()	== res_st->i4.xyzw())	and
				All( st2.u3.xyz()	== res_st->u3.xyz() )	);

	LOG( "BufferRange - OK", ELog::Info );
	return true;
}
