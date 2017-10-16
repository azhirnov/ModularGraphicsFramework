// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	// Shared
	static constexpr GModID::type	InputManagerModuleID	= "input.mngr"_GModID;
	static constexpr TModID::type	InputThreadModuleID		= "input.thrd"_TModID;

	// Windows
	static constexpr GModID::type	WinPlatformModuleID		= "win.platform"_GModID;
	static constexpr TModID::type	WinWindowModuleID		= "win.window"_TModID;
	static constexpr OModID::type	WinKeyInputModuleID		= "win.keys"_OModID;
	static constexpr OModID::type	WinMouseInputModuleID	= "win.mouse"_OModID;


}	// Platforms
}	// Engine
