// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/Platforms/Public/OS/Display.h"
#include "Engine/Platforms/Public/OS/Platform.h"

namespace Engine
{
namespace PlatformSDL
{
	using namespace Engine::Platforms;


	//
	// SDL Display Helper
	//

	struct SDLDisplay final
	{
	// types
	private:
		using Displays_t	= OSMsg::GetDisplays::Displays_t;


	// variables
	private:
		Displays_t		_displays;


	// methods
	public:
		SDLDisplay ();

		bool Update ();

		ArrayCRef<Display> GetDisplays () const		{ return _displays; }

	private:
	};


}	// PlatformSDL
}	// Engine

#endif	// PLATFORM_SDL
