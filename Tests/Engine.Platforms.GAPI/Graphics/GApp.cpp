// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"


GApp::GApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();
	
	tests	<< &GApp::_Test_Texture2DNearestFilter
			<< &GApp::_Test_Texture2DBilinearFilter
		;
}


bool GApp::Initialize (GAPI::type api, StringCRef device)
{
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	OSModuleIDs	os_ids;
	{
		ModulePtr	platform;
		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::Platform{}, OUT platform ) );
		ms->Send< ModuleMsg::AttachModule >({ platform });

		Message< OSMsg::GetOSModules >	req_ids;
		platform->Send( req_ids );
		os_ids = *req_ids->result;
	}

	{
		ModulePtr	context;
		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ api }, OUT context ) );
		ms->Send< ModuleMsg::AttachModule >({ context });

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		context->Send( req_ids );
		gpuIDs = *req_ids->graphics;
		computeIDs = *req_ids->compute;
	}

	auto		thread	= ms->GlobalSystems()->parallelThread;
	
	ModulePtr	window;
	factory->Create( 0, ms->GlobalSystems(), CreateInfo::Window{}, OUT window );
	thread->Send< ModuleMsg::AttachModule >({ window });

	thread->AddModule( gpuIDs.thread,
						CreateInfo::GpuThread{
							GraphicsSettings{
								api,
								CreateInfo::GpuContext::EFlags::DebugContext
							} }
					 );

	auto	gthread		= thread->GetModuleByID( gpuIDs.thread );

	window->Subscribe( this, &GApp::_OnWindowClosed );
	gthread->Subscribe( this, &GApp::_Draw );
	gthread->Subscribe( this, &GApp::_GInit );
	gthread->Subscribe( this, &GApp::_GDelete );

	// finish initialization
	ModuleUtils::Initialize({ ms });
	return true;
}


void GApp::Quit ()
{
	looping = false;
	
	tests.Clear();
}


bool GApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send< ModuleMsg::Update >({});
	return true;
}


bool GApp::_OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &)
{
	looping = false;
	return true;
}


bool GApp::_Draw (const Message< ModuleMsg::Update > &)
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


bool GApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
{
	gpuThread = PlatformTools::GPUThreadHelper::FindGraphicsThread( ms->GlobalSystems() );
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


bool GApp::_GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	gpuThread	= null;
	syncManager	= null;
	return true;
}
