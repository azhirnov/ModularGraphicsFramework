// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CodeGenApp.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"

//extern bool Test_Bruteforce ();

namespace CodeGen
{
	extern bool Test_BigInt ();
	extern void Test_BigInteger ();

/*
=================================================
	constructor
=================================================
*/
	CodeGenApp::CodeGenApp ()
	{
		Platforms::RegisterPlatforms();
	}
	
/*
=================================================
	destructor
=================================================
*/
	CodeGenApp::~CodeGenApp ()
	{
	}

/*
=================================================
	Initialize
=================================================
*/
	bool CodeGenApp::Initialize (GAPI::type api)
	{
		auto	ms		= GetMainSystemInstance();
		auto	factory	= ms->GlobalSystems()->modulesFactory;

		ms->AddModule( 0, CreateInfo::Platform{} );

		ComputeModuleIDs	gpu_ids;
		{
			ModulePtr	context;
			CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ api }, OUT context ) );
			ms->Send< ModuleMsg::AttachModule >({ context });

			Message< GpuMsg::GetGraphicsModules >	req_ids;
			context->Send( req_ids );
			gpu_ids = *req_ids->compute;
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
							gpu_ids.thread,
							ms->GlobalSystems(), CreateInfo::GpuThread{
								ComputeSettings{ api, "", bool("debug") }
							},
							OUT gthread ) );
		thread->Send< ModuleMsg::AttachModule >({ gthread });

		window->Subscribe( this, &CodeGenApp::_OnWindowClosed );
		gthread->Subscribe( this, &CodeGenApp::_Draw );
		gthread->Subscribe( this, &CodeGenApp::_GInit );
		gthread->Subscribe( this, &CodeGenApp::_GDelete );

		// finish initialization
		ModuleUtils::Initialize({ ms });
		return true;
	}

/*
=================================================
	Quit
=================================================
*/
	void CodeGenApp::Quit ()
	{
		_looping = false;

		if ( _gpuThread )
			_gpuThread->Send< ModuleMsg::Delete >({});
	}
	
/*
=================================================
	Update
=================================================
*/
	bool CodeGenApp::Update ()
	{
		if ( not _looping )
			return false;

		GetMainSystemInstance()->Send< ModuleMsg::Update >({});
		return true;
	}
	
/*
=================================================
	_OnWindowClosed
=================================================
*/
	bool CodeGenApp::_OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &)
	{
		_looping = false;
		return true;
	}
	
/*
=================================================
	_Draw
=================================================
*/
	bool CodeGenApp::_Draw (const Message< ModuleMsg::Update > &)
	{
		if ( not _looping )
			return false;

		if ( not _cmdQueue.Empty() )
		{
			_cmdQueue.Front()();
			_cmdQueue.PopFront();
		}
		return true;
	}
	
/*
=================================================
	_GInit
=================================================
*/
	bool CodeGenApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
	{
		_gpuThread = PlatformTools::GPUThreadHelper::FindComputeThread( GetMainSystemInstance()->GlobalSystems() );
		CHECK_ERR( _gpuThread );

		_RunTests();
		_BindToScript();
		return true;
	}

/*
=================================================
	_GDelete
=================================================
*/
	bool CodeGenApp::_GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_gpuThread	= null;
		return true;
	}
	
/*
=================================================
	_RunTests
=================================================
*/
	void CodeGenApp::_RunTests ()
	{
		Test_BigInteger();
		Test_BigInt();
		//Run( LAMBDA() () { Test_Bruteforce(); } );
	}
	
/*
=================================================
	Run
=================================================
*/
	void CodeGenApp::Run (Function<void()> &&func)
	{
		_cmdQueue.PushBack( RVREF(func) );
	}
	
/*
=================================================
	RunScript
=================================================
*/
	bool CodeGenApp::RunScript (StringCRef fname)
	{
		using namespace GX_STL::GXScript;
		
		LOG( "Run script: '"_str << fname << "'", ELog::Debug );

		// load from file
		File::RFilePtr	file = File::HddRFile::New( fname );
		CHECK_ERR( file );

		const usize	len		= usize(file->RemainingSize());
		String		buf;	buf.Resize( len );

		CHECK_ERR( file->Read( buf.ptr(), buf.LengthInBytes() ) );


		// run script
		ScriptModule	mod { &_scriptEngine };

		CHECK_ERR( mod.Create( fname ) );
		CHECK_ERR( mod.Run( buf, "main" ) );

		return true;
	}

/*
=================================================
	Exit
=================================================
*/
	void CodeGenApp::Exit ()
	{
		_looping = false;
	}

}	// CodeGen


/*
=================================================
	main
=================================================
*/
int main (int argc, char** argv)
{
	using namespace CodeGen;

	CHECK_ERR( argv > 0 );

	Logger::GetInstance()->Open( "log", false );
	CHECK( GetMainSystemInstance()->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Projects/CodeGen" ) );
	
	// find input scripts
	Array<String>	script_files;
	String			api = "CL 1.2";

	for (int i = 1; i < argc; ++i)
	{
		StringCRef	key = argv[i];
		StringCRef	value;

		if ( ++i < argc )
			value = argv[i];

		if ( key.EqualsIC( "-g" ) )
		{
			script_files << value;
		}
		else
		if ( key.EqualsIC( "-api" ) )
		{
			api = value;
		}
		else
		{
			RETURN_ERR( "unsupported command arg: " << key, 1 );
		}
	}

	// run application
	{
		CodeGenApp	app;
		//app.Initialize( "VK 1.0"_GAPI );
		//app.Initialize( "GL 4.5"_GAPI );
		//app.Initialize( "CL 1.2"_GAPI );
		//app.Initialize( "SW 1.0"_GAPI );
		app.Initialize( GAPI::FromString( api ) );

		app.Run( LAMBDA( &script_files, &app ) ()
				{
					FOR( i, script_files ) {
						app.RunScript( script_files[i] );
					}
					app.Exit();
				} );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	return 0;
}
