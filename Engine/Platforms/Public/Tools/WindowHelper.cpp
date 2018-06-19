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
	WindowHelper::WinAPIWindow::WinAPIWindow (void *inst, void *wnd) :
		window{Cast<HWND>(wnd)}, instance{Cast<HMODULE>(inst)}
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

		if ( window->GetSupportedMessages().HasAllTypes< SDLMsgList_t >() )
		{
			OSMsg::GetSDLWindowHandle	req_wnd;
			CHECK( window->Send( req_wnd ) );

			SDL_SysWMinfo	info = {};
			SDL_VERSION( &info.version );

			CHECK_ERR( SDL_GetWindowWMInfo( *req_wnd.result, OUT &info ) == SDL_TRUE );
			CHECK_ERR( info.subsystem == SDL_SYSWM_WINDOWS );

			CHECK_ERR( func( WinAPIWindow( info.info.win.hinstance, info.info.win.window ) ) );
			return true;
		}
	#endif
		
		using WinMsgList_t	= ModuleMsg::MessageListFrom< OSMsg::GetWinWindowHandle >;
		
		if ( window->GetSupportedMessages().HasAllTypes< WinMsgList_t >() )
		{
			OSMsg::GetWinWindowHandle	req_wnd;
			CHECK( window->Send( req_wnd ) );

			void* inst = ReferenceCast<void*>(::GetWindowLongPtrA( req_wnd.result->Get<HWND>(), GWLP_HINSTANCE ));

			CHECK_ERR( func( WinAPIWindow( inst, req_wnd.result->Get<HWND>() ) ) );
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
	WindowHelper::AndroidWindow::AndroidWindow (void *wnd) :
		window{Cast<ANativeWindow*>(wnd)}
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

		if ( window->GetSupportedMessages().HasAllTypes< SDLMsgList_t >() )
		{
			OSMsg::GetSDLWindowHandle>	req_wnd;
			CHECK( window->Send( req_wnd ) );

			SDL_SysWMinfo	info = {};
			SDL_VERSION( &info.version );

			CHECK_ERR( SDL_GetWindowWMInfo( *req_wnd->result, OUT &info ) == SDL_TRUE );
			CHECK_ERR( info.subsystem == SDL_SYSWM_ANDROID );

			CHECK_ERR( func( AndroidWindow( info.info.android.window ) ) );
			return true;
		}
	#endif
		
		return false;
	}

#endif	// PLATFORM_ANDROID
//-----------------------------------------------------------------------------
		
}	// PlatformTools
}	// Engine
