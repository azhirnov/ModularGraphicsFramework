// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"
#include "Engine/Config/Engine.Config.h"

#if defined( GRAPHICS_API_SOFT ) and defined( PLATFORM_WINDOWS )

#include "Engine/Platforms/Soft/Windows/SwWinSurface.h"
#include "Engine/STL/OS/Windows/WinHeader.h"

namespace Engine
{
namespace PlatformSW
{

/*
=================================================
	constructor
=================================================
*/
	SwWinSurface::SwWinSurface () :
		_window{ UninitializedT<HWND>() },
		_mainDC{ UninitializedT<HDC>() },
		_secondaryDC{ UninitializedT<HDC>() }
	{}
		
/*
=================================================
	Create
=================================================
*/
	bool SwWinSurface::Create (const HWND_t &wnd, INOUT GraphicsSettings &vs)
	{
		//TODO( "" );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void SwWinSurface::Destroy ()
	{
		//TODO( "" );
	}
		
/*
=================================================
	SwapBuffers
=================================================
*/
	void SwWinSurface::SwapBuffers ()
	{
		// use BitBlit
		TODO( "" );
	}

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT and PLATFORM_WINDOWS
