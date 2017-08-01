// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Texture.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Buffer.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	const Runtime::VirtualTypeList	Vk1Memory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Memory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Memory::Vk1Memory (const GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
		_mem( VK_NULL_HANDLE ),
		_mappedPtr( null ),
		// TODO: ci.maxSize ignored
		_flags( ci.memFlags ),
		_access( ci.access & _GpuMemoryToMemoryAccess( ci.memFlags ) ),
		_heapIndex( -1 ),
		_binding( EBindingState::Unbinded ),
		_isMapped( false )
	{
		SetDebugName( "Vk1Memory" );

		_SubscribeOnMsg( this, &Vk1Memory::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_FindModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_Compose );
		_SubscribeOnMsg( this, &Vk1Memory::_Delete );
		_SubscribeOnMsg( this, &Vk1Memory::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Memory::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1Memory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &Vk1Memory::_ReadFromStream );
		_SubscribeOnMsg( this, &Vk1Memory::_WriteToStream );
		_SubscribeOnMsg( this, &Vk1Memory::_MapGpuMemoryToCpu );
		_SubscribeOnMsg( this, &Vk1Memory::_FlushGpuMemoryRange );
		_SubscribeOnMsg( this, &Vk1Memory::_UnmapGpuMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_CopyGpuMemory );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
	}

/*
=================================================
	destructor
=================================================
*/
	Vk1Memory::~Vk1Memory ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1Memory::_IsCreated () const
	{
		return _mem != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_AllocMemory
=================================================
*
	bool Vk1Memory::_AllocMemory ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );

		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= (VkDeviceSize) _size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( uint(-1), Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );

		VK_CHECK( vkAllocateMemory( GetLogicalDevice(), &info, null, &_mem ) );

		_heapIndex = info.memoryTypeIndex;
		return true;
	}
	
/*
=================================================
	_AllocForImage
=================================================
*/
	bool Vk1Memory::_AllocForImage ()
	{
		using namespace vk;
		
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< ModuleMsg::GetVkTextureID >	msg;
		SendTo( _GetParents().Front(), msg );

		VkImage	img_id	= msg->result.Get( VkImage(VK_NULL_HANDLE) );
		CHECK_ERR( img_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetImageMemoryRequirements( GetLogicalDevice(), img_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= (VkDeviceSize) mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetLogicalDevice(), &info, null, &_mem ) );
		

		// bind memory to image
		VK_CHECK( vkBindImageMemory( GetLogicalDevice(), img_id, _mem, 0 ) );

		_binding = EBindingState::BindedToImage;
		return true;
	}
	
/*
=================================================
	_AllocForBuffer
=================================================
*/
	bool Vk1Memory::_AllocForBuffer ()
	{
		using namespace vk;
		
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< ModuleMsg::GetVkBufferID >	msg;
		SendTo( _GetParents().Front(), msg );

		VkBuffer	buf_id	= msg->result.Get( VkBuffer(VK_NULL_HANDLE) );
		CHECK_ERR( buf_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetBufferMemoryRequirements( GetLogicalDevice(), buf_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= (VkDeviceSize) mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetLogicalDevice(), &info, null, &_mem ) );


		// bind memory to image
		VK_CHECK( vkBindBufferMemory( GetLogicalDevice(), buf_id, _mem, 0 ) );

		_binding = EBindingState::BindedToBuffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*/
	void Vk1Memory::_FreeMemory ()
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
		_binding		= EBindingState::Unbinded;
		_isMapped		= false;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Memory::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetParents().Count() == 1 );

		ModulePtr const&	parent = _GetParents().Front();

		if ( parent->GetSupportedMessages().HasType< ModuleMsg::GetVkTextureID >() )
		{
			CHECK( _AllocForImage() );
			CHECK( Module::_Compose_Impl( msg ) );
		}
		else
		if ( parent->GetSupportedMessages().HasType< ModuleMsg::GetVkBufferID >() )
		{
			CHECK( _AllocForBuffer() );
			CHECK( Module::_Compose_Impl( msg ) );
		}

		if ( _IsComposedState( GetState() ) )
		{
			CHECK( _SendEvent( Message< ModuleMsg::OnGpuMemoryBindingChanged >{ parent, _binding } ) );
		}
		else
		{
			CHECK( _SetState( EState::ComposingFailed ) );
		}
		return true;
	}

/*
=================================================
	_GetStreamDescriptor
=================================================
*/
	bool Vk1Memory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
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
*/
	bool Vk1Memory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		CHECK_ERR( _isMapped );
		CHECK_ERR( msg->offset < _mappedSize );

		const usize		offset	= (usize) msg->offset;
		const usize		size	= (usize) Min( _mappedSize - msg->offset, msg->size.Get( BytesUL(-1) ) );

		msg->result.Set( BinArrayCRef( _mappedPtr + offset, size ) );

		_SendEvent( Message< ModuleMsg::DataRegionChanged >{ EMemoryAccess::CpuRead, BytesU(offset), BytesU(size) } );
		return true;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool Vk1Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		CHECK_ERR( _isMapped );
		CHECK_ERR( msg->offset < _mappedSize );
		
		const usize		offset		= (usize) msg->offset;
		const usize		size		= Min( usize(_mappedSize), usize(msg->data.Size()) );

		MemCopy( BinArrayRef( _mappedPtr + offset, size ), msg->data );

		msg->wasWritten.Set( BytesUL(size) );

		_SendEvent( Message< ModuleMsg::DataRegionChanged >{ EMemoryAccess::CpuWrite, BytesU(offset), BytesU(size) } );
		return true;
	}
	
/*
=================================================
	_MapGpuMemoryToCpu
=================================================
*/
	bool Vk1Memory::_MapGpuMemoryToCpu (const Message< ModuleMsg::MapGpuMemoryToCpu > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _IsCreated() and not _isMapped );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	size = Min( _size, msg->size );

		VK_CHECK( vkMapMemory( GetLogicalDevice(), _mem, (VkDeviceSize) msg->offset, (VkDeviceSize) size, 0, (void **)&_mappedPtr ) );

		_isMapped		= true;
		_mappedSize		= size;
		_mappedOffset	= msg->offset;
		return true;
	}
	
/*
=================================================
	_FlushGpuMemoryRange
=================================================
*/
	bool Vk1Memory::_FlushGpuMemoryRange (const Message< ModuleMsg::FlushGpuMemoryRange > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_UnmapGpuMemory
=================================================
*/
	bool Vk1Memory::_UnmapGpuMemory (const Message< ModuleMsg::UnmapGpuMemory > &msg)
	{
		using namespace vk;

		CHECK_ERR( _isMapped );
		
		vkUnmapMemory( GetLogicalDevice(), _mem );

		_isMapped	= false;
		_mappedSize	= BytesUL(0);
		_mappedPtr	= null;

		return true;
	}
	
/*
=================================================
	_CopyGpuMemory
=================================================
*/
	bool Vk1Memory::_CopyGpuMemory (const Message< ModuleMsg::CopyGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );

		// TODO: create buffer for transfer and copy memory in gpu
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1Memory::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_IsGpuMemoryCompatibleWith
=================================================
*
	bool Vk1Memory::_IsGpuMemoryCompatibleWith (const Message< ModuleMsg::IsGpuMemoryCompatibleWith > &msg)
	{
		using namespace vk;

		CHECK_ERR( _IsCreated() );

		bool	compatible = true;

		if ( _heapIndex < BitsU::SizeOf( _heapIndex ) )
		{
			compatible &= ( (msg->memReq.memoryTypeBits & (1 << _heapIndex)) != 0 );
		}

		compatible &= ( msg->memReq.size < (VkDeviceSize) _size );

		msg->result.Set( compatible );
		return true;
	}
	
/*
=================================================
	_BindGpuMemoryForObject
=================================================
*
	bool Vk1Memory::_BindGpuMemoryForObject (const Message< ModuleMsg::BindGpuMemoryForObject > &msg)
	{
		using namespace vk;

		using Image_t	= ModuleMsg::BindGpuMemoryForObject::Image_t;
		using Buffer_t	= ModuleMsg::BindGpuMemoryForObject::Buffer_t;
		
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _binding == EMemBinding::Unbinded );

		if ( msg->obj.Is< Image_t >() )
		{
			VK_CHECK( vkBindImageMemory( GetLogicalDevice(), msg->obj.Get< Image_t >(), _mem, 0 ) );

			_binding = EMemBinding::BindToImage;
			return true;
		}

		if ( msg->obj.Is< Buffer_t >() )
		{
			VK_CHECK( vkBindBufferMemory( GetLogicalDevice(), msg->obj.Get< Buffer_t >(), _mem, 0 ) );

			_binding = EMemBinding::BindToBuffer;
			return true;
		}

		RETURN_ERR( "Unknown object type '" << ToString( msg->obj.GetCurrentTypeId() ) << "'" );
	}

/*
=================================================
	_GpuMemoryToMemoryAccess
=================================================
*/
	EMemoryAccess::bits Vk1Memory::_GpuMemoryToMemoryAccess (EGpuMemory::bits flags)
	{
		EMemoryAccess::bits		result;

		result |= EMemoryAccess::GpuRead;
		result |= EMemoryAccess::GpuWrite;

		if ( flags.Get( EGpuMemory::CoherentWithCPU ) or flags.Get( EGpuMemory::CachedInCPU ) )
		{
			result |= EMemoryAccess::CpuRead;
			result |= EMemoryAccess::CpuWrite;
		}

		return result;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
