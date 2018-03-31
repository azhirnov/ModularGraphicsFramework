// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Soft/Impl/SWEnums.h"

#if defined( GRAPHICS_API_SOFT ) and defined( PLATFORM_ANDROID )

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Surface
	//
	class SwAndSurface
	{
	// types
	private:
		using AWindow_t	= DeferredTypeFrom<void*>;


	// variables
	private:
		AWindow_t	_nativeWindow;
		uint2		_size;


	// methods
	public:
		SwAndSurface ();
		
		bool Create (const AWindow_t &wnd, INOUT GraphicsSettings &vs);
		void Destroy ();
		
		void SwapBuffers ();
	};

	
	using SWSurface	= SwAndSurface;

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT and PLATFORM_ANDROID
