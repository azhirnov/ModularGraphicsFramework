// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Projects/ShaderEditor/ShaderEditorApp.h"

namespace ShaderEditor
{

/*
=================================================
	constructor
=================================================
*/
	ShaderEditorApp::ShaderEditorApp () :
		_renderer( GetMainSystemInstance()->GlobalSystems() ),
		_looping(true)
	{
		Platforms::RegisterPlatforms();
		Scene::RegisterScene();
	}
	
/*
=================================================
	Initialize
=================================================
*/
	bool ShaderEditorApp::Initialize (GAPI::type api)
	{
		CreateInfo::SceneManager scene_mngr_ci{};
		scene_mngr_ci.settings.version = api;
		scene_mngr_ci.settings.flags |= GraphicsSettings::EFlags::DebugContext;

		GetMainSystemInstance()->AddModule( SceneManagerModuleID, scene_mngr_ci );
	
		ModulePtr	scene_mngr	= GetMainSystemInstance()->GetModuleByID( SceneManagerModuleID );
		
		scene_mngr->Subscribe( this, &ShaderEditorApp::_OnWindowClosed );
		scene_mngr->Subscribe( this, &ShaderEditorApp::_Init );
		scene_mngr->Subscribe( this, &ShaderEditorApp::_GInit );

		// create SceneMain, SceneRenderer, ScenePhysics and SceneLogic here or it will be created automaticaly

		// finish initialization
		ModuleUtils::Initialize({ GetMainSystemInstance() });
		return true;
	}
	
/*
=================================================
	_OnWindowClosed
=================================================
*/
	bool ShaderEditorApp::_OnWindowClosed (const Message< OSMsg::WindowBeforeDestroy > &)
	{
		_looping = false;
		return true;
	}
	
/*
=================================================
	_Init
=================================================
*/
	bool ShaderEditorApp::_Init (const Message< ModuleMsg::Compose > &)
	{
		// create camera and surface
		{
			auto	gs = GetMainSystemInstance()->GlobalSystems();

			CHECK_ERR( gs->modulesFactory->Create(
								Scene::FreeCameraModuleID,
								gs,
								CreateInfo::Camera{},
								OUT _camera ) );

			CHECK_ERR( gs->modulesFactory->Create(
								Scene::WindowSurfaceModuleID,
								gs,
								CreateInfo::RenderSurface{},
								OUT _surface ) );

			_camera->Send< ModuleMsg::AttachModule >({ "surface", _surface });
			_camera->Subscribe( this, &ShaderEditorApp::_Draw );
			gs->parallelThread->Send< ModuleMsg::AttachModule >({ "surface", _surface });

			ModuleUtils::Initialize({ _camera });
		}
		return true;
	}
	
/*
=================================================
	_GInit
=================================================
*/
	bool ShaderEditorApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
	{
		// setup shader
		{
			CHECK_ERR( _renderer.Inititalize() );

			Renderer::ShaderDescr	sh_main;
			sh_main.Pipeline( Pipelines::Create_voronnoirecursion );
			CHECK_ERR( _renderer.Add( "main", sh_main ) );
		}
		return true;
	}

/*
=================================================
	_Draw
=================================================
*/
	bool ShaderEditorApp::_Draw (const Message< SceneMsg::CameraRequestUpdate > &msg)
	{
		CHECK_ERR( _renderer.Update( *msg ) );
		return true;
	}
	
/*
=================================================
	Quit
=================================================
*/
	void ShaderEditorApp::Quit ()
	{
		_looping = false;
	}
	
/*
=================================================
	Update
=================================================
*/
	bool ShaderEditorApp::Update ()
	{
		if ( not _looping )
			return false;

		GetMainSystemInstance()->Send< ModuleMsg::Update >({});
		return true;
	}

}	// ShaderEditor


/*
=================================================
	main
=================================================
*/
int main ()
{
	using namespace ShaderEditor;

	Logger::GetInstance()->Open( "log", false );
	CHECK( GetMainSystemInstance()->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Projects/ShaderEditor" ) );
	
	{
		ShaderEditorApp	app;
	
		app.Initialize( "VK 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	return 0;
}