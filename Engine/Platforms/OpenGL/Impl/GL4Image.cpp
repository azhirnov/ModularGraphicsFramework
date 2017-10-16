// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4Image.h"
#include "Engine/Platforms/OpenGL/OpenGLThread.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
	const Runtime::VirtualTypeList	GL4Image::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	GL4Image::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Image::GL4Image (const GlobalSystemsRef gs, const CreateInfo::GpuImage &ci) :
		GL4BaseModule( gs, ci.gpuThread, ModuleConfig{ GLImageModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_imageId( 0 )
	{
		SetDebugName( "GL4Image" );

		_SubscribeOnMsg( this, &GL4Image::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4Image::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4Image::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4Image::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4Image::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4Image::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4Image::_Link_Impl );
		_SubscribeOnMsg( this, &GL4Image::_Compose );
		_SubscribeOnMsg( this, &GL4Image::_Delete );
		_SubscribeOnMsg( this, &GL4Image::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4Image::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &GL4Image::_GetGLImageID );
		_SubscribeOnMsg( this, &GL4Image::_GetImageDescriptor );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, GLThreadModuleID, true );

		Utils::ValidateDescriptor( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Image::~GL4Image ()
	{
		ASSERT( _imageId == 0 );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4Image::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateImage() );

		CHECK_ERR( Module::_DefCompose( true ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool GL4Image::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyImage();

		return Module::_Delete_Impl( msg );
	}
		
/*
=================================================
	_IsCreated
=================================================
*/
	bool GL4Image::_IsCreated () const
	{
		return _imageId != 0;
	}

/*
=================================================
	_CreateImage
=================================================
*/
	bool GL4Image::_CreateImage ()
	{
		using namespace gl;
		
		CHECK_ERR( _imageId == 0 );

		const GLenum	target = GL4Enum( _descr.target );
		
		GL_CHECK( glGenTextures( 1, &_imageId ) );
		
		GL_CALL( glActiveTexture( GL_TEXTURE0 ) );
		GL_CALL( glBindTexture( target, _imageId ) );

		switch ( _descr.target )
		{
			case EImage::Tex2D :
			{
				GL4InternalPixelFormat	ifmt;
				GL4PixelFormat			fmt;
				GL4PixelType			type;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt, OUT fmt, OUT type ) );

				for (uint level = 0; level < _descr.maxLevel.Get(); ++level)
				{
					const uint2	size = Max( _descr.dimension.xy() >> level, uint2(1) );
					GL_CHECK( glTexImage2D( target, level, ifmt, size.x, size.y, 0, fmt, type, null ) );
				}
				break;
			}

			case EImage::Tex2DMS :
			{
				GL4InternalPixelFormat	ifmt;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt ) );
				
				GL_CHECK( glTexImage2DMultisample( target, _descr.samples.Get(), ifmt, _descr.dimension.x, _descr.dimension.y, true ) );
				break;
			}
			
			case EImage::Tex2DArray :
			{
				GL4InternalPixelFormat	ifmt;
				GL4PixelFormat			fmt;
				GL4PixelType			type;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt, OUT fmt, OUT type ) );

				for (uint level = 0; level < _descr.maxLevel.Get(); ++level)
				{
					const uint2	size = Max( _descr.dimension.xy() >> level, uint2(1) );
					GL_CHECK( glTexImage3D( target, level, ifmt, size.x, size.y, _descr.dimension.w, 0, fmt, type, null ) );
				}
				break;
			}

			case EImage::Tex2DMSArray :
			{
				GL4InternalPixelFormat	ifmt;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt ) );

				GL_CHECK( glTexImage3DMultisample( target, _descr.samples.Get(), ifmt, _descr.dimension.x, _descr.dimension.y,
													_descr.dimension.w, true ) );
				break;
			}
			
			case EImage::Tex3D :
			{
				GL4InternalPixelFormat	ifmt;
				GL4PixelFormat			fmt;
				GL4PixelType			type;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt, OUT fmt, OUT type ) );

				for (uint level = 0; level < _descr.maxLevel.Get(); ++level)
				{
					const uint3	size = Max( _descr.dimension.xyz() >> level, uint3(1) );
					GL_CHECK( glTexImage3D( target, level, ifmt, size.x, size.y, size.z, 0, fmt, type, null ) );
				}
				break;
			}
			
			case EImage::TexCube :
			{
				GL4InternalPixelFormat	ifmt;
				GL4PixelFormat			fmt;
				GL4PixelType			type;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt, OUT fmt, OUT type ) );

				for (uint level = 0; level < _descr.maxLevel.Get(); ++level)
				{
					const uint2	size = Max( _descr.dimension.xy() >> level, uint2(1) );

					for (uint i = 0; i < _descr.dimension.z; ++i) {
						GL_CHECK( glTexImage2D( gl::GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, ifmt,
												size.x, size.y, 0, fmt, type, null ) );
					}
				}
				break;
			}
			
			case EImage::TexCubeArray :
			{
				GL4InternalPixelFormat	ifmt;
				GL4PixelFormat			fmt;
				GL4PixelType			type;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt, OUT fmt, OUT type ) );

				for (uint level = 0; level < _descr.maxLevel.Get(); ++level)
				{
					const uint2	size = Max( _descr.dimension.xy() >> level, uint2(1) );
					GL_CHECK( glTexImage3D( target, level, ifmt, size.x, size.y, _descr.dimension.z * _descr.dimension.w,
											0, fmt, type, null ) );
				}
				break;
			}
			
			default :
			{
				_DestroyImage();
				RETURN_ERR( "invalid texture target" );
			}
		}
		
		GL_CALL( glTexParameteri( target, GL_TEXTURE_BASE_LEVEL, GLint(0) ) );
		GL_CALL( glTexParameteri( target, GL_TEXTURE_MAX_LEVEL,  GLint(_descr.maxLevel.Get()) ) );
		return true;
	}

/*
=================================================
	_DestroyImage
=================================================
*/
	void GL4Image::_DestroyImage ()
	{
		using namespace gl;
		
		GL_CALL( glDeleteTextures( 1, &_imageId ) );

		_imageId	= 0;
		_descr		= Uninitialized;
	}

/*
=================================================
	_GetGLImageID
=================================================
*/
	bool GL4Image::_GetGLImageID (const Message< GpuMsg::GetGLImageID > &msg)
	{
		msg->result.Set( _imageId );
		return true;
	}

/*
=================================================
	_GetImageDescriptor
=================================================
*/
	bool GL4Image::_GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
