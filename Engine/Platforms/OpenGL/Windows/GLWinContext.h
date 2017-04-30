// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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

	class GLWinContext final
	{
	// types
	private:
		using Handle_t			= OS::HiddenOSTypeFrom<void*>;
		using FuncPtr_t			= OS::HiddenOSTypeFrom<void*>;
		using VideoSettings_t	= CreateInfo::GpuContext;
		using EFlags			= VideoSettings_t::EFlags;


	// variables
	private:
		FuncPtr_t		wglSwapInterval;			// PFNWGLSWAPINTERVALEXTPROC
		FuncPtr_t		wglGetSwapInterval;			// PFNWGLGETSWAPINTERVALEXTPROC
		FuncPtr_t		wglChoosePixelFormat;		// PFNWGLCHOOSEPIXELFORMATARBPROC
		FuncPtr_t		wglCreateContextAttribs;	// PFNWGLCREATECONTEXTATTRIBSARBPROC
		bool			swapControlSupported;

		Handle_t		_window;
		Handle_t		_deviceContext;				// HDC
		Handle_t		_renderContext;				// HGLRC

		
	// methods
	public:
		GLWinContext ();

		bool Create (const Handle_t &wnd, INOUT VideoSettings_t &vs);
		void Destroy ();

		void MakeCurrent ();
		void ResetCurrent ();

		bool IsCreated () const;
		bool IsCurrent ();

		void SwapBuffers ();

	private:
		bool _InitOpenGL (INOUT VideoSettings_t &vs);
		bool _InitOpenGL2 ();

		bool _GetApiVersion (const VideoSettings_t &vs, OUT uint2 &version);
	};


	using GLRenderingContext	= GLWinContext;


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_WINDOWS
