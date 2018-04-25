// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"

namespace Engine
{
namespace PlatformGL
{
	
	//
	// OpenGL System Framebuffer
	//

	class GL4Device::GL4SystemFramebuffer final : public GL4BaseModule
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
		gl::GLuint					_framebufferId;


	// methods
	public:
		explicit GL4SystemFramebuffer (GlobalSystemsRef gs);
		~GL4SystemFramebuffer ();

		bool CreateFramebuffer (const uint2 &surfaceSize, EPixelFormat::type colorFmt,
								EPixelFormat::type depthStencilFmt, MultiSamples samples);

		void Resize (const uint2 &surfaceSize);


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLFramebufferID (const Message< GpuMsg::GetGLFramebufferID > &);
		bool _GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &);

	private:
		bool _IsCreated () const;
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4Device::GL4SystemFramebuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4Device::GL4SystemFramebuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Device::GL4SystemFramebuffer::GL4SystemFramebuffer (GlobalSystemsRef gs) :
		GL4BaseModule( gs, ModuleConfig{ GLFramebufferModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_framebufferId( 0 )
	{
		SetDebugName( "GL4SystemFramebuffer" );

		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_Delete );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_GetGLFramebufferID );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_GetFramebufferDescriptor );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4SystemFramebuffer::_GetGLPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( null ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Device::GL4SystemFramebuffer::~GL4SystemFramebuffer ()
	{
	}
	
/*
=================================================
	CreateFramebuffer
=================================================
*/
	bool GL4Device::GL4SystemFramebuffer::CreateFramebuffer (const uint2 &surfaceSize, EPixelFormat::type colorFmt,
															 EPixelFormat::type depthStencilFmt, MultiSamples samples)
	{
		using namespace gl;

		_framebufferId	= 0;
		_descr			= Uninitialized;

		GL_CALL( glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&_framebufferId ) );

		const bool	is_multisampled	= samples.Get() > 1;
		
		if ( colorFmt != EPixelFormat::Unknown )
		{
			_descr.ColorRenderbuffer( "color", is_multisampled ? EImage::Tex2DMS : EImage::Tex2D );
		}

		if ( depthStencilFmt != EPixelFormat::Unknown )
		{
			_descr.DepthStencilRenderbuffer( "depth", is_multisampled ? EImage::Tex2DMS : EImage::Tex2D );
		}

		Resize( surfaceSize );

		CHECK( _SetState( EState::ComposedImmutable ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterCompose >({});
		return true;
	}
	
/*
=================================================
	Resize
=================================================
*/
	void GL4Device::GL4SystemFramebuffer::Resize (const uint2 &surfaceSize)
	{
		_descr.size		= surfaceSize;
		_descr.layers	= 1;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GL4Device::GL4SystemFramebuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_descr			= Uninitialized;
		_framebufferId	= 0;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGLFramebufferID
=================================================
*/
	bool GL4Device::GL4SystemFramebuffer::_GetGLFramebufferID (const Message< GpuMsg::GetGLFramebufferID > &msg)
	{
		msg->result.Set( _framebufferId );
		return true;
	}

/*
=================================================
	_GetFramebufferDescriptor
=================================================
*/
	bool GL4Device::GL4SystemFramebuffer::_GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
