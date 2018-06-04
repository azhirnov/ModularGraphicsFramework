// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Projects/ShaderEditor/ShaderEditorApp.h"
#include "Engine/Profilers/Engine.Profilers.h"
#include "Engine/ImportExport/Engine.ImportExport.h"

namespace ShaderEditor
{

/*
=================================================
	constructor
=================================================
*/
	ShaderEditorApp::ShaderEditorApp () :
		_renderer( GetMainSystemInstance()->GlobalSystems() ),
		_currSample{ UMax },	_nextSample{ 0 },
		_looping{ true },
		_vrMode{ false }
	{
		Platforms::RegisterPlatforms();
		Profilers::RegisterProfilers();
		Graphics::RegisterGraphics();
		Scene::RegisterScene();
		ImportExport::RegisterImportExport();
	}
	
/*
=================================================
	Initialize
=================================================
*/
	bool ShaderEditorApp::Initialize (GAPI::type api)
	{
		CreateInfo::SceneManager scene_mngr_ci{};
		scene_mngr_ci.settings.version	 = api;
		scene_mngr_ci.settings.flags	|= GraphicsSettings::EFlags::DebugContext;

		scene_mngr_ci.vrSettings.enabled				= _vrMode;
		scene_mngr_ci.vrSettings.eyeTextureDimension	= uint2(1024);

		GetMainSystemInstance()->AddModule( SceneManagerModuleID, scene_mngr_ci );
	
		ModulePtr	scene_mngr	= GetMainSystemInstance()->GetModuleByID( SceneManagerModuleID );
		
		scene_mngr->Subscribe( this, &ShaderEditorApp::_OnWindowClosed );
		scene_mngr->Subscribe( this, &ShaderEditorApp::_Init );
		scene_mngr->Subscribe( this, &ShaderEditorApp::_GInit );

		// create SceneMain, SceneRenderer, ScenePhysics and SceneLogic here or they will be created automaticaly

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
			auto		gs			= GetMainSystemInstance()->GlobalSystems();
			const auto	camera_id	= _vrMode ? Scene::FreeVRCameraModuleID : Scene::FreeCameraModuleID;
			const auto	surface_id	= _vrMode ? Scene::VRSurfaceModuleID : Scene::WindowSurfaceModuleID;

			CameraSettings	settings;
			settings.mouseSens = float2(-1.0f, 1.0f);
			settings.fovY = Rad( 90.0_deg );

			CHECK_ERR( gs->modulesFactory->Create(
								camera_id,
								gs,
								CreateInfo::Camera{ null, settings },
								OUT _camera ) );

			CHECK_ERR( gs->modulesFactory->Create(
								surface_id,
								gs,
								CreateInfo::RenderSurface{},
								OUT _surface ) );

			_camera->Send< ModuleMsg::AttachModule >({ "surface", _surface });
			_camera->Subscribe( this, &ShaderEditorApp::_Draw );
			gs->parallelThread->Send< ModuleMsg::AttachModule >({ "surface", _surface });
			gs->parallelThread->Send< ModuleMsg::AttachModule >({ "camera",  _camera });

			ModuleUtils::Initialize({ _camera });
		}

		// subscribe on input
		{
			ModulePtr	input = GetMainSystemInstance()->GlobalSystems()->parallelThread->GetModuleByID( InputThreadModuleID );
			CHECK_ERR( input );

			input->Send< ModuleMsg::InputKeyBind >({ this, &ShaderEditorApp::_OnKey, "["_KeyID, EKeyState::OnKeyDown });
			input->Send< ModuleMsg::InputKeyBind >({ this, &ShaderEditorApp::_OnKey, "]"_KeyID, EKeyState::OnKeyDown });
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

			_InitSamples();
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
		// select sample
		if ( _currSample != _nextSample )
		{
			_nextSample = Wrap( _nextSample, 0u, _samples.LastIndex() );
			_currSample = _nextSample;

			_renderer.Reset();
			_samples[_currSample].Call( _renderer );
		}

		CHECK_ERR( _renderer.Update( *msg ) );
		return true;
	}
	
/*
=================================================
	_OnKey
=================================================
*/
	void ShaderEditorApp::_OnKey (const ModuleMsg::InputKey &msg)
	{
		switch ( msg.key )
		{
			case "["_KeyID :	--_nextSample;	break;
			case "]"_KeyID :	++_nextSample;	break;
		}
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
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Projects/ShaderEditor" ) );
	
	{
		ShaderEditorApp	app;
	
		app.Initialize( "GL 4.5"_GAPI );
		//app.Initialize( "VK 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	return 0;
}
