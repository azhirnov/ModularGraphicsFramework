// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/SDL/SDLDisplay.h"

#ifdef PLATFORM_SDL

namespace Engine
{
namespace PlatformSDL
{

/*
=================================================
	constructor
=================================================
*/
	SDLDisplay::SDLDisplay ()
	{
	}
	
/*
=================================================
	Update
=================================================
*/
	bool SDLDisplay::Update ()
	{
		_displays.Clear();
		
		const int	count = SDL_GetNumVideoDisplays();

		_displays.Resize( count );

		FOR( i, _displays )
		{
			auto&			disp	= _displays[i];

			SDL_DisplayMode	mode	= {};
			SDL_Rect		warea	= {};
			SDL_Rect		area	= {};
			float3			dpi;

			SDL_CALL( SDL_GetCurrentDisplayMode( i, OUT &mode ) );
			SDL_CALL( SDL_GetDisplayBounds( i, OUT &area ) );
			SDL_CALL( SDL_GetDisplayUsableBounds( i, OUT &warea ) );
			SDL_CALL( SDL_GetDisplayDPI( i, OUT &dpi[0], OUT &dpi[1], OUT &dpi[2] ) );

			disp = Platforms::Display( i,
									   RectI(warea.x, warea.y, warea.x + warea.w, warea.y + warea.h),
									   RectI(area.x, area.y, area.x + area.w, area.y + area.h),
									   dpi[0],
									   mode.refresh_rate,
									   Display::EOrientation::Default );
		}
		
		CHECK_ERR( not _displays.Empty() );
		return true;
	}


}	// PlatformSDL
}	// Engine

#endif	// PLATFORM_SDL
