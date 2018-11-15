// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{

	// Shared
	static constexpr GModID::type	InputManagerModuleID	= "input.mngr"_GModID;
	static constexpr TModID::type	InputThreadModuleID		= "input.thrd"_TModID;

	// TODO: remove platform-dependent ids
	// Windows
	static constexpr GModID::type	WinPlatformModuleID		= "win.platform"_GModID;
	static constexpr TModID::type	WinWindowModuleID		= "win.window"_TModID;
	static constexpr OModID::type	WinKeyInputModuleID		= "win.keys"_OModID;
	static constexpr OModID::type	WinMouseInputModuleID	= "win.mouse"_OModID;
	
	// SDL
	static constexpr GModID::type	SDLPlatformModuleID		= "sdl.platform"_GModID;
	static constexpr TModID::type	SDLWindowModuleID		= "sdl.window"_TModID;
	static constexpr OModID::type	SDLKeyInputModuleID		= "sdl.keys"_OModID;
	static constexpr OModID::type	SDLMouseInputModuleID	= "sdl.mouse"_OModID;
	

	//
	// OS Modules
	//
	struct OSModuleIDs
	{
		GModID::type	platform	= GModID::type(0);
		TModID::type	window		= TModID::type(0);
		OModID::type	keyInput	= OModID::type(0);
		OModID::type	mouseInput	= OModID::type(0);

		// TODO: touch input, joystick input, gamepad input, desktop window, wallpaper window ...
		// output devices?
	};


}	// Platforms
}	// Engine
