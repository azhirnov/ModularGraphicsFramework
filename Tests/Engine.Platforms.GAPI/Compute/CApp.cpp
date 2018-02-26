// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "Engine/Platforms/Shared/Tools/GPUThreadHelper.h"


CApp::CApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();

	tests	<< &CApp::_Test_CopyBuffer
			<< &CApp::_Test_ClearBuffer
			<< &CApp::_Test_UpdateBuffer
			<< &CApp::_Test_BufferAlign
			//<< &CApp::_Test_CopyImage2D
			//<< &CApp::_Test_CopyImage3D
			//<< &CApp::_Test_PushConstants
		;
}


CApp::~CApp ()
{
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
		gpuIDs = *req_ids->compute;
	}

	auto		thread	= ms->GlobalSystems()->parallelThread;
	
	ModulePtr	window;
	CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::Window{}, OUT window ) );
	thread->Send< ModuleMsg::AttachModule >({ window });

	ModulePtr	gthread;
	CHECK_ERR( factory->Create(
						gpuIDs.thread,
						ms->GlobalSystems(), CreateInfo::GpuThread{
							GraphicsSettings{
								api,
								CreateInfo::GpuContext::EFlags::bits() | CreateInfo::GpuContext::EFlags::DebugContext
							} },
						OUT gthread ) );
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
	looping = false;

	tests.Clear();

	if ( gpuThread )
		gpuThread->Send< ModuleMsg::Delete >({});
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

	if ( not tests.Empty() )
	{
		TestFunc_t	func = tests.Front();

		CHECK( (this->*func)() );

		tests.PopFront();
	}
	else
		Quit();

	return true;
}


bool CApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
{
	gpuThread = PlatformTools::GPUThreadHelper::FindComputeThread( ms->GlobalSystems() );
	CHECK_ERR( gpuThread );

	syncManager = gpuThread->GetModuleByMsg<CompileTime::TypeListFrom< Message<GpuMsg::CreateFence> >>();
	CHECK_ERR( syncManager );

	CHECK_ERR( ms->GlobalSystems()->modulesFactory->Create(
					gpuIDs.commandBuilder,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{},
					OUT cmdBuilder )
	);
	ModuleUtils::Initialize({ cmdBuilder });
	return true;
}


bool CApp::_GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	gpuThread	= null;
	syncManager	= null;
	return true;
}
