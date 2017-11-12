// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

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
		SHARED_POINTER( Vk1BaseModule );

		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetCommandBufferDescriptor,
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
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		CommandBufferDescriptor		_descr;
		vk::VkCommandBuffer			_cmdId;
		
		UsedResources_t				_resources;
		ERecordingState				_recordingState;


	// methods
	public:
		Vk1CommandBuffer (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~Vk1CommandBuffer ();


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _OnModuleAttached (const Message< ModuleMsg::OnModuleAttached > &);
		bool _OnModuleDetached (const Message< ModuleMsg::OnModuleDetached > &);
		bool _GetVkCommandBufferID (const Message< GpuMsg::GetVkCommandBufferID > &);
		bool _GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		
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
	};
//-----------------------------------------------------------------------------



	const Runtime::VirtualTypeList	Vk1CommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandBuffer::Vk1CommandBuffer (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkCommandBufferModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_cmdId( VK_NULL_HANDLE ),
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
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetVkCommandBufferID );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetCommandBufferDescriptor );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetVkLogicDevice );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetCommandBufferState );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );
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
	bool Vk1CommandBuffer::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK_COMPOSING( _CreateCmdBuffer() );

		CHECK( _SetState( EState::ComposedMutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1CommandBuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyCmdBuffer();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnModuleAttached
=================================================
*/
	bool Vk1CommandBuffer::_OnModuleAttached (const Message< ModuleMsg::OnModuleAttached > &msg)
	{
		if ( msg->attachedModule == this			and
			 _OnAttachedToParent( msg->parent )		and
			 msg->parent->GetSupportedMessages().HasAllTypes< CmdPoolMsgList_t >() )
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
	bool Vk1CommandBuffer::_OnModuleDetached (const Message< ModuleMsg::OnModuleDetached > &msg)
	{
		if ( msg->detachedModule == this )
		{
			if ( msg->parent->GetSupportedMessages().HasAllTypes< CmdPoolMsgList_t >() )
			{
				_DestroyCmdBuffer();
				CHECK( _SetState( EState::Initial ) );
			}
			_OnDetachedFromParent( msg->parent );
		}
		else
		{
			UnsubscribeAll( msg->detachedModule );
		}
		return true;
	}

/*
=================================================
	_GetVkCommandBufferID
=================================================
*/
	bool Vk1CommandBuffer::_GetVkCommandBufferID (const Message< GpuMsg::GetVkCommandBufferID > &msg)
	{
		msg->result.Set( _cmdId );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool Vk1CommandBuffer::_GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool Vk1CommandBuffer::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources = RVREF( msg->resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool Vk1CommandBuffer::_SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &msg)
	{
		if ( _IsCreated() )
		{
			switch ( msg->newState )
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
	_GetCommandBufferDescriptor
=================================================
*/
	bool Vk1CommandBuffer::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
	{
		msg->result.Set( _recordingState );
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
		
		ModulePtr	builder;
		CHECK_ERR( _FindParentWithMessages< CmdPoolMsgList_t >( _GetParents(), OUT builder ) );

		Message< GpuMsg::GetVkCommandPoolID >	req_pool_id;
		SendTo( builder, req_pool_id );

		VkCommandBufferAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext				= null;
		info.commandPool		<< req_pool_id->result;
		info.level				= _descr.isSecondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount	= 1;

		VK_CHECK( vkAllocateCommandBuffers( GetLogicalDevice(), &info, OUT &_cmdId ) );
		
		GetDevice()->SetObjectName( (uint64_t)_cmdId, GetDebugName(), EGpuObject::CommandBuffer );

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
		
		if ( _cmdId	!= VK_NULL_HANDLE )
		{
			ModulePtr	builder;
			CHECK_ERR( _FindParentWithMessages< CmdPoolMsgList_t >( _GetParents(), OUT builder ), void() );

			Message< GpuMsg::GetVkCommandPoolID >	req_pool_id;
			SendTo( builder, req_pool_id );

			VkDevice		dev		= GetLogicalDevice();
			VkCommandPool	pool;	pool << req_pool_id->result;

			if ( dev  != VK_NULL_HANDLE and pool != VK_NULL_HANDLE )
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
		using namespace vk;

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
				  (_recordingState == ERecordingState::Executable and _descr.implicitResetable) );

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
		using namespace vk;

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
		using namespace vk;
		
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

		_SendEvent< GpuMsg::OnCommandBufferStateChanged >({ old_state, newState });
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanContext::_CreateVk1CommandBuffer (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformVK::Vk1CommandBuffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN