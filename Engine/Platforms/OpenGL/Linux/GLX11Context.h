// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_LINUX )

#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Public/Tools/WindowHelper.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace Engine::Platforms;


	//
	// Linux OpenGL Surface
	//

	class GLX11Context
	{
	// types
	protected:
		using FuncPtr_t			= DeferredTypeFrom<void*>;
		using Display_t			= DeferredTypeFrom<void*>;
		using Window_t			= DeferredTypeFrom<void*>;
		using GLXContext_t		= DeferredTypeFrom<void*>;
		using EFlags			= GraphicsSettings::EFlags;


	// variables
	protected:
		FuncPtr_t		glxSwapIntervalEXT;
		FuncPtr_t		glxSwapIntervalMESA;
		FuncPtr_t		glxGetSwapIntervalMESA;
		FuncPtr_t		glxSwapIntervalSGI;
		FuncPtr_t		glxCreateContextAttribsARB;

		Display_t		_display;			// Display*
		Window_t		_window;			// Window
		GLXContext_t	_renderContext;		// GLXContext

		
	// methods
	public:
		GLX11Context ();

		bool Create (const PlatformTools::WindowHelper::XlibWindow &, INOUT GraphicsSettings &vs);
		void Destroy ();

		void MakeCurrent ();
		void ResetCurrent ();

		bool IsCreated () const;
		bool IsCurrent ();

		void SwapBuffers ();

	protected:
		bool _InitOpenGL (INOUT GraphicsSettings &vs);
		
		bool _IsExtensionSupported (StringCRef extList, StringCRef extension) const;
	};


	using GLRenderingContext	= GLX11Context;


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_LINUX
