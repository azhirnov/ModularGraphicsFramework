// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Memory Manager
	//

	class Vk1MemoryManager final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::CopyMemory,
											GpuMsg::GpuMemoryRegionChanged
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::DataRegionChanged,
											GpuMsg::OnMemoryBindingChanged
										> >;
		
		using ImageMessages_t		= MessageListFrom< GpuMsg::GetVkImageID >;
		using BufferMessages_t		= MessageListFrom< GpuMsg::GetVkBufferID >;

		using EBindingTarget		= GpuMsg::OnMemoryBindingChanged::EBindingTarget;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;
		

	// variables
	private:
		vk::VkDeviceMemory		_mem;
		BytesUL					_maxSize;
		
		EGpuMemory::bits		_flags;
		EMemoryAccess::bits		_access;
		EBindingTarget			_binding;


	// methods
	public:
		Vk1MemoryManager (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~Vk1MemoryManager ();


	// message handlers
	private:
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< ModuleMsg::WriteToStream > &);
		bool _MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _UnmapMemory (const Message< GpuMsg::UnmapMemory > &);
		bool _CopyMemory (const Message< GpuMsg::CopyMemory > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		bool _IsCreated () const;
		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1MemoryManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1MemoryManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*
	Vk1MemoryManager::Vk1MemoryManager (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkMemoryManagerModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_mem( VK_NULL_HANDLE ),
		_mappedPtr( null ),
		// TODO: ci.maxSize ignored
		_flags( ci.memFlags ),
		_access( ci.access & _GpuMemoryToMemoryAccess( ci.memFlags ) ),
		_heapIndex( ~0u ),
		_binding( EBindingTarget::Unbinded ),
		_isMapped( false ),
		_isMappedMemChanged( false )
	{
		SetDebugName( "Vk1MemoryManager" );

		_SubscribeOnMsg( this, &Vk1MemoryManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_Compose );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_Delete );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_ReadFromStream );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_WriteToStream );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_FlushMemoryRange );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_UnmapMemory );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_CopyMemory );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1MemoryManager::_GpuMemoryRegionChanged );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*
	Vk1MemoryManager::~Vk1MemoryManager ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_IsCreated
=================================================
*
	bool Vk1MemoryManager::_IsCreated () const
	{
		return _mem != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_AllocForImage
=================================================
*
	bool Vk1MemoryManager::_AllocForImage ()
	{
		using namespace vk;
		
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< GpuMsg::GetVkImageID >	msg;
		SendTo( _GetParents().Front(), msg );

		VkImage	img_id	= msg->result.Get( VkImage(VK_NULL_HANDLE) );
		CHECK_ERR( img_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetImageMemoryRequirements( GetLogicalDevice(), img_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetLogicalDevice(), &info, null, OUT &_mem ) );
		
		GetDevice()->SetObjectName( _mem, GetDebugName(), EGpuObject::DeviceMemory );

		// bind memory to image
		VK_CHECK( vkBindImageMemory( GetLogicalDevice(), img_id, _mem, 0 ) );
		
		_size	 = BytesUL( mem_reqs.size );
		_binding = EBindingTarget::Image;
		return true;
	}
	
/*
=================================================
	_AllocForBuffer
=================================================
*
	bool Vk1MemoryManager::_AllocForBuffer ()
	{
		using namespace vk;
		
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< GpuMsg::GetVkBufferID >	msg;
		SendTo( _GetParents().Front(), msg );

		VkBuffer	buf_id	= msg->result.Get( VkBuffer(VK_NULL_HANDLE) );
		CHECK_ERR( buf_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetBufferMemoryRequirements( GetLogicalDevice(), buf_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetLogicalDevice(), &info, null, OUT &_mem ) );
		
		GetDevice()->SetObjectName( _mem, GetDebugName(), EGpuObject::DeviceMemory );

		// bind memory to image
		VK_CHECK( vkBindBufferMemory( GetLogicalDevice(), buf_id, _mem, 0 ) );
		
		_size	 = BytesUL( mem_reqs.size );
		_binding = EBindingTarget::Buffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*
	void Vk1MemoryManager::_FreeMemory ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _mem != VK_NULL_HANDLE )
		{
			vkFreeMemory( dev, _mem, null );
		}

		_mem			= VK_NULL_HANDLE;
		_mappedPtr		= null;
		_size			= Uninitialized;
		_mappedOffset	= Uninitialized;
		_mappedSize		= Uninitialized;
		_flags			= Uninitialized;
		_access			= Uninitialized;
		_heapIndex		= -1;
		_binding		= EBindingTarget::Unbinded;
		_isMapped		= false;
	}
	
/*
=================================================
	_Compose
=================================================
*
	bool Vk1MemoryManager::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetParents().Count() == 1 );

		ModulePtr const&	parent = _GetParents().Front();

		if ( parent->GetSupportedMessages().HasAllTypes< ImageMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForImage() );
		}
		else
		if ( parent->GetSupportedMessages().HasAllTypes< BufferMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForBuffer() );
		}

		CHECK( _DefCompose( false ) );
		CHECK( _SendEvent( Message< GpuMsg::OnMemoryBindingChanged >{ parent, _binding } ) );

		return true;
	}

/*
=================================================
	_GetStreamDescriptor
=================================================
*
	bool Vk1MemoryManager::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;

		descr.memoryFlags	= _access;
		descr.available		= _mappedSize;
		descr.totalSize		= _IsCreated() ? _size : BytesUL();

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadFromStream
=================================================
*
	bool Vk1MemoryManager::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		CHECK_ERR( _isMapped );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg->offset < _mappedSize );

		const usize		offset	= (usize) msg->offset;
		const usize		size	= (usize) Min( _mappedSize - msg->offset, msg->size.Get( BytesUL(-1) ) );

		msg->result.Set( BinArrayCRef( _mappedPtr + offset, size ) );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, BytesU(offset), BytesU(size) });
		return true;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*
	bool Vk1MemoryManager::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		CHECK_ERR( _isMapped );
		CHECK_ERR( _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg->offset < _mappedSize );
		
		const usize		offset		= (usize) msg->offset;
		const usize		size		= Min( usize(_mappedSize), usize(msg->data.Size()) );

		MemCopy( BinArrayRef( _mappedPtr + offset, size ), msg->data );

		msg->wasWritten.Set( BytesUL(size) );

		_isMappedMemChanged = true;

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, BytesU(offset), BytesU(size) });
		return true;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*
	bool Vk1MemoryManager::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _IsCreated() and not _isMapped );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] or _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	size = Min( _size, msg->size );

		VK_CHECK( vkMapMemory( GetLogicalDevice(), _mem, (VkDeviceSize) msg->offset, (VkDeviceSize) size, 0, (void **)&_mappedPtr ) );

		_isMapped			= true;
		_mappedSize			= size;
		_mappedOffset		= msg->offset;
		_isMappedMemChanged	= false;
		return true;
	}
	
/*
=================================================
	_FlushMemoryRange
=================================================
*
	bool Vk1MemoryManager::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		using namespace vk;

		CHECK_ERR( _isMapped );
		CHECK( not _flags[EGpuMemory::CoherentWithCPU] );	// it is not needed becouse of coherency flag

		TODO( "" );

		_isMappedMemChanged = false;	// it is simple check, without regard to regions
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*
	bool Vk1MemoryManager::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &msg)
	{
		using namespace vk;

		CHECK_ERR( _isMapped );
		CHECK( _flags[EGpuMemory::CoherentWithCPU] or not _isMappedMemChanged );	// changes must be flushed
		
		vkUnmapMemory( GetLogicalDevice(), _mem );

		_isMapped			= false;
		_mappedSize			= BytesUL(0);
		_mappedPtr			= null;
		_isMappedMemChanged	= false;

		return true;
	}
	
/*
=================================================
	_CopyMemory
----
	copy from other buffer to this buffer
=================================================
*
	bool Vk1MemoryManager::_CopyMemory (const Message< GpuMsg::CopyMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::GpuWrite] );

		// TODO: create buffer for transfer and copy memory in gpu
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*
	bool Vk1MemoryManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GpuMemoryToMemoryAccess
=================================================
*
	EMemoryAccess::bits Vk1MemoryManager::_GpuMemoryToMemoryAccess (EGpuMemory::bits flags)
	{
		EMemoryAccess::bits		result;

		result |= EMemoryAccess::GpuRead;
		result |= EMemoryAccess::GpuWrite;

		if ( flags[EGpuMemory::CoherentWithCPU] or flags[EGpuMemory::CachedInCPU] )
		{
			result |= EMemoryAccess::CpuRead;
			result |= EMemoryAccess::CpuWrite;
		}

		return result;
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*
	bool Vk1MemoryManager::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
	{
		CHECK_ERR( _access[EMemoryAccess::GpuWrite] );	// this message allowed only for gpu-writable memory

		// request memory barrier
		TODO( "" );
		return false;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanContext::_CreateVk1MemoryManager (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformVK::Vk1MemoryManager >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
*/