// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/110/Vk1ResourceCache.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

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
											DSMsg::GetDataSourceDescriptor,
											DSMsg::ReadRegion,
											DSMsg::WriteRegion,
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
											//ModuleMsg::DataRegionChanged,
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
		Vk1ManagedMemory (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~Vk1ManagedMemory ();


	// message handlers
	private:
		bool _GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &);
		bool _ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &);
		bool _WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &);
		bool _ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &);
		bool _WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _ReadRegion (const Message< DSMsg::ReadRegion > &);
		bool _WriteRegion (const Message< DSMsg::WriteRegion > &);
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
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1ManagedMemory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1ManagedMemory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1ManagedMemory::Vk1ManagedMemory (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_GetDataSourceDescriptor );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_ReadRegion );
		_SubscribeOnMsg( this, &Vk1ManagedMemory::_WriteRegion );
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

		VkImage		img_id	= req_id->result->id;
		
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
	_GetDataSourceDescriptor
=================================================
*/
	bool Vk1ManagedMemory::_GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &msg)
	{
		DataSourceDescriptor	descr;

		descr.memoryFlags	= _memMapper.MappingAccess();
		descr.available		= _memMapper.MappedSize();
		descr.totalSize		= _size;

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadRegion
=================================================
*/
	bool Vk1ManagedMemory::_ReadRegion (const Message< DSMsg::ReadRegion > &msg)
	{
		BinArrayCRef	data;
		CHECK_ERR( _memMapper.Read( msg->position, BytesUL(msg->writableBuffer->Size()), OUT data ) );

		MemCopy( *msg->writableBuffer, data );

		msg->result.Set( msg->writableBuffer->SubArray( 0, data.Count() ) );

		//_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, _memMapper.MappedOffset() + msg->offset, BytesUL(data.Size()) });
		return true;
	}
	
/*
=================================================
	_WriteRegion
=================================================
*/
	bool Vk1ManagedMemory::_WriteRegion (const Message< DSMsg::WriteRegion > &msg)
	{
		BytesUL	written;
		CHECK_ERR( _memMapper.Write( msg->data, msg->position, OUT written ) );
		msg->wasWritten.Set( written );

		//_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, _memMapper.MappedOffset() + msg->offset, written });
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
		CHECK_ERR( msg->position < _size );
		
		const BytesUL	offset	= msg->position + _offset;
		const BytesUL	size	= Min( _size, msg->size );
		void *			ptr		= null;

		VK_CHECK( vkMapMemory( GetVkDevice(), _mem, VkDeviceSize(offset), VkDeviceSize(size), 0, &ptr ) );
		_memMapper.OnMapped( ptr, msg->position, size, msg->flags );
		
		msg->result.Set( BinArrayRef::FromVoid( ptr, BytesU(size) ) );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool Vk1ManagedMemory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &msg)
	{
		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg->flags ) );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported yet
	
		Message< GpuMsg::GetImageMemoryLayout >		req_layout{ msg->mipLevel, msg->layer };
		SendTo( _GetParents().Front(), req_layout );

		// validate and map memory
		CHECK_ERR( req_layout->result->offset + req_layout->result->size <= _size );

		VkDeviceSize	level_offset	= VkDeviceSize(req_layout->result->offset + _offset);
		VkDeviceSize	level_size		= VkDeviceSize(req_layout->result->size);
		void *			ptr				= null;

		VK_CHECK( vkMapMemory( GetVkDevice(), _mem, level_offset, level_size, 0, OUT &ptr ) );

		// write output
		msg->range.Set({ req_layout->result->offset, req_layout->result->size });
		msg->dimension.Set( req_layout->result->dimension );

		_memMapper.OnMapped( ptr, req_layout->result->offset, req_layout->result->size, msg->flags );
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
	bool Vk1ManagedMemory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &)
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
		ASSERT( _IsCreated() );

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
		CHECK_ERR( msg->writableBuffer->Size() > 0 );

		const bool		was_mapped	= _memMapper.IsMapped();
		const BytesUL	req_size	= BytesUL(msg->writableBuffer->Size());

		// map memory
		if ( not was_mapped )
		{
			Message< GpuMsg::MapMemoryToCpu >	map_msg;
			map_msg->flags		= GpuMsg::EMappingFlags::Read;
			map_msg->position	= msg->position;
			map_msg->size		= req_size;

			CHECK_ERR( _MapMemoryToCpu( map_msg ) );
		}

		CHECK_ERR( msg->position >= _memMapper.MappedOffset() and
				   msg->position + req_size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

		// read
		Message< DSMsg::ReadRegion >	read_stream;
		read_stream->position		= msg->position - _memMapper.MappedOffset();
		read_stream->writableBuffer	= msg->writableBuffer;

		CHECK( _ReadRegion( read_stream ) );

		// copy to writable buffer
		CHECK( msg->writableBuffer->Size() >= read_stream->result->Size() );

		msg->result.Set( msg->writableBuffer->SubArray( 0, usize(read_stream->result->Size()) ) );

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
			map_msg->flags		= GpuMsg::EMappingFlags::Write;
			map_msg->position	= msg->position;
			map_msg->size		= BytesUL( msg->data.Size() );

			CHECK_ERR( _MapMemoryToCpu( map_msg ) );
		}
		
		CHECK_ERR( msg->position >= _memMapper.MappedOffset() and
				   msg->position + BytesUL(msg->data.Size()) <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

		// write
		Message< DSMsg::WriteRegion >		write_stream;
		write_stream->position	= msg->position - _memMapper.MappedOffset();
		write_stream->data		= msg->data;

		CHECK( _WriteRegion( write_stream ) );

		// flush
		if ( not _flags[ EGpuMemory::CoherentWithCPU ] )
		{
			Message< GpuMsg::FlushMemoryRange >	flush;
			flush->offset	= write_stream->position;
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
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported yet
		
		const bool	was_mapped	= _memMapper.IsMapped();
		const auto	img_res		= GetResourceCache()->GetImageID( _GetParents().Front() );
		
		Message< GpuMsg::GetImageMemoryLayout >		req_layout{ msg->mipLevel, msg->layer };	// TODO: buffer not support image memory layout!
		SendTo( _GetParents().Front(), req_layout );

		// map memory
		if ( not was_mapped )
		{
			Message< GpuMsg::MapImageToCpu >	map_msg;
			map_msg->flags		= GpuMsg::EMappingFlags::Read;
			map_msg->mipLevel	= msg->mipLevel;
			map_msg->layer		= msg->layer;
			map_msg->memOffset	= msg->memOffset;

			CHECK( _MapImageToCpu( map_msg ) );
			
			req_layout->result->offset = BytesUL(0);	// offset added when mapped memory
		}
		else
		{
			// mapped memory must be in same range as image level (layer)
			CHECK_ERR( req_layout->result->offset >= _memMapper.MappedOffset() and
					   req_layout->result->offset + req_layout->result->size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );
		}
		
		CHECK_ERR( All( msg->offset + msg->dimension <= img_res.Get<1>().dimension.xyz() ) );
		CHECK_ERR( msg->layer.Get() < img_res.Get<1>().dimension.w );

		// read
		BytesUL		readn;
		CHECK_ERR( _memMapper.ReadImage( OUT readn, OUT *msg->writableBuffer, EPixelFormat::BitPerPixel(img_res.Get<1>().format),
										 msg->dimension, BytesUL(msg->rowPitch), BytesUL(msg->slicePitch),
										 msg->offset, req_layout->result->dimension, req_layout->result->offset, req_layout->result->size,
										 req_layout->result->rowPitch, req_layout->result->slicePitch ) );

		msg->result.Set( msg->writableBuffer->SubArray( 0, usize(readn) ) );
		
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
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported yet
		CHECK_ERR( msg->data.Size() >= msg->dimension.z * msg->slicePitch );
		CHECK_ERR( IsNotZero( msg->dimension ) );
		
		const bool	was_mapped	= _memMapper.IsMapped();
		const auto	img_res		= GetResourceCache()->GetImageID( _GetParents().Front() );
		
		Message< GpuMsg::GetImageMemoryLayout >		req_layout{ msg->mipLevel, msg->layer };	// TODO: buffer not support image memory layout!
		SendTo( _GetParents().Front(), req_layout );

		// map memory
		if ( not was_mapped )
		{
			Message< GpuMsg::MapImageToCpu >	map_msg;
			map_msg->flags		= GpuMsg::EMappingFlags::Write;
			map_msg->mipLevel	= msg->mipLevel;
			map_msg->layer		= msg->layer;
			map_msg->memOffset	= msg->memOffset;

			CHECK( _MapImageToCpu( map_msg ) );

			req_layout->result->offset = BytesUL(0);	// offset added when mapped memory
		}
		else
		{
			// mapped memory must be in smae range as image level (layer)
			CHECK_ERR( req_layout->result->offset >= _memMapper.MappedOffset() and
					   req_layout->result->offset + req_layout->result->size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );
		}
		
		CHECK_ERR( All( msg->offset + msg->dimension <= img_res.Get<1>().dimension.xyz() ) );
		CHECK_ERR( msg->layer.Get() < img_res.Get<1>().dimension.w );

		// write
		BytesUL	written;
		CHECK_ERR( _memMapper.WriteImage( OUT written, OUT msg->data, EPixelFormat::BitPerPixel(img_res.Get<1>().format),
										  msg->dimension.xyz(), BytesUL(msg->rowPitch), BytesUL(msg->slicePitch),
										  msg->offset, req_layout->result->dimension, req_layout->result->offset, req_layout->result->size,
										  req_layout->result->rowPitch, req_layout->result->slicePitch ) );
		
		// flush
		if ( not _flags[ EGpuMemory::CoherentWithCPU ] )
		{
			Message< GpuMsg::FlushMemoryRange >	flush{ req_layout->result->offset, req_layout->result->size };

			CHECK( _FlushMemoryRange( flush ) );
		}

		msg->wasWritten.Set( written );

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
	ModulePtr VulkanObjectsConstructor::CreateVk1ManagedMemory (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformVK::Vk1ManagedMemory >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
