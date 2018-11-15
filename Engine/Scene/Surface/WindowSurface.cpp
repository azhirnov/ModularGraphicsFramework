// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Public/Surface.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"
#include "Engine/Platforms/Public/Tools/WindowHelper.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"

namespace Engine
{
namespace Scene
{
	using namespace Engine::Platforms;


	//
	// Window Surface
	//

	class WindowSurface final : public BaseSceneModule
	{
	// types
	protected:
		using SupportedMessages_t	= BaseSceneModule::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >::Append< MessageListFrom<
											SceneMsg::SurfaceGetDescription,
											ModuleMsg::Update
										> >;

		using SupportedEvents_t		= BaseSceneModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::Update,
											SceneMsg::SurfaceOnResize,
											SceneMsg::SurfaceRequestUpdate
										> >;
		
		using CameraEventList_t		= MessageListFrom< SceneMsg::SurfaceRequestUpdate >;

		using CmdBufferMngrMsgList_t = MessageListFrom<
											GraphicsMsg::CmdBeginFrame,
											GraphicsMsg::CmdEndFrame,
											GraphicsMsg::CmdBegin,
											GraphicsMsg::CmdEnd >;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr		_thread;
		ModulePtr		_builder;

		uint2			_size;


	// methods
	public:
		WindowSurface (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci);
		~WindowSurface ();
		

	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _Update (const ModuleMsg::Update &);
		bool _SurfaceGetDescription (const SceneMsg::SurfaceGetDescription &);
		
		// event handlers
		bool _WindowDescriptionChanged (const OSMsg::WindowDescriptionChanged &);
		bool _DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _AfterCompose (const ModuleMsg::AfterCompose &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	WindowSurface::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	WindowSurface::WindowSurface (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci) :
		BaseSceneModule( gs, ModuleConfig{ id, UMax }, &_eventTypes )
	{
		SetDebugName( "Scene.WindowSurface" );

		_SubscribeOnMsg( this, &WindowSurface::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_FindModule_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_Link );
		_SubscribeOnMsg( this, &WindowSurface::_Delete );
		_SubscribeOnMsg( this, &WindowSurface::_Update );
		_SubscribeOnMsg( this, &WindowSurface::_SurfaceGetDescription );
		_SubscribeOnMsg( this, &WindowSurface::_OnManagerChanged );
		_SubscribeOnMsg( this, &WindowSurface::_GetScenePrivateClasses );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( ci.scene, SceneRendererModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	WindowSurface::~WindowSurface ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool WindowSurface::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );
		CHECK_ERR( _GetManager() );

		_SendForEachAttachments( msg );
		
		ModulePtr	window;
		CHECK_ATTACHMENT( window = PlatformTools::WindowHelper::FindWindow( GlobalSystems() ) );
		CHECK_ATTACHMENT( _thread = PlatformTools::GPUThreadHelper::FindGraphicsThread( GlobalSystems() ) );
		
		window->Subscribe( this, &WindowSurface::_WindowDescriptionChanged );
		_thread->Subscribe( this, &WindowSurface::_DeviceBeforeDestroy );
		_GetManager()->Subscribe( this, &WindowSurface::_AfterCompose );

		_builder = _GetManager()->GetModuleByMsg< CmdBufferMngrMsgList_t >();
		CHECK_ERR( _builder );

		CHECK( _SetState( EState::Linked ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterLink{} );
		return true;
	}
	
/*
=================================================
	_AfterCompose
=================================================
*/
	bool WindowSurface::_AfterCompose (const ModuleMsg::AfterCompose &)
	{
		return _DefCompose( false );
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool WindowSurface::_Update (const ModuleMsg::Update &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;
		
		// update dependencies
		Module::_Update_Impl( msg );

		GraphicsMsg::CmdBeginFrame	begin_frame;
		_builder->Send( begin_frame );
		
		ModulePtr	system_fb	= begin_frame.result->framebuffer;

		SceneMsg::SurfaceRequestUpdate	req_upd;
		req_upd.framebuffers.PushBack({ system_fb, float4x4(), float4x4(), 0 });
		req_upd.cmdBuilder = _builder;

		CHECK( _SendEvent( req_upd ) );

		_builder->Send( GraphicsMsg::CmdEndFrame{} );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool WindowSurface::_Delete (const ModuleMsg::Delete &msg)
	{
		if ( _thread ) {
			_thread->UnsubscribeAll( this );
		}

		_thread		= null;
		_builder	= null;

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_SurfaceGetDescription
=================================================
*/
	bool WindowSurface::_SurfaceGetDescription (const SceneMsg::SurfaceGetDescription &msg)
	{
		if ( not _thread )
			return false;

		GpuMsg::GetGraphicsSettings	req_settings;
		CHECK( _thread->Send( req_settings ) );

		msg.result.Set({ _size, req_settings.result->colorFmt, req_settings.result->depthStencilFmt });
		return true;
	}
	
/*
=================================================
	_WindowDescriptionChanged
=================================================
*/
	bool WindowSurface::_WindowDescriptionChanged (const OSMsg::WindowDescriptionChanged &msg)
	{
		if ( _IsComposedState( GetState() )								and
			 msg.descr.visibility != WindowDesc::EVisibility::Invisible	and
			 Any( msg.descr.surfaceSize != _size ) )
		{
			_size = msg.descr.surfaceSize;

			_SendEvent( SceneMsg::SurfaceOnResize{ _size });
		}
		return true;
	}
	
/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool WindowSurface::_DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		Send( ModuleMsg::Delete{} );
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateWindowSurface
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateWindowSurface (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci)
	{
		return New< WindowSurface >( id, gs, ci );
	}

}	// Scene
}	// Engine
