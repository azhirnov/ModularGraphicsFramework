// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/Window.h"
#include "Engine/Platforms/Shared/OS/Platform.h"

#ifdef PLATFORM_SDL

namespace Engine
{
namespace OSMsg
{

	//
	// Request Window Handle
	//
	struct GetSDLWindowHandle
	{
		Out< SDL_Window *>	result;
	};


	//
	// Window Raw Message from OS
	//
	struct OnSDLWindowRawMessage
	{
	// variables
		SDL_Event		event;
		mutable bool	wasProcessed	= false;
		
	// methods
		OnSDLWindowRawMessage () : event{}
		{}
	};


	//
	// Platform Created Message
	//
	struct OnSDLPlatformCreated
	{
	// variables
		// TODO

	// methods
		OnSDLPlatformCreated ()
		{}
	};


}	// OSMsg
}	// Engine

#endif	// PLATFORM_SDL
