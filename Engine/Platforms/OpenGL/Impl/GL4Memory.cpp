// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#if defined( GRAPHICS_API_OPENGL )

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
		GL4Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
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
		bool _IsMapped () const;

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	GL4Memory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4Memory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Memory::GL4Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLMemoryModuleID, 1 }, &_msgTypes, &_eventTypes ),
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
		//CHECK_ERR( _GetParents().IsExist( msg.Sender() ) );

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

		GL_CALL( ptr = glMapNamedBufferRange( _objectId, GLintptr(msg->offset), GLsizeiptr(Min(_size, msg->size)), map_flags ) );

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
		CHECK_ERR( not msg->size or not msg->writableBuffer or (*msg->size == BytesUL(msg->writableBuffer->Size())) );
		
		const bool		was_mapped	= _memMapper.IsMapped();
		const usize		size		= Min( usize(_size), usize(msg->writableBuffer->Size()) );

		// just read from mapped memory
		if ( was_mapped )
		{
			TODO( "" );
			return false;
		}
		
		// read without mapping
		CHECK_ERR( msg->offset < _size );
		CHECK_ERR( msg->writableBuffer and not msg->writableBuffer->Empty() );
		
		GL_CALL( glGetNamedBufferSubData( _objectId,
										  (GLintptr) msg->offset,
										  (GLsizei) size,
										  (void*) msg->writableBuffer->ptr() ) );

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
		
		const bool		was_mapped	= _memMapper.IsMapped();
		const usize		size		= Min( usize(_size), usize(msg->data.Size()) );
		
		// just write to mapped memory
		if ( was_mapped )
		{
			TODO( "" );
			return false;
		}
		
		// write without mapping
		CHECK_ERR( msg->offset < _size );

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
	_ReadFromImageMemory
=================================================
*/
	bool GL4Memory::_ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &msg)
	{
		TODO( "copy image part to buffer and then read from buffer,"
			  " there is no way instead of glGetTextureSubImage (GL 4.5) to get subregion of image" );
		return false;

		/*
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported
		
		// just read from mapped memory
		if ( _IsMapped() )
		{
			TODO( "" );
			return false;
		}
		
		// read without mapping
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		SendTo( _GetParents().Front(), req_descr );

		const uint4		level_size	= ImageUtils::LevelDimension( req_descr->result->imageType, req_descr->result->dimension, msg->level.Get() );
		const BytesU	bpp			= BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));
		const GLenum	target		= GL4Enum( req_descr->result->imageType );

		CHECK_ERR( msg->writableBuffer and not msg->writableBuffer->Empty() );
		CHECK_ERR( All( msg->offset + msg->dimension <= level_size ) );
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( req_descr->result->format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( msg->rowPitch % 4_b == 0 )		row_align = 4_b;	else
		if ( msg->rowPitch % 2_b == 0 )		row_align = 2_b;	else
											row_align = 1_b;
		
		ASSERT( msg->rowPitch == GXMath::ImageUtils::AlignedRowSize( msg->dimension.x, bpp, row_align ) );
		ASSERT( msg->slicePitch == GXMath::ImageUtils::AlignedSliceSize( msg->dimension.xy(), bpp, row_align, row_align ) );
		CHECK_ERR( msg->writableBuffer->Size() == msg->dimension.z * msg->slicePitch );

		GL_CALL( glBindTexture( target, _objectId ) );
		GL_CALL( glPixelStorei( GL_PACK_ALIGNMENT, (GLint) isize(row_align) ) );

		switch ( req_descr->result->imageType )
		{
			case EImage::Buffer :
			case EImage::Tex1D :
			{
				CHECK_ERR( IsZero( msg->offset.yzw() ) and IsZero( msg->dimension.yzw() ) );
				CHECK_ERR( IsNotZero( msg->dimension.x ) );

				GL_CALL( glGetTextureSubImage( _objectId, msg->level.Get(),
												msg->offset.x, 0, 0,
												msg->dimension.x, 1, 1,
												fmt, type,
												(GLsizei) isize(msg->writableBuffer->Size()),
												(void*) msg->writableBuffer->ptr() ) );
				break;
			}
			case EImage::Tex2D :
			{
				CHECK_ERR( IsZero( msg->offset.zw() ) and IsZero( msg->dimension.zw() ) );
				CHECK_ERR( IsNotZero( msg->dimension.xy() ) );

				GL_CALL( glGetTextureSubImage( _objectId, msg->level.Get(),
												msg->offset.x, msg->offset.y, 0,
												msg->dimension.x, msg->dimension.y, 1,
												fmt, type,
												(GLsizei) isize(msg->writableBuffer->Size()),
												(void*) msg->writableBuffer->ptr() ) );
				break;
			}
			case EImage::Tex3D :
			{
				CHECK_ERR( IsZero( msg->offset.w ) and IsZero( msg->dimension.w ) );
				CHECK_ERR( IsNotZero( msg->dimension.xyz() ) );

				GL_CALL( glGetTextureSubImage( _objectId, msg->level.Get(),
													msg->offset.x, msg->offset.y, msg->offset.z,
													msg->dimension.x, msg->dimension.y, msg->dimension.z,
												    fmt, type,
													(GLsizei) isize(msg->writableBuffer->Size()),
												    (void*) msg->writableBuffer->ptr() ) );
				break;
			}
			case EImage::Tex2DArray :
			{
				CHECK_ERR( IsZero( msg->offset.z ) and IsZero( msg->dimension.z ) );
				CHECK_ERR( IsNotZero( msg->dimension.xyw() ) );

				GL_CALL( glGetSubImageFromTexture( _objectId, target, msg->level.Get(),
													msg->offset.x, msg->offset.y, msg->offset.w,
													msg->dimension.x, msg->dimension.y, msg->dimension.w,
												    fmt, type,
													(GLsizei) isize(msg->writableBuffer->Size()),
												    (void*) msg->writableBuffer->ptr() ) );
				break;
			}
			case EImage::TexCube :
			{
				CHECK_ERR( msg->offset.z < 6 and msg->offset.z + msg->dimension.z <= 6 );
				CHECK_ERR( IsZero( msg->offset.w ) and IsZero( msg->dimension.w ) );
				CHECK_ERR( IsNotZero( msg->dimension.xyz() ) );

				GL_CALL( glGetSubImageFromTexture( _objectId, target, msg->level.Get(),
													msg->offset.x, msg->offset.y, msg->offset.z,
													msg->dimension.x, msg->dimension.y, msg->dimension.z,
													fmt, type,
													(GLsizei) isize(msg->writableBuffer->Size()),
													(void*) msg->writableBuffer->ptr() ) );
				break;
			}
			default :
				RETURN_ERR( "unsupported image type!" );
		}
		
		msg->result.Set( *msg->writableBuffer );	// TODO: calc image size
		return true;*/
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool GL4Memory::_WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported
		CHECK_ERR( not msg->data.Empty() );
		
		// write without mapping
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		SendTo( _GetParents().Front(), req_descr );
		
		const uint4		level_size	= ImageUtils::LevelDimension( req_descr->result->imageType, req_descr->result->dimension, msg->level.Get() );
		const BytesU	bpp			= BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));
		const GLenum	target		= GL4Enum( req_descr->result->imageType );
		
		CHECK_ERR( All( msg->offset + msg->dimension <= level_size ) );
		CHECK_ERR( msg->level < req_descr->result->maxLevel );
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( req_descr->result->format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( msg->rowPitch % 4_b == 0 )		row_align = 4_b;	else
		if ( msg->rowPitch % 2_b == 0 )		row_align = 2_b;	else
											row_align = 1_b;
		
		ASSERT( msg->rowPitch == GXImageUtils::AlignedRowSize( msg->dimension.x, bpp, row_align ) );
		ASSERT( msg->slicePitch == GXImageUtils::AlignedSliceSize( msg->dimension.xy(), bpp, row_align, row_align ) );
		CHECK_ERR( msg->data.Size() == msg->dimension.z * msg->slicePitch );
		
		GL_CALL( glBindTexture( target, _objectId ) );
		GL_CALL( glPixelStorei( GL_UNPACK_ALIGNMENT, (GLint) isize(row_align) ) );

		switch ( req_descr->result->imageType )
		{
			case EImage::Tex1D :
			{
				CHECK_ERR( level_size.x <= (msg->offset.x + msg->dimension.x) );
				CHECK_ERR( IsNotZero( msg->dimension.x ) );
				CHECK_ERR( IsZero( msg->offset.yzw() ) );
				CHECK_ERR( All( msg->dimension.yzw() == uint3(1) ) );

				GL_CALL( glTexSubImage1D(	target, msg->level.Get(),
											msg->offset.x, msg->dimension.x,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::Tex2D :
			{
				CHECK_ERR( All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ) );
				CHECK_ERR( IsNotZero( msg->dimension.xy() ) );
				CHECK_ERR( IsZero( msg->offset.zw() ) );
				CHECK_ERR( All( msg->dimension.zw() == uint2(1) ) );

				GL_CALL( glTexSubImage2D(	target, msg->level.Get(),
											msg->offset.x, msg->offset.y,
											msg->dimension.x, msg->dimension.y,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::Tex2DArray :
			{
				CHECK_ERR( All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ) );
				CHECK_ERR( IsNotZero( msg->dimension.xy() ) );
				CHECK_ERR( msg->offset.z == 0 and msg->dimension.z == 1 );

				GL_CALL( glTexSubImage3D(	target, msg->level.Get(),
											msg->offset.x, msg->offset.y, msg->offset.w,
											msg->dimension.x, msg->dimension.y, 1,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::Tex3D :
			{
				CHECK_ERR( All( level_size.xyz() >= (msg->offset.xyz() + msg->dimension.xyz()) ) );
				CHECK_ERR( IsNotZero( msg->dimension.xyw() ) );
				CHECK_ERR( msg->offset.w == 0 and msg->dimension.w == 1 );
				
				GL_CALL( glTexSubImage3D(	target, msg->level.Get(),
											msg->offset.x, msg->offset.y, msg->offset.z,
											msg->dimension.x, msg->dimension.y, msg->dimension.z,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::TexCube :
			{
				CHECK_ERR( All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ) );
				CHECK_ERR( IsNotZero( msg->dimension.xy() ) );
				CHECK_ERR( IsZero( msg->offset.zw() ) );
				CHECK_ERR( All( msg->dimension.zw() == uint(1) ) );
				
				GL_CALL( glBindTexture( GL_TEXTURE_CUBE_MAP_POSITIVE_X + msg->offset.z, 0 ) );
				GL_CALL( glTexSubImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X + msg->offset.z, msg->level.Get(),
											msg->offset.x, msg->offset.y,
											msg->dimension.x, msg->dimension.y,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::TexCubeArray :
			{
				CHECK_ERR( All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ) );
				CHECK_ERR( IsNotZero( msg->dimension.xy() ) );
				CHECK_ERR( msg->offset.z == 0 and msg->dimension.z == 1 );

				GL_CALL( glTexSubImage3D(	target, msg->level.Get(),
											msg->offset.x, msg->offset.y, msg->offset.w * req_descr->result->dimension.z + msg->offset.z,
											msg->dimension.x, msg->dimension.y, 1,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
		}

		GL_CALL( glBindTexture( target, 0 ) );
		
		msg->wasWritten.Set( BytesUL(msg->data.Size()) );	// TODO: calc real size
		
		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, msg->memOffset, BytesUL(msg->data.Size()) });	// TODO: calc mipmap and layer offset
		return true;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformGL::GL4Memory >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
