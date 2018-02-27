// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/Window.h"
#include "Engine/Platforms/Shared/OS/Platform.h"

#ifdef PLATFORM_WINDOWS

namespace Engine
{
namespace OSMsg
{	

	//
	// Request Window Handle
	//
	struct GetWinWindowHandle
	{
		using HWND_t = OS::HiddenOSTypeFrom<void*>;

		Out< HWND_t >	result;
	};


	//
	// Window Raw Message from OS
	//
	struct OnWinWindowRawMessage
	{
	// variables
		const uint		uMsg			= 0;
		const usize		wParam			= 0;
		const isize		lParam			= 0;
		Editable<bool>	wasProcessed;
		
	// methods
		OnWinWindowRawMessage (uint uMsg, usize wParam, isize lParam) :
			uMsg(uMsg), wParam(wParam), lParam(lParam), wasProcessed{false}
		{}
	};
	

	//
	// Platform Created Message
	//
	struct OnWinPlatformCreated
	{
	// types
		using HMODULE_t		= OS::HiddenOSTypeFrom<void*>;

	// variables
		HMODULE_t		instance;
		String			className;

	// methods
		OnWinPlatformCreated (HMODULE_t instance, StringCRef className) :
			instance(instance), className(className)
		{}
	};


}	// OSMsg
}	// Engine

#endif	// PLATFORM_WINDOWS
