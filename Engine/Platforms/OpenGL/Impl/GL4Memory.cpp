// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLContext.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{

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


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		gl::GLuint				_objectId;
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

		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
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
		_objectId( 0 ),							_mappedPtr( null ),
		_flags( ci.memFlags ),					_access( ci.access & _GpuMemoryToMemoryAccess( ci.memFlags ) ),
		_binding( EBindingTarget::Unbinded ),	_isMappedMemChanged( false )
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
		_SubscribeOnMsg( this, &GL4Memory::_DeviceBeforeDestroy );
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

		_AttachSelfToManager( ci.gpuThread, Platforms::GLThreadModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	GL4Memory::~GL4Memory ()
	{
		ASSERT( not _IsCreated() );
		ASSERT( not _IsMapped() );
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
	_IsMapped
=================================================
*/
	bool GL4Memory::_IsMapped () const
	{
		return _mappedPtr != null;
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
		_objectId			= 0;
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
	bool GL4Memory::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool GL4Memory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
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
	bool GL4Memory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
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
	bool GL4Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
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
	bool GL4Memory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool GL4Memory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &msg)
	{
		TODO( "" );
		return true;
	}

/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool GL4Memory::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool GL4Memory::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &)
	{
		TODO( "" );
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
	_GpuMemoryToMemoryAccess
=================================================
*/
	EMemoryAccess::bits GL4Memory::_GpuMemoryToMemoryAccess (EGpuMemory::bits flags)
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
	bool GL4Memory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
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
	bool GL4Memory::_GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &msg)
	{
		GpuMemoryDescriptor		descr;

		descr.flags	= _flags;
		descr.size	= _size;

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
		using namespace gl;

		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( not msg->size or not msg->writableBuffer or (*msg->size == BytesUL(msg->writableBuffer->Size())) );
		
		// just read from mapped memory
		if ( _IsMapped() )
		{
			TODO( "" );
			return false;
		}
		
		// read without mapping
		CHECK_ERR( msg->offset < _size );
		CHECK_ERR( msg->writableBuffer and not msg->writableBuffer->Empty() );
		
		const usize	size = Min( usize(_size), usize(msg->writableBuffer->Size()) );
		
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
		using namespace gl;

		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		
		// just write to mapped memory
		if ( _IsMapped() )
		{
			TODO( "" );
			return false;
		}
		
		// write without mapping
		CHECK_ERR( msg->offset < _size );
		
		const usize	size = Min( usize(_size), usize(msg->data.Size()) );

		GL_CALL( glBindBuffer( GL_COPY_READ_BUFFER, _objectId ) );
		GL_CALL( glBufferSubData( GL_COPY_READ_BUFFER, (GLintptr) msg->offset, (GLsizei) size, msg->data.ptr() ) );

		/*GL_CALL( glNamedBufferSubData( _objectId,
									   (GLintptr) msg->offset,
									   (GLsizei) size,
									   msg->data.ptr() ) );*/

		msg->wasWritten.Set( BytesUL(size) );
		// TODO: event - region changed
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
			  " there is no way instead of glGetTextureSubImage (4.5) to get subregion of image" );
		return false;

		/*using namespace gl;

		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		
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
		using namespace gl;

		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( not msg->data.Empty() );
		
		// just write to mapped memory
		if ( _IsMapped() )
		{
			TODO( "" );
			return false;
		}
		
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
		
		ASSERT( msg->rowPitch == GXMath::ImageUtils::AlignedRowSize( msg->dimension.x, bpp, row_align ) );
		ASSERT( msg->slicePitch == GXMath::ImageUtils::AlignedSliceSize( msg->dimension.xy(), bpp, row_align, row_align ) );
		CHECK_ERR( msg->data.Size() == msg->dimension.z * msg->slicePitch );
		
		GL_CALL( glBindTexture( target, _objectId ) );
		GL_CALL( glPixelStorei( GL_UNPACK_ALIGNMENT, (GLint) isize(row_align) ) );

		switch ( req_descr->result->imageType )
		{
			case EImage::Tex1D :
			{
				const bool	fullsize = All( level_size.x == msg->dimension.x );
				
				CHECK_ERR( level_size.x <= (msg->offset.x + msg->dimension.x) );
				CHECK_ERR( IsNotZero( msg->dimension.x ) );
				CHECK_ERR( IsZero( msg->offset.yzw() ) );
				CHECK_ERR( All( msg->dimension.yzw() == uint3(1) ) );
				
				if ( IsZero( msg->offset.x ) and fullsize )
				{
					GL_CALL( glTexImage1D( target, msg->level.Get(), ifmt,
											msg->dimension.x, 0,
											fmt, type,
											msg->data.ptr() ) );
					break;
				}

				GL_CALL( glTexSubImage1D(	target, msg->level.Get(),
											msg->offset.x, msg->dimension.x,
											fmt, type,
											msg->data.ptr() ) );
				break;
			}
			case EImage::Tex2D :
			{
				const bool	fullsize = All( level_size.xy() == msg->dimension.xy() );

				CHECK_ERR( All( level_size.xy() >= (msg->offset.xy() + msg->dimension.xy()) ) );
				CHECK_ERR( IsNotZero( msg->dimension.xy() ) );
				CHECK_ERR( IsZero( msg->offset.zw() ) );
				CHECK_ERR( All( msg->dimension.zw() == uint2(1) ) );

				if ( IsZero( msg->offset.xy() ) and fullsize )
				{
					GL_CALL( glTexImage2D( target, msg->level.Get(), ifmt,
											msg->dimension.x, msg->dimension.y,
											0, fmt, type,
											msg->data.ptr() ) );
					break;
				}

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
				const bool	fullsize = All( level_size.xyz() == msg->dimension.xyz() );

				CHECK_ERR( All( level_size.xyz() >= (msg->offset.xyz() + msg->dimension.xyz()) ) );
				CHECK_ERR( IsNotZero( msg->dimension.xyw() ) );
				CHECK_ERR( msg->offset.w == 0 and msg->dimension.w == 1 );
				
				if ( IsZero( msg->offset.xyz() ) and fullsize )
				{
					GL_CALL( glTexImage3D( target, msg->level.Get(), ifmt,
											msg->dimension.x, msg->dimension.y, msg->dimension.z,
											0, fmt, type,
											msg->data.ptr() ) );
					break;
				}

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
		// TODO: event - region changed
		return true;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLContext::_CreateGL4Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformGL::GL4Memory >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
