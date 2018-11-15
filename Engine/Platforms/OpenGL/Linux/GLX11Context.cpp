// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_LINUX )

#include "Engine/Platforms/OpenGL/Linux/GLX11Context.h"
#include "Core/STL/OS/Posix/PosixHeader.h"
#include "External/opengl/glxext.h"
#include <GL/gl.h>

namespace Engine
{
namespace PlatformGL
{
	
/*
=================================================
	constructor
=================================================
*/
	GLX11Context::GLX11Context () :
		glxSwapIntervalEXT( UninitializedT< PFNGLXSWAPINTERVALEXTPROC >() ),
		glxSwapIntervalMESA( UninitializedT< PFNGLXSWAPINTERVALMESAPROC >() ),
		glxSwapIntervalSGI( UninitializedT< PFNGLXSWAPINTERVALSGIPROC >() ),
		glxGetSwapIntervalMESA( UninitializedT< PFNGLXGETSWAPINTERVALMESAPROC >() ),
		glxCreateContextAttribsARB( UninitializedT< PFNGLXCREATECONTEXTATTRIBSARBPROC >() ),
		_display( UninitializedT< Display *>() ),
		_window( UninitializedT< Window >() ),
		_renderContext( UninitializedT< GLXContext >() )
	{
	}
	
/*
=================================================
	_IsExtensionSupported
=================================================
*/
	bool GLX11Context::_IsExtensionSupported (StringCRef extension) const
	{
		Display*	display		= wnd.display.Get< Display *>();
		StringCRef	glx_exts	= glXQueryExtensionsString( display, ::DefaultScreen( display ) );
		value		pos			= 0;

		if ( glx_exts.FindIC( extension, OUT pos ) )
		{
			if ( glx_exts[pos-1] == ' ' and glx_exts[pos + extension.Length() + 1] == ' ' )
				return true;
		}

		return false;
	}

/*
=================================================
	Create
=================================================
*/
	bool GLX11Context::Create (const PlatformTools::WindowHelper::XlibWindow &wnd, INOUT GraphicsSettings &vs)
	{
		Destroy();
		
		_display	= wnd.display;
		_window		= wnd.window;

		glxSwapIntervalEXT = Cast<PFNGLXSWAPINTERVALEXTPROC>( glXGetProcAddress("glXSwapIntervalEXT") );
		glxSwapIntervalMESA = Cast<PFNGLXSWAPINTERVALMESAPROC>( glXGetProcAddress("glXSwapIntervalMESA") );
		glxSwapIntervalSGI = Cast<PFNGLXSWAPINTERVALSGIPROC>( glXGetProcAddress("glXSwapIntervalSGI") );
		glxGetSwapIntervalMESA = Cast<PFNGLXGETSWAPINTERVALMESAPROC>( glXGetProcAddress("glXGetSwapIntervalMESA") );
		glxCreateContextAttribsARB = Cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>( glXGetProcAddress("glXCreateContextAttribsARB") );

		CHECK_ERR( _InitOpenGL( INOUT vs ) );
		return true;
	}

/*
=================================================
	Destroy
=================================================
*/
	void GLX11Context::Destroy ()
	{
		if ( _display.IsNotNull< Display *>()	and
			 _window.IsNotNull< Window >() )
		{
			glXMakeCurrent( _display.Get< Display *>(), _window.Get< Window >(), null );
		}

		if ( _renderContext.IsNotNull< GLXContext >() )
		{
	        glXDestroyContext( _display.Get< Display *>(), _renderContext.Get< GLXContext >() );
		}

		glxSwapIntervalEXT			= null;
		glxSwapIntervalMESA			= null;
		glxSwapIntervalSGI			= null;
		glxGetSwapIntervalMESA		= null;
		glxCreateContextAttribsARB	= null;

		_display		= null;
		_window			= null;
		_renderContext	= null;
	}
	
/*
=================================================
	_GetApiVersion
=================================================
*/
	static bool _GetApiVersion (const GraphicsSettings &vs, OUT uint &version)
	{
		using namespace Engine::CreateInfo;

		auto	api_name = vs.GetAPIName();
		CHECK_ERR( api_name.StartsWithIC( "GL" ) or api_name.StartsWithIC( "opengl" ) );

		version = vs.GetAPIVersion();
		return true;
	}
	
/*
=================================================
	_ConvertColorFormat
=================================================
*/
	static EPixelFormat::type _ConvertColorFormat (uint colorBits, uint)
	{
		if ( colorBits == 32 )
			return EPixelFormat::RGBA8_UNorm;
		else
			return EPixelFormat::RGB8_UNorm;
	}

/*
=================================================
	_ConvertDepthStencilFormat
=================================================
*/
	static EPixelFormat::type _ConvertDepthStencilFormat (uint depthBits, uint stencilBits)
	{
		if ( depthBits == 32 )
		{
			if ( stencilBits == 8 )
				return EPixelFormat::Depth32F_Stencil8;
			else
				return EPixelFormat::Depth32F;
		}

		if ( depthBits == 24 )
		{
			if ( stencilBits == 8 )
				return EPixelFormat::Depth24_Stencil8;
			else
				return EPixelFormat::Depth24;
		}

		if ( depthBits == 16 )
		{
			if ( stencilBits == 8 )
				return EPixelFormat::Depth16_Stencil8;
			else
				return EPixelFormat::Depth16;
		}

		return EPixelFormat::Unknown;
	}
	
/*
=================================================
	_ConvertGLVersion
=================================================
*/
	static GAPI::type _ConvertGLVersion (uint version)
	{
		return GAPI::FromString( "GL "_str << (version / 100) << '.' << ((version / 10) % 10) );
	}

/*
=================================================
	_InitOpenGL
=================================================
*/
	bool GLX11Context::_InitOpenGL (INOUT GraphicsSettings &vs)
	{
		vs.flags.Reset( EFlags::NoSurface );
		
		// extract data from video settings
		uint	version;
		CHECK_ERR( _GetApiVersion( vs, OUT version ) );
		
		const auto	color_bits			= EPixelFormat::BitsPerChannel( vs.colorFmt ).To< uint4 >();
		const auto	depth_stencil_bits	= EPixelFormat::BitsPerChannel( vs.depthStencilFmt ).To< uint2 >();

		CHECK_ERR( glxCreateContextAttribsARB.IsNotNull<PFNGLXCREATECONTEXTATTRIBSARBPROC>() );
		
		Display *		display			= _display.Get< Display *>();
		GLXFBConfig		native			= null;
		int				native_count	= 0;
		GLXContext		rc				= null;
		GLXFBConfig *	native_configs	= glXGetFBConfigs( display, DefaultScreen( display ), OUT &native_count );

		CHECK_ERR( native_configs and native_count > 0 );


		const bool	is_core_version =	( version >= 330 );	// 3.3 or higher

		// Create OpenGL Context //
		int	context_attribs[] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, int(version / 100),
			GLX_CONTEXT_MINOR_VERSION_ARB, int((version / 10) % 10),
			GLX_CONTEXT_FLAGS_ARB,         GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB |
												(vs.flags[ EFlags::DebugContext ] ? GLX_CONTEXT_DEBUG_BIT_ARB : 0) |
												(vs.flags[ EFlags::NoErrorContext ] ? GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR : 0),
			GLX_CONTEXT_PROFILE_MASK_ARB,  is_core_version ?
												GLX_CONTEXT_CORE_PROFILE_BIT_ARB :
												GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};

		// search for valid context attribs
		for (;;)
		{
			rc = glxCreateContextAttribsARB.Get<PFNGLXCREATECONTEXTATTRIBSARBPROC>()( display,
																					  native,
																					  null,
																					  True,
																					  context_attribs );
			if ( rc != null or context_attribs[1] <= 2 )
				break;
				
			// for opengl 4.x
			if ( context_attribs[1] < 4 )
			{
				if ( context_attribs[5] & GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR )
				{
					context_attribs[5] = context_attribs[5] & ~GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR;
					vs.flags.Reset( EFlags::NoErrorContext );
					continue;
				}
				else
				if ( context_attribs[5] & GLX_CONTEXT_DEBUG_BIT_ARB )
				{
					context_attribs[5] = context_attribs[5] & ~GLX_CONTEXT_DEBUG_BIT_ARB;
					vs.flags.Reset( EFlags::DebugContext );
					continue;
				}
			}

			// decrease version number
			if ( context_attribs[3] == 0 )
			{
				context_attribs[1]--;
				context_attribs[3] = 9;
				continue;
			}
			
			context_attribs[3]--;
		}
		
		// create compatibility profile context
		if ( rc == null )
		{
			rc = glXCreateNewContext( display,
									  fbconfig,
									  GLX_RGBA_TYPE,
									  null,
									  True );
		}

		// Synchronize to make sure all commands are processed
		XSync( display, False );
		XSetErrorHandler( null );
		
		CHECK_ERR( rc != null );
		_renderContext = rc;

		MakeCurrent();
		
		// set vertical synchronization
		if ( glxSwapIntervalEXT )
		{
			glxSwapIntervalEXT.Get<PFNGLXSWAPINTERVALEXTPROC>()( display,
																 _window.Get< Window >(),
															     int(vs.flags[ EFlags::VSync ]) );
		}
		else
		if ( glxSwapIntervalMESA )
		{
			glxSwapIntervalMESA.Get<PFNGLXSWAPINTERVALMESAPROC>()( int(vs.flags[ EFlags::VSync ]) );
		}
		else
		if ( glxSwapIntervalSGI )
		{
			glxSwapIntervalSGI.Get<PFNGLXSWAPINTERVALSGIPROC>()( int(vs.flags[ EFlags::VSync ]) );
		}
		else
			vs.flags.Reset( EFlags::VSync );
		
		// update settings
		{
			vs.device	= Cast<const char*>(glGetString( GL_RENDERER ));
			vs.version	= _ConvertGLVersion( version );
		}

		return true;
	}
	
/*
=================================================
	MakeCurrent
=================================================
*/
	void GLX11Context::MakeCurrent ()
	{
		ASSERT( IsCreated() );
		CHECK( glXMakeCurrent( _display.Get< Display *>(), _window.Get< Window >(), _renderContext.Get< GLXContext >() ) );
	}
	
/*
=================================================
	ResetCurrent
=================================================
*/
	void GLX11Context::ResetCurrent ()
	{
		ASSERT( IsCreated() );
		CHECK( glXMakeCurrent( _display.Get< Display *>(), _window.Get< Window >(), null ) );
	}
	
/*
=================================================
	IsCreated
=================================================
*/
	bool GLX11Context::IsCreated () const
	{
		return _display.IsNotNull< Display *>() and _renderContext.IsNotNull< GLXContext >();
	}
	
/*
=================================================
	IsCurrent
=================================================
*/
	bool GLX11Context::IsCurrent ()
	{
		return IsCreated() and glXGetCurrentContext() == _renderContext.Get< GLXContext >();
	}
	
/*
=================================================
	SwapBuffers
=================================================
*/
	void GLX11Context::SwapBuffers ()
	{
		ASSERT( IsCreated() );
		if ( IsCreated() )
		{
			glXSwapBuffers( _display.Get<Display *>(), _window.Get<Window>() );
		}
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_LINUX
