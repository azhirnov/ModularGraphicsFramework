// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"
#include "Engine/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

#include "Engine/Platforms/Public/GPU/Context.h"

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
		using HWND_t			= DeferredTypeFrom<void*>;
		using HDC_t				= DeferredTypeFrom<void*>;
		using HGLRC_t			= DeferredTypeFrom<void*>;
		using FuncPtr_t			= DeferredTypeFrom<void*>;
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
