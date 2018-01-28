// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;


	//
	// Vulkan Memory Manager
	//

	class Vk1MemoryManager final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::VkAllocMemForImage,
											GpuMsg::VkAllocMemForBuffer,
											GpuMsg::VkFreeMemory
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using EBindingTarget		= GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		struct Memory
		{
			VkDeviceMemory			mem		= VK_NULL_HANDLE;
			BytesUL					size;
			BytesUL					align;
			EGpuMemory::bits		flags;
			EBindingTarget			binding	= EBindingTarget::Unbinded;
		};

		/*struct Block
		{
			usize		memIndex;
			BytesUL		offset;
			BytesUL		size;
		};

		using MemArray_t			= Array< Memory >;
		using BlockMap_t			= Map< usize, Block >;			// key - mem index
		using ModuleMemBlockMap_t	= Map< ModuleWPtr, usize >;		// value - block index*/

		using MemoryMap_t		= Map< ModuleWPtr, Memory >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;
		

	// variables
	private:
		MemoryMap_t		_memBlocks;


	// methods
	public:
		Vk1MemoryManager (GlobalSystemsRef gs, const CreateInfo::GpuMemoryManager &ci);
		~Vk1MemoryManager ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		
		bool _VkAllocMemForImage (const Message< GpuMsg::VkAllocMemForImage > &);
		bool _VkAllocMemForBuffer (const Message< GpuMsg::VkAllocMemForBuffer > &);
		bool _VkFreeMemory (const Message< GpuMsg::VkFreeMemory > &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1MemoryManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1MemoryManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1MemoryManager::Vk1MemoryManager (GlobalSystemsRef gs, const CreateInfo::GpuMemoryManager &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkMemoryManagerModuleID, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "Vk1MemoryManager" );

		_SubscribeOnMsg( this, &Vk1MemoryManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_Compose_Impl );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_Delete );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_VkAllocMemForImage );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_VkAllocMemForBuffer );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_VkFreeMemory );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}

/*
=================================================
	destructor
=================================================
*/
	Vk1MemoryManager::~Vk1MemoryManager ()
	{
	}
	
/*
=================================================
	_VkAllocMemForImage
=================================================
*/
	bool Vk1MemoryManager::_VkAllocMemForImage (const Message< GpuMsg::VkAllocMemForImage > &msg)
	{
		CHECK_ERR( not _memBlocks.IsExist( msg->module ) );
		CHECK_ERR( msg->image != VK_NULL_HANDLE );

		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetImageMemoryRequirements( GetVkDevice(), msg->image, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( msg->flags ), OUT info.memoryTypeIndex ) );
		
		VkDeviceMemory		mem_id;
		VK_CHECK( vkAllocateMemory( GetVkDevice(), &info, null, OUT &mem_id ) );
		
		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(mem_id), GetDebugName(), EGpuObject::DeviceMemory );
		

		Memory	block;
		block.binding	= EBindingTarget::Buffer;
		block.align		= BytesUL( mem_reqs.alignment );
		block.flags		= msg->flags;
		block.mem		= mem_id;
		block.size		= BytesUL( mem_reqs.size );
		
		msg->result.Set({ block.mem, BytesUL(0), block.size });

		_memBlocks.Add( msg->module, block );
		return true;
	}
	
/*
=================================================
	_VkAllocMemForBuffer
=================================================
*/
	bool Vk1MemoryManager::_VkAllocMemForBuffer (const Message< GpuMsg::VkAllocMemForBuffer > &msg)
	{
		CHECK_ERR( not _memBlocks.IsExist( msg->module ) );
		CHECK_ERR( msg->buffer != VK_NULL_HANDLE );

		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetBufferMemoryRequirements( GetVkDevice(), msg->buffer, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( msg->flags ), OUT info.memoryTypeIndex ) );
		
		VkDeviceMemory		mem_id;
		VK_CHECK( vkAllocateMemory( GetVkDevice(), &info, null, OUT &mem_id ) );
		
		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(mem_id), GetDebugName(), EGpuObject::DeviceMemory );


		Memory	block;
		block.binding	= EBindingTarget::Buffer;
		block.align		= BytesUL( mem_reqs.alignment );
		block.flags		= msg->flags;
		block.mem		= mem_id;
		block.size		= BytesUL( mem_reqs.size );
		
		msg->result.Set({ block.mem, BytesUL(0), block.size });

		_memBlocks.Add( msg->module, block );
		return true;
	}
	
/*
=================================================
	_VkFreeMemory
=================================================
*/
	bool Vk1MemoryManager::_VkFreeMemory (const Message< GpuMsg::VkFreeMemory > &msg)
	{
		auto	dev = GetVkDevice();

		MemoryMap_t::iterator	iter;
		
		if ( _memBlocks.Find( msg->module, OUT iter ) )
		{
			vkFreeMemory( dev, iter->second.mem, null );

			_memBlocks.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1MemoryManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE )
		{
			while ( not _memBlocks.Empty() )
			{
				ModulePtr	mod = _memBlocks.Front().first.Lock();

				vkFreeMemory( dev, _memBlocks.Front().second.mem, null );
				_memBlocks.EraseByIndex( 0 );

				if ( mod ) {
					mod->Send( msg );
				}
			}
		}

		_memBlocks.Clear();

		return Module::_Delete_Impl( msg );
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1MemoryManager (GlobalSystemsRef gs, const CreateInfo::GpuMemoryManager &ci)
	{
		return New< PlatformVK::Vk1MemoryManager >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
