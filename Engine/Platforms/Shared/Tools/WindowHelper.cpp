// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/Tools/WindowHelper.h"

#ifdef PLATFORM_WINDOWS
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/STL/OS/Windows/WinHeader.h"
#endif

#ifdef PLATFORM_SDL
#include "Engine/Platforms/SDL/SDLMessages.h"
#include "External/SDL2/include/SDL_syswm.h"
#undef FindWindow
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
		using MsgList_t		= CompileTime::TypeListFrom< Message<OSMsg::GetOSModules> >;
		using EventList_t	= CompileTime::TypeListFrom< Message<OSMsg::OnWinPlatformCreated> >;

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
		using MsgList_t		= CompileTime::TypeListFrom< Message<OSMsg::GetOSModules> >;
		using EventList_t	= CompileTime::TypeListFrom< Message<OSMsg::OnSDLPlatformCreated> >;

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
		using MsgList_t		= CompileTime::TypeListFrom<
								Message<OSMsg::WindowSetDescriptor>,
								Message<OSMsg::GetWinWindowHandle>
							>;
		using EventList_t	= CompileTime::TypeListFrom<
								Message<OSMsg::WindowCreated>,
								Message<OSMsg::WindowBeforeDestroy>,
								Message<OSMsg::WindowAfterDestroy>,
								Message<OSMsg::OnWinWindowRawMessage>
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
		using MsgList_t		= CompileTime::TypeListFrom<
								Message<OSMsg::WindowSetDescriptor>,
								Message<OSMsg::GetSDLWindowHandle>
							>;
		using EventList_t	= CompileTime::TypeListFrom<
								Message<OSMsg::WindowCreated>,
								Message<OSMsg::WindowBeforeDestroy>,
								Message<OSMsg::WindowAfterDestroy>,
								Message<OSMsg::OnSDLWindowRawMessage>
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
		return FindWindowInAttachment( gs->parallelThread.Ptr() );
	}
	
/*
=================================================
	FindWindowInAttachment
=================================================
*/
	ModulePtr  WindowHelper::FindWindowInAttachment (const ModulePtr &mod)
	{
		using MsgList_t		= CompileTime::TypeListFrom<
								Message<OSMsg::WindowSetDescriptor>
							>;
		using EventList_t	= CompileTime::TypeListFrom<
								Message<OSMsg::WindowCreated>,
								Message<OSMsg::WindowBeforeDestroy>,
								Message<OSMsg::WindowAfterDestroy>
							>;

		return mod->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	}

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
		using SDLMsgList_t	= CompileTime::TypeListFrom< Message<OSMsg::GetSDLWindowHandle> >;

		if ( window->GetSupportedMessages().HasAllTypes< SDLMsgList_t >() )
		{
			Message<OSMsg::GetSDLWindowHandle>	req_wnd;
			CHECK( window->Send( req_wnd ) );

			SDL_SysWMinfo	info = {};
			SDL_VERSION( &info.version );

			CHECK_ERR( SDL_GetWindowWMInfo( *req_wnd->result, OUT &info ) == SDL_TRUE );
			CHECK_ERR( info.subsystem == SDL_SYSWM_WINDOWS );

			CHECK_ERR( func( WinAPIWindow( info.info.win.hinstance, info.info.win.window ) ) );
			return true;
		}
	#endif
		
	#ifdef PLATFORM_WINDOWS
		using WinMsgList_t	= CompileTime::TypeListFrom< Message<OSMsg::GetWinWindowHandle> >;
		
		if ( window->GetSupportedMessages().HasAllTypes< WinMsgList_t >() )
		{
			Message<OSMsg::GetWinWindowHandle>	req_wnd;
			CHECK( window->Send( req_wnd ) );

			void* inst = ReferenceCast<void*>(::GetWindowLongPtrA( req_wnd->result->Get<HWND>(), GWLP_HINSTANCE ));

			CHECK_ERR( func( WinAPIWindow( inst, req_wnd->result->Get<HWND>() ) ) );
			return true;
		}
	#endif

		return false;
	}

		
}	// PlatformTools
}	// Engine
