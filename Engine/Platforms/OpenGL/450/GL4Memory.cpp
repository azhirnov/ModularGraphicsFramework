// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/Public/Tools/ImageUtils.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/450/GL4ResourceCache.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace gl;



	//
	// OpenGL Memory
	//

	class GL4Memory final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
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
											GpuMsg::GpuMemoryRegionChanged
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t::Append< MessageListFrom<
											//ModuleMsg::DataRegionChanged,
											GpuMsg::OnMemoryBindingChanged
										> >;
		
		using ImageMessages_t		= MessageListFrom< GpuMsg::GetGLImageID >;
		using BufferMessages_t		= MessageListFrom< GpuMsg::GetGLBufferID >;

		using EBindingTarget		= GpuMsg::OnMemoryBindingChanged::EBindingTarget;
		using MemMapper_t			= PlatformTools::MemoryMapperHelper;
		
		using ImageUtils			= PlatformTools::ImageUtils;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		GLuint				_objectId;
		MemMapper_t			_memMapper;
		BytesUL				_size;
		EGpuMemory::bits	_flags;
		EBindingTarget		_binding;


	// methods
	public:
		GL4Memory (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~GL4Memory ();


	// message handlers
	private:
		bool _GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &);
		bool _ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &);
		bool _WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &);
		bool _ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &);
		bool _WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &);
		bool _ReadRegion (const Message< DSMsg::ReadRegion > &);
		bool _WriteRegion (const Message< DSMsg::WriteRegion > &);
		bool _MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &);
		bool _MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _UnmapMemory (const Message< GpuMsg::UnmapMemory > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		bool _IsCreated () const;

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();
		
		bool _ReadImage200 (const GpuMsg::ReadFromImageMemory &msg, const uint4 &levelSize, usize resultSize,
							GL4PixelFormat fmt, GL4PixelType type, const ImageDescriptor &descr);

		bool _ReadImage450 (const GpuMsg::ReadFromImageMemory &msg, usize resultSize, GL4PixelFormat fmt,
							GL4PixelType type, const ImageDescriptor &descr);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	GL4Memory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4Memory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Memory::GL4Memory (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_objectId( 0 ),			_memMapper( ci.memFlags, ci.access ),
		_flags( ci.memFlags ),	_binding( EBindingTarget::Unbinded )
	{
		SetDebugName( "GL4Memory" );

		_SubscribeOnMsg( this, &GL4Memory::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4Memory::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4Memory::_AttachModule_Empty );
		_SubscribeOnMsg( this, &GL4Memory::_DetachModule_Empty );
		_SubscribeOnMsg( this, &GL4Memory::_FindModule_Empty );
		_SubscribeOnMsg( this, &GL4Memory::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &GL4Memory::_Link_Impl );
		_SubscribeOnMsg( this, &GL4Memory::_Compose );
		_SubscribeOnMsg( this, &GL4Memory::_Delete );
		_SubscribeOnMsg( this, &GL4Memory::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4Memory::_GetDataSourceDescriptor );
		_SubscribeOnMsg( this, &GL4Memory::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &GL4Memory::_WriteToImageMemory );
		_SubscribeOnMsg( this, &GL4Memory::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &GL4Memory::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &GL4Memory::_ReadRegion );
		_SubscribeOnMsg( this, &GL4Memory::_WriteRegion );
		_SubscribeOnMsg( this, &GL4Memory::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &GL4Memory::_MapImageToCpu );
		_SubscribeOnMsg( this, &GL4Memory::_FlushMemoryRange );
		_SubscribeOnMsg( this, &GL4Memory::_UnmapMemory );
		_SubscribeOnMsg( this, &GL4Memory::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4Memory::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4Memory::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4Memory::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &GL4Memory::_GetGpuMemoryDescriptor );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}

/*
=================================================
	destructor
=================================================
*/
	GL4Memory::~GL4Memory ()
	{
		ASSERT( not _IsCreated() );
		ASSERT( not _memMapper.IsMapped() );
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool GL4Memory::_IsCreated () const
	{
		return _objectId != 0;
	}
	
/*
=================================================
	_AllocForImage
=================================================
*/
	bool GL4Memory::_AllocForImage ()
	{
		CHECK_ERR( not _IsCreated() );

		Message< GpuMsg::GetGLImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		
		SendTo( _GetParents().Front(), req_id );
		SendTo( _GetParents().Front(), req_descr );
		
		_objectId = req_id->result.Get(0);
		CHECK_ERR( _objectId != 0 );
	
		// TODO: calc size
		_binding = EBindingTarget::Image;
		return true;
	}
	
/*
=================================================
	_AllocForBuffer
=================================================
*/
	bool GL4Memory::_AllocForBuffer ()
	{
		CHECK_ERR( not _IsCreated() );

		Message< GpuMsg::GetGLBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		SendTo( _GetParents().Front(), req_id );
		SendTo( _GetParents().Front(), req_descr );
		
		_objectId = req_id->result.Get(0);
		CHECK_ERR( _objectId != 0 );

		_size	 = req_descr->result->size;
		_binding = EBindingTarget::Buffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*/
	void GL4Memory::_FreeMemory ()
	{
		_objectId	= 0;
		_size		= Uninitialized;
		_flags		= Uninitialized;
		_binding	= EBindingTarget::Unbinded;
		_memMapper.Clear();
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4Memory::_Compose (const Message< ModuleMsg::Compose > &)
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
	bool GL4Memory::_GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &msg)
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
	bool GL4Memory::_ReadRegion (const Message< DSMsg::ReadRegion > &msg)
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
	bool GL4Memory::_WriteRegion (const Message< DSMsg::WriteRegion > &msg)
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
	bool GL4Memory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg->flags ) );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->position < _size );
		
		const BytesUL	size		= Min( _size, msg->size );
		GLenum			map_flags	= 0;
		void *			ptr			= null;

		switch ( msg->flags )
		{
			case GpuMsg::EMappingFlags::Read :			map_flags |= GL_MAP_READ_BIT;									break;
			case GpuMsg::EMappingFlags::Write :			map_flags |= GL_MAP_WRITE_BIT;									break;
			case GpuMsg::EMappingFlags::ReadWrite :		map_flags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;				break;
			case GpuMsg::EMappingFlags::WriteDiscard :	map_flags |= GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;	break;
		}

		if ( _flags[EGpuMemory::CoherentWithCPU] )
			map_flags |= GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// TODO: check

		//map_flags |= GL_MAP_UNSYNCHRONIZED_BIT;	// TODO: is that needed for vulkan compatibility?

		GL_CALL( ptr = glMapNamedBufferRange( _objectId, GLintptr(msg->position), GLsizei(size), map_flags ) );

		_memMapper.OnMapped( ptr, msg->position, size, msg->flags );
		
		msg->result.Set( BinArrayRef::FromVoid( ptr, BytesU(size) ) );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool GL4Memory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &)
	{
		TODO( "not supported, you can copy image to buffer and then map buffer" );
		return true;
	}

/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool GL4Memory::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( _memMapper.FlushMemoryRange( msg->offset, msg->size ) );

		TODO( "glFlushMappedBufferRange" );
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool GL4Memory::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( _memMapper.Unmap() );

		GL_CALL( glUnmapNamedBuffer( _objectId ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool GL4Memory::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool GL4Memory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &)
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
	bool GL4Memory::_GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &msg)
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
	_ReadFromGpuMemory
=================================================
*/
	bool GL4Memory::_ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->writableBuffer->Size() > 0 );
		CHECK_ERR( msg->position < _size );
		
		const BytesUL	req_size = BytesUL(msg->writableBuffer->Size());

		// read from mapped memory
		if ( _memMapper.IsMapped() )
		{
			CHECK_ERR( msg->position >= _memMapper.MappedOffset() and
					   msg->position + req_size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

			// read
			Message< DSMsg::ReadRegion >	read_stream;
			read_stream->position		= msg->position - _memMapper.MappedOffset();
			read_stream->writableBuffer	= msg->writableBuffer;

			CHECK( _ReadRegion( read_stream ) );
			
			// copy to writable buffer
			CHECK( msg->writableBuffer->Size() >= read_stream->result->Size() );

			MemCopy( *msg->writableBuffer, *read_stream->result );
			msg->result.Set( msg->writableBuffer->SubArray( 0, usize(read_stream->result->Size()) ) );
			return true;
		}
		
		// read without mapping
		const usize		size = Min( usize(_size - msg->position), usize(req_size) );
		
		GL_CALL( glGetNamedBufferSubData( _objectId,
										  GLintptr(msg->position),
										  GLsizei(size),
										  OUT msg->writableBuffer->ptr() ) );

		msg->result.Set( msg->writableBuffer->SubArray( 0, size ) );
		return true;
	}
	
/*
=================================================
	_WriteToGpuMemory
=================================================
*/
	bool GL4Memory::_WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->position < _size );
		
		// write to mapped memory
		if ( _memMapper.IsMapped() )
		{
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
			return true;
		}
		
		// write without mapping
		const usize		size = Min( usize(_size - msg->position), usize(msg->data.Size()) );

		GL_CALL( glNamedBufferSubData( _objectId,
									   GLintptr(msg->position),
									   GLsizei(size),
									   msg->data.ptr() ) );

		msg->wasWritten.Set( BytesUL(size) );
		
		//_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, msg->offset, BytesUL(size) });
		return true;
	}

/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool GL4Memory::_WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported
		CHECK_ERR( IsNotZero( msg->dimension ) );
		
		// write without mapping
		const auto&		img_res		= GetResourceCache()->GetImageID( _GetParents().Front() );
		const uint3		level_size	= Max( ImageUtils::LevelDimension( img_res.Get<1>().imageType, img_res.Get<1>().dimension, msg->mipLevel.Get() ).xyz(), 1u );
		const BytesU	bpp			= BytesU(EPixelFormat::BitPerPixel( img_res.Get<1>().format ));
		const GLenum	target		= GL4Enum( img_res.Get<1>().imageType );
		
		CHECK_ERR( All( msg->offset + msg->dimension <= level_size ) );
		CHECK_ERR( msg->layer.Get() < img_res.Get<1>().dimension.w );
		CHECK_ERR( msg->mipLevel < img_res.Get<1>().maxLevel );
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( img_res.Get<1>().format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( bpp % 8_b == 0 )	row_align = 8_b;	else
		if ( bpp % 4_b == 0 )	row_align = 4_b;	else
		if ( bpp % 2_b == 0 )	row_align = 2_b;	else
								row_align = 1_b;

		const usize		row_length	= usize(msg->rowPitch / bpp);
		BytesUL			data_size;

		GL_CALL( glBindTexture( target, _objectId ) );
		GL_CALL( glPixelStorei( GL_UNPACK_ALIGNMENT, GLint(row_align) ) );
		GL_CALL( glPixelStorei( GL_UNPACK_ROW_LENGTH, GLint(row_length) ) );
		
		if ( img_res.Get<1>().imageType == EImage::Tex3D )
		{
			CHECK_ERR( msg->data.Size() == msg->dimension.z * msg->slicePitch );

			data_size = msg->dimension.z * BytesUL(msg->slicePitch);

			GL_CALL( glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, GLint(msg->slicePitch / msg->rowPitch) ) );
		}
		else
		{
			CHECK_ERR( msg->dimension.z == 1 );
			CHECK_ERR( msg->data.Size() >= msg->dimension.y * msg->rowPitch );
			
			data_size = msg->dimension.y * BytesUL(msg->rowPitch);
			
			GL_CALL( glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, 0 ) );
		}

		switch ( img_res.Get<1>().imageType )
		{
			case EImage::Tex1D :
			{
				CHECK_ERR( level_size.x >= (msg->offset.x + msg->dimension.x) );
				CHECK_ERR(All( msg->offset.yz() == 0 ));
				CHECK_ERR(All( msg->dimension.yz() == 1 ));

				GL_CALL( glTexSubImage1D(	target, msg->mipLevel.Get(),
											msg->offset.x, msg->dimension.x,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::Tex2D :
			{
				CHECK_ERR(All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ));
				CHECK_ERR( msg->offset.z == 0 and msg->dimension.z == 1 );

				GL_CALL( glTexSubImage2D(	target, msg->mipLevel.Get(),
											msg->offset.x, msg->offset.y,
											msg->dimension.x, msg->dimension.y,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::Tex2DArray :
			{
				CHECK_ERR(All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ));
				CHECK_ERR( msg->offset.z == 0 and msg->dimension.z == 1 );

				GL_CALL( glTexSubImage3D(	target, msg->mipLevel.Get(),
											msg->offset.x, msg->offset.y, msg->layer.Get(),
											msg->dimension.x, msg->dimension.y, 1,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::Tex3D :
			{
				CHECK_ERR(All( level_size.xyz() >= (msg->offset.xyz() + msg->dimension.xyz()) ));
				
				GL_CALL( glTexSubImage3D(	target, msg->mipLevel.Get(),
											msg->offset.x, msg->offset.y, msg->offset.z,
											msg->dimension.x, msg->dimension.y, msg->dimension.z,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::TexCube :
			{
				CHECK_ERR(All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ));
				CHECK_ERR( msg->offset.z == 0 and msg->dimension.z == 1 );
				
				GL_CALL( glBindTexture(		GL_TEXTURE_CUBE_MAP_POSITIVE_X + msg->layer.Get(), _objectId ) );
				GL_CALL( glTexSubImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X + msg->layer.Get(), msg->mipLevel.Get(),
											msg->offset.x, msg->offset.y,
											msg->dimension.x, msg->dimension.y,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::TexCubeArray :
			{
				CHECK_ERR(All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ));
				CHECK_ERR( msg->offset.z == 0 and msg->dimension.z == 1 );

				GL_CALL( glTexSubImage3D(	target, msg->mipLevel.Get(),
											msg->offset.x, msg->offset.y, msg->layer.Get(),
											msg->dimension.x, msg->dimension.y, 1,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
		}

		GL_CALL( glBindTexture( target, 0 ) );
		
		msg->wasWritten.Set( data_size );
		
		//_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, msg->memOffset, BytesUL(msg->data.Size()) });	// TODO: calc mipmap and layer offset
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool GL4Memory::_ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported
		
		// read without mapping
		const auto&		img_res		= GetResourceCache()->GetImageID( _GetParents().Front() );
		const uint4		level_size	= Max( ImageUtils::LevelDimension( img_res.Get<1>().imageType, img_res.Get<1>().dimension, msg->mipLevel.Get() ), 1u );
		const BytesU	bpp			= BytesU(EPixelFormat::BitPerPixel( img_res.Get<1>().format ));

		CHECK_ERR( All( msg->offset + msg->dimension <= level_size.xyz() ) );
		CHECK_ERR( msg->layer.Get() < img_res.Get<1>().dimension.w );
		CHECK_ERR( msg->mipLevel < img_res.Get<1>().maxLevel );
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( img_res.Get<1>().format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( bpp % 8_b == 0 )	row_align = 8_b;	else
		if ( bpp % 4_b == 0 )	row_align = 4_b;	else
		if ( bpp % 2_b == 0 )	row_align = 2_b;	else
								row_align = 1_b;
		
		const usize	row_length	= usize(msg->rowPitch / bpp);
		usize		data_size;
		
		GL_CALL( glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
		GL_CALL( glPixelStorei( GL_PACK_ALIGNMENT, GLint(row_align) ) );
		GL_CALL( glPixelStorei( GL_PACK_ROW_LENGTH, GLint(row_length) ) );
		
		if ( img_res.Get<1>().imageType == EImage::Tex3D )
		{
			CHECK_ERR( msg->writableBuffer->Size() >= msg->dimension.z * msg->slicePitch );

			data_size = usize(msg->dimension.z * msg->slicePitch);

			GL_CALL( glPixelStorei( GL_PACK_IMAGE_HEIGHT, GLint(msg->slicePitch / msg->rowPitch) ) );
		}
		else
		{
			CHECK_ERR( msg->dimension.z == 1 );
			CHECK_ERR( msg->writableBuffer->Size() >= msg->dimension.y * msg->rowPitch );
			
			data_size = usize(msg->dimension.y * msg->rowPitch);
			
			GL_CALL( glPixelStorei( GL_PACK_IMAGE_HEIGHT, GLint(0) ) );
		}

		if ( GRAPHICS_API_OPENGL >= 450 and gl::GL4_GetVersion() >= 450 )
			return _ReadImage450( *msg, data_size, fmt, type, img_res.Get<1>() );
		else
			return _ReadImage200( *msg, level_size, data_size, fmt, type, img_res.Get<1>() );
	}

/*
=================================================
	_ReadImage200
=================================================
*/
	bool GL4Memory::_ReadImage200 (const GpuMsg::ReadFromImageMemory &msg, const uint4 &levelSize, usize resultSize,
								   GL4PixelFormat fmt, GL4PixelType type, const ImageDescriptor &descr)
	{
		CHECK_ERR( All( msg.offset == 0 ) and msg.layer.Get() == 0 );
		CHECK_ERR( All( msg.dimension.xyz() == levelSize.xyz() ) );
		CHECK_ERR( levelSize.w == 1 );

		const GLenum	target	= GL4Enum( descr.imageType );

		GL_CALL( glBindTexture( target, _objectId ) );

		switch ( descr.imageType )
		{
			case EImage::Buffer :
			case EImage::Tex1D :
			{
				GL_CALL( glGetTexImage( target, msg.mipLevel.Get(),
										fmt, type,
										OUT msg.writableBuffer->ptr() ) );
				break;
			}
			case EImage::Tex2D :
			{
				GL_CALL( glGetTexImage( target, msg.mipLevel.Get(),
										fmt, type,
										OUT msg.writableBuffer->ptr() ) );
				break;
			}
			/*case EImage::Tex3D :
			{
				GL_CALL( glGetTexImage( target, msg.mipLevel.Get(),
										fmt, type,
										OUT msg.writableBuffer->ptr() ) );
				break;
			}*/
			/*case EImage::Tex2DArray :
			{
				GL_CALL( glGetTexImage( target, msg.mipLevel.Get(),
										fmt, type,
										OUT msg.writableBuffer->ptr() ) );
				break;
			}*/
			/*case EImage::TexCube :
			{
				CHECK_ERR( msg.layer.Get() < 6 );

				GL_CALL( glGetTexImage( target, msg.mipLevel.Get(),
										fmt, type,
										OUT msg.writableBuffer->ptr() ) );
				break;
			}*/
			default :
				RETURN_ERR( "unsupported image type!" );
		}
		
		msg.result.Set( msg.writableBuffer->SubArray( 0, resultSize ) );
		return true;
	}
	
/*
=================================================
	_ReadImage450
=================================================
*/
	bool GL4Memory::_ReadImage450 (const GpuMsg::ReadFromImageMemory &msg, usize resultSize, GL4PixelFormat fmt,
								   GL4PixelType type, const ImageDescriptor &descr)
	{
	#if GRAPHICS_API_OPENGL >= 450

		switch ( descr.imageType )
		{
			case EImage::Buffer :
			case EImage::Tex1D :
			{
				CHECK_ERR(All( msg.offset.yz() == 0 ));
				CHECK_ERR(All( msg.dimension.yz() == 1 ));
				CHECK_ERR( msg.dimension.x > 0 );

				GL_CALL( glGetTextureSubImage(	_objectId, msg.mipLevel.Get(),
												msg.offset.x, 0, 0,
												msg.dimension.x, 1, 1,
												fmt, type,
												GLsizei(msg.writableBuffer->Size()),
												OUT msg.writableBuffer->ptr() ) );
				break;
			}
			case EImage::Tex2D :
			{
				CHECK_ERR(All( msg.dimension.xy() > 0 ));
				CHECK_ERR( msg.offset.z == 0 );
				CHECK_ERR( msg.dimension.z == 1 );

				GL_CALL( glGetTextureSubImage(	_objectId, msg.mipLevel.Get(),
												msg.offset.x, msg.offset.y, 0,
												msg.dimension.x, msg.dimension.y, 1,
												fmt, type,
												GLsizei(msg.writableBuffer->Size()),
												OUT msg.writableBuffer->ptr() ) );
				break;
			}
			case EImage::Tex3D :
			{
				CHECK_ERR(All( msg.dimension.xyz() > 0 ));

				GL_CALL( glGetTextureSubImage(	_objectId, msg.mipLevel.Get(),
												msg.offset.x, msg.offset.y, msg.offset.z,
												msg.dimension.x, msg.dimension.y, msg.dimension.z,
												fmt, type,
												GLsizei(msg.writableBuffer->Size()),
												OUT msg.writableBuffer->ptr() ) );
				break;
			}
			case EImage::Tex2DArray :
			{
				CHECK_ERR( msg.offset.z == 0 );
				CHECK_ERR( msg.dimension.z == 1 );
				CHECK_ERR(All( msg.dimension.xy() > 0 ));

				GL_CALL( glGetTextureSubImage(	_objectId, msg.mipLevel.Get(),
												msg.offset.x, msg.offset.y, msg.layer.Get(),
												msg.dimension.x, msg.dimension.y, 1,
												fmt, type,
												GLsizei(msg.writableBuffer->Size()),
												OUT msg.writableBuffer->ptr() ) );
				break;
			}
			case EImage::TexCube :
			{
				CHECK_ERR( msg.layer.Get() < 6 );
				CHECK_ERR( msg.offset.z == 0 and msg.dimension.z == 1 );
				CHECK_ERR(All( msg.dimension.xy() > 0 ));

				GL_CALL( glGetTextureSubImage(	_objectId, msg.mipLevel.Get(),
												msg.offset.x, msg.offset.y, msg.layer.Get(),
												msg.dimension.x, msg.dimension.y, 1,
												fmt, type,
												GLsizei(msg.writableBuffer->Size()),
												OUT msg.writableBuffer->ptr() ) );
				break;
			}
			default :
				RETURN_ERR( "unsupported image type!" );
		}
		
		msg.result.Set( msg.writableBuffer->SubArray( 0, resultSize ) );
		return true;

	#else
		RETURN_ERR( "not supported!" );
	#endif
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4Memory (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformGL::GL4Memory >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
