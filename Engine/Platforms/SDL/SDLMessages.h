// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

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
	struct GetSDLWindowHandle : _MsgBase_
	{
		Out< SDL_Window *>	result;
	};


	//
	// Window Raw Message from OS
	//
	struct OnSDLWindowRawMessage : _MsgBase_
	{
	// variables
		SDL_Event		event;
		Editable<bool>	wasProcessed;
		
	// methods
		OnSDLWindowRawMessage () : event{}, wasProcessed{false}
		{}
	};


	//
	// Platform Created Message
	//
	struct OnSDLPlatformCreated : _MsgBase_
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
