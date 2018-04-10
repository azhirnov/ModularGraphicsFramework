// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Public/Surface.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"
#include "Engine/Platforms/Public/Tools/WindowHelper.h"

namespace Engine
{
namespace Scene
{

	//
	// VR Surface
	//

	class VRSurface final : public BaseSceneModule
	{
	// types
	protected:
		using SupportedMessages_t	= BaseSceneModule::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >::Append< MessageListFrom<
											SceneMsg::SurfaceGetDescriptor,
											ModuleMsg::Update
										> >;

		using SupportedEvents_t		= BaseSceneModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::Update,
											SceneMsg::SurfaceOnResize,
											SceneMsg::SurfaceRequestUpdate
										> >;
		
		using CameraEventList_t		= MessageListFrom< SceneMsg::SurfaceRequestUpdate >;

		using VRThreadMsgList_t		= MessageListFrom< 
											GpuMsg::GetVRDeviceInfo,
											GpuMsg::ThreadBeginVRFrame,
											GpuMsg::ThreadEndVRFrame >;

		using VRThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;
		
		using CmdBufferMngrMsgList_t = MessageListFrom<
											GraphicsMsg::CmdBeginFrame,
											GraphicsMsg::CmdEndFrame,
											GraphicsMsg::CmdBegin,
											GraphicsMsg::CmdEnd >;

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr		_vrthread;
		ModulePtr		_builder;

		uint2			_size;


	// methods
	public:
		VRSurface (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci);
		~VRSurface ();
		

	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _SurfaceGetDescriptor (const Message< SceneMsg::SurfaceGetDescriptor > &);
		
		// event handlers
		bool _AfterCompose (const Message< ModuleMsg::AfterCompose > &);
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	VRSurface::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	VRSurface::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	VRSurface::VRSurface (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci) :
		BaseSceneModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "Scene.VRSurface" );

		_SubscribeOnMsg( this, &VRSurface::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &VRSurface::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &VRSurface::_AttachModule_Impl );
		_SubscribeOnMsg( this, &VRSurface::_DetachModule_Impl );
		_SubscribeOnMsg( this, &VRSurface::_FindModule_Impl );
		_SubscribeOnMsg( this, &VRSurface::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &VRSurface::_Link );
		_SubscribeOnMsg( this, &VRSurface::_Delete );
		_SubscribeOnMsg( this, &VRSurface::_Update );
		_SubscribeOnMsg( this, &VRSurface::_SurfaceGetDescriptor );
		_SubscribeOnMsg( this, &VRSurface::_OnManagerChanged );
		_SubscribeOnMsg( this, &VRSurface::_GetScenePrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.scene, SceneRendererModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	VRSurface::~VRSurface ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool VRSurface::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		_SendForEachAttachments( msg );
		
		CHECK_ATTACHMENT(( _vrthread = GlobalSystems()->parallelThread->GetModuleByMsgEvent< VRThreadMsgList_t, VRThreadEventList_t >() ));
		_vrthread->Subscribe( this, &VRSurface::_DeviceBeforeDestroy );

		_GetManager()->Subscribe( this, &VRSurface::_AfterCompose );
		
		_builder = _GetManager()->GetModuleByMsg< CmdBufferMngrMsgList_t >();
		CHECK_ERR( _builder );

		CHECK( _SetState( EState::Linked ) );
		return true;
	}
	
/*
=================================================
	_AfterCompose
=================================================
*/
	bool VRSurface::_AfterCompose (const Message< ModuleMsg::AfterCompose > &)
	{
		return _DefCompose( false );
	}

/*
=================================================
	_Update
=================================================
*/
	bool VRSurface::_Update (const Message< ModuleMsg::Update > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;
		
		// update dependencies
		Module::_Update_Impl( msg );
		
		Message< GraphicsMsg::CmdBeginVRFrame >		begin_frame;
		_builder->Send( begin_frame );
		
		Message< SceneMsg::SurfaceRequestUpdate >	req_upd;
		req_upd->framebuffers.PushBack( begin_frame->result->leftEye );
		req_upd->framebuffers.PushBack( begin_frame->result->rightEye );
		req_upd->cmdBuilder = _builder;

		CHECK( _SendEvent( req_upd ) );

		_builder->Send< GraphicsMsg::CmdEndVRFrame >({});
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool VRSurface::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		if ( _vrthread ) {
			_vrthread->UnsubscribeAll( this );
		}

		_vrthread	= null;
		_builder	= null;

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_SurfaceGetDescriptor
=================================================
*/
	bool VRSurface::_SurfaceGetDescriptor (const Message< SceneMsg::SurfaceGetDescriptor > &msg)
	{
		if ( not _vrthread )
			return false;

		Message< GpuMsg::GetGraphicsSettings >	req_settings;
		CHECK( _vrthread->Send( req_settings ) );

		msg->result.Set({ _size, req_settings->result->colorFmt, req_settings->result->depthStencilFmt });
		return true;
	}
	
/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool VRSurface::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		Send< ModuleMsg::Delete >({});
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateVRSurface
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateVRSurface (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RenderSurface &ci)
	{
		return New< VRSurface >( id, gs, ci );
	}

}	// Scene
}	// Engine
