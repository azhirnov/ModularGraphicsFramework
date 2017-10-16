// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/Window.h"
#include "Engine/Platforms/Shared/OS/Platform.h"

#if defined( PLATFORM_WINDOWS )

namespace Engine
{
namespace ModuleMsg
{

	//
	// On Window Created Message
	//
	struct WindowCreated
	{
		Platforms::WindowDesc			descr;
		OS::HiddenOSTypeFrom<void*>		hwnd;
	};
	

	//
	// Request Window Handle
	//
	struct WindowGetHandle
	{
		Out< OS::HiddenOSTypeFrom<void*> >	hwnd;
	};


	//
	// Window Raw Message from OS
	//
	struct WindowRawMessage
	{
		const uint		uMsg			= 0;
		const usize		wParam			= 0;
		const isize		lParam			= 0;
		mutable bool	wasProcessed	= false;

		WindowRawMessage (uint uMsg, usize wParam, isize lParam) :
			uMsg(uMsg), wParam(wParam), lParam(lParam)
		{}
	};
	

	//
	// Platform Created Message
	//
	struct PlatformCreated
	{
	// variables
		OS::HiddenOSTypeFrom<void*>		instance;
		String							className;
		Platforms::Display				display;

	// methods
		PlatformCreated (const OS::HiddenOSTypeFrom<void*> &instance,
						 StringCRef							className,
						 const Platforms::Display			&display) :
			instance(instance),
			className(className),
			display(display)
		{}
	};


}	// ModuleMsg
}	// Engine

#endif	// PLATFORM_WINDOWS
