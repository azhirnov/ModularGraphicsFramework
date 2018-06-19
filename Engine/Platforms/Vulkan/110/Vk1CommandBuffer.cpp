// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Command Buffer
	//

	class Vk1CommandBuffer final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetCommandBufferDescription,
											GpuMsg::GetVkCommandBufferID,
											GpuMsg::SetCommandBufferState,
											GpuMsg::GetCommandBufferState,
											GpuMsg::SetCommandBufferDependency
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::OnCommandBufferStateChanged
										> >;
		
		using CmdPoolMsgList_t		= MessageListFrom< GpuMsg::GetVkCommandPoolID >;

		using UsedResources_t		= Set< ModulePtr >;
		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		CommandBufferDescription	_descr;
		vk::VkCommandBuffer			_cmdId;
		
		UsedResources_t				_resources;
		ERecordingState				_recordingState;


	// methods
	public:
		Vk1CommandBuffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~Vk1CommandBuffer ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _OnModuleAttached (const ModuleMsg::OnModuleAttached &);
		bool _OnModuleDetached (const ModuleMsg::OnModuleDetached &);
		bool _GetVkCommandBufferID (const GpuMsg::GetVkCommandBufferID &);
		bool _GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &);
		bool _SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &);
		bool _SetCommandBufferState (const GpuMsg::SetCommandBufferState &);
		bool _GetCommandBufferState (const GpuMsg::GetCommandBufferState &);
		
	private:
		bool _IsCreated () const;
		bool _CreateCmdBuffer ();
		void _DestroyCmdBuffer ();
		
		bool _Submit ();
		bool _BeginRecording ();
		bool _EndRecording ();
		bool _ResetCmdBuffer ();
		bool _OnCompleted ();

		void _ChangeState (ERecordingState newState);

		static void _ValidateDescription (INOUT CommandBufferDescription &descr);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1CommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandBuffer::Vk1CommandBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),		_cmdId( VK_NULL_HANDLE ),	
		_recordingState( ERecordingState::Deleted )
	{
		SetDebugName( "Vk1CommandBuffer" );

		_SubscribeOnMsg( this, &Vk1CommandBuffer::_OnModuleAttached );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_OnModuleDetached );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_Compose );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_Delete );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetVkCommandBufferID );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetCommandBufferDescription );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetCommandBufferState );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		_ValidateDescription( _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1CommandBuffer::~Vk1CommandBuffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1CommandBuffer::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK_COMPOSING( _CreateCmdBuffer() );

		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1CommandBuffer::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyCmdBuffer();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnModuleAttached
=================================================
*/
	bool Vk1CommandBuffer::_OnModuleAttached (const ModuleMsg::OnModuleAttached &msg)
	{
		if ( msg.attachedModule == this			and
			 _OnAttachedToParent( msg.parent )		and
			 msg.parent->GetSupportedMessages().HasAllTypes< CmdPoolMsgList_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
		}
		return true;
	}
	
/*
=================================================
	_OnModuleDetached
=================================================
*/
	bool Vk1CommandBuffer::_OnModuleDetached (const ModuleMsg::OnModuleDetached &msg)
	{
		if ( msg.detachedModule == this )
		{
			if ( msg.isLast )
			{
				if ( msg.parent->GetSupportedMessages().HasAllTypes< CmdPoolMsgList_t >() )
				{
					_DestroyCmdBuffer();
					CHECK( _SetState( EState::Initial ) );
				}
				_OnDetachedFromParent( msg.parent );
			}
		}
		else
		{
			UnsubscribeAll( msg.detachedModule );
		}
		return true;
	}

/*
=================================================
	_GetVkCommandBufferID
=================================================
*/
	bool Vk1CommandBuffer::_GetVkCommandBufferID (const GpuMsg::GetVkCommandBufferID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set({ _cmdId });
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool Vk1CommandBuffer::_GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool Vk1CommandBuffer::_SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &msg)
	{
		_resources = RVREF( msg.resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool Vk1CommandBuffer::_SetCommandBufferState (const GpuMsg::SetCommandBufferState &msg)
	{
		if ( _IsCreated() )
		{
			switch ( msg.newState )
			{
				case ERecordingState::Initial :		CHECK( _ResetCmdBuffer() );	break;
				case ERecordingState::Recording :	CHECK( _BeginRecording() );	break;
				case ERecordingState::Executable :	CHECK( _EndRecording() );	break;
				case ERecordingState::Pending :		CHECK( _Submit() );			break;
				case ERecordingState::Completed :	CHECK( _OnCompleted() );	break;
				default :							WARNING( "unsupported state" );
			}
		}
		return true;
	}
	
/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool Vk1CommandBuffer::_GetCommandBufferState (const GpuMsg::GetCommandBufferState &msg)
	{
		msg.result.Set( _recordingState );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1CommandBuffer::_IsCreated () const
	{
		return _cmdId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreateCmdBuffer
=================================================
*/
	bool Vk1CommandBuffer::_CreateCmdBuffer ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );
		
		// create command buffer
		{
			ModulePtr	builder;
			CHECK_ERR( builder = GetParentByMsg< CmdPoolMsgList_t >() );

			GpuMsg::GetVkCommandPoolID		req_pool_id;
			builder->Send( req_pool_id );
			CHECK_ERR( req_pool_id.result and *req_pool_id.result != VK_NULL_HANDLE );

			VkCommandBufferAllocateInfo	info = {};
			info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			info.pNext				= null;
			info.commandPool		= *req_pool_id.result;
			info.level				= _descr.flags[ ECmdBufferCreate::Secondary ] ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			info.commandBufferCount	= 1;

			VK_CHECK( vkAllocateCommandBuffers( GetVkDevice(), &info, OUT &_cmdId ) );
		
			GetDevice()->SetObjectName( uint64_t(_cmdId), GetDebugName(), EGpuObject::CommandBuffer );
		}

		_ChangeState( ERecordingState::Initial );
		return true;
	}

/*
=================================================
	_DestroyCmdBuffer
=================================================
*/
	void Vk1CommandBuffer::_DestroyCmdBuffer ()
	{
		using namespace vk;
		
		VkDevice	dev = GetVkDevice();

		if ( _cmdId	!= VK_NULL_HANDLE )
		{
			ModulePtr	builder;
			CHECK_ERR( builder = GetParentByMsg< CmdPoolMsgList_t >(), void() );

			GpuMsg::GetVkCommandPoolID	req_pool_id;
			builder->Send( req_pool_id );

			VkCommandPool	pool = req_pool_id.result.Get( VK_NULL_HANDLE );

			if ( dev != VK_NULL_HANDLE and pool != VK_NULL_HANDLE )
			{
				vkFreeCommandBuffers( dev, pool, 1, &_cmdId );
			}
		}

		_cmdId	= VK_NULL_HANDLE;
		_descr	= Uninitialized;

		_ChangeState( ERecordingState::Deleted );
		_resources.Clear();
	}
	
/*
=================================================
	_ResetCmdBuffer
=================================================
*/
	bool Vk1CommandBuffer::_ResetCmdBuffer ()
	{
		using namespace vk;

		CHECK_ERR( _IsCreated() );

		if ( _recordingState == ERecordingState::Initial )
			return true;

		CHECK_ERR( _recordingState == ERecordingState::Pending );

		VK_CHECK( vkResetCommandBuffer( _cmdId, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT ) );

		_ChangeState( ERecordingState::Initial );
		_resources.Clear();

		return true;
	}

/*
=================================================
	_Submit
=================================================
*/
	bool Vk1CommandBuffer::_Submit ()
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _recordingState == ERecordingState::Executable );

		_ChangeState( ERecordingState::Pending );
		return true;
	}
	
/*
=================================================
	_BeginRecording
=================================================
*/
	bool Vk1CommandBuffer::_BeginRecording ()
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _recordingState == ERecordingState::Initial or
				  ((_recordingState == ERecordingState::Executable or
					_recordingState == ERecordingState::Pending) and
				    _descr.flags[ECmdBufferCreate::ImplicitResetable]) );
		
		if ( _recordingState == ERecordingState::Pending )
		{
			_resources.Clear();
		}

		_ChangeState( ERecordingState::Recording );
		return true;
	}
	
/*
=================================================
	_EndRecording
=================================================
*/
	bool Vk1CommandBuffer::_EndRecording ()
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _recordingState == ERecordingState::Recording );

		_ChangeState( ERecordingState::Executable );
		return true;
	}
	
/*
=================================================
	_OnCompleted
=================================================
*/
	bool Vk1CommandBuffer::_OnCompleted ()
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _recordingState == ERecordingState::Pending );

		_resources.Clear();

		_ChangeState( ERecordingState::Executable );
		return true;
	}
		
/*
=================================================
	_ChangeState
=================================================
*/
	void Vk1CommandBuffer::_ChangeState (ERecordingState newState)
	{
		const auto	old_state = _recordingState;

		_recordingState = newState;

		_SendEvent( GpuMsg::OnCommandBufferStateChanged{ old_state, newState });
	}
	
/*
=================================================
	_ValidateDescription
=================================================
*/
	void Vk1CommandBuffer::_ValidateDescription (INOUT CommandBufferDescription &)
	{
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1CommandBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformVK::Vk1CommandBuffer >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
