// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_WINDOWS

#include "Engine/Platforms/Public/OS/Display.h"
#include "Engine/Platforms/Public/OS/Window.h"
#include "Engine/Platforms/Public/OS/Platform.h"

namespace Engine
{
namespace OSMsg
{	

	//
	// Request Window Handle
	//
	struct GetWinWindowHandle : _MsgBase_
	{
		using HWND_t = DeferredTypeFrom<void*>;

		Out< HWND_t >	result;
	};


	//
	// Window Raw Message from OS
	//
	struct OnWinWindowRawMessage : _MsgBase_
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
	struct OnWinPlatformCreated : _MsgBase_
	{
	// types
		using HMODULE_t		= DeferredTypeFrom<void*>;

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
