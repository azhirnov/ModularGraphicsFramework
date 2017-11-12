// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Common.h"


class WindowApp final : public StaticRefCountedObject
{
// variables
private:
	CreateInfo::Window	wndDescr;
	ModulePtr			taskMngr;
	String				name;
	bool				looping		= true;


// methods
public:
	WindowApp (const ModulePtr &taskMngr, const CreateInfo::Window &descr) :
		taskMngr( taskMngr ), wndDescr( descr )
	{}

	void Initialize (GlobalSystemsRef gs)
	{
		ModulePtr	thread = gs->Get< ParallelThread >();

		if ( not gs->Get< TaskModule >() )
			thread->AddModule( TaskModuleModuleID, CreateInfo::TaskModule{ taskMngr } );

		thread->AddModule( WinWindowModuleID, wndDescr );
		thread->AddModule( InputThreadModuleID, CreateInfo::InputThread() );

		auto	window	= thread->GetModuleByID( WinWindowModuleID );
		auto	input	= thread->GetModuleByID( InputThreadModuleID );
	
		window->AddModule( WinKeyInputModuleID, CreateInfo::RawInputHandler() );
		window->AddModule( WinMouseInputModuleID, CreateInfo::RawInputHandler() );

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
		GetMainSystemInstace()->Send< ModuleMsg::Update >({});
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

	auto ms = GetMainSystemInstace();

	Platforms::RegisterPlatforms();
	
	CHECK( ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );
	
	ms->AddModule( WinPlatformModuleID, CreateInfo::Platform() );
	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager() );
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager() );
	
	{
		auto	thread		= ms->GlobalSystems()->Get< ParallelThread >();
		auto	task_mngr	= ms->GetModuleByID( TaskManagerModuleID );

		WindowAppPtr	app1 = New< WindowApp >( task_mngr, CreateInfo::Window{ "window-0", EFlags::bits() | EFlags::Resizable, uint2(800,600), int2(800,600) } );
		WindowAppPtr	app2 = New< WindowApp >( task_mngr, CreateInfo::Window{ "window-1", EFlags::bits(), uint2(800,600), int2(-900,400) } );

		app1->Initialize( thread->GlobalSystems() );
		
		// create second thread with window
		CHECK( ms->GlobalSystems()->Get< ModulesFactory >()->Create(
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
					OUT (ModulePtr &) thread ) );
		
		thread = null;

		// finish initialization
		ModuleUtils::Initialize({ ms });

		// main loop
		for (; app1->Update();) {}

		app1->Quit();
	}

	ms->Send< ModuleMsg::Delete >({});

	WARNING( "Window test succeeded!" );
}
