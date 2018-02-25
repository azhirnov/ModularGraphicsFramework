// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

#ifdef GRAPHICS_API_VULKAN

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;
		

	//
	// Vulkan Managed Memory
	//

	class Vk1ManagedMemory final : public Vk1BaseModule
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
		using MemMapper_t			= PlatformTools::MemoryMapperHelper;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		VkDeviceMemory		_mem;
		MemMapper_t			_memMapper;
		BytesUL				_offset;		// offset from '_mem' must be aligned
		BytesUL				_size;
		EBindingTarget		_binding;
		EGpuMemory::bits	_flags;
		ModulePtr			_memManager;


	// methods
	public:
		Vk1ManagedMemory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~Vk1ManagedMemory ();


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

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		bool _GetSubResource (uint mipmapLevel, uint arrayLayer, OUT VkSubresourceLayout &result,
							  OUT EImage::type &imgType, OUT uint4 &imgDimension) const;
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1ManagedMemory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1ManagedMemory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1ManagedMemory::Vk1ManagedMemory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkManagedMemoryModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_mem( VK_NULL_HANDLE ),					_memMapper( ci.memFlags, ci.access ),
		_binding( EBindingTarget::Unbinded ),	_flags( ci.memFlags ),
		_memManager( ci.memManager )
	{
		SetDebugName( "Vk1ManagedMemory" );

		_SubscribeOnMsg( this, &Vk1ManagedMemory::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_FindModule_Empty );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_Compose );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_Delete );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_ReadFromStream );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_WriteToStream );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_MapImageToCpu );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_FlushMemoryRange );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_UnmapMemory );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GetGpuMemoryDescriptor );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GetVkMemoryID );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_WriteToImageMemory );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		if ( not _memManager )
		{
			Message< GpuMsg::GetVkPrivateClasses >	req_classes;
			_SendMsg( req_classes );
			_memManager = req_classes->result->memManager;
		}
		CHECK( _memManager );
	}

/*
=================================================
	destructor
=================================================
*/
	Vk1ManagedMemory::~Vk1ManagedMemory ()
	{
		ASSERT( not _IsCreated() );
		ASSERT( not _memMapper.IsMapped() );
		ASSERT( not _memManager );
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1ManagedMemory::_IsCreated () const
	{
		return _mem != VK_NULL_HANDLE;
	}

/*
=================================================
	_AllocForImage
=================================================
*/
	bool Vk1ManagedMemory::_AllocForImage ()
	{
		CHECK_ERR( not _IsCreated() );
		
		// request image ID
		Message< GpuMsg::GetVkImageID >		req_id;
		SendTo( _GetParents().Front(), req_id );

		VkImage		img_id	= req_id->result.Get( VkImage(VK_NULL_HANDLE) );
		CHECK_ERR( img_id != VK_NULL_HANDLE );
		
		// allocate memory
		Message< GpuMsg::VkAllocMemForImage >	alloc{ this, img_id, _flags };
		SendTo( _memManager, alloc );
		CHECK_ERR( alloc->result );
		
		// bind memory to image
		VK_CHECK( vkBindImageMemory( GetVkDevice(), img_id, alloc->result->mem, VkDeviceSize(alloc->result->offset) ) );

		_mem		= alloc->result->mem;
		_offset		= alloc->result->offset;
		_size		= alloc->result->size;
		_binding	= EBindingTarget::Image;
		return true;
	}

/*
=================================================
	_AllocForBuffer
=================================================
*/
	bool Vk1ManagedMemory::_AllocForBuffer ()
	{
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< GpuMsg::GetVkBufferID >	req_id;
		SendTo( _GetParents().Front(), req_id );

		VkBuffer	buf_id	= req_id->result.Get( VkBuffer(VK_NULL_HANDLE) );
		CHECK_ERR( buf_id != VK_NULL_HANDLE );
		
		// allocate memory
		Message< GpuMsg::VkAllocMemForBuffer >	alloc{ this, buf_id, _flags };
		SendTo( _memManager, alloc );
		CHECK_ERR( alloc->result );

		// bind memory to buffer
		VK_CHECK( vkBindBufferMemory( GetVkDevice(), buf_id, alloc->result->mem, VkDeviceSize(alloc->result->offset) ) );
		
		_mem		= alloc->result->mem;
		_offset		= alloc->result->offset;
		_size		= alloc->result->size;
		_binding	= EBindingTarget::Buffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*/
	void Vk1ManagedMemory::_FreeMemory ()
	{
		if ( _memManager and _mem != VK_NULL_HANDLE )
		{
			CHECK( SendTo< GpuMsg::VkFreeMemory >( _memManager, { this, _mem, _offset, _size }) );
		}

		_memManager	= null;
		_mem		= VK_NULL_HANDLE;
		_offset		= Uninitialized;
		_size		= Uninitialized;
		_binding	= EBindingTarget::Unbinded;
		_flags		= Uninitialized;
		_memMapper.Clear();
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1ManagedMemory::_Compose (const Message< ModuleMsg::Compose > &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

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
	bool Vk1ManagedMemory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;

		descr.memoryFlags	= _memMapper.MappingAccess();
		descr.available		= _memMapper.MappedSize();
		descr.totalSize		= _size;

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadFromStream
=================================================
*/
	bool Vk1ManagedMemory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		BinArrayCRef	data;
		CHECK_ERR( _memMapper.Read( msg->offset, msg->size.Get( UMax ), OUT data ) );
		msg->result.Set( data );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, _memMapper.MappedOffset() + msg->offset, BytesUL(data.Size()) });
		return true;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool Vk1ManagedMemory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		BytesUL	written;
		CHECK_ERR( _memMapper.Write( msg->data, msg->offset, OUT written ) );
		msg->wasWritten.Set( written );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, _memMapper.MappedOffset() + msg->offset, written });
		return true;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*/
	bool Vk1ManagedMemory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg->flags ) );
		CHECK_ERR( _binding == EBindingTarget::Buffer or _binding == EBindingTarget::Image );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	offset	= msg->offset + _offset;
		const BytesUL	size	= Min( _size, msg->size );
		void *			ptr		= null;

		VK_CHECK( vkMapMemory( GetVkDevice(), _mem, (VkDeviceSize) offset, (VkDeviceSize) size, 0, &ptr ) );
		_memMapper.OnMapped( ptr, msg->offset, size, msg->flags );

		return true;
	}
	
/*
=================================================
	_GetSubResource
=================================================
*/
	bool Vk1ManagedMemory::_GetSubResource (uint mipmapLevel, uint arrayLayer, OUT VkSubresourceLayout &result, OUT EImage::type &imgType, OUT uint4 &imgDimension) const
	{
		// request image info
		Message< GpuMsg::GetVkImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;

		SendTo( _GetParents().Front(), req_id );
		SendTo( _GetParents().Front(), req_descr );

		// get subresource layout
		VkImageSubresource	sub_resource	= {};

		const bool			is_color		= EPixelFormat::IsColor( req_descr->result->format );
		const bool			is_depth		= EPixelFormat::IsDepth( req_descr->result->format );
		const uint4			lvl_dim			= ImageUtils::LevelDimension( req_descr->result->imageType, req_descr->result->dimension, mipmapLevel );

		CHECK_ERR( is_color or is_depth );
		sub_resource.aspectMask	= is_color ? VK_IMAGE_ASPECT_COLOR_BIT : VK_IMAGE_ASPECT_DEPTH_BIT;
		sub_resource.mipLevel	= Clamp( mipmapLevel, 0u, req_descr->result->maxLevel.Get()-1 );
		sub_resource.arrayLayer	= Clamp( arrayLayer, 0u, lvl_dim.w-1 );

		vkGetImageSubresourceLayout( GetVkDevice(), req_id->result.Get(0), &sub_resource, OUT &result );

		imgType			= req_descr->result->imageType;
		imgDimension	= lvl_dim;
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool Vk1ManagedMemory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMappingAllowed( msg->flags ) );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported yet
	
		VkSubresourceLayout sub_res_layout	= {};
		EImage::type		img_type;
		uint4				img_dim;	// unused

		_GetSubResource( msg->level.Get(), 0, OUT sub_res_layout, OUT img_type, OUT img_dim );

		// validate and map memory
		CHECK_ERR( sub_res_layout.offset + sub_res_layout.size <= (ulong)_size );

		VkDeviceSize	offset	= sub_res_layout.offset + (ulong)_offset;
		void *			ptr		= null;
		VK_CHECK( vkMapMemory( GetVkDevice(), _mem, offset, sub_res_layout.size, 0, &ptr ) );

		// write output
		msg->range.Set({ BytesUL(sub_res_layout.offset), BytesUL(sub_res_layout.size) });

		if ( img_type == EImage::Tex3D ) {
			ASSERT( sub_res_layout.arrayPitch <= 1 );
			msg->pixelAlign.Set({ BytesUL(sub_res_layout.rowPitch), BytesUL(sub_res_layout.depthPitch) });
		} else {
			ASSERT( sub_res_layout.depthPitch <= 1 );
			msg->pixelAlign.Set({ BytesUL(sub_res_layout.rowPitch), BytesUL(sub_res_layout.arrayPitch) });
		}

		_memMapper.OnMapped( ptr, BytesUL(sub_res_layout.offset), BytesUL(sub_res_layout.size), msg->flags );
		return true;
	}

/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool Vk1ManagedMemory::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		CHECK_ERR( _memMapper.FlushMemoryRange( msg->offset, msg->size ) );

		TODO( "" );
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool Vk1ManagedMemory::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &)
	{
		CHECK_ERR( _memMapper.Unmap() );
		
		vkUnmapMemory( GetVkDevice(), _mem );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1ManagedMemory::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool Vk1ManagedMemory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
	{
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::GpuWrite] );	// this message allowed only for gpu-writable memory

		// request memory barrier
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GetGpuMemoryDescriptor
=================================================
*/
	bool Vk1ManagedMemory::_GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &msg)
	{
		GpuMemoryDescriptor		descr;

		descr.flags		= _flags;
		descr.size		= _size;
		descr.access	= _memMapper.MemoryAccess();

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_GetVkMemoryID
=================================================
*/
	bool Vk1ManagedMemory::_GetVkMemoryID (const Message< GpuMsg::GetVkMemoryID > &msg)
	{
		msg->result.Set( _mem );
		return true;
	}
	
/*
=================================================
	_ReadFromGpuMemory
=================================================
*/
	bool Vk1ManagedMemory::_ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg->writableBuffer.Size() > 0 );

		const bool		was_mapped	= _memMapper.IsMapped();
		const BytesUL	req_size	= BytesUL(msg->writableBuffer.Size());

		// map memory
		if ( not was_mapped )
		{
			Message< GpuMsg::MapMemoryToCpu >	map_msg;
			map_msg->flags	= GpuMsg::MapMemoryToCpu::EMappingFlags::Read;
			map_msg->offset	= msg->offset;
			map_msg->size	= req_size;

			CHECK_ERR( _MapMemoryToCpu( map_msg ) );
		}

		CHECK_ERR( msg->offset >= _memMapper.MappedOffset() and
				   msg->offset + req_size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

		// read
		Message< ModuleMsg::ReadFromStream >	read_stream;
		read_stream->offset	= msg->offset - _memMapper.MappedOffset();
		read_stream->size	= req_size;

		CHECK( _ReadFromStream( read_stream ) );

		// copy to writable buffer
		CHECK( msg->writableBuffer.Size() >= read_stream->result->Size() );

		MemCopy( msg->writableBuffer, *read_stream->result );
		msg->result.Set( msg->writableBuffer.SubArray( 0, usize(read_stream->result->Size()) ) );

		// unmap
		if ( not was_mapped )
		{
			CHECK( _UnmapMemory({}) );
		}
		return true;
	}
	
/*
=================================================
	_WriteToGpuMemory
=================================================
*/
	bool Vk1ManagedMemory::_WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		
		const bool	was_mapped	= _memMapper.IsMapped();

		// map memory
		if ( not was_mapped )
		{
			Message< GpuMsg::MapMemoryToCpu >	map_msg;
			map_msg->flags	= GpuMsg::MapMemoryToCpu::EMappingFlags::WriteDiscard;
			map_msg->offset	= msg->offset;
			map_msg->size	= BytesUL( msg->data.Size() );

			CHECK_ERR( _MapMemoryToCpu( map_msg ) );
		}
		
		CHECK_ERR( msg->offset >= _memMapper.MappedOffset() and
				   msg->offset + BytesUL(msg->data.Size()) <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

		// write
		Message< ModuleMsg::WriteToStream >		write_stream;
		write_stream->offset	= msg->offset - _memMapper.MappedOffset();
		write_stream->data		= msg->data;

		CHECK( _WriteToStream( write_stream ) );

		// flush
		if ( not _flags[ EGpuMemory::CoherentWithCPU ] )
		{
			Message< GpuMsg::FlushMemoryRange >	flush;
			flush->offset	= write_stream->offset;
			flush->size		= write_stream->wasWritten.Get( UMax );

			CHECK( _FlushMemoryRange( flush ) );
		}

		msg->wasWritten.Set( write_stream->wasWritten.Get() );

		// unmap
		if ( not was_mapped )
		{
			CHECK( _UnmapMemory({}) );
		}
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool Vk1ManagedMemory::_ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported yet
		
		const bool	was_mapped	= _memMapper.IsMapped();

		TODO( "" );

		
		// unmap
		if ( not was_mapped )
		{
			CHECK( _UnmapMemory({}) );
		}
		return true;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool Vk1ManagedMemory::_WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported yet
		
		const bool	was_mapped	= _memMapper.IsMapped();

		BytesUL		dst_offset;
		BytesUL		dst_size;
		BytesUL		dst_row_pitch;
		BytesUL		dst_slice_pitch;
		uint4		dst_dim;

		// map memory
		if ( not was_mapped )
		{
			Message< GpuMsg::MapImageToCpu >	map_msg;
			map_msg->flags		= GpuMsg::MapImageToCpu::EMappingFlags::WriteDiscard;
			map_msg->level		= msg->level;
			map_msg->layer		= ImageLayer( msg->offset.w );
			map_msg->memOffset	= msg->memOffset;

			CHECK( _MapImageToCpu( map_msg ) );

			dst_offset		= BytesUL(0);
			dst_size		= map_msg->range->size;
			dst_row_pitch	= map_msg->pixelAlign->rowPitch;
			dst_slice_pitch	= map_msg->pixelAlign->slicePitch;
			dst_dim			= *map_msg->dimension;
		}
		else
		{
			// mapped memory must be in smae range as image level (layer)
			VkSubresourceLayout sub_res_layout	= {};
			EImage::type		img_type;

			_GetSubResource( msg->level.Get(), 0, OUT sub_res_layout, OUT img_type, OUT dst_dim );

			dst_offset		= BytesUL(sub_res_layout.offset);
			dst_size		= BytesUL(sub_res_layout.size);
			dst_row_pitch	= BytesUL(sub_res_layout.rowPitch);
			dst_slice_pitch	= BytesUL(img_type == EImage::Tex3D ? sub_res_layout.depthPitch : sub_res_layout.arrayPitch);

			CHECK_ERR( sub_res_layout.offset >= _memMapper.MappedOffset() and
					   sub_res_layout.offset + sub_res_layout.size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );
		}

		// write
		BytesUL	written;
		CHECK_ERR( _memMapper.WriteImage( OUT written, OUT msg->data, msg->dimension, BytesUL(msg->rowPitch), BytesUL(msg->slicePitch),
										  msg->offset, dst_dim, dst_offset, dst_size, dst_row_pitch, dst_slice_pitch ) );
		
		// flush
		if ( not _flags[ EGpuMemory::CoherentWithCPU ] )
		{
			Message< GpuMsg::FlushMemoryRange >	flush;
			flush->offset	= dst_offset;
			flush->size		= dst_size;

			CHECK( _FlushMemoryRange( flush ) );
		}

		// unmap
		if ( not was_mapped )
		{
			CHECK( _UnmapMemory({}) );
		}
		return true;
	}
		
}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1ManagedMemory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformVK::Vk1ManagedMemory >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
