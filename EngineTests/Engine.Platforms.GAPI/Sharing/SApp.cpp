// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "SApp.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"


/*
=================================================
	constructor
=================================================
*/
SApp::SApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();

	tests	<< &SApp::_Test_BufferSharing
			<< &SApp::_Test_ImageSharing
		;
}

/*
=================================================
	destructor
=================================================
*/
SApp::~SApp ()
{
}

/*
=================================================
	Initialize
=================================================
*/
bool SApp::Initialize (GAPI::type api, GAPI::type sharedApi, StringCRef dev)
{
	CHECK_ERR( _CreateMainGPU( api, dev ) );

	sharedGpu.api = sharedApi;

	// finish initialization
	ModuleUtils::Initialize({ ms });
	return true;
}

/*
=================================================
	_CreateMainGPU
=================================================
*/
bool SApp::_CreateMainGPU (GAPI::type api, StringCRef dev)
{
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	ms->AddModule( 0, CreateInfo::Platform{} );

	ComputeSettings	settings;
	settings.version	= api;
	settings.device		= dev;
	settings.isDebug	= true;

	{
		ModulePtr	context;
		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ settings }, OUT context ) );
		ms->Send( ModuleMsg::AttachModule{ context });

		GpuMsg::GetGraphicsModules	req_ids;
		context->Send( req_ids );
		gpuIDs = *req_ids.compute;
	}

	auto		thread	= ms->GlobalSystems()->parallelThread;
	
	ModulePtr	window;
	CHECK_ERR( factory->Create( 0, ms->GlobalSystems(),
								CreateInfo::Window{
								   Uninitialized,
								   Uninitialized,
								   uint2(1,1),
								   int2(),
								   CreateInfo::Window::EVisibility::Invisible
								},
								OUT window ) );
	thread->Send( ModuleMsg::AttachModule{ window });

	ModulePtr	gthread;
	CHECK_ERR( factory->Create(
						gpuIDs.thread,
						ms->GlobalSystems(),
						CreateInfo::GpuThread{
							ComputeSettings{ settings }
						},
						OUT gthread ) );
	thread->Send( ModuleMsg::AttachModule{ gthread });

	window->Subscribe( this, &SApp::_OnWindowClosed );
	gthread->Subscribe( this, &SApp::_Draw );
	gthread->Subscribe( this, &SApp::_GInit );
	gthread->Subscribe( this, &SApp::_GDelete );

	gpuThread = gthread;
	return true;
}

/*
=================================================
	_CreateSharedGPU
=================================================
*/
bool SApp::_CreateSharedGPU (GAPI::type api, StringCRef dev)
{
	CHECK_ERR( gpuThread );

	auto	factory	= ms->GlobalSystems()->modulesFactory;

	ComputeSettings	settings;
	settings.version	= api;
	settings.device		= dev;
	settings.isDebug	= true;
	
	ModulePtr	context;
	CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ settings }, OUT context ) );
	ms->Send( ModuleMsg::AttachModule{ context });

	GpuMsg::GetGraphicsModules	req_ids;
	context->Send( req_ids );
	sharedGpu.gpuIDs = *req_ids.compute;

	ModulePtr	gthread;
	CHECK_ERR( factory->Create(
						sharedGpu.gpuIDs.thread,
						ms->GlobalSystems(),
						CreateInfo::GpuThread{
							ComputeSettings{ settings }, context, gpuThread
						},
						OUT gthread ) );
	ms->GlobalSystems()->parallelThread->Send( ModuleMsg::AttachModule{ gthread });

	gthread->Subscribe( this, &SApp::_GInit2 );
	gthread->Subscribe( this, &SApp::_GDelete2 );

	sharedGpu.gpuThread = gthread;

	if ( ms->GetState() == Module::EState::ComposedMutable ) {
		ModuleUtils::Initialize({ context });
		ModuleUtils::Initialize({ gthread });
	}
	return true;
}

/*
=================================================
	Quit
=================================================
*/
void SApp::Quit ()
{
	looping = false;

	tests.Clear();

	if ( gpuThread )
		gpuThread->Send( ModuleMsg::Delete{} );
}

/*
=================================================
	Update
=================================================
*/
bool SApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send( ModuleMsg::Update{} );
	return true;
}

/*
=================================================
	_OnWindowClosed
=================================================
*/
bool SApp::_OnWindowClosed (const OSMsg::WindowAfterDestroy &)
{
	looping = false;
	return true;
}

/*
=================================================
	_Draw
=================================================
*/
bool SApp::_Draw (const ModuleMsg::Update &)
{
	if ( not looping or not sharedThreadInitialized )
		return false;

	if ( not tests.Empty() )
	{
		TestFunc_t	func	= tests.Front();
		bool		passed	= (this->*func)();

		testsPassed += uint(passed);
		testsFailed += uint(not passed);

		tests.PopFront();
	}
	else
	{
		Quit();

		LOG( "Tests passed: "_str << testsPassed << ", failed: " << testsFailed, ELog::Info );
		CHECK_FATAL( testsFailed == 0 );
	}

	return true;
}

/*
=================================================
	_GInit
=================================================
*/
bool SApp::_GInit (const GpuMsg::DeviceCreated &)
{
	syncManager = gpuThread->GetModuleByMsg<ModuleMsg::MessageListFrom< GpuMsg::CreateFence >>();
	CHECK_ERR( syncManager );
	
	GpuMsg::GetComputeSettings	req_settings;
	gpuThread->Send( req_settings );
	CHECK_ERR( _CreateSharedGPU( sharedGpu.api, req_settings.result->device ) );

	CHECK_ERR( ms->GlobalSystems()->modulesFactory->Create(
					gpuIDs.commandBuilder,
					ms->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{},
					OUT cmdBuilder )
	);
	ModuleUtils::Initialize({ cmdBuilder });
	return true;
}

/*
=================================================
	_GDelete
=================================================
*/
bool SApp::_GDelete (const GpuMsg::DeviceBeforeDestroy &)
{
	gpuThread	= null;
	syncManager	= null;
	cmdBuilder	= null;
	return true;
}

/*
=================================================
	_GInit2
=================================================
*/
bool SApp::_GInit2 (const GpuMsg::DeviceCreated &)
{
	sharedGpu.syncManager = sharedGpu.gpuThread->GetModuleByMsg<ModuleMsg::MessageListFrom< GpuMsg::CreateFence >>();
	CHECK_ERR( sharedGpu.syncManager );

	CHECK_ERR( ms->GlobalSystems()->modulesFactory->Create(
					sharedGpu.gpuIDs.commandBuilder,
					ms->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{},
					OUT sharedGpu.cmdBuilder )
	);
	ModuleUtils::Initialize({ sharedGpu.cmdBuilder });

	sharedThreadInitialized = true;
	return true;
}

/*
=================================================
	_GDelete2
=================================================
*/
bool SApp::_GDelete2 (const GpuMsg::DeviceBeforeDestroy &)
{
	sharedGpu.gpuThread		= null;
	sharedGpu.syncManager	= null;
	sharedGpu.cmdBuilder	= null;
	return true;
}
