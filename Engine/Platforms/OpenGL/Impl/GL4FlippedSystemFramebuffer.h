// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	System framebuffer for compatibility with Vulkan and DirectX window coordinate system.
*/

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4Device.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"

#ifdef GRAPHICS_API_OPENGL

namespace Engine
{
namespace PlatformGL
{
	
	//
	// OpenGL Flipped System Framebuffer
	//

	class GL4Device::GL4FlippedSystemFramebuffer final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											GpuMsg::GetFramebufferDescriptor,
											GpuMsg::GetGLFramebufferID
										> >;

		using SupportedEvents_t		= MessageListFrom< ModuleMsg::Delete >;
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		FramebufferDescriptor		_descr;
		gl::GLuint					_framebufferId;			// top - y max, bottom - 0
		gl::GLuint					_flippedFramebufferId;	// top - 0, bottom - y max
		gl::GLuint					_colorBufferId;
		gl::GLuint					_depthBufferId;

		EPixelFormat::type			_colorFormat;
		EPixelFormat::type			_depthStencilFormat;
		MultiSamples				_samples;


	// methods
	public:
		explicit GL4FlippedSystemFramebuffer (GlobalSystemsRef gs);
		~GL4FlippedSystemFramebuffer ();

		bool CreateFramebuffer (const uint2 &surfaceSize, EPixelFormat::type colorFmt,
								EPixelFormat::type depthStencilFmt, MultiSamples samples);

		void Resize (const uint2 &surfaceSize);
		void Present ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLFramebufferID (const Message< GpuMsg::GetGLFramebufferID > &);
		bool _GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &);

	private:
		bool _IsCreated () const;

		bool _CreateFlippedFramebuffer ();
		void _DestroyFlippedFramebuffer ();

		static bool _InitializeRenderbuffer (OUT gl::GLenum &attachment, OUT gl::GLuint &id, uint2 size, EPixelFormat::type format, MultiSamples samples);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4Device::GL4FlippedSystemFramebuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4Device::GL4FlippedSystemFramebuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Device::GL4FlippedSystemFramebuffer::GL4FlippedSystemFramebuffer (GlobalSystemsRef gs) :
		GL4BaseModule( gs, ModuleConfig{ GLFramebufferModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_framebufferId( 0 ),			_flippedFramebufferId( 0 ),
		_colorBufferId( 0 ),			_depthBufferId( 0 ),
		_colorFormat( Uninitialized ),	_depthStencilFormat( Uninitialized )
	{
		SetDebugName( "GL4FlippedSystemFramebuffer" );

		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_Delete );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_GetGLFramebufferID );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_GetFramebufferDescriptor );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4FlippedSystemFramebuffer::_GetGLPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( null ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Device::GL4FlippedSystemFramebuffer::~GL4FlippedSystemFramebuffer ()
	{
		_DestroyFlippedFramebuffer();
	}
	
/*
=================================================
	_InitializeRenderbuffer
=================================================
*/
	bool GL4Device::GL4FlippedSystemFramebuffer::_InitializeRenderbuffer (OUT gl::GLenum &attachment, OUT gl::GLuint &id, uint2 size, EPixelFormat::type format, MultiSamples samples)
	{
		using namespace gl;
		
		GL4InternalPixelFormat	ifmt;
		CHECK_ERR( GL4Enum( format, OUT ifmt ) );

		GL_CALL( glGenRenderbuffers( 1, &id ) );
		CHECK_ERR( id != 0 );
		GL_CALL( glBindRenderbuffer( GL_RENDERBUFFER, id ) );
		
		if ( samples.Get() > 1 ) {
			GL_CALL( glRenderbufferStorageMultisample( GL_RENDERBUFFER, samples.Get(), ifmt, size.x, size.y ) );
		} else {
			GL_CALL( glRenderbufferStorage( GL_RENDERBUFFER, ifmt, size.x, size.y ) );
		}

		GL_CALL( glBindRenderbuffer( GL_RENDERBUFFER, 0 ) );

		attachment = 0;

		if ( EPixelFormat::IsColor( format ) )
			attachment = GL_COLOR_ATTACHMENT0;
		else
		if ( EPixelFormat::HasDepth( format ) and EPixelFormat::HasStencil( format ) )
			attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		else
		if ( EPixelFormat::HasDepth( format ) )
			attachment = GL_DEPTH_ATTACHMENT;
		else
		if ( EPixelFormat::HasStencil( format ) )
			attachment = GL_STENCIL_ATTACHMENT;

		return true;
	}
	
/*
=================================================
	_CreateFlippedFramebuffer
=================================================
*/
	bool GL4Device::GL4FlippedSystemFramebuffer::_CreateFlippedFramebuffer ()
	{
		using namespace gl;

		GLenum		attachment = 0;
		
		// create flipped framebuffer
		GL_CALL( glGenFramebuffers( 1, &_flippedFramebufferId ) );
		CHECK_ERR( _flippedFramebufferId != 0 );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _flippedFramebufferId ) );

		// create color attachment
		if ( _colorFormat != EPixelFormat::Unknown )
		{
			CHECK_ERR( _InitializeRenderbuffer( OUT attachment, OUT _colorBufferId, _descr.size, _colorFormat, _samples ) );
			GL_CALL( glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _colorBufferId ) );
		}

		// create depth stencil attachment
		if ( _depthStencilFormat != EPixelFormat::Unknown )
		{
			CHECK_ERR( _InitializeRenderbuffer( OUT attachment, OUT _depthBufferId, _descr.size, _depthStencilFormat, _samples ) );
			GL_CALL( glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _depthBufferId ) );
		}

		// check status
		GLenum   status = 0;
		GL_CALL( status = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER ) );
		CHECK_ERR( status == GL_FRAMEBUFFER_COMPLETE );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );

		return true;
	}
	
/*
=================================================
	_DestroyFlippedFramebuffer
=================================================
*/
	void GL4Device::GL4FlippedSystemFramebuffer::_DestroyFlippedFramebuffer ()
	{
		using namespace gl;

		if ( _flippedFramebufferId != 0 ) {
			GL_CALL( glDeleteFramebuffers( 1, &_flippedFramebufferId ) );
		}

		if ( _colorBufferId != 0 ) {
			GL_CALL( glDeleteRenderbuffers( 1, &_colorBufferId ) );
		}

		if ( _depthBufferId != 0 ) {
			GL_CALL( glDeleteRenderbuffers( 1, &_depthBufferId ) );
		}

		_colorBufferId			= 0;
		_depthBufferId			= 0;
		_flippedFramebufferId	= 0;
	}

/*
=================================================
	CreateFramebuffer
=================================================
*/
	bool GL4Device::GL4FlippedSystemFramebuffer::CreateFramebuffer (const uint2 &surfaceSize, EPixelFormat::type colorFmt,
															 EPixelFormat::type depthStencilFmt, MultiSamples samples)
	{
		using namespace gl;

		_framebufferId		= 0;
		_descr				= Uninitialized;
		_colorFormat		= colorFmt;
		_depthStencilFormat	= depthStencilFmt;
		_samples			= samples;

		// get current binded framebuffer, it must be default framebuffer!
		GL_CALL( glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&_framebufferId ) );

		const bool	is_multisampled	= samples.Get() > 1;
		
		if ( colorFmt != EPixelFormat::Unknown )
		{
			_descr.ColorImage( "color", is_multisampled ? EImage::Tex2DMS : EImage::Tex2D );
		}

		if ( depthStencilFmt != EPixelFormat::Unknown )
		{
			_descr.DepthStencilImage( "depth", is_multisampled ? EImage::Tex2DMS : EImage::Tex2D );
		}

		Resize( surfaceSize );

		CHECK( _SetState( EState::ComposedImmutable ) );
		return true;
	}
	
/*
=================================================
	Resize
=================================================
*/
	void GL4Device::GL4FlippedSystemFramebuffer::Resize (const uint2 &surfaceSize)
	{
		_descr.size		= surfaceSize;
		_descr.layers	= 1;

		_DestroyFlippedFramebuffer();
		_CreateFlippedFramebuffer();
	}
	
/*
=================================================
	Present
=================================================
*/
	void GL4Device::GL4FlippedSystemFramebuffer::Present ()
	{
		using namespace gl;

		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, _flippedFramebufferId ) );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _framebufferId ) );
		GL_CALL( glDrawBuffer( GL_BACK ) );

		GLenum	filter	= GL_NEAREST;
		GLenum	mask	= 0;
		uint2	size	= _descr.size-1;

		if ( _colorFormat != EPixelFormat::Unknown ) {
			mask |= GL_COLOR_BUFFER_BIT;
		}
		if ( _depthStencilFormat != EPixelFormat::Unknown ) {
			mask |= (EPixelFormat::HasDepth( _depthStencilFormat ) ? GL_DEPTH_BUFFER_BIT : 0) |
					(EPixelFormat::HasStencil( _depthStencilFormat ) ? GL_STENCIL_BUFFER_BIT : 0);
		}

		GL_CALL( glDisable( GL_SCISSOR_TEST ) );

		GL_CALL( glBlitFramebuffer( 0, 0, size.x, size.y, 0, size.y, size.x, 0, mask, filter ) );
		
		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 ) );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GL4Device::GL4FlippedSystemFramebuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyFlippedFramebuffer();

		_descr					= Uninitialized;
		_framebufferId			= 0;
		_flippedFramebufferId	= 0;
		_colorBufferId			= 0;
		_depthBufferId			= 0;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGLFramebufferID
=================================================
*/
	bool GL4Device::GL4FlippedSystemFramebuffer::_GetGLFramebufferID (const Message< GpuMsg::GetGLFramebufferID > &msg)
	{
		msg->result.Set( _flippedFramebufferId );
		return true;
	}

/*
=================================================
	_GetFramebufferDescriptor
=================================================
*/
	bool GL4Device::GL4FlippedSystemFramebuffer::_GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
