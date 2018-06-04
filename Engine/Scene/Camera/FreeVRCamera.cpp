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
	// Free VR Camera
	//

	class FreeVRCamera final : public BaseSceneModule
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

		using VRThreadMsgList_t		= MessageListFrom<
											GpuMsg::ThreadBeginVRFrame,
											GpuMsg::ThreadEndVRFrame >;

		using CameraState_t			= SceneMsg::CameraGetState::State;
		using CameraStates_t		= StaticArray< CameraState_t, 2 >;

		using Frustum_t				= SceneMsg::CameraGetState::Frustum_t;
		using Transform_t			= SceneMsg::CameraGetState::Transform_t;
		using Mat4x4_t				= SceneMsg::CameraGetState::Mat4x4_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		PerspectiveCamera<float>	_camera;
		CameraSettings				_settings;
		
		CameraStates_t				_states;
		float3						_positionDelta;


	// methods
	public:
		FreeVRCamera (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::Camera &ci);
		~FreeVRCamera ();
		

	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _CameraBindKeys (const Message< SceneMsg::CameraBindKeys > &);
		bool _CameraGetState (const Message< SceneMsg::CameraGetState > &);
		bool _CameraUpdateSettings (const Message< SceneMsg::CameraUpdateSettings > &);
		bool _CameraGetSettings (const Message< SceneMsg::CameraGetSettings > &);

	private:
		bool _SurfaceOnResize (const Message< SceneMsg::SurfaceOnResize > &);
		bool _SurfaceRequestUpdate (const Message< SceneMsg::SurfaceRequestUpdate > &);

		void _OnKeyStepForward (const ModuleMsg::InputKey &);
		void _OnKeyStepBackward (const ModuleMsg::InputKey &);
		void _OnKeyStepLeft (const ModuleMsg::InputKey &);
		void _OnKeyStepRight (const ModuleMsg::InputKey &);
		void _OnKeyStepDown (const ModuleMsg::InputKey &);
		void _OnKeyStepUp (const ModuleMsg::InputKey &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	FreeVRCamera::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	FreeVRCamera::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	FreeVRCamera::FreeVRCamera (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Camera &ci) :
		BaseSceneModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_settings{ ci.settings }
	{
		SetDebugName( "Scene.FreeVRCamera" );

		_SubscribeOnMsg( this, &FreeVRCamera::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_AttachModule_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_DetachModule_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_FindModule_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_Link );
		_SubscribeOnMsg( this, &FreeVRCamera::_Compose_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_Delete_Impl );
		_SubscribeOnMsg( this, &FreeVRCamera::_Update );
		_SubscribeOnMsg( this, &FreeVRCamera::_CameraBindKeys );
		_SubscribeOnMsg( this, &FreeVRCamera::_CameraGetState );
		_SubscribeOnMsg( this, &FreeVRCamera::_CameraUpdateSettings );
		_SubscribeOnMsg( this, &FreeVRCamera::_CameraGetSettings );
		_SubscribeOnMsg( this, &FreeVRCamera::_OnManagerChanged );
		_SubscribeOnMsg( this, &FreeVRCamera::_GetScenePrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.scene, SceneModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	FreeVRCamera::~FreeVRCamera ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool FreeVRCamera::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		// subscribe to input events
		{
			ModulePtr	input;
			CHECK_LINKING( input = GlobalSystems()->parallelThread->GetModuleByMsg< InputThreadMsgList_t >() );

			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeVRCamera::_OnKeyStepForward,	"W"_KeyID,		EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeVRCamera::_OnKeyStepBackward,	"S"_KeyID,		EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeVRCamera::_OnKeyStepLeft,		"A"_KeyID,		EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeVRCamera::_OnKeyStepRight,		"D"_KeyID,		EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeVRCamera::_OnKeyStepDown,		"Ctrl"_KeyID,	EKeyState::OnKeyPressed });
			input->Send< ModuleMsg::InputKeyBind >({ this, &FreeVRCamera::_OnKeyStepUp,			"Space"_KeyID,	EKeyState::OnKeyPressed });
		}

		// subscribe to surface events
		{
			ModulePtr	surface;
			
			if ( not (surface = GetModuleByEvent< SurfaceEventList_t >()) )
			{
				CHECK_LINKING( surface = GetParentByEvent< SurfaceEventList_t >() );
			}

			surface->Subscribe( this, &FreeVRCamera::_SurfaceOnResize );
			surface->Subscribe( this, &FreeVRCamera::_SurfaceRequestUpdate );
		}
		
		_SendForEachAttachments( msg );

		CHECK( _SetState( EState::Linked ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterLink >({});
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool FreeVRCamera::_Update (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_positionDelta.Normalize() *= _settings.velocity * float(msg->timeDelta.Seconds());

		if ( IsNotZero( _positionDelta ) )
			_camera.MoveFPSFree( _positionDelta );


		_positionDelta	= Uninitialized;

		return Module::_Update_Impl( msg );
	}
	
/*
=================================================
	_SurfaceOnResize
=================================================
*/
	bool FreeVRCamera::_SurfaceOnResize (const Message< SceneMsg::SurfaceOnResize > &msg)
	{
		_camera.Resize( _camera.GetFovY(), float(msg->newSize.x) / float(msg->newSize.y) );
		return true;
	}
	
/*
=================================================
	_SurfaceRequestUpdate
=================================================
*/
	bool FreeVRCamera::_SurfaceRequestUpdate (const Message< SceneMsg::SurfaceRequestUpdate > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( msg->framebuffers.Count() == CameraStates_t::Count() );

		_states[0].frustum.Setup( msg->framebuffers[0].projMat * msg->framebuffers[0].viewMat );
		_states[0].viewMat = msg->framebuffers[0].viewMat;
		_states[0].projMat = msg->framebuffers[0].projMat;

		_states[1].frustum.Setup( msg->framebuffers[1].projMat * msg->framebuffers[1].viewMat );
		_states[1].viewMat = msg->framebuffers[1].viewMat;
		_states[1].projMat = msg->framebuffers[1].projMat;

		CHECK_ERR( _SendEvent< SceneMsg::CameraRequestUpdate >({ _states, msg->framebuffers, msg->cmdBuilder }) );

		return true;
	}

/*
=================================================
	_CameraBindKeys
=================================================
*/
	bool FreeVRCamera::_CameraBindKeys (const Message< SceneMsg::CameraBindKeys > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_CameraGetState
=================================================
*/
	bool FreeVRCamera::_CameraGetState (const Message< SceneMsg::CameraGetState > &msg)
	{
		msg->result.Set({ ArrayCRef<CameraState_t>( _states ) });
		return true;
	}
	
/*
=================================================
	_CameraUpdateSettings
=================================================
*/
	bool FreeVRCamera::_CameraUpdateSettings (const Message< SceneMsg::CameraUpdateSettings > &msg)
	{
		_settings = msg->settings;
		return true;
	}
	
/*
=================================================
	_CameraGetSettings
=================================================
*/
	bool FreeVRCamera::_CameraGetSettings (const Message< SceneMsg::CameraGetSettings > &msg)
	{
		msg->result.Set( _settings );
		return true;
	}

/*
=================================================
	_OnKey***
=================================================
*/
	void FreeVRCamera::_OnKeyStepForward (const ModuleMsg::InputKey &)
	{
		_positionDelta.x += 1.0f;
	}

	void FreeVRCamera::_OnKeyStepBackward (const ModuleMsg::InputKey &)
	{
		_positionDelta.x -= 1.0f;
	}

	void FreeVRCamera::_OnKeyStepLeft (const ModuleMsg::InputKey &)
	{
		_positionDelta.y -= 1.0f;
	}

	void FreeVRCamera::_OnKeyStepRight (const ModuleMsg::InputKey &)
	{
		_positionDelta.y += 1.0f;
	}

	void FreeVRCamera::_OnKeyStepDown (const ModuleMsg::InputKey &)
	{
		_positionDelta.z -= 1.0f;
	}

	void FreeVRCamera::_OnKeyStepUp (const ModuleMsg::InputKey &)
	{
		_positionDelta.z += 1.0f;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateFreeVRCamera
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateFreeVRCamera (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Camera &ci)
	{
		return New< FreeVRCamera >( id, gs, ci );
	}

}	// Scene
}	// Engine
