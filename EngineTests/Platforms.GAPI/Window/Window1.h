// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Common.h"
#include "Core/STL/ThreadSafe/AtomicBitfield.h"


class WindowApp1 final : public StaticRefCountedObject
{
// types
public:
	using EVisibility	= CreateInfo::Window::EVisibility;
	using TestCounter	= AtomicBitfield<2>;


// variables
private:
	TModID::type		windowID	= Uninitialized;
	CreateInfo::Window	wndDescr;
	ModulePtr			taskMngr;
	String				name;
	bool				looping		= true;
	TestCounter&		testPassed;
	const usize			ID;


// methods
public:
	WindowApp1 (usize id, const ModulePtr &taskMngr, TModID::type wndID, const CreateInfo::Window &descr, TestCounter& cnt) :
		windowID{ wndID }, wndDescr{ descr }, taskMngr{ taskMngr }, testPassed{ cnt }, ID{ id }
	{}

	void Initialize (GlobalSystemsRef gs)
	{
		auto	thread = gs->parallelThread;

		if ( not gs->taskModule )
			thread->AddModule( TaskModuleModuleID, CreateInfo::TaskModule{ taskMngr } );

		thread->AddModule( windowID, wndDescr );
		thread->AddModule( InputThreadModuleID, CreateInfo::InputThread() );

		auto	window	= thread->GetModuleByID( windowID );

		window->Subscribe( this, &WindowApp1::_OnWindowClosed );
		window->Subscribe( this, &WindowApp1::_OnWindowVisibilityChanged );
	}

	void Quit ()
	{
	}

	// only for main thread
	bool Update ()
	{
		GetMainSystemInstance()->Send( ModuleMsg::Update{} );
		return looping and testPassed.Load() != (1|2);
	}
	
private:
	bool _OnWindowClosed (const OSMsg::WindowAfterDestroy &)
	{
		looping = false;
		return true;
	}

	bool _OnWindowVisibilityChanged (const OSMsg::WindowVisibilityChanged &msg)
	{
		if ( msg.state == EVisibility::VisibleFocused )
		{
			testPassed.Set( ID );
		}
		return true;
	}
};


static void Test_WindowApp1 ()
{
	SHARED_POINTER( WindowApp1 );
	using EFlags		= CreateInfo::Window::EWindowFlags;
	using TestCounter	= WindowApp1::TestCounter;

	auto		ms	= GetMainSystemInstance();
	auto		mf	= ms->GlobalSystems()->modulesFactory;
	TestCounter	tests_passed{ 0 };

	Platforms::RegisterPlatforms();
	
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "EngineTests/Base" ) );
	
	ModulePtr	platform;
	CHECK( mf->Create( 0, ms->GlobalSystems(), CreateInfo::Platform{}, OUT platform ) );
	ms->Send( ModuleMsg::AttachModule{ platform });

	OSMsg::GetOSModules	req_ids;
	CHECK( platform->Send( req_ids ) );

	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager() );
	ms->AddModule( DataProviderManagerModuleID, CreateInfo::DataProviderManager() );
	
	{
		auto		thread		= ms->GlobalSystems()->parallelThread;
		auto		task_mngr	= ms->GetModuleByID( TaskManagerModuleID );
		ModulePtr	thread2;

		WindowApp1Ptr	app1 = New< WindowApp1 >( 0, task_mngr,
												  req_ids.result->window,
												  CreateInfo::Window{ "window-0", EFlags::Resizable, uint2(800,600), int2(800,600) },
												  tests_passed );

		WindowApp1Ptr	app2 = New< WindowApp1 >( 1, task_mngr,
												  req_ids.result->window,
												  CreateInfo::Window{ "window-1", EFlags::bits(), uint2(800,600), int2(300,400) },
												  tests_passed );

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
					OUT (ModulePtr &)(thread2) ) );

		thread2 = null;

		// finish initialization
		ModuleUtils::Initialize({ ms });

		// main loop
		for (; app1->Update();) {}

		app1->Quit();
	}

	ms->Send( ModuleMsg::Delete{} );

	CHECK_FATAL( tests_passed.Load() == (1|2) );
}
