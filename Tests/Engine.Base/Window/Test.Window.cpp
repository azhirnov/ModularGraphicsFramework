// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Common.h"


class WindowApp final : public StaticRefCountedObject
{
// variables
private:
	TModID::type		windowID	= Uninitialized;
	CreateInfo::Window	wndDescr;
	ModulePtr			taskMngr;
	String				name;
	bool				looping		= true;


// methods
public:
	WindowApp (const ModulePtr &taskMngr, TModID::type wndID, const CreateInfo::Window &descr) :
		windowID( wndID ), wndDescr( descr ), taskMngr( taskMngr )
	{}

	void Initialize (GlobalSystemsRef gs)
	{
		auto	thread = gs->parallelThread;

		if ( not gs->taskModule )
			thread->AddModule( TaskModuleModuleID, CreateInfo::TaskModule{ taskMngr } );

		thread->AddModule( windowID, wndDescr );
		thread->AddModule( InputThreadModuleID, CreateInfo::InputThread() );

		auto	window	= thread->GetModuleByID( windowID );
		auto	input	= thread->GetModuleByID( InputThreadModuleID );
	
		//window->AddModule( WinKeyInputModuleID, CreateInfo::RawInputHandler() );
		//window->AddModule( WinMouseInputModuleID, CreateInfo::RawInputHandler() );

		window->Subscribe( this, &WindowApp::_OnWindowClosed );
		window->Subscribe( this, &WindowApp::_OnWindowUpdate );
		input->Subscribe( this, &WindowApp::_OnKey );
	}

	void Quit ()
	{
	}

	// only for main thread
	bool Update ()
	{
		GetMainSystemInstance()->Send< ModuleMsg::Update >({});
		return looping;
	}
	
private:
	bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &)
	{
		looping = false;
		return true;
	}

	bool _OnWindowUpdate (const Message<ModuleMsg::Update> &)
	{
		return true;
	}

	bool _OnKey (const Message< ModuleMsg::InputKey > &)
	{
		return true;
	}
};

SHARED_POINTER( WindowApp );


extern void Test_Window ()
{
	using EFlags = CreateInfo::Window::EWindowFlags;

	auto	ms = GetMainSystemInstance();
	auto	mf = ms->GlobalSystems()->modulesFactory;

	Platforms::RegisterPlatforms();
	
	CHECK( ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Base" ) );
	
	ModulePtr	platform;
	CHECK( mf->Create( 0, ms->GlobalSystems(), CreateInfo::Platform{}, OUT platform ) );
	ms->Send< ModuleMsg::AttachModule >({ platform });

	Message< OSMsg::GetOSModules >	req_ids;
	CHECK( platform->Send( req_ids ) );

	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager() );
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager() );
	
	{
		auto		thread		= ms->GlobalSystems()->parallelThread;
		auto		task_mngr	= ms->GetModuleByID( TaskManagerModuleID );
		ModulePtr	thread2;

		WindowAppPtr	app1 = New< WindowApp >( task_mngr, req_ids->result->window, CreateInfo::Window{ "window-0", EFlags::bits() | EFlags::Resizable, uint2(800,600), int2(800,600) } );
		WindowAppPtr	app2 = New< WindowApp >( task_mngr, req_ids->result->window, CreateInfo::Window{ "window-1", EFlags::bits(), uint2(800,600), int2(-900,400) } );

		app1->Initialize( thread->GlobalSystems() );
		
		// create second thread with window
		CHECK( ms->GlobalSystems()->modulesFactory->Create(
					ParallelThreadModuleID,
					ms->GlobalSystems(),
					CreateInfo::Thread{
						"SecondThread",
						null,
						LAMBDA( app2 ) (GlobalSystemsRef gs)
						{
							app2->Initialize( gs );
						}
					},
					OUT (ModulePtr &) thread2 ) );

		thread2 = null;

		// finish initialization
		ModuleUtils::Initialize({ ms });

		// main loop
		for (; app1->Update();) {}

		app1->Quit();
	}

	ms->Send< ModuleMsg::Delete >({});

	WARNING( "Window test succeeded!" );
}
