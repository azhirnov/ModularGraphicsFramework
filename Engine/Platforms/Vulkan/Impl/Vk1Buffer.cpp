// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Buffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Memory.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	const Runtime::VirtualTypeList	Vk1Buffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Buffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Buffer::Vk1Buffer (const GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_bufferId( VK_NULL_HANDLE )
	{
		SetDebugName( "Vk1Buffer" );

		_SubscribeOnMsg( this, &Vk1Buffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_Link );
		_SubscribeOnMsg( this, &Vk1Buffer::_Compose );
		_SubscribeOnMsg( this, &Vk1Buffer::_Delete );
		_SubscribeOnMsg( this, &Vk1Buffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Buffer::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1Buffer::_GetVkBufferID );
		_SubscribeOnMsg( this, &Vk1Buffer::_GetGpuBufferDescriptor );
		_SubscribeOnMsg( this, &Vk1Buffer::_OnGpuMemoryBindingChanged );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Buffer::~Vk1Buffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool Vk1Buffer::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( _IsMutableState( GetState() ) );
		
		using RequestedEvents_t = MessageListFrom< ModuleMsg::OnGpuMemoryBindingChanged >;

		ModulePtr	mem;
		CHECK( _FindModuleWithEvents< RequestedEvents_t >( _GetAttachments(), OUT mem ) );

		if ( not mem )
		{
			CHECK( _SetState( EState::LinkageFailed ) );
			return false;
		}

		mem->Subscribe( this, &Vk1Buffer::_OnGpuMemoryBindingChanged );

		CHECK_ERR( Module::_Link_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Buffer::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( _CreateBuffer() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );
		
		// composed state will be changed when memory binded to image
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1Buffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyBuffer();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGpuBufferDescriptor
=================================================
*/
	bool Vk1Buffer::_GetGpuBufferDescriptor (const Message< ModuleMsg::GetGpuBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetVkBufferID
=================================================
*/
	bool Vk1Buffer::_GetVkBufferID (const Message< ModuleMsg::GetVkBufferID > &msg)
	{
		msg->result.Set( _bufferId );
		return true;
	}
	
/*
=================================================
	_OnGpuMemoryBindingChanged
=================================================
*/
	bool Vk1Buffer::_OnGpuMemoryBindingChanged (const Message< ModuleMsg::OnGpuMemoryBindingChanged > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked or _IsComposedState( GetState() ) );

		using EBindingState = ModuleMsg::OnGpuMemoryBindingChanged::EBindingState;

		if (  msg->targetObject == this )
		{
			_isBindedToMemory = ( msg->newState == EBindingState::BindedToBuffer );

			if ( _isBindedToMemory )
			{
				CHECK( _SetState( EState::ComposedMutable ) );
			}
			else
			{
				CHECK( _SetState( EState::Linked ) );
			}
		}
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1Buffer::_IsCreated () const
	{
		return _bufferId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreateBuffer
=================================================
*/
	bool Vk1Buffer::_CreateBuffer ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );

		// create buffer
		VkBufferCreateInfo	info = {};

		info.sType			= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.pNext			= null;
		info.flags			= 0;
		info.usage			= Vk1Enum( _descr.usage );
		info.size			= (VkDeviceSize) _descr.size;
		info.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK( vkCreateBuffer( GetLogicalDevice(), &info, null, OUT &_bufferId ) );
		return true;
	}
	
/*
=================================================
	_DestroyBuffer
=================================================
*/
	void Vk1Buffer::_DestroyBuffer ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _bufferId != VK_NULL_HANDLE )
		{
			vkDestroyBuffer( dev, _bufferId, null );
		}

		_bufferId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
