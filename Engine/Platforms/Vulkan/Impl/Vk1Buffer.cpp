// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Buffer
	//

	class Vk1Buffer final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetBufferDescriptor,
											GpuMsg::GetVkBufferID
											//GpuMsg::OnMemoryBindingChanged
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		BufferDescriptor		_descr;
		vk::VkBuffer			_bufferId;
		bool					_isBindedToMemory;


	// methods
	public:
		Vk1Buffer (const GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
		~Vk1Buffer ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _GetVkBufferID (const Message< GpuMsg::GetVkBufferID > &);
		bool _GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &);
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);

	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();

		void _DestroyAll ();
		void _OnMemoryUnbinded ();
	};
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	Vk1Buffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Buffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Buffer::Vk1Buffer (const GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkBufferModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_bufferId( VK_NULL_HANDLE )
	{
		SetDebugName( "Vk1Buffer" );

		_SubscribeOnMsg( this, &Vk1Buffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_AttachModule );
		_SubscribeOnMsg( this, &Vk1Buffer::_DetachModule );
		_SubscribeOnMsg( this, &Vk1Buffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1Buffer::_Link );
		_SubscribeOnMsg( this, &Vk1Buffer::_Compose );
		_SubscribeOnMsg( this, &Vk1Buffer::_Delete );
		_SubscribeOnMsg( this, &Vk1Buffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Buffer::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1Buffer::_GetVkBufferID );
		_SubscribeOnMsg( this, &Vk1Buffer::_GetBufferDescriptor );
		//_SubscribeOnMsg( this, &Vk1Buffer::_OnMemoryBindingChanged );
		_SubscribeOnMsg( this, &Vk1Buffer::_GetVkLogicDevice );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );

		ASSERT( ci.allocMemory == false );	// TODO
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
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		ModulePtr	mem;
		CHECK_ATTACHMENT( _FindModuleWithEvents< MemoryEvents_t >( _GetAttachments(), OUT mem ) );

		mem->Subscribe( this, &Vk1Buffer::_OnMemoryBindingChanged );

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
		if ( _IsComposedState( GetState() ) or _IsCreated() )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateBuffer() );

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
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool Vk1Buffer::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK( _Attach( msg->name, msg->newModule, true ) );

		if ( msg->newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
			_OnMemoryUnbinded();
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool Vk1Buffer::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );

		if ( msg->oldModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
			_OnMemoryUnbinded();
		}
		return true;
	}

/*
=================================================
	_GetBufferDescriptor
=================================================
*/
	bool Vk1Buffer::_GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetVkBufferID
=================================================
*/
	bool Vk1Buffer::_GetVkBufferID (const Message< GpuMsg::GetVkBufferID > &msg)
	{
		msg->result.Set( _bufferId );
		return true;
	}
	
/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool Vk1Buffer::_OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &msg)
	{
		CHECK_ERR( _IsComposedOrLinkedState( GetState() ) );

		using EBindingState = GpuMsg::OnMemoryBindingChanged::EBindingState;

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
		_OnMemoryUnbinded();

		// create buffer
		VkBufferCreateInfo	info = {};

		info.sType			= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.pNext			= null;
		info.flags			= 0;
		info.usage			= Vk1Enum( _descr.usage );
		info.size			= (VkDeviceSize) _descr.size;
		info.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK( vkCreateBuffer( GetLogicalDevice(), &info, null, OUT &_bufferId ) );

		GetDevice()->SetObjectName( _bufferId, GetDebugName(), EGpuObject::Buffer );
		return true;
	}
	
/*
=================================================
	_DestroyAll
=================================================
*/
	void Vk1Buffer::_DestroyAll ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _bufferId != VK_NULL_HANDLE )
		{
			vkDestroyBuffer( dev, _bufferId, null );
		}

		_OnMemoryUnbinded();

		_bufferId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_OnMemoryUnbinded
=================================================
*/
	void Vk1Buffer::_OnMemoryUnbinded ()
	{
		_isBindedToMemory = false;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanContext::_CreateVk1Buffer (const GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformVK::Vk1Buffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
