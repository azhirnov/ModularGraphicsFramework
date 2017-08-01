// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuilder.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	const Runtime::VirtualTypeList	Vk1CommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandBuffer::Vk1CommandBuffer (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_builder( ci.builder ),
		_cmdId( VK_NULL_HANDLE ),
		_recordingState( ERecordingState::Deleted )
	{
		SetDebugName( "Vk1CommandBuffer" );

		_SubscribeOnMsg( this, &Vk1CommandBuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_Compose );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_Delete );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetVkCommandBufferID );
		_SubscribeOnMsg( this, &Vk1CommandBuffer::_GetGpuCommandBufferDescriptor );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
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
		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		if ( _CreateCmdBuffer() )
		{
			CHECK( _SetState( EState::ComposedMutable ) );
		}
		else
		{
			CHECK( _SetState( EState::ComposingFailed ) );
		}
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
	_GetVkCommandBufferID
=================================================
*/
	bool Vk1CommandBuffer::_GetVkCommandBufferID (const Message< ModuleMsg::GetVkCommandBufferID > &msg)
	{
		msg->result.Set( _cmdId );
		return true;
	}

/*
=================================================
	_GetGpuCommandBufferDescriptor
=================================================
*/
	bool Vk1CommandBuffer::_GetGpuCommandBufferDescriptor (const Message< ModuleMsg::GetGpuCommandBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
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

		CHECK_ERR( _builder );
		CHECK_ERR( not _IsCreated() );

		// delete command buffer when deleted command pool
		_builder->Subscribe( this, &Vk1CommandBuffer::_Delete );
		
		VkCommandBufferAllocateInfo	info = {};

		info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext				= null;
		info.commandPool		= _builder->GetCmdPoolID();
		info.level				= _descr.isSecondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount	= 1;

		VK_CHECK( vkAllocateCommandBuffers( GetLogicalDevice(), &info, OUT &_cmdId ) );

		_recordingState	= ERecordingState::Initial;
		return true;
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

		VK_CHECK( vkResetCommandBuffer( _cmdId, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT ) );

		_recordingState	= ERecordingState::Initial;
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

		auto	dev		= GetLogicalDevice();
		auto	pool	= _builder ? _builder->GetCmdPoolID() : VK_NULL_HANDLE;

		if ( dev != VK_NULL_HANDLE	and
			 pool != VK_NULL_HANDLE	and
			 _cmdId != VK_NULL_HANDLE )
		{
			vkFreeCommandBuffers( dev, pool, 1, &_cmdId );
		}

		_cmdId			= VK_NULL_HANDLE;
		_builder		= null;
		_descr			= Uninitialized;
		_recordingState	= ERecordingState::Deleted;

		_resources.Clear();
	}

/*
=================================================
	_Submit
=================================================
*/
	bool Vk1CommandBuffer::_Submit ()
	{
		CHECK_ERR( _IsCreated() );

		_recordingState	= ERecordingState::Pending;

		TODO( "" );
		return true;
	}
	
/*
=================================================
	_BeginRecording
=================================================
*/
	bool Vk1CommandBuffer::_BeginRecording ()
	{
		using namespace vk;

		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _recordingState == ERecordingState::Initial );

		VkCommandBufferBeginInfo	cmd_info = {};

		cmd_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmd_info.pNext				= null;
		cmd_info.flags				= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		cmd_info.pInheritanceInfo	= null;	// TODO

		VK_CHECK( vkBeginCommandBuffer( _cmdId, &cmd_info ) );

		_recordingState	= ERecordingState::Recording;
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
		
		VK_CHECK( vkEndCommandBuffer( _cmdId ) );

		_recordingState	= ERecordingState::Executable;
		return true;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
