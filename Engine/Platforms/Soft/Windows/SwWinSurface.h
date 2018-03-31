// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Soft/Impl/SWEnums.h"

#if defined( GRAPHICS_API_SOFT ) and defined( PLATFORM_WINDOWS )

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Surface
	//
	class SwWinSurface
	{
	// types
	private:
		using HWND_t	= DeferredTypeFrom<void*>;
		using HDC_t		= DeferredTypeFrom<void*>;


	// variables
	private:
		HWND_t		_window;
		HDC_t		_mainDC;
		HDC_t		_secondaryDC;
		uint2		_size;


	// methods
	public:
		SwWinSurface ();
		
		bool Create (const HWND_t &wnd, INOUT GraphicsSettings &vs);
		void Destroy ();
		
		void SwapBuffers ();
	};

	
	using SWSurface	= SwWinSurface;

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT and PLATFORM_WINDOWS
