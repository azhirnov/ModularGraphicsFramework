// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_SOFT ) and defined( PLATFORM_ANDROID )

#include "Engine/Platforms/Soft/Android/SwAndSurface.h"
#include <android/native_window.h>

namespace Engine
{
namespace PlatformSW
{

/*
=================================================
	constructor
=================================================
*/
	SwAndSurface::SwAndSurface () :
		_nativeWindow{UninitializedT<ANativeWindow*>()}
	{}
		
/*
=================================================
	Create
=================================================
*/
	bool SwAndSurface::Create (const AWindow_t &wnd, INOUT GraphicsSettings &vs)
	{
		//TODO( "" );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void SwAndSurface::Destroy ()
	{
		//TODO( "" );
	}
		
/*
=================================================
	SwapBuffers
=================================================
*/
	void SwAndSurface::SwapBuffers ()
	{
		// use BitBlit
		TODO( "" );
	}

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT and PLATFORM_ANDROID
