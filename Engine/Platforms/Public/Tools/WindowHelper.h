// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/OS/Window.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Platforms;


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
			using HWND_t	= OS::HiddenOSTypeFrom<void*>;
			using HMODULE_t	= OS::HiddenOSTypeFrom<void*>;

		// variables
			HWND_t		window;
			HMODULE_t	instance;

		// methods
			WinAPIWindow (void *inst, void *wnd);
		};


	// methods
	public:
		static ModulePtr  FindWinAPIPlatform (GlobalSystemsRef gs);
		static ModulePtr  FindSDLPlatform (GlobalSystemsRef gs);
		static ModulePtr  FindPlatform (GlobalSystemsRef gs);

		static ModulePtr  FindWinAPIWindow (GlobalSystemsRef gs);
		static ModulePtr  FindSDLWindow (GlobalSystemsRef gs);
		static ModulePtr  FindWindow (GlobalSystemsRef gs);
		static ModulePtr  FindWindowInAttachment (const ModulePtr &mod);

		//template <typename MsgList, typename EventList>
		//static ModulePtr  FindWindowMsgEvent (GlobalSystemsRef gs);

		static bool GetWindowHandle (const ModulePtr &window, const Function<bool (const WinAPIWindow &)> &func);
	};


}	// PlatformTools
}	// Engine
