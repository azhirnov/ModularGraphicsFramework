// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "../Pipelines/all_pipelines.h"


inline ModulePtr CApp::GetGThread (GlobalSystemsRef gs)
{
	using CThreadMsgList_t		= CompileTime::TypeListFrom< Message<GpuMsg::ThreadBeginFrame>, Message<GpuMsg::ThreadEndFrame>, Message<GpuMsg::GetDeviceInfo> >;
	using CThreadEventList_t	= CompileTime::TypeListFrom< Message<GpuMsg::DeviceCreated>, Message<GpuMsg::DeviceBeforeDestroy> >;

	return gs->parallelThread->GetModuleByMsgEvent< CThreadMsgList_t, CThreadEventList_t >();
}


CApp::CApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();
}


bool CApp::Initialize (GAPI::type api)
{
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	ms->AddModule( 0, CreateInfo::Platform{} );

	{
		ModulePtr	context;
		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ api }, OUT context ) );
		ms->Send< ModuleMsg::AttachModule >({ context });

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		context->Send( req_ids );
		ids << req_ids->compute;
	}

	auto		thread	= ms->GlobalSystems()->parallelThread;
	
	ModulePtr	window;
	CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::Window{}, OUT window ) );
	thread->Send< ModuleMsg::AttachModule >({ window });

	ModulePtr	gthread;
	CHECK_ERR( factory->Create( ids.thread, ms->GlobalSystems(), CreateInfo::GpuThread{
					   GraphicsSettings{
							api,
							CreateInfo::GpuContext::EFlags::bits() | CreateInfo::GpuContext::EFlags::DebugContext
						} }, OUT gthread ) );
	thread->Send< ModuleMsg::AttachModule >({ gthread });

	window->Subscribe( this, &CApp::_OnWindowClosed );
	gthread->Subscribe( this, &CApp::_Draw );
	gthread->Subscribe( this, &CApp::_GInit );
	gthread->Subscribe( this, &CApp::_GDelete );

	// finish initialization
	ModuleUtils::Initialize({ ms });
	return true;
}


void CApp::Quit ()
{
	Message< ModuleMsg::Delete >	del_msg;

	if ( pipelineTemplate )
		pipelineTemplate->Send( del_msg );

	looping = false;
}


bool CApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send< ModuleMsg::Update >({});
	return true;
}


bool CApp::_OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &)
{
	looping = false;
	return true;
}


bool CApp::_Draw (const Message< ModuleMsg::Update > &)
{
	if ( not looping )
		return false;

	auto		gthread	= GetGThread( ms->GlobalSystems() );
	uint		prev_idx= cmdBufIndex % cmdBuffers.Count();
	uint		index	= (++cmdBufIndex) % cmdBuffers.Count();

	syncManager->Send< GpuMsg::ClientWaitFence >({ cmdFence[prev_idx] });

	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmdBuffers[index] });
	
	cmdBuilder->Send< GpuMsg::CmdBindComputePipeline >({ cpipeline });
	cmdBuilder->Send< GpuMsg::CmdBindComputeResourceTable >({ resourceTable });
	cmdBuilder->Send< GpuMsg::CmdDispatch >({ uint3(1, 1, 1) });

	cmdBuilder->Send< GpuMsg::CmdCopyImageToBuffer >({});

	Message< GpuMsg::CmdEnd >	cmd_end = {};
	cmdBuilder->Send( cmd_end );

	gthread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(null), cmdFence[index] });
	return true;
}


bool CApp::_CreatePipeline ()
{
	auto	gthread	= GetGThread( ms->GlobalSystems() );
	auto	factory	= ms->GlobalSystems()->modulesFactory;
	
	CreateInfo::PipelineTemplate	pp_templ;
	Pipelines::Create_defaultcompute2( OUT pp_templ.descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pp_templ,
					OUT pipelineTemplate )
	);

	Message< GpuMsg::CreateComputePipeline >	create_cpp;
	
	create_cpp->gpuThread	= gthread;
	create_cpp->moduleID	= ids.pipeline;

	pipelineTemplate->Send( create_cpp );
	cpipeline = create_cpp->result.Get();

	CHECK_ERR( factory->Create(
					ids.resourceTable,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resourceTable )
	);

	resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", cpipeline });
	resourceTable->Send< ModuleMsg::AttachModule >({ "un_OutImage", image });
	return true;
}


bool CApp::_CreateCmdBuffers ()
{
	auto	gthread = GetGThread( ms->GlobalSystems() );
	auto	factory = ms->GlobalSystems()->modulesFactory;

	CHECK_ERR( factory->Create(
					ids.commandBuilder,
					gthread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{ gthread },
					OUT cmdBuilder )
	);
	
	cmdBufIndex = 0;
	cmdBuffers.Resize( 6 );
	cmdFence.Resize( 6 );

	FOR( i, cmdBuffers )
	{
		CHECK_ERR( factory->Create(
						ids.commandBuffer,
						gthread->GlobalSystems(),
						CreateInfo::GpuCommandBuffer{
							gthread,
							CommandBufferDescriptor{ ECmdBufferCreate::bits() | ECmdBufferCreate::ImplicitResetable }
						},
						OUT cmdBuffers[i] )
		);
		cmdBuilder->Send< ModuleMsg::AttachModule >({ ""_str << i, cmdBuffers[i] });
	}

	FOR( i, cmdFence )
	{
		Message< GpuMsg::CreateFence >	ctor;
		syncManager->Send( ctor );
		cmdFence[i] = *ctor->result;
	}
	return true;
}


bool CApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
{
	auto	gthread = GetGThread( ms->GlobalSystems() );
	auto	factory = gthread->GlobalSystems()->modulesFactory;

	syncManager = gthread->GetModuleByMsg<CompileTime::TypeListFrom< Message<GpuMsg::CreateFence> >>();
	CHECK_ERR( syncManager );

	CHECK_ERR( factory->Create(
					ids.buffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	BufferDescriptor{
												SizeOf<Pipelines::UB>(),
												EBufferUsage::bits() | EBufferUsage::TransferDst
											},
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::All },
					OUT buffer ) );

	CHECK_ERR( factory->Create(
					ids.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{	ImageDescriptor{
												EImage::Tex2D,
												uint4( 1024, 1024, 0, 0 ),
												EPixelFormat::RGBA8_UNorm,
												EImageUsage::bits() | EImageUsage::Storage | EImageUsage::TransferSrc
											},
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::GpuReadWrite },
					OUT image ) );

	CHECK_ERR( _CreateCmdBuffers() );
	CHECK_ERR( ModuleUtils::Initialize({ buffer, image, cmdBuilder }) );

	CHECK_ERR( _CreatePipeline() );
	CHECK_ERR( ModuleUtils::Initialize({ cpipeline, resourceTable }) );

	return true;
}


bool CApp::_GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	return true;
}
