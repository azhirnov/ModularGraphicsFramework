// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Common.h"


class WindowApp final : public StaticRefCountedObject
{
// variables
private:
	ModulePtr	taskMngr;
	String		name;
	bool		looping		= true;


// methods
public:
	WindowApp (const ModulePtr &taskMngr, StringCRef name) :
		taskMngr( taskMngr ), name( name )
	{}

	void Initialize (const ModulePtr &thread)
	{
		if ( not thread->GlobalSystems()->Get< TaskModule >() )
			thread->AddModule( TaskModule::GetStaticID(), CreateInfo::TaskModule{ taskMngr } );

		thread->AddModule( "win.window"_TModID, CreateInfo::Window{ name } );
		thread->AddModule( "input.thrd"_TModID, CreateInfo::InputThread() );

		auto	window	= thread->GetModule( "win.window"_TModID );
		auto	input	= thread->GetModule( "input.thrd"_TModID );
	
		window->AddModule( "win.keys"_OModID, CreateInfo::RawInputHandler() );
		window->AddModule( "win.mouse"_OModID, CreateInfo::RawInputHandler() );

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
		GetMainSystemInstace()->Send( Message< ModuleMsg::Update >() );
		return looping;
	}
	
private:
	bool _OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &)
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
	auto ms = GetMainSystemInstace();

	Platforms::RegisterPlatforms();
	
	CHECK( ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );
	
	ms->AddModule( "win.platform"_GModID, CreateInfo::Platform() );
	ms->AddModule( "input.mngr"_GModID, CreateInfo::InputManager() );
	ms->AddModule( "stream.mngr"_GModID, CreateInfo::StreamManager() );
	
	{
		auto	thread		= ms->GlobalSystems()->Get< ParallelThread >();
		auto	task_mngr	= ms->GetModule( "task.mngr"_GModID );

		WindowAppPtr	app1 = New< WindowApp >( task_mngr, "window-0" );
		WindowAppPtr	app2 = New< WindowApp >( task_mngr, "window-1" );

		app1->Initialize( ModulePtr( thread ) );
		
		// create second thread with window
		CHECK( ms->GlobalSystems()->Get< ModulesFactory >()->Create(
					ParallelThread::GetStaticID(),
					ms->GlobalSystems(),
					CreateInfo::Thread{
						"SecondThread",
						null,
						LAMBDA( &app2 ) (const ModulePtr &thread)
						{
							app2->Initialize( thread );
						}
					},
					OUT (ModulePtr &) thread ) );
		
		thread = null;

		// finish initialization
		ms->Send( Message< ModuleMsg::Link >() );
		ms->Send( Message< ModuleMsg::Compose >() );

		// main loop
		for (; app1->Update();) {}

		app1->Quit();
	}

	ms->Send( Message< ModuleMsg::Delete >() );

	WARNING( "Window test succeeded!" );
}
