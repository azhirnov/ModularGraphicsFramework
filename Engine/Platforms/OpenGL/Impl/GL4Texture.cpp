// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4Texture.h"
#include "Engine/Platforms/OpenGL/OpenGLThread.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
	const Runtime::VirtualTypeList	GL4Texture::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	GL4Texture::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Texture::GL4Texture (const GlobalSystemsRef gs, const CreateInfo::GpuTexture &ci) :
		GL4BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_imageId( 0 )
	{
		SetDebugName( "GL4Texture" );

		_SubscribeOnMsg( this, &GL4Texture::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_Link_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_Compose );
		_SubscribeOnMsg( this, &GL4Texture::_Delete );
		_SubscribeOnMsg( this, &GL4Texture::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4Texture::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &GL4Texture::_GetGLTextureID );
		_SubscribeOnMsg( this, &GL4Texture::_GetGpuTextureDescriptor );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, OpenGLThread::GetStaticID(), true );

		Utils::ValidateDescriptor( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Texture::~GL4Texture ()
	{
		ASSERT( _imageId == 0 );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4Texture::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( _CreateTexture() );

		CHECK_ERR( Module::_Compose( true ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool GL4Texture::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyTexture();

		return Module::_Delete_Impl( msg );
	}
		
/*
=================================================
	_IsCreated
=================================================
*/
	bool GL4Texture::_IsCreated () const
	{
		return _imageId != 0;
	}

/*
=================================================
	_CreateTexture
=================================================
*/
	bool GL4Texture::_CreateTexture ()
	{
		using namespace gl;
		
		CHECK_ERR( _imageId == 0 );

		const GLenum	target = GL4Enum( _descr.target );
		
		GL_CHECK( glGenTextures( 1, &_imageId ) );
		
		GL_CALL( glActiveTexture( GL_TEXTURE0 ) );
		GL_CALL( glBindTexture( target, _imageId ) );

		switch ( _descr.target )
		{
			case ETexture::Tex2D :
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

			case ETexture::Tex2DMS :
			{
				GL4InternalPixelFormat	ifmt;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt ) );
				
				GL_CHECK( glTexImage2DMultisample( target, _descr.samples.Get(), ifmt, _descr.dimension.x, _descr.dimension.y, true ) );
				break;
			}
			
			case ETexture::Tex2DArray :
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

			case ETexture::Tex2DMSArray :
			{
				GL4InternalPixelFormat	ifmt;
				CHECK_ERR( GL4Enum( _descr.format, OUT ifmt ) );

				GL_CHECK( glTexImage3DMultisample( target, _descr.samples.Get(), ifmt, _descr.dimension.x, _descr.dimension.y,
													_descr.dimension.w, true ) );
				break;
			}
			
			case ETexture::Tex3D :
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
			
			case ETexture::TexCube :
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
			
			case ETexture::TexCubeArray :
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
				_DestroyTexture();
				RETURN_ERR( "invalid texture target" );
			}
		}
		
		GL_CALL( glTexParameteri( target, GL_TEXTURE_BASE_LEVEL, GLint(0) ) );
		GL_CALL( glTexParameteri( target, GL_TEXTURE_MAX_LEVEL,  GLint(_descr.maxLevel.Get()) ) );
		return true;
	}

/*
=================================================
	_DestroyTexture
=================================================
*/
	void GL4Texture::_DestroyTexture ()
	{
		using namespace gl;
		
		GL_CALL( glDeleteTextures( 1, &_imageId ) );

		_imageId	= 0;
		_descr		= Uninitialized;
	}

/*
=================================================
	_GetGLTextureID
=================================================
*/
	bool GL4Texture::_GetGLTextureID (const Message< ModuleMsg::GetGLTextureID > &msg)
	{
		msg->result.Set( _imageId );
		return true;
	}

/*
=================================================
	_GetGpuTextureDescriptor
=================================================
*/
	bool GL4Texture::_GetGpuTextureDescriptor (const Message< ModuleMsg::GetGpuTextureDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
