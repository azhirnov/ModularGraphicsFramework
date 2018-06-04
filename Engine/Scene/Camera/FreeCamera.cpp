// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Public/Camera.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"
#include "Engine/STL/Math/3D/PerspectiveCamera.h"

namespace Engine
{
namespace Scene
{
	using namespace Engine::Platforms;


	//
	// Free Camera
	//

	class FreeCamera final : public BaseSceneModule
	{
	// types
	protected:
		using SupportedMessages_t	= BaseSceneModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::Update,
											SceneMsg::CameraBindKeys,
											SceneMsg::CameraUpdateSettings,
											SceneMsg::CameraGetSettings,
											SceneMsg::CameraGetState
										> >;

		using SupportedEvents_t		= BaseSceneModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::Update,
											SceneMsg::CameraRequestUpdate
										> >;
		
		using InputThreadMsgList_t	= MessageListFrom< 
											ModuleMsg::InputKeyBind,
											ModuleMsg::InputMotionBind,
											ModuleMsg::InputKeyUnbindAll,
											ModuleMsg::InputMotionUnbindAll >;

		using SurfaceEventList_t	= MessageListFrom<
											SceneMsg::SurfaceOnResize,
											SceneMsg::SurfaceRequestUpdate >;

		using CameraState_t			= SceneMsg::CameraGetState::State;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		PerspectiveCamera<float>	_camera;
		CameraSettings				_settings;

		float2						_mouseDelta;
		float3						_positionDelta;
		float						_zoom;
		bool						_mousePressed;


	// methods
	public:
		FreeCamera (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::Camera &ci);
		~FreeCamera ();
		

	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _CameraBindKeys (const Message< SceneMsg::CameraBindKeys > &);
		bool _CameraGetState (const Message< SceneMsg::CameraGetState > &);
		bool _CameraUpdateSettings (const Message< SceneMsg::CameraUpdateSettings > &);
		bool _CameraGetSettings (const Message< SceneMsg::CameraGetSettings > &);

	private:
		bool _SurfaceOnResize (const Message< SceneMsg::SurfaceOnResize > &);
		bool _SurfaceRequestUpdate (const Message< SceneMsg::SurfaceRequestUpdate > &);

		void _OnMouseX (const ModuleMsg::InputMotion &);
		void _OnMouseY (const ModuleMsg::InputMotion &);
		void _OnMouseWheel (const ModuleMsg::InputMotion &);
		void _OnMouseLBPressed (const ModuleMsg::InputKey &);

		void _OnKeyStepForward (const ModuleMsg::InputKey &);
		void _OnKeyStepBackward (const ModuleMsg::InputKey &);
		void _OnKeyStepLeft (const ModuleMsg::InputKey &);
		void _OnKeyStepRight (const ModuleMsg::InputKey &);
		void _OnKeyStepDown (const ModuleMsg::InputKey &);
		void _OnKeyStepUp (const ModuleMsg::InputKey &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	FreeCamera::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	FreeCamera::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	FreeCamera::FreeCamera (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Camera &ci) :
		BaseSceneModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_settings{ ci.settings },
		_mousePressed{ false }
	{
		SetDebugName( "Scene.FreeCamera" );

		_SubscribeOnMsg( this, &FreeCamera::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FreeCamera::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FreeCamera::_AttachModule_Impl );
		_SubscribeOnMsg( this, &FreeCamera::_DetachModule_Impl );
		_SubscribeOnMsg( this, &FreeCamera::_FindModule_Impl );
		_SubscribeOnMsg( this, &FreeCamera::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &FreeCamera::_Link );
		_SubscribeOnMsg( this, &FreeCamera::_Compose );
		_SubscribeOnMsg( this, &FreeCamera::_Delete_Impl );
		_SubscribeOnMsg( this, &FreeCamera::_Update );
		_SubscribeOnMsg( this, &FreeCamera::_CameraBindKeys );
		_SubscribeOnMsg( this, &FreeCamera::_CameraGetState );
		_SubscribeOnMsg( this, &FreeCamera::_CameraUpdateSettings );
		_SubscribeOnMsg( this, &FreeCamera::_CameraGetSettings );
		_SubscribeOnMsg( this, &FreeCamera::_OnManagerChanged );
		_SubscribeOnMsg( this, &FreeCamera::_GetScenePrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.scene, SceneModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	FreeCamera::~FreeCamera ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool FreeCamera::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		// subscribe to input events
		{
			ModulePtr	input;
			CHECK_LINKING( input = GlobalSystems()->parallelThread->GetModuleByMsg< InputThreadMsgList_t >() );

			input->Send< ModuleMsg::InputMotionBind >({ this, &FreeCamera::_OnMouseX,		"mouse.x"_MotionID });
			input->Send< ModuleMsg::InputMotionBind >({ this, &FreeCamera::_OnMouseY,		"mouse.y"_MotionID });
			input->Send< ModuleMsg::InputMotionBind >({ this, &FreeCamera::_OnMouseWheel,	"mouse.wheel"_MotionID });

			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnMouseLBPressed,	"mouse 0"_KeyID,	EKeyState::OnKeyDown });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnMouseLBPressed,	"mouse 0"_KeyID,	EKeyState::OnKeyUp });

			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnKeyStepForward,	"W"_KeyID,			EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnKeyStepBackward,	"S"_KeyID,			EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnKeyStepLeft,		"A"_KeyID,			EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnKeyStepRight,	"D"_KeyID,			EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnKeyStepDown,		"Ctrl"_KeyID,		EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeCamera::_OnKeyStepUp,		"Space"_KeyID,		EKeyState::OnKeyPressed });
		}

		// subscribe to surface events
		{
			ModulePtr	surface;
			
			if ( not (surface = GetModuleByEvent< SurfaceEventList_t >()) )
			{
				CHECK_LINKING( surface = GetParentByEvent< SurfaceEventList_t >() );
			}

			surface->Subscribe( this, &FreeCamera::_SurfaceOnResize );
			surface->Subscribe( this, &FreeCamera::_SurfaceRequestUpdate );
		}
		
		_SendForEachAttachments( msg );

		CHECK( _SetState( EState::Linked ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterLink >({});
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool FreeCamera::_Compose (const Message< ModuleMsg::Compose > &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		// create camera
		{
			ModulePtr	surface;
			
			if ( not (surface = GetModuleByEvent< SurfaceEventList_t >()) )
			{
				CHECK_COMPOSING( surface = GetParentByEvent< SurfaceEventList_t >() );
			}
			
			Message< SceneMsg::SurfaceGetDescriptor >	req_size;
			surface->Send( req_size );
			
			float	aspect = SafeDiv( float(req_size->result->size.x), float(req_size->result->size.y), 1.0f );

			_camera.Create( Uninitialized, _settings.fovY, aspect, _settings.clipPlanes );
		}

		return _DefCompose( false );
	}

/*
=================================================
	_Update
=================================================
*/
	bool FreeCamera::_Update (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_mouseDelta *= _settings.mouseSens;
		_zoom		*= _settings.zoomStep;

		_positionDelta.Normalize() *= _settings.velocity * float(msg->timeDelta.Seconds());
		

		if ( IsNotZero( _zoom ) )
		{
			_zoom += _camera.GetZoom();
			_camera.Increase( Clamp( _zoom, _settings.zoomLimits.x, _settings.zoomLimits.y ) );
		}

		if ( IsNotZero( _mouseDelta ) )
			_camera.RotateFPS( Deg2( _mouseDelta / _camera.GetZoom() ) );

		if ( IsNotZero( _positionDelta ) )
			_camera.MoveFPSFree( _positionDelta );


		_mouseDelta		= Uninitialized;
		_positionDelta	= Uninitialized;
		_zoom			= 0.0f;

		return Module::_Update_Impl( msg );
	}
	
/*
=================================================
	_SurfaceOnResize
=================================================
*/
	bool FreeCamera::_SurfaceOnResize (const Message< SceneMsg::SurfaceOnResize > &msg)
	{
		_camera.Resize( _camera.GetFovY(), SafeDiv( float(msg->newSize.x), float(msg->newSize.y), 1.0f ) );
		return true;
	}
	
/*
=================================================
	_SurfaceRequestUpdate
=================================================
*/
	bool FreeCamera::_SurfaceRequestUpdate (const Message< SceneMsg::SurfaceRequestUpdate > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( msg->framebuffers.Count() == 1 );

		CameraState_t	state{ _camera.GetFrustum(), _camera.Transform(), _camera.ViewMatrix(), _camera.ProjMatrix() };

		CHECK_ERR( _SendEvent< SceneMsg::CameraRequestUpdate >({ ArrayCRef<CameraState_t>{ state }, msg->framebuffers, msg->cmdBuilder }) );

		return true;
	}

/*
=================================================
	_CameraBindKeys
=================================================
*/
	bool FreeCamera::_CameraBindKeys (const Message< SceneMsg::CameraBindKeys > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_CameraGetState
=================================================
*/
	bool FreeCamera::_CameraGetState (const Message< SceneMsg::CameraGetState > &msg)
	{
		msg->result.Set({{ _camera.GetFrustum(), _camera.Transform(), _camera.ViewMatrix(), _camera.ProjMatrix() }});
		return true;
	}
	
/*
=================================================
	_CameraUpdateSettings
=================================================
*/
	bool FreeCamera::_CameraUpdateSettings (const Message< SceneMsg::CameraUpdateSettings > &msg)
	{
		_settings = msg->settings;
		return true;
	}
	
/*
=================================================
	_CameraGetSettings
=================================================
*/
	bool FreeCamera::_CameraGetSettings (const Message< SceneMsg::CameraGetSettings > &msg)
	{
		msg->result.Set( _settings );
		return true;
	}

/*
=================================================
	_OnMouse*
=================================================
*/
	void FreeCamera::_OnMouseX (const ModuleMsg::InputMotion &m)
	{
		if ( _mousePressed )
			_mouseDelta.x += m.relative;
	}

	void FreeCamera::_OnMouseY (const ModuleMsg::InputMotion &m)
	{
		if ( _mousePressed )
			_mouseDelta.y += m.relative;
	}
	
	void FreeCamera::_OnMouseWheel (const ModuleMsg::InputMotion &m)
	{
		_zoom += m.relative;
	}
	
	void FreeCamera::_OnMouseLBPressed (const ModuleMsg::InputKey &k)
	{
		_mousePressed = k.IsDown();
	}

/*
=================================================
	_OnKey***
=================================================
*/
	void FreeCamera::_OnKeyStepForward (const ModuleMsg::InputKey &)
	{
		_positionDelta.x += 1.0f;
	}

	void FreeCamera::_OnKeyStepBackward (const ModuleMsg::InputKey &)
	{
		_positionDelta.x -= 1.0f;
	}

	void FreeCamera::_OnKeyStepLeft (const ModuleMsg::InputKey &)
	{
		_positionDelta.y -= 1.0f;
	}

	void FreeCamera::_OnKeyStepRight (const ModuleMsg::InputKey &)
	{
		_positionDelta.y += 1.0f;
	}

	void FreeCamera::_OnKeyStepDown (const ModuleMsg::InputKey &)
	{
		_positionDelta.z -= 1.0f;
	}

	void FreeCamera::_OnKeyStepUp (const ModuleMsg::InputKey &)
	{
		_positionDelta.z += 1.0f;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateFreeCamera
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateFreeCamera (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Camera &ci)
	{
		return New< FreeCamera >( id, gs, ci );
	}

}	// Scene
}	// Engine
