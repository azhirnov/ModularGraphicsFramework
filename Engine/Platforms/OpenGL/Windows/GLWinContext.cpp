// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

#include "Engine/Platforms/OpenGL/Windows/GLWinContext.h"
#include "Core/STL/OS/Windows/WinHeader.h"
#include "External/opengl/wglext.h"
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
	GLWinContext::GLWinContext () :
		wglSwapInterval( UninitializedT< PFNWGLSWAPINTERVALEXTPROC >() ),
		wglGetSwapInterval( UninitializedT< PFNWGLGETSWAPINTERVALEXTPROC >() ),
		wglChoosePixelFormat( UninitializedT< PFNWGLCHOOSEPIXELFORMATARBPROC >() ),
		wglCreateContextAttribs( UninitializedT< PFNWGLCREATECONTEXTATTRIBSARBPROC >() ),
		swapControlSupported(false),
		_window( UninitializedT< HWND >() ),
		_deviceContext( UninitializedT< HDC >() ),
		_renderContext( UninitializedT< HGLRC >() )
	{
	}
	
/*
=================================================
	Create
=================================================
*/
	bool GLWinContext::Create (const PlatformTools::WindowHelper::WinAPIWindow &wnd, INOUT GraphicsSettings &vs)
	{
		Destroy();
		
		_window = wnd.window;
		CHECK_ERR( _window.IsNotNull<HWND>() );

		_deviceContext = ::GetDC( _window.Get<HWND>() );
		CHECK_ERR( _deviceContext.IsNotNull<HDC>() );

		CHECK_ERR( _InitOpenGL( vs ) );
		return true;
	}

/*
=================================================
	Destroy
=================================================
*/
	void GLWinContext::Destroy ()
	{
		if ( _deviceContext.IsNotNull<HDC>() and
			 _window.IsNotNull<HWND>() )
		{
			CHECK( wglMakeCurrent( _deviceContext.Get<HDC>(), null ) != FALSE );

			::ReleaseDC( _window.Get<HWND>(), _deviceContext.Get<HDC>() );
		}

		if ( _renderContext.IsNotNull<HGLRC>() )
		{
			CHECK( wglDeleteContext( _renderContext.Get<HGLRC>() ) != FALSE );
		}

		wglSwapInterval			= null;
		wglGetSwapInterval		= null;
		wglChoosePixelFormat	= null;
		wglCreateContextAttribs	= null;
		swapControlSupported	= false;

		_deviceContext			= null;
		_renderContext			= null;
		_window					= null;
	}
	
/*
=================================================
	_InitOpenGL2
=================================================
*/
	bool GLWinContext::_InitOpenGL2 ()
	{
		PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
									  32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };

		const HDC	dc			= _deviceContext.Get<HDC>();
		HGLRC		rc			= null;
		uint		curr_fmt	= GetPixelFormat( dc );
		const uint	pixformat	= curr_fmt == 0 ? ChoosePixelFormat( dc, OUT &pfd ) : curr_fmt;
		bool		res			= false;

		if ( pixformat != 0 )
		{
			// An application can only set the pixel format of a window one time. (MDSN)
			if ( pixformat != curr_fmt )
			{
				CHECK( SetPixelFormat( dc, pixformat, &pfd ) != FALSE );
			}

			rc = wglCreateContext( dc );

			if ( rc != null and wglMakeCurrent( dc, rc ) != FALSE )
			{
				wglChoosePixelFormat	= Cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
				wglCreateContextAttribs	= Cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
		
				wglSwapInterval			= Cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
				wglGetSwapInterval		= Cast<PFNWGLGETSWAPINTERVALEXTPROC>(wglGetProcAddress("wglGetSwapIntervalEXT"));

				swapControlSupported	= wglSwapInterval.IsNotNull<PFNWGLSWAPINTERVALEXTPROC>() and
										  wglGetSwapInterval.IsNotNull<PFNWGLGETSWAPINTERVALEXTPROC>();

				res = true;
			}
			CHECK( wglMakeCurrent( dc, null ) != FALSE );

			CHECK( wglDeleteContext( rc ) != FALSE );
		}
		
		return res;
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
	bool GLWinContext::_InitOpenGL (INOUT GraphicsSettings &vs)
	{
		//ASSERT( not vs.flags[ EFlags::NoSurface ] );
		vs.flags.Reset( EFlags::NoSurface );

		// extract data from video settings
		uint	version;
		CHECK_ERR( _GetApiVersion( vs, OUT version ) );

		const auto	color_bits			= EPixelFormat::BitsPerChannel( vs.colorFmt ).To< uint4 >();
		const auto	depth_stencil_bits	= EPixelFormat::BitsPerChannel( vs.depthStencilFmt ).To< uint2 >();


		const HDC	dc	= _deviceContext.Get<HDC>();
		HGLRC		rc	= null;

		if ( dc == null )
			RETURN_ERR( "Window must be created before init OpenGL!" );

		CHECK_ERR( _InitOpenGL2() );

		CHECK_ERR( wglChoosePixelFormat.IsNotNull<PFNWGLCHOOSEPIXELFORMATARBPROC>() );
		CHECK_ERR( wglCreateContextAttribs.IsNotNull<PFNWGLCREATECONTEXTATTRIBSARBPROC>() );
		

		// Chose Pixel Format //
		PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
									  32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };

		uint	pixformat		= 0;

		float	ctx_float_attribs[]	= {0.0, 0.0};

		int		ctx_int_attribs[]	=
		{
			WGL_DRAW_TO_WINDOW_ARB,				true,
			WGL_SUPPORT_OPENGL_ARB,				true,
			WGL_ACCELERATION_ARB,				WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,					color_bits[3] == 0 ? 24 : 32,
			WGL_ALPHA_BITS_ARB,					color_bits[3] == 0 ? 1 : int(color_bits[3]),
			WGL_DEPTH_BITS_ARB,					int(depth_stencil_bits[0]),
			WGL_STENCIL_BITS_ARB,				int(depth_stencil_bits[1]),
			WGL_DOUBLE_BUFFER_ARB,				true,
			WGL_STEREO_ARB,						false,	//vs.flags[ EFlags::Stereo ],
			WGL_PIXEL_TYPE_ARB,					WGL_TYPE_RGBA_ARB,
			WGL_SAMPLE_BUFFERS_ARB,				vs.samples.IsEnabled(),
			WGL_SAMPLES_ARB,					int(vs.samples.Get()),
            WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB,	true,
			0,0
		};

		int rv = 0;

		// validate pixel format
		while ( rv == 0 )
		{
			int		pixelFormat	= 0;
			uint	numFormats	= 0;
			int		valid		= wglChoosePixelFormat.Get<PFNWGLCHOOSEPIXELFORMATARBPROC>()
										( dc, ctx_int_attribs, ctx_float_attribs, 1, OUT &pixelFormat, OUT &numFormats );

			if ( valid != 0 and numFormats > 0 )
				rv = pixelFormat;
			else
			{
				if ( ctx_int_attribs[21] != 0 )		// msaa
					ctx_int_attribs[21] >>= 1;
				else
				if ( ctx_int_attribs[13] != 0 )		// stencil buffer
					ctx_int_attribs[13] = 0;
				else
				if ( ctx_int_attribs[11] > 24 )		// depth buffer
					ctx_int_attribs[11] = 24;
				else
				if ( ctx_int_attribs[11] != 16 )	// depth buffer
					ctx_int_attribs[11] = 16;
				else
					break;
			}
		}

		if ( rv != 0 )
		{
			pixformat			= rv;
			vs.colorFmt			= _ConvertColorFormat( ctx_int_attribs[7], ctx_int_attribs[9] );
			vs.depthStencilFmt	= _ConvertDepthStencilFormat( ctx_int_attribs[11], ctx_int_attribs[13] );
			vs.samples			= MultiSamples( ctx_int_attribs[21] );
			
			if ( not DescribePixelFormat( dc, pixformat, sizeof(pfd), &pfd ) )
				LOG( "Can't describe pixel format", ELog::Warning );
		}

		const bool	is_core_version =	( version >= 330 );	// 3.3 or higher

		const uint	CONTEXT_FLAG_NO_ERROR_BIT_KHR = 0x00000008;

		// Create OpenGL Context //
		int	context_attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, int(version / 100),
			WGL_CONTEXT_MINOR_VERSION_ARB, int((version / 10) % 10),
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB |
												(vs.flags[ EFlags::DebugContext ] ? WGL_CONTEXT_DEBUG_BIT_ARB : 0) |
												(vs.flags[ EFlags::NoErrorContext ] ? CONTEXT_FLAG_NO_ERROR_BIT_KHR : 0),
			WGL_CONTEXT_PROFILE_MASK_ARB,  is_core_version ?
												WGL_CONTEXT_CORE_PROFILE_BIT_ARB :
												WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};

		// search for valid context attribs
		for (;;)
		{
			rc = wglCreateContextAttribs.Get<PFNWGLCREATECONTEXTATTRIBSARBPROC>()( dc, null, context_attribs );

			if ( rc != null or context_attribs[1] <= 2 )
				break;
				
			// for opengl 4.x
			if ( context_attribs[1] < 4 )
			{
				if ( context_attribs[5] & CONTEXT_FLAG_NO_ERROR_BIT_KHR )
				{
					context_attribs[5] = context_attribs[5] & ~CONTEXT_FLAG_NO_ERROR_BIT_KHR;
					vs.flags.Reset( EFlags::NoErrorContext );
					continue;
				}
				else
				if ( context_attribs[5] & WGL_CONTEXT_DEBUG_BIT_ARB )
				{
					context_attribs[5] = context_attribs[5] & ~WGL_CONTEXT_DEBUG_BIT_ARB;
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
			rc = wglCreateContext( dc );
		}

		CHECK_ERR( rc != null );
		_renderContext = rc;

		CHECK_ERR( wglMakeCurrent( dc, rc ) != FALSE );

		// set vertical synchronization
		if ( swapControlSupported )
		{
			CHECK( wglSwapInterval.Get<PFNWGLSWAPINTERVALEXTPROC>()( int(vs.flags[ EFlags::VSync ]) ) != FALSE );

			vs.flags.SetAt( EFlags::VSync, wglGetSwapInterval.Get<PFNWGLGETSWAPINTERVALEXTPROC>()() != 0 );
		}
		else
			vs.flags.Reset( EFlags::VSync );

		// update settings
		{
			vs.device	= Cast<const char*>(glGetString( GL_RENDERER ));
			vs.version	= _ConvertGLVersion( version );
		}

		//wglMakeCurrent( dc, null );
		return true;
	}
	
/*
=================================================
	MakeCurrent
=================================================
*/
	void GLWinContext::MakeCurrent ()
	{
		ASSERT( IsCreated() );
		CHECK( wglMakeCurrent( _deviceContext.Get<HDC>(), _renderContext.Get<HGLRC>() ) != FALSE );
	}
	
/*
=================================================
	ResetCurrent
=================================================
*/
	void GLWinContext::ResetCurrent ()
	{
		ASSERT( IsCreated() );
		CHECK( wglMakeCurrent( _deviceContext.Get<HDC>(), null ) != FALSE );
	}
	
/*
=================================================
	IsCreated
=================================================
*/
	bool GLWinContext::IsCreated () const
	{
		return _deviceContext.IsNotNull<HDC>() and _renderContext.IsNotNull<HGLRC>();
	}
	
/*
=================================================
	IsCurrent
=================================================
*/
	bool GLWinContext::IsCurrent ()
	{
		return IsCreated() and wglGetCurrentContext() == _renderContext.Get<HGLRC>();
	}
	
/*
=================================================
	SwapBuffers
=================================================
*/
	void GLWinContext::SwapBuffers ()
	{
		ASSERT( IsCreated() );
		if ( IsCreated() )
		{
			::SwapBuffers( _deviceContext.Get<HDC>() );
		}
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_WINDOWS
