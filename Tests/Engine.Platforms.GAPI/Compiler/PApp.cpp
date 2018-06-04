// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PApp.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"


/*
=================================================
	constructor
=================================================
*/
PApp::PApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();

	tests	<< &PApp::_Test_FindMSB
			<< &PApp::_Test_FindLSB
			//<< &PApp::_Test_BitfieldReverse
			<< &PApp::_Test_AtomicAdd
			<< &PApp::_Test_InlineAll
			<< &PApp::_Test_VecSwizzle
			<< &PApp::_Test_GlobalToLocal
			<< &PApp::_Test_UnnamedBuffer
		;
}

/*
=================================================
	destructor
=================================================
*/
PApp::~PApp ()
{
}

/*
=================================================
	Initialize
=================================================
*/
bool PApp::Initialize (GAPI::type api, StringCRef dev)
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
		ms->Send< ModuleMsg::AttachModule >({ context });

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		context->Send( req_ids );
		gpuIDs = *req_ids->compute;
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
	thread->Send< ModuleMsg::AttachModule >({ window });

	ModulePtr	gthread;
	CHECK_ERR( factory->Create(
						gpuIDs.thread,
						ms->GlobalSystems(), CreateInfo::GpuThread{
							ComputeSettings{ settings }
						},
						OUT gthread ) );
	thread->Send< ModuleMsg::AttachModule >({ gthread });

	window->Subscribe( this, &PApp::_OnWindowClosed );
	gthread->Subscribe( this, &PApp::_Draw );
	gthread->Subscribe( this, &PApp::_GInit );
	gthread->Subscribe( this, &PApp::_GDelete );

	// finish initialization
	ModuleUtils::Initialize({ ms });
	return true;
}

/*
=================================================
	Quit
=================================================
*/
void PApp::Quit ()
{
	looping = false;

	tests.Clear();

	if ( gpuThread )
		gpuThread->Send< ModuleMsg::Delete >({});
}

/*
=================================================
	Update
=================================================
*/
bool PApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send< ModuleMsg::Update >({});
	return true;
}

/*
=================================================
	_OnWindowClosed
=================================================
*/
bool PApp::_OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &)
{
	looping = false;
	return true;
}

/*
=================================================
	_Draw
=================================================
*/
bool PApp::_Draw (const Message< ModuleMsg::Update > &)
{
	if ( not looping )
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
bool PApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
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

/*
=================================================
	_GDelete
=================================================
*/
bool PApp::_GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	gpuThread	= null;
	syncManager	= null;
	cmdBuilder	= null;
	return true;
}
