// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Windows/GLWinContext.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"
#include "External/opengl/wglext.h"

#pragma comment (lib, "opengl32.lib")

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
	bool GLWinContext::Create (const Handle_t &wnd, INOUT VideoSettings_t &vs)
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
			CHECK( wglMakeCurrent( _deviceContext.Get<HDC>(), null ) == TRUE );

			::ReleaseDC( _window.Get<HWND>(), _deviceContext.Get<HDC>() );
		}

		if ( _renderContext.IsNotNull<HGLRC>() )
		{
			CHECK( wglDeleteContext( _renderContext.Get<HGLRC>() ) == TRUE );
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
		const uint	pixformat	= ChoosePixelFormat( dc, &pfd );
		bool		res			= false;

		if ( pixformat != 0 )
		{
			CHECK( SetPixelFormat( dc, pixformat, &pfd ) == TRUE );
			rc = wglCreateContext( dc );

			if ( rc != null and wglMakeCurrent( dc, rc ) == TRUE )
			{
				wglChoosePixelFormat	= (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
				wglCreateContextAttribs	= (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
		
				wglSwapInterval			= (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
				wglGetSwapInterval		= (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");

				swapControlSupported	= wglSwapInterval.IsNotNull<PFNWGLSWAPINTERVALEXTPROC>() and
										  wglGetSwapInterval.IsNotNull<PFNWGLGETSWAPINTERVALEXTPROC>();

				res = true;
			}
			CHECK( wglMakeCurrent( dc, null ) == TRUE );

			CHECK( wglDeleteContext( rc ) == TRUE );
		}
		
		return res;
	}
	
/*
=================================================
	_GetApiVersion
=================================================
*/
	bool GLWinContext::_GetApiVersion (const VideoSettings_t &vs, OUT uint2 &version)
	{
		version = uint2();

		const auto	api_version = CreateInfo::GAPI::ToString( vs.version );

		Array< StringCRef >	tokens;
		StringParser::DivideString_CPP( api_version, OUT tokens );

		// check api name
		{
			bool	is_gl = false;

			if ( tokens.Count() > 1 and (tokens[0].EqualsIC( "GL" ) or tokens[0].EqualsIC( "opengl" )) )
			{
				is_gl = true;
			}

			if ( tokens.Count() > 2 and (tokens[1].EqualsIC( "ES" )) )
			{
				is_gl = false;
			}

			CHECK_ERR( is_gl );
		}

		// check api version
		{
			float	num = 0.0f;

			FOR( i, tokens )
			{
				const char	c = tokens[i].Front();
			
				if ( c >= '0' and c <= '9' )
				{
					num = StringUtils::ToFloat(String( tokens[i] ));
					break;
				}
			}

			version[0] = (uint) Floor( num );
			version[1] = (uint) (Fract( num ) * 10.0f);

			CHECK_ERR( version[0] >= 2 and version[0] <= 4 and version[1] <= 5 );
		}

		return true;
	}

/*
=================================================
	_InitOpenGL
=================================================
*/
	bool GLWinContext::_InitOpenGL (INOUT VideoSettings_t &vs)
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
			WGL_ALPHA_BITS_ARB,					color_bits[3] == 0 ? 1 : color_bits[3],
			WGL_DEPTH_BITS_ARB,					depth_stencil_bits[0],
			WGL_STENCIL_BITS_ARB,				depth_stencil_bits[1],
			WGL_DOUBLE_BUFFER_ARB,				true,
			WGL_STEREO_ARB,						vs.flags.Get( EFlags::Stereo ),
			WGL_PIXEL_TYPE_ARB,					WGL_TYPE_RGBA_ARB,
			//WGL_SAMPLE_BUFFERS_ARB,				vs.samples > 1 ? true : false,	// TODO
			//WGL_SAMPLES_ARB,					vs.samples,
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
										( dc, ctx_int_attribs, ctx_float_attribs, 1, &pixelFormat, &numFormats );

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
			//vs.depthBits	= (GpuContext::EDepthFromat) ctx_int_attribs[11];
			//vs.stencilBits	= (GpuContext::EStencilFormat) ctx_int_attribs[13];
			//vs.samples	= (ubyte) ctx_int_attribs[21];
			
			if ( not DescribePixelFormat( dc, pixformat, sizeof(pfd), &pfd ) )
				LOG( "Can't describe pixel format", ELog::Warning );
		}

		const bool	is_core_version =	( version[0] > 3 or (version[0] == 3 and version[1] == 3) );	// 3.3 or higher

		const uint	CONTEXT_FLAG_NO_ERROR_BIT_KHR = 0x00000008;

		// Create OpenGL Context //
		int	context_attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, version[0],
			WGL_CONTEXT_MINOR_VERSION_ARB, version[1],
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB |
												(vs.flags.Get( EFlags::DebugContext ) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0) |
												(vs.flags.Get( EFlags::NoErrorContext ) ? CONTEXT_FLAG_NO_ERROR_BIT_KHR : 0),
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

		CHECK_ERR( wglMakeCurrent( dc, rc ) == TRUE );

		// set vertical synchronization
		if ( swapControlSupported )
		{
			CHECK( wglSwapInterval.Get<PFNWGLSWAPINTERVALEXTPROC>()( int(vs.flags.Get( EFlags::VSync )) ) == TRUE );

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
		CHECK( wglMakeCurrent( _deviceContext.Get<HDC>(), _renderContext.Get<HGLRC>() ) == TRUE );
	}
	
/*
=================================================
	ResetCurrent
=================================================
*/
	void GLWinContext::ResetCurrent ()
	{
		ASSERT( IsCreated() );
		CHECK( wglMakeCurrent( _deviceContext.Get<HDC>(), null ) == TRUE );
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
