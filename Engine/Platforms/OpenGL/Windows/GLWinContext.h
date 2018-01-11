// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Context.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

namespace Engine
{
namespace PlatformGL
{
	using namespace Platforms;


	//
	// Windows OpenGL Surface
	//

	class GLWinContext
	{
	// types
	protected:
		using HWND_t			= OS::HiddenOSTypeFrom<void*>;
		using HDC_t				= OS::HiddenOSTypeFrom<void*>;
		using HGLRC_t			= OS::HiddenOSTypeFrom<void*>;
		using FuncPtr_t			= OS::HiddenOSTypeFrom<void*>;
		using EFlags			= GraphicsSettings::EFlags;


	// variables
	protected:
		FuncPtr_t		wglSwapInterval;			// PFNWGLSWAPINTERVALEXTPROC
		FuncPtr_t		wglGetSwapInterval;			// PFNWGLGETSWAPINTERVALEXTPROC
		FuncPtr_t		wglChoosePixelFormat;		// PFNWGLCHOOSEPIXELFORMATARBPROC
		FuncPtr_t		wglCreateContextAttribs;	// PFNWGLCREATECONTEXTATTRIBSARBPROC
		bool			swapControlSupported;

		HWND_t			_window;
		HDC_t			_deviceContext;
		HGLRC_t			_renderContext;

		
	// methods
	public:
		GLWinContext ();

		bool Create (const HWND_t &wnd, INOUT GraphicsSettings &vs);
		void Destroy ();

		void MakeCurrent ();
		void ResetCurrent ();

		bool IsCreated () const;
		bool IsCurrent ();

		void SwapBuffers ();

	protected:
		bool _InitOpenGL (INOUT GraphicsSettings &vs);
		bool _InitOpenGL2 ();

		bool _GetApiVersion (const GraphicsSettings &vs, OUT uint2 &version);
	};


	using GLRenderingContext	= GLWinContext;


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_WINDOWS
