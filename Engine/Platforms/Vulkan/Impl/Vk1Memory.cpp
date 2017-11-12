// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;
		

	//
	// Vulkan Memory
	//

	class Vk1Memory final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											GpuMsg::ReadFromGpuMemory,
											GpuMsg::WriteToGpuMemory,
											GpuMsg::ReadFromImageMemory,
											GpuMsg::WriteToImageMemory,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::MapImageToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::GetGpuMemoryDescriptor,
											GpuMsg::GpuMemoryRegionChanged,
											GpuMsg::GetVkMemoryID
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
		VkDeviceMemory			_mem;
		ubyte *					_mappedPtr;

		BytesUL					_size;
		BytesUL					_mappedOffset;
		BytesUL					_mappedSize;
		
		EGpuMemory::bits		_flags;
		EMemoryAccess::bits		_access;
		EBindingTarget			_binding;
		bool					_isMappedMemChanged;


	// methods
	public:
		Vk1Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~Vk1Memory ();


	// message handlers
	private:
		bool _GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &);
		bool _WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &);
		bool _ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &);
		bool _WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _ReadFromStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< ModuleMsg::WriteToStream > &);
		bool _MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &);
		bool _MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &);
		bool _GetVkMemoryID (const Message< GpuMsg::GetVkMemoryID > &);
		bool _UnmapMemory (const Message< GpuMsg::UnmapMemory > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		bool _IsCreated () const;
		bool _IsMapped () const;

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1Memory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1Memory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Memory::Vk1Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkMemoryModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_mem( VK_NULL_HANDLE ),					_mappedPtr( null ),
		_flags( ci.memFlags ),					_access( ci.access & _GpuMemoryToMemoryAccess( ci.memFlags ) ),
		_binding( EBindingTarget::Unbinded ),	_isMappedMemChanged( false )
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
		_SubscribeOnMsg( this, &Vk1Memory::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1Memory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &Vk1Memory::_ReadFromStream );
		_SubscribeOnMsg( this, &Vk1Memory::_WriteToStream );
		_SubscribeOnMsg( this, &Vk1Memory::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &Vk1Memory::_MapImageToCpu );
		_SubscribeOnMsg( this, &Vk1Memory::_FlushMemoryRange );
		_SubscribeOnMsg( this, &Vk1Memory::_UnmapMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Memory::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Memory::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1Memory::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &Vk1Memory::_GetGpuMemoryDescriptor );
		_SubscribeOnMsg( this, &Vk1Memory::_GetVkMemoryID );
		_SubscribeOnMsg( this, &Vk1Memory::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_WriteToImageMemory );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	Vk1Memory::~Vk1Memory ()
	{
		ASSERT( not _IsCreated() );
		ASSERT( not _IsMapped() );
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
	_IsMapped
=================================================
*/
	bool Vk1Memory::_IsMapped () const
	{
		return _mappedPtr != null;
	}

/*
=================================================
	_AllocForImage
=================================================
*/
	bool Vk1Memory::_AllocForImage ()
	{
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< GpuMsg::GetVkImageID >		req_id;
		SendTo( _GetParents().Front(), req_id );

		VkImage		img_id	= req_id->result.Get( VkImage(VK_NULL_HANDLE) );
		CHECK_ERR( img_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetImageMemoryRequirements( GetVkDevice(), img_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetVkDevice(), &info, null, OUT &_mem ) );
		
		GetDevice()->SetObjectName( _mem, GetDebugName(), EGpuObject::DeviceMemory );


		// bind memory to image
		VK_CHECK( vkBindImageMemory( GetVkDevice(), img_id, _mem, 0 ) );

		_size	 = BytesUL( mem_reqs.size );
		_binding = EBindingTarget::Image;
		return true;
	}
	
/*
=================================================
	_AllocForBuffer
=================================================
*/
	bool Vk1Memory::_AllocForBuffer ()
	{
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< GpuMsg::GetVkBufferID >	req_id;
		SendTo( _GetParents().Front(), req_id );

		VkBuffer	buf_id	= req_id->result.Get( VkBuffer(VK_NULL_HANDLE) );
		CHECK_ERR( buf_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetBufferMemoryRequirements( GetVkDevice(), buf_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetVkDevice(), &info, null, OUT &_mem ) );
		
		GetDevice()->SetObjectName( _mem, GetDebugName(), EGpuObject::DeviceMemory );


		// bind memory to image
		VK_CHECK( vkBindBufferMemory( GetVkDevice(), buf_id, _mem, 0 ) );
		
		_size	 = BytesUL( mem_reqs.size );
		_binding = EBindingTarget::Buffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*/
	void Vk1Memory::_FreeMemory ()
	{
		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _mem != VK_NULL_HANDLE )
		{
			vkFreeMemory( dev, _mem, null );
		}

		_mem				= VK_NULL_HANDLE;
		_mappedPtr			= null;
		_size				= Uninitialized;
		_mappedOffset		= Uninitialized;
		_mappedSize			= Uninitialized;
		_flags				= Uninitialized;
		_access				= Uninitialized;
		_binding			= EBindingTarget::Unbinded;
		_isMappedMemChanged	= false;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Memory::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		CHECK_ERR( _GetParents().IsExist( msg.Sender() ) );

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetParents().Count() >= 1 );

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
		else {
			CHECK_COMPOSING( false );
		}

		CHECK( _DefCompose( false ) );
		CHECK( _SendEvent( Message< GpuMsg::OnMemoryBindingChanged >{ parent, _binding } ) );

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
		descr.totalSize		= _size;

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
		CHECK_ERR( _IsMapped() );
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
*/
	bool Vk1Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		CHECK_ERR( _IsMapped() );
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
*/
	bool Vk1Memory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		CHECK_ERR( _IsCreated() and not _IsMapped() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] or _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	size = Min( _size, msg->size );

		VK_CHECK( vkMapMemory( GetVkDevice(), _mem, (VkDeviceSize) msg->offset, (VkDeviceSize) size, 0, (void **)&_mappedPtr ) );

		_mappedSize			= size;
		_mappedOffset		= msg->offset;
		_isMappedMemChanged	= false;
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool Vk1Memory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &msg)
	{
		CHECK_ERR( _IsCreated() and not _IsMapped() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] or _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		
		// request image info
		Message< GpuMsg::GetVkImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;

		SendTo( _GetParents().Front(), req_id );
		SendTo( _GetParents().Front(), req_descr );

		// get subresource layout
		VkImageSubresource	sub_resource	= {};
		VkSubresourceLayout sub_res_layout	= {};

		const bool			is_color		= EPixelFormat::IsColor( req_descr->result->format );
		const bool			is_depth		= EPixelFormat::IsDepth( req_descr->result->format );

		CHECK_ERR( is_color or is_depth );
		sub_resource.aspectMask = is_color ? VK_IMAGE_ASPECT_COLOR_BIT : VK_IMAGE_ASPECT_DEPTH_BIT;

		vkGetImageSubresourceLayout( GetVkDevice(), req_id->result.Get(0), &sub_resource, OUT &sub_res_layout );
	
		// validate and map memory
		CHECK_ERR( All( msg->offset + msg->dimension <= req_descr->result->dimension ) );
		CHECK_ERR( sub_res_layout.offset + sub_res_layout.size <= (ulong)_size );

		VK_CHECK( vkMapMemory( GetVkDevice(), _mem, sub_res_layout.offset, sub_res_layout.size, 0, (void **)&_mappedPtr ) );

		// write output
		msg->range.Set({ BytesUL(sub_res_layout.offset), BytesUL(sub_res_layout.size) });

		if ( req_descr->result->imageType == EImage::Tex3D ) {
			ASSERT( sub_res_layout.arrayPitch <= 1 );
			msg->pixelAlign.Set({ BytesUL(sub_res_layout.rowPitch), BytesUL(sub_res_layout.depthPitch) });
		} else {
			ASSERT( sub_res_layout.depthPitch <= 1 );
			msg->pixelAlign.Set({ BytesUL(sub_res_layout.rowPitch), BytesUL(sub_res_layout.arrayPitch) });
		}

		_mappedSize			= BytesUL( sub_res_layout.size );
		_mappedOffset		= BytesUL( sub_res_layout.offset );
		_isMappedMemChanged	= false;
		return true;
	}

/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool Vk1Memory::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		CHECK_ERR( _IsMapped() );
		CHECK( not _flags[EGpuMemory::CoherentWithCPU] );	// it is not needed becouse of coherency flag

		TODO( "" );

		_isMappedMemChanged = false;	// it is simple check, without regard to regions
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool Vk1Memory::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &)
	{
		CHECK_ERR( _IsMapped() );
		CHECK( _flags[EGpuMemory::CoherentWithCPU] or not _isMappedMemChanged );	// changes must be flushed
		
		vkUnmapMemory( GetVkDevice(), _mem );

		_mappedSize			= BytesUL();
		_mappedOffset		= BytesUL();
		_mappedPtr			= null;
		_isMappedMemChanged	= false;

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
	_GpuMemoryToMemoryAccess
=================================================
*/
	EMemoryAccess::bits Vk1Memory::_GpuMemoryToMemoryAccess (EGpuMemory::bits flags)
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
*/
	bool Vk1Memory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
	{
		CHECK_ERR( _access[EMemoryAccess::GpuWrite] );	// this message allowed only for gpu-writable memory

		// request memory barrier
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GetGpuMemoryDescriptor
=================================================
*/
	bool Vk1Memory::_GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &msg)
	{
		GpuMemoryDescriptor		descr;

		descr.flags	= _flags;
		descr.size	= _size;

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_GetVkMemoryID
=================================================
*/
	bool Vk1Memory::_GetVkMemoryID (const Message< GpuMsg::GetVkMemoryID > &msg)
	{
		msg->result.Set( _mem );
		return true;
	}
	
/*
=================================================
	_ReadFromGpuMemory
=================================================
*/
	bool Vk1Memory::_ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] );
		CHECK_ERR( not msg->size or not msg->writableBuffer or (*msg->size == BytesUL(msg->writableBuffer->Size())) );

		const bool		was_mapped	= _IsMapped();
		const BytesUL	req_size	= msg->writableBuffer ? BytesUL(msg->writableBuffer->Size()) : (msg->size ? *msg->size : BytesUL());

		// map memory
		if ( not _IsMapped() )
		{
			Message< GpuMsg::MapMemoryToCpu >	map_msg;
			map_msg->flags	= GpuMsg::MapMemoryToCpu::EMappingFlags::Read;
			map_msg->offset	= msg->offset;
			map_msg->size	= req_size;

			CHECK_ERR( _SendMsg( map_msg ) );
		}

		CHECK_ERR( msg->offset >= _mappedOffset );

		// read
		Message< ModuleMsg::ReadFromStream >	read_stream;
		read_stream->offset	= msg->offset - _mappedOffset;
		read_stream->size	= req_size;

		CHECK( _SendMsg( read_stream ) );

		// copy to writable buffer, it is a little slow in Vulkan,
		// but in other realizations, which doesn't support memory mapping,
		// using preallocated buffers is required.
		if ( msg->writableBuffer )
		{
			CHECK( msg->writableBuffer->Size() >= read_stream->result->Size() );	// never gonna happen, but...

			MemCopy( *msg->writableBuffer, *read_stream->result );

			msg->result.Set( *msg->writableBuffer );
		}
		else
			msg->result.Set( read_stream->result.Get() );

		// unmap
		if ( not was_mapped )
		{
			CHECK( _SendMsg< GpuMsg::UnmapMemory >({}) );
		}
		return true;
	}
	
/*
=================================================
	_WriteToGpuMemory
=================================================
*/
	bool Vk1Memory::_WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuWrite] );
		
		const bool	was_mapped	= _IsMapped();

		// map memory
		if ( not _IsMapped() )
		{
			Message< GpuMsg::MapMemoryToCpu >	map_msg;
			map_msg->flags	= GpuMsg::MapMemoryToCpu::EMappingFlags::WriteDiscard;
			map_msg->offset	= msg->offset;
			map_msg->size	= BytesUL( msg->data.Size() );

			CHECK_ERR( _SendMsg( map_msg ) );
		}

		CHECK_ERR( msg->offset >= _mappedOffset );

		// write
		Message< ModuleMsg::WriteToStream >		write_stream;
		write_stream->offset	= msg->offset - _mappedOffset;
		write_stream->data		= msg->data;

		CHECK( _SendMsg( write_stream ) );

		// flush
		if ( not _flags[ EGpuMemory::CoherentWithCPU ] )
		{
			Message< GpuMsg::FlushMemoryRange >	flush;
			flush->offset	= write_stream->offset;
			flush->size		= write_stream->wasWritten.Get( BytesUL(-1) );

			CHECK( _SendMsg( flush ) );
		}

		msg->wasWritten.Set( write_stream->wasWritten.Get() );

		// unmap
		if ( not was_mapped )
		{
			CHECK( _SendMsg< GpuMsg::UnmapMemory >({}) );
		}
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool Vk1Memory::_ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool Vk1Memory::_WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &msg)
	{
		TODO( "" );
		return true;
	}
		
}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanContext::_CreateVk1Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformVK::Vk1Memory >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
