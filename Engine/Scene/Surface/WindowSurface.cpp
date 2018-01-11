// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Shared/Surface.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"
#include "Engine/Platforms/Windows/WinMessages.h"

namespace Engine
{
namespace Scene
{

	//
	// Window Surface
	//

	class WindowSurface final : public BaseSceneModule
	{
	// types
	protected:
		using SupportedMessages_t	= BaseSceneModule::SupportedMessages_t::Append< MessageListFrom<
											SceneMsg::SurfaceGetDescriptor,
											ModuleMsg::Update
										> >;

		using SupportedEvents_t		= BaseSceneModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::Update,
											SceneMsg::SurfaceOnResize,
											SceneMsg::SurfaceRequestUpdate
										> >;
		
		using CameraEventList_t		= MessageListFrom< SceneMsg::SurfaceRequestUpdate >;

		using WindowMsgList_t		= MessageListFrom< OSMsg::GetWinWindowHandle >;
		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy, OSMsg::OnWinWindowRawMessage >;

		using GThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetDeviceInfo >;
		using GThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr		_thread;
		ModulePtr		_window;

		uint2			_size;


	// methods
	public:
		WindowSurface (GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci);
		~WindowSurface ();
		

	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _SurfaceGetDescriptor (const Message< SceneMsg::SurfaceGetDescriptor > &);
		
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	WindowSurface::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	WindowSurface::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	WindowSurface::WindowSurface (GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci) :
		BaseSceneModule( gs, ModuleConfig{ WindowSurfaceModuleID, UMax }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "Scene.WindowSurface" );

		_SubscribeOnMsg( this, &WindowSurface::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_FindModule_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_Link );
		_SubscribeOnMsg( this, &WindowSurface::_Compose_Impl );
		_SubscribeOnMsg( this, &WindowSurface::_Delete );
		_SubscribeOnMsg( this, &WindowSurface::_Update );
		_SubscribeOnMsg( this, &WindowSurface::_SurfaceGetDescriptor );
		_SubscribeOnMsg( this, &WindowSurface::_OnManagerChanged );
		_SubscribeOnMsg( this, &WindowSurface::_GetScenePrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

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
	bool WindowSurface::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		_SendForEachAttachments( msg );
		
		CHECK_ATTACHMENT(( _thread = GlobalSystems()->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventList_t >() ));
		CHECK_ATTACHMENT(( _window = GlobalSystems()->parallelThread->GetModuleByMsgEvent< WindowMsgList_t, WindowEventList_t >() ));
		
		_window->Subscribe( this, &WindowSurface::_WindowDescriptorChanged );
		_thread->Subscribe( this, &WindowSurface::_DeviceBeforeDestroy );

		CHECK( _SetState( EState::Linked ) );
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool WindowSurface::_Update (const Message< ModuleMsg::Update > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;
		
		// update dependencies
		Module::_Update_Impl( msg );

		Message< GpuMsg::ThreadBeginFrame >	begin_frame;
		_thread->Send( begin_frame );
		
		ModulePtr	system_fb	= begin_frame->result->framebuffer;

		Message< SceneMsg::SurfaceRequestUpdate >	req_upd;
		req_upd->framebuffers.PushBack({ system_fb, float4x4(), float4x4(), 0 });

		CHECK( _SendEvent( req_upd ) );

		CHECK_ERR( req_upd->cmdBuffers.Count() == 1 );	// supported only 1 buffer yet

		_thread->Send< GpuMsg::ThreadEndFrame >({ system_fb, req_upd->cmdBuffers.Front() });

		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool WindowSurface::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		if ( _window ) {
			_window->UnsubscribeAll( this );
		}

		if ( _thread ) {
			_thread->UnsubscribeAll( this );
		}

		_thread	= null;
		_window	= null;

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_SurfaceGetDescriptor
=================================================
*/
	bool WindowSurface::_SurfaceGetDescriptor (const Message< SceneMsg::SurfaceGetDescriptor > &msg)
	{
		if ( not _thread )
			return false;

		Message< GpuMsg::GetGraphicsSettings >	req_settings;
		CHECK( _thread->Send( req_settings ) );

		msg->result.Set({ _size, req_settings->result->colorFmt, req_settings->result->depthStencilFmt });
		return true;
	}
	
/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool WindowSurface::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &msg)
	{
		if ( msg->desc.visibility != WindowDesc::EVisibility::Invisible	and
			 Any( msg->desc.surfaceSize != _size ) )
		{
			_size = msg->desc.surfaceSize;

			_SendEvent< SceneMsg::SurfaceOnResize >({ _size });
		}
		return true;
	}
	
/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool WindowSurface::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		Send< ModuleMsg::Delete >({});
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateWindowSurface
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateWindowSurface (GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci)
	{
		return New< WindowSurface >( gs, ci );
	}

}	// Scene
}	// Engine
