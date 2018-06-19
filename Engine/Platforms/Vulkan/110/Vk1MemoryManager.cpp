// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

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
											GpuMsg::VkAllocMemory,
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
		Vk1MemoryManager (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuMemoryManager &ci);
		~Vk1MemoryManager ();


	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		
		bool _VkAllocMemory (const GpuMsg::VkAllocMemory &);
		bool _VkFreeMemory (const GpuMsg::VkFreeMemory &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1MemoryManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1MemoryManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1MemoryManager::Vk1MemoryManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemoryManager &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
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
		_SubscribeOnMsg( this, &Vk1MemoryManager::_VkAllocMemory );
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
	_VkAllocMemory
=================================================
*/
	bool Vk1MemoryManager::_VkAllocMemory (const GpuMsg::VkAllocMemory &msg)
	{
		CHECK_ERR( not _memBlocks.IsExist( msg.module ) );

		// allocate memory
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= msg.memReqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( msg.memReqs.memoryTypeBits, Vk1Enum( msg.flags ), OUT info.memoryTypeIndex ) );
		
		VkDeviceMemory		mem_id;
		VK_CHECK( vkAllocateMemory( GetVkDevice(), &info, null, OUT &mem_id ) );
		
		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(mem_id), GetDebugName(), EGpuObject::DeviceMemory );
		

		Memory	block;
		block.binding	= EBindingTarget::Buffer;
		block.align		= BytesUL( msg.memReqs.alignment );
		block.flags		= msg.flags;
		block.mem		= mem_id;
		block.size		= BytesUL( msg.memReqs.size );
		
		msg.result.Set({ block.mem, BytesUL(0), block.size });

		_memBlocks.Add( msg.module, block );
		return true;
	}
	
/*
=================================================
	_VkFreeMemory
=================================================
*/
	bool Vk1MemoryManager::_VkFreeMemory (const GpuMsg::VkFreeMemory &msg)
	{
		auto	dev = GetVkDevice();

		MemoryMap_t::iterator	iter;
		
		if ( _memBlocks.Find( msg.module, OUT iter ) )
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
	bool Vk1MemoryManager::_Delete (const ModuleMsg::Delete &msg)
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
	ModulePtr VulkanObjectsConstructor::CreateVk1MemoryManager (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemoryManager &ci)
	{
		return New< PlatformVK::Vk1MemoryManager >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
