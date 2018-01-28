// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Windows/GLWinContext.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"
#include "External/opengl/wglext.h"

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
	bool GLWinContext::Create (const HWND_t &wnd, INOUT GraphicsSettings &vs)
	{
		Destroy();
		
		_window = wnd;
		CHECK_ERR( _window.IsNotNull<HWND>() );

		_deviceContext = ::GetDC( wnd.Get<HWND>() );
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
		const uint	pixformat	= ChoosePixelFormat( dc, OUT &pfd );
		bool		res			= false;

		if ( pixformat != 0 )
		{
			CHECK( SetPixelFormat( dc, pixformat, &pfd ) != FALSE );
			rc = wglCreateContext( dc );

			if ( rc != null and wglMakeCurrent( dc, rc ) != FALSE )
			{
				wglChoosePixelFormat	= (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
				wglCreateContextAttribs	= (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
		
				wglSwapInterval			= (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
				wglGetSwapInterval		= (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");

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
	bool GLWinContext::_GetApiVersion (const GraphicsSettings &vs, OUT uint2 &version)
	{
		using namespace CreateInfo;

		switch ( vs.version )
		{
			case "GL 4.3"_GAPI :
			case "opengl 4.3"_GAPI :	version = uint2(4,3);	break;
			case "GL 4.4"_GAPI :
			case "opengl 4.4"_GAPI :	version = uint2(4,4);	break;
			case "GL 4.5"_GAPI :
			case "opengl 4.5"_GAPI :	version = uint2(4,5);	break;
			case "GL 4.6"_GAPI :
			case "opengl 4.6"_GAPI :	version = uint2(4,6);	break;
			case GAPI::type(0) :		version = uint2(4,5);	break;
			default :					RETURN_ERR( "unsupported OpenGL version" );
		}
		return true;
	}

/*
=================================================
	_InitOpenGL
=================================================
*/
	bool GLWinContext::_InitOpenGL (INOUT GraphicsSettings &vs)
	{
		// extract data from video settings
		uint2	version;
		CHECK_ERR( _GetApiVersion( vs, version ) );

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
			WGL_SAMPLE_BUFFERS_ARB,				vs.samples.Get() > 1 ? true : false,
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
			pixformat		= rv;
			//vs.depthBits	= (GpuContext::EDepthFromat) ctx_int_attribs[11];		// TODO
			//vs.stencilBits	= (GpuContext::EStencilFormat) ctx_int_attribs[13];
			vs.samples	= MultiSamples( ctx_int_attribs[21] );
			
			if ( not DescribePixelFormat( dc, pixformat, sizeof(pfd), &pfd ) )
				LOG( "Can't describe pixel format", ELog::Warning );
		}

		const bool	is_core_version =	( version[0] > 3 or (version[0] == 3 and version[1] == 3) );	// 3.3 or higher

		const uint	CONTEXT_FLAG_NO_ERROR_BIT_KHR = 0x00000008;

		// Create OpenGL Context //
		int	context_attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, int(version[0]),
			WGL_CONTEXT_MINOR_VERSION_ARB, int(version[1]),
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
					continue;
				}
				else
				if ( context_attribs[5] & WGL_CONTEXT_DEBUG_BIT_ARB )
				{
					context_attribs[5] = context_attribs[5] & ~WGL_CONTEXT_DEBUG_BIT_ARB;
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
