// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/Tools/WindowHelper.h"

#ifdef PLATFORM_WINDOWS
#	include "Engine/Platforms/Windows/WinMessages.h"
#	include "Core/STL/OS/Windows/WinHeader.h"
#endif

#ifdef PLATFORM_SDL
#	include "Engine/Platforms/SDL/SDLMessages.h"
#	include "SDL2/include/SDL_syswm.h"
#	undef FindWindow
#endif

#ifdef PLATFORM_ANDROID
#	include <android/native_window.h>
#endif

namespace Engine
{
namespace PlatformTools
{

/*
=================================================
	FindWinAPIPlatform
=================================================
*/
	ModulePtr  WindowHelper::FindWinAPIPlatform (GlobalSystemsRef gs)
	{
	#ifdef PLATFORM_WINDOWS
		using MsgList_t		= ModuleMsg::MessageListFrom< OSMsg::GetOSModules >;
		using EventList_t	= ModuleMsg::MessageListFrom< OSMsg::OnWinPlatformCreated >;

		return gs->mainSystem->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	#else
		return null;
	#endif
	}
	
/*
=================================================
	FindSDLPlatform
=================================================
*/
	ModulePtr  WindowHelper::FindSDLPlatform (GlobalSystemsRef gs)
	{
	#ifdef PLATFORM_SDL
		using MsgList_t		= ModuleMsg::MessageListFrom< OSMsg::GetOSModules >;
		using EventList_t	= ModuleMsg::MessageListFrom< OSMsg::OnSDLPlatformCreated >;

		return gs->mainSystem->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	#else
		return null;
	#endif
	}
	
/*
=================================================
	FindPlatform
=================================================
*/
	ModulePtr  WindowHelper::FindPlatform (GlobalSystemsRef gs)
	{
		ModulePtr	mod;

		if ( (mod = FindWinAPIPlatform( gs )) )
			return mod;

		if ( (mod = FindSDLPlatform( gs )) )
			return mod;

		return null;
	}

/*
=================================================
	FindWinAPIWindow
=================================================
*/
	ModulePtr  WindowHelper::FindWinAPIWindow (GlobalSystemsRef gs)
	{
	#ifdef PLATFORM_WINDOWS
		using MsgList_t		= ModuleMsg::MessageListFrom<
								OSMsg::WindowSetDescription,
								OSMsg::GetWinWindowHandle
							>;
		using EventList_t	= ModuleMsg::MessageListFrom<
								OSMsg::WindowCreated,
								OSMsg::WindowBeforeDestroy,
								OSMsg::WindowAfterDestroy,
								OSMsg::OnWinWindowRawMessage
							>;

		return gs->parallelThread->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	#else
		return null;
	#endif
	}
	
/*
=================================================
	FindSDLWindow
=================================================
*/
	ModulePtr  WindowHelper::FindSDLWindow (GlobalSystemsRef gs)
	{
	#ifdef PLATFORM_SDL
		using MsgList_t		= ModuleMsg::MessageListFrom<
								OSMsg::WindowSetDescription,
								OSMsg::GetSDLWindowHandle
							>;
		using EventList_t	= ModuleMsg::MessageListFrom<
								OSMsg::WindowCreated,
								OSMsg::WindowBeforeDestroy,
								OSMsg::WindowAfterDestroy,
								OSMsg::OnSDLWindowRawMessage
							>;

		return gs->parallelThread->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	#else
		return null;
	#endif
	}
	
/*
=================================================
	FindWindow
=================================================
*/
	ModulePtr  WindowHelper::FindWindow (GlobalSystemsRef gs)
	{
		return FindWindowInAttachment( gs->parallelThread.ptr() );
	}
	
/*
=================================================
	FindWindowInAttachment
=================================================
*/
	ModulePtr  WindowHelper::FindWindowInAttachment (const ModulePtr &mod)
	{
		using MsgList_t		= ModuleMsg::MessageListFrom<
								OSMsg::WindowSetDescription
							>;
		using EventList_t	= ModuleMsg::MessageListFrom<
								OSMsg::WindowCreated,
								OSMsg::WindowBeforeDestroy,
								OSMsg::WindowAfterDestroy
							>;

		return mod->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	}
//-----------------------------------------------------------------------------

	
#ifdef PLATFORM_WINDOWS
/*
=================================================
	WinAPIWindow
=================================================
*/
	WindowHelper::WinAPIWindow::WinAPIWindow () :
		window{ UninitializedT<HWND>() },
		instance{ UninitializedT<HMODULE>() }
	{}

/*
=================================================
	GetWindowHandle
=================================================
*/
	bool WindowHelper::GetWindowHandle (const ModulePtr &window, const Function<bool (const WinAPIWindow &)> &func)
	{
	#ifdef PLATFORM_SDL
		using SDLMsgList_t	= ModuleMsg::MessageListFrom< OSMsg::GetSDLWindowHandle >;

		if ( window->SupportsAllMessages< SDLMsgList_t >() )
		{
			OSMsg::GetSDLWindowHandle	req_wnd;
			CHECK( window->Send( req_wnd ) );

			SDL_SysWMinfo	info = {};
			SDL_VERSION( OUT &info.version );

			CHECK_ERR( SDL_GetWindowWMInfo( *req_wnd.result, OUT &info ) == SDL_TRUE );
			CHECK_ERR( info.subsystem == SDL_SYSWM_WINDOWS );

			WinAPIWindow	handle;
			handle.instance	= info.info.win.hinstance;
			handle.window	= info.info.win.window;

			CHECK_ERR( func( handle ) );
			return true;
		}
	#endif
		
		using WinMsgList_t	= ModuleMsg::MessageListFrom< OSMsg::GetWinWindowHandle >;
		
		if ( window->SupportsAllMessages< WinMsgList_t >() )
		{
			OSMsg::GetWinWindowHandle	req_wnd;
			CHECK( window->Send( req_wnd ) );

			WinAPIWindow	handle;
			handle.instance	= ReferenceCast<HMODULE>(::GetWindowLongPtrA( req_wnd.result->Get<HWND>(), GWLP_HINSTANCE ));
			handle.window	= req_wnd.result->Get<HWND>();

			CHECK_ERR( func( handle ) );
			return true;
		}

		return false;
	}

#endif	// PLATFORM_WINDOWS
//-----------------------------------------------------------------------------
	

#ifdef PLATFORM_ANDROID
/*
=================================================
	AndroidWindow
=================================================
*/
	WindowHelper::AndroidWindow::AndroidWindow () :
		window{ UninitializedT<ANativeWindow*>() }
	{}

/*
=================================================
	GetWindowHandle
=================================================
*/
	bool WindowHelper::GetWindowHandle (const ModulePtr &window, const Function<bool (const AndroidWindow &)> &func)
	{
	#ifdef PLATFORM_SDL
		using SDLMsgList_t	= CompileTime::TypeListFrom< OSMsg::GetSDLWindowHandle> >;

		if ( window->SupportsAllMessages< SDLMsgList_t >() )
		{
			OSMsg::GetSDLWindowHandle>	req_wnd;
			CHECK( window->Send( req_wnd ) );

			SDL_SysWMinfo	info = {};
			SDL_VERSION( OUT &info.version );

			CHECK_ERR( SDL_GetWindowWMInfo( *req_wnd->result, OUT &info ) == SDL_TRUE );
			CHECK_ERR( info.subsystem == SDL_SYSWM_ANDROID );

			AndroidWindow	handle;
			handle.window	= info.info.android.window;

			CHECK_ERR( func( handle ) );
			return true;
		}
	#endif
		
		return false;
	}

#endif	// PLATFORM_ANDROID
//-----------------------------------------------------------------------------
		

#ifdef PLATFORM_LINUX
/*
=================================================
	XlibWindow
=================================================
*/
	WindowHelper::XlibWindow::XlibWindow () :
		display{ UninitializedT<Display*>() },
		window{ UninitializedT<Window>() }
	{}

/*
=================================================
	GetWindowHandle
=================================================
*/
	bool WindowHelper::GetWindowHandle (const ModulePtr &window, const Function<bool (const XlibWindow &)> &func)
	{
	#ifdef PLATFORM_SDL
		using SDLMsgList_t	= ModuleMsg::MessageListFrom< OSMsg::GetSDLWindowHandle >;

		if ( window->SupportsAllMessages< SDLMsgList_t >() )
		{
			OSMsg::GetSDLWindowHandle	req_wnd;
			CHECK( window->Send( req_wnd ) );

			SDL_SysWMinfo	info = {};
			SDL_VERSION( OUT &info.version );

			CHECK_ERR( SDL_GetWindowWMInfo( *req_wnd.result, OUT &info ) == SDL_TRUE );
			CHECK_ERR( info.subsystem == SDL_SYSWM_X11 );

			XlibWindow	handle;
			handle.display	= info.info.x11.display;
			handle.window	= info.info.x11.window;

			CHECK_ERR( func( handle ) );
			return true;
		}
	#endif

		return false;
	}

#endif	// PLATFORM_LINUX
//-----------------------------------------------------------------------------

}	// PlatformTools
}	// Engine
