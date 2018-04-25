// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
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
											GpuMsg::GpuMemoryRegionChanged
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::DataRegionChanged,
											GpuMsg::OnMemoryBindingChanged
										> >;
		
		using ImageMessages_t		= MessageListFrom< GpuMsg::GetGLImageID >;
		using BufferMessages_t		= MessageListFrom< GpuMsg::GetGLBufferID >;

		using EBindingTarget		= GpuMsg::OnMemoryBindingChanged::EBindingTarget;
		using MemMapper_t			= PlatformTools::MemoryMapperHelper;


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
		bool _GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &);
		bool _WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &);
		bool _ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &);
		bool _WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &);
		bool _ReadFromStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< ModuleMsg::WriteToStream > &);
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
		
		bool _ReadImage200 (const GpuMsg::ReadFromImageMemory &msg, const uint4 &levelSize,
							GL4PixelFormat fmt, GL4PixelType type, const ImageDescriptor &descr);

		bool _ReadImage450 (const GpuMsg::ReadFromImageMemory &msg, GL4PixelFormat fmt,
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
		_SubscribeOnMsg( this, &GL4Memory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &GL4Memory::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &GL4Memory::_WriteToImageMemory );
		_SubscribeOnMsg( this, &GL4Memory::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &GL4Memory::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &GL4Memory::_ReadFromStream );
		_SubscribeOnMsg( this, &GL4Memory::_WriteToStream );
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
	bool GL4Memory::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool GL4Memory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
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
	bool GL4Memory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
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
	bool GL4Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
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
	bool GL4Memory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		using EMappingFlags = GpuMsg::MapMemoryToCpu::EMappingFlags;

		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg->flags ) );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	size		= Min( _size, msg->size );
		GLenum			map_flags	= 0;
		void *			ptr			= null;

		switch ( msg->flags )
		{
			case EMappingFlags::Read :			map_flags |= GL_MAP_READ_BIT;									break;
			case EMappingFlags::Write :			map_flags |= GL_MAP_WRITE_BIT;									break;
			case EMappingFlags::ReadWrite :		map_flags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;				break;
			case EMappingFlags::WriteDiscard :	map_flags |= GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;	break;
		}

		if ( _flags[EGpuMemory::CoherentWithCPU] )
			map_flags |= GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// TODO: check

		//map_flags |= GL_MAP_UNSYNCHRONIZED_BIT;	// TODO: is that needed for vulkan compatibility?

		GL_CALL( ptr = glMapNamedBufferRange( _objectId, GLintptr(msg->offset), GLsizeiptr(size), map_flags ) );

		_memMapper.OnMapped( ptr, msg->offset, size, msg->flags );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool GL4Memory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &msg)
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
	bool GL4Memory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
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
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	req_size = BytesUL(msg->writableBuffer->Size());

		// read from mapped memory
		if ( _memMapper.IsMapped() )
		{
			CHECK_ERR( msg->offset >= _memMapper.MappedOffset() and
					   msg->offset + req_size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

			// read
			Message< ModuleMsg::ReadFromStream >	read_stream;
			read_stream->offset	= msg->offset - _memMapper.MappedOffset();
			read_stream->size	= req_size;

			CHECK( _ReadFromStream( read_stream ) );
			
			// copy to writable buffer
			CHECK( msg->writableBuffer->Size() >= read_stream->result->Size() );

			MemCopy( *msg->writableBuffer, *read_stream->result );
			msg->result.Set( msg->writableBuffer->SubArray( 0, usize(read_stream->result->Size()) ) );
			return true;
		}
		
		// read without mapping
		const usize		size = Min( usize(_size - msg->offset), usize(req_size) );
		
		GL_CALL( glGetNamedBufferSubData( _objectId,
										  (GLintptr) msg->offset,
										  (GLsizei) size,
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
		CHECK_ERR( msg->offset < _size );
		
		// write to mapped memory
		if ( _memMapper.IsMapped() )
		{
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
			return true;
		}
		
		// write without mapping
		const usize		size = Min( usize(_size - msg->offset), usize(msg->data.Size()) );

		GL_CALL( glNamedBufferSubData( _objectId,
									   (GLintptr) msg->offset,
									   (GLsizei) size,
									   msg->data.ptr() ) );

		msg->wasWritten.Set( BytesUL(size) );
		
		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, msg->offset, BytesUL(size) });
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
		CHECK_ERR( msg->data.Size() >= msg->dimension.z * msg->slicePitch );
		CHECK_ERR( All( msg->dimension > 0 ) );
		
		// write without mapping
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		SendTo( _GetParents().Front(), req_descr );
		
		const uint3		level_size	= Max( ImageUtils::LevelDimension( req_descr->result->imageType, req_descr->result->dimension, msg->mipLevel.Get() ).xyz(), 1u );
		const BytesU	bpp			= BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));
		const GLenum	target		= GL4Enum( req_descr->result->imageType );
		
		CHECK_ERR( All( msg->offset + msg->dimension <= level_size ) );
		CHECK_ERR( msg->layer.Get() < req_descr->result->dimension.w );
		CHECK_ERR( msg->mipLevel < req_descr->result->maxLevel );
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( req_descr->result->format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( bpp % 8_b == 0 )	row_align = 8_b;	else
		if ( bpp % 4_b == 0 )	row_align = 4_b;	else
		if ( bpp % 2_b == 0 )	row_align = 2_b;	else
								row_align = 1_b;

		const usize		row_length	= usize(msg->rowPitch / bpp);
		const usize		img_height	= usize(msg->slicePitch / msg->rowPitch);

		CHECK_ERR( msg->data.Size() == msg->dimension.z * msg->slicePitch );
		
		GL_CALL( glBindTexture( target, _objectId ) );
		GL_CALL( glPixelStorei( GL_UNPACK_ALIGNMENT, GLint(row_align) ) );
		GL_CALL( glPixelStorei( GL_UNPACK_ROW_LENGTH, GLint(row_length) ) );
		GL_CALL( glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, GLint(img_height) ) );

		switch ( req_descr->result->imageType )
		{
			case EImage::Tex1D :
			{
				CHECK_ERR( level_size.x >= (msg->offset.x + msg->dimension.x) );
				CHECK_ERR( msg->dimension.x > 0 );
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
				CHECK_ERR(All( msg->dimension.xy() > 0 ));
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
				CHECK_ERR(All( msg->dimension.xy() > 0 ));
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
				CHECK_ERR(All( msg->dimension.xy() > 0 ));
				
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
				CHECK_ERR(All( msg->dimension.xy() > 0 ));
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
				CHECK_ERR(All( msg->dimension.xy() > 0 ));
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
		
		msg->wasWritten.Set( BytesUL(msg->dimension.z * msg->slicePitch) );
		
		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, msg->memOffset, BytesUL(msg->data.Size()) });	// TODO: calc mipmap and layer offset
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
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		SendTo( _GetParents().Front(), req_descr );

		const uint4		level_size	= Max( ImageUtils::LevelDimension( req_descr->result->imageType, req_descr->result->dimension, msg->mipLevel.Get() ), 1u );
		const BytesU	bpp			= BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));

		CHECK_ERR( All( msg->offset + msg->dimension <= level_size.xyz() ) );
		CHECK_ERR( msg->layer.Get() < req_descr->result->dimension.w );
		CHECK_ERR( msg->mipLevel < req_descr->result->maxLevel );
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( req_descr->result->format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( bpp % 8_b == 0 )	row_align = 8_b;	else
		if ( bpp % 4_b == 0 )	row_align = 4_b;	else
		if ( bpp % 2_b == 0 )	row_align = 2_b;	else
								row_align = 1_b;
		
		const usize		row_length	= usize(msg->rowPitch / bpp);
		const usize		img_height	= usize(msg->slicePitch / msg->rowPitch);
		
		CHECK_ERR( msg->writableBuffer->Size() >= msg->dimension.z * msg->slicePitch );
		
		GL_CALL( glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 ) );
		GL_CALL( glPixelStorei( GL_PACK_ALIGNMENT, GLint(row_align) ) );
		GL_CALL( glPixelStorei( GL_PACK_ROW_LENGTH, GLint(row_length) ) );
		GL_CALL( glPixelStorei( GL_PACK_IMAGE_HEIGHT, GLint(img_height) ) );

		if ( GRAPHICS_API_OPENGL >= 450 and gl::GL4_GetVersion() >= 450 )
			return _ReadImage450( *msg, fmt, type, *req_descr->result );
		else
			return _ReadImage200( *msg, level_size, fmt, type, *req_descr->result );
	}

/*
=================================================
	_ReadImage200
=================================================
*/
	bool GL4Memory::_ReadImage200 (const GpuMsg::ReadFromImageMemory &msg, const uint4 &levelSize,
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
		
		msg.result.Set( msg.writableBuffer->SubArray( 0, usize(msg.dimension.z * msg.slicePitch) ) );
		return true;
	}
	
/*
=================================================
	_ReadImage450
=================================================
*/
	bool GL4Memory::_ReadImage450 (const GpuMsg::ReadFromImageMemory &msg, GL4PixelFormat fmt, GL4PixelType type, const ImageDescriptor &descr)
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
												(GLsizei) isize(msg.writableBuffer->Size()),
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
												(GLsizei) isize(msg.writableBuffer->Size()),
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
												(GLsizei) isize(msg.writableBuffer->Size()),
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
												(GLsizei) isize(msg.writableBuffer->Size()),
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
												(GLsizei) isize(msg.writableBuffer->Size()),
												OUT msg.writableBuffer->ptr() ) );
				break;
			}
			default :
				RETURN_ERR( "unsupported image type!" );
		}
		
		msg.result.Set( msg.writableBuffer->SubArray( 0, usize(msg.dimension.z * msg.slicePitch) ) );
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