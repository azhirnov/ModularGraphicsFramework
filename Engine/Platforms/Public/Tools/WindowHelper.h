// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/OS/Window.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Engine::Platforms;


	//
	// Window Helper
	//

	class WindowHelper final : public Noninstancable
	{
	// types
	public:
		struct WinAPIWindow
		{
		// types
			using HWND_t	= DeferredTypeFrom<void*>;
			using HMODULE_t	= DeferredTypeFrom<void*>;

		// variables
			HWND_t		window;
			HMODULE_t	instance;

		// methods
			WinAPIWindow (void *inst, void *wnd);
		};

		struct AndroidWindow
		{
		// types
			using AWindow_t	= DeferredTypeFrom<void*>;

		// variables
			AWindow_t	window;

		// methods
			AndroidWindow (void *wnd);
		};


	// methods
	public:
		ND_ static ModulePtr  FindWinAPIPlatform (GlobalSystemsRef gs);
		ND_ static ModulePtr  FindSDLPlatform (GlobalSystemsRef gs);
		ND_ static ModulePtr  FindPlatform (GlobalSystemsRef gs);

		ND_ static ModulePtr  FindWinAPIWindow (GlobalSystemsRef gs);
		ND_ static ModulePtr  FindSDLWindow (GlobalSystemsRef gs);
		ND_ static ModulePtr  FindWindow (GlobalSystemsRef gs);
		ND_ static ModulePtr  FindWindowInAttachment (const ModulePtr &mod);

		//template <typename MsgList, typename EventList>
		//static ModulePtr  FindWindowMsgEvent (GlobalSystemsRef gs);
		
#		ifdef PLATFORM_WINDOWS
		ND_ static bool GetWindowHandle (const ModulePtr &window, const Function<bool (const WinAPIWindow &)> &func);
#		endif
#		ifdef PLATFORM_ANDROID
		ND_ static bool GetWindowHandle (const ModulePtr &window, const Function<bool (const AndroidWindow &)> &func);
#		endif
	};


}	// PlatformTools
}	// Engine
