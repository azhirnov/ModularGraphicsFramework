// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#ifdef PLATFORM_WINDOWS

#include "Engine/Platforms/Public/OS/Window.h"
#include "Engine/Platforms/Public/OS/Display.h"
#include "Engine/Platforms/Public/OS/Platform.h"
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/Windows/WinObjectsConstructor.h"
#include "Core/STL/OS/Windows/WinHeader.h"
#include "Engine/Platforms/Windows/WinDisplay.h"

namespace Engine
{
namespace PlatformWin
{

	//
	// Windows Window
	//

	class WinWindow final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom<
											OSMsg::WindowSetDescription,
											OSMsg::WindowGetDescription,
											OSMsg::OnWinPlatformCreated,
											OSMsg::GetWinWindowHandle,
											OSMsg::GetDisplays,
											OSMsg::GetOSModules
										>;
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Update,
											ModuleMsg::Delete,
											OSMsg::WindowDescriptionChanged,
											OSMsg::WindowVisibilityChanged,
											OSMsg::WindowBeforeCreate,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											OSMsg::WindowAfterDestroy,
											OSMsg::OnWinWindowRawMessage
										>;
		
		using HWND_t				= DeferredTypeFrom<void*>;

		using EVisibility			= CreateInfo::Window::EVisibility;
		using EWindowFlags			= CreateInfo::Window::EWindowFlags;
		

	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		CreateInfo::Window	_createInfo;

		WindowDesc			_windowDesc;
		WinDisplay			_display;

		HWND_t				_wnd;

		TimeProfilerD		_timer;

		bool				_requestQuit;
		bool				_looping;


	// methods
	public:
		WinWindow (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::Window &ci);
		~WinWindow ();
		

	// message handlers
	private:
		bool _Update (const ModuleMsg::Update &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _OnWinPlatformCreated (const OSMsg::OnWinPlatformCreated &);
		bool _PlatformDeleted (const ModuleMsg::Delete &);
		bool _WindowSetDescription (const OSMsg::WindowSetDescription &);
		bool _WindowGetDescription (const OSMsg::WindowGetDescription &);
		bool _GetWinWindowHandle (const OSMsg::GetWinWindowHandle &);
		bool _GetDisplays (const OSMsg::GetDisplays &);
		bool _GetOSModules (const OSMsg::GetOSModules &);


	private:
		bool _IsCreated () const;
		bool _Create (const ModulePtr &sender, const OSMsg::OnWinPlatformCreated &platformInfo);
		bool _CreateWindow (const Display &disp, const CreateInfo::Window &info, StringCRef className, HWND_t instance);
		
		isize _ProcessMessage (uint uMsg, usize wParam, isize lParam);

		void _ShowWindow (EVisibility flags);
		void _StartMessageProc ();
		void _WindowTick ();

		bool _UpdateDescription ();
		void _Destroy ();

		Ptr<const Display> _GetDisplayByCoord (const int2 &point) const;
	};
//-----------------------------------------------------------------------------



	const TypeIdList	WinWindow::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinWindow::WinWindow (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Window &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_createInfo( ci ),
		_wnd( UninitializedT< HWND >() ),
		_requestQuit( false ),
		_looping( false )
	{
		SetDebugName( "WinWindow" );

		_SubscribeOnMsg( this, &WinWindow::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &WinWindow::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &WinWindow::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinWindow::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WinWindow::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &WinWindow::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinWindow::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &WinWindow::_Delete );
		_SubscribeOnMsg( this, &WinWindow::_Update );
		_SubscribeOnMsg( this, &WinWindow::_WindowSetDescription );
		_SubscribeOnMsg( this, &WinWindow::_WindowGetDescription );
		_SubscribeOnMsg( this, &WinWindow::_OnWinPlatformCreated );
		_SubscribeOnMsg( this, &WinWindow::_GetWinWindowHandle );
		_SubscribeOnMsg( this, &WinWindow::_GetDisplays );
		_SubscribeOnMsg( this, &WinWindow::_GetOSModules );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( null, WinPlatformModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	WinWindow::~WinWindow ()
	{
		//LOG( "WinWindow finalized", ELog::Debug );

		CHECK( not _IsCreated() );
		ASSERT( not _looping );
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool WinWindow::_Update (const ModuleMsg::Update &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return true;
		
		_WindowTick();

		_SendForEachAttachments( msg );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool WinWindow::_Delete (const ModuleMsg::Delete &msg)
	{
		_requestQuit	= true;
		_looping		= false;

		_Destroy();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_OnWinPlatformCreated
=================================================
*/
	bool WinWindow::_OnWinPlatformCreated (const OSMsg::OnWinPlatformCreated &msg)
	{
		CHECK_ERR( not _IsComposedState( GetState() ) );

		CHECK( _Create( _GetManager(), msg ) );

		if ( _IsCreated() and _looping )
		{
			CHECK( _ValidateAllSubscriptions() );
			CHECK( _SetState( EState::ComposedMutable ) );

			LOG( "window created", ELog::Debug );
			
			_SendForEachAttachments( ModuleMsg::Link{} );
			_SendForEachAttachments( ModuleMsg::Compose{} );
			
			_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		}
		else
		{
			CHECK( _SetState( EState::ComposingFailed ) );
		}
		return true;
	}
		
/*
=================================================
	_PlatformDeleted
=================================================
*/
	bool WinWindow::_PlatformDeleted (const ModuleMsg::Delete &msg)
	{
		_Delete( msg );
		return true;
	}

/*
=================================================
	_WindowSetDescription
=================================================
*/
	bool WinWindow::_WindowSetDescription (const OSMsg::WindowSetDescription &msg)
	{
		if ( not _IsCreated() )
		{
			CHECK_ERR( GetState() != EState::Deleting );

			_createInfo.caption				= msg.descr.caption;
			_createInfo.flags				= msg.descr.flags;
			_createInfo.initialVisibility	= msg.descr.visibility;
			_createInfo.orientation			= msg.descr.orientation;
			_createInfo.position			= msg.descr.position;
			_createInfo.surfaceSize			= msg.descr.surfaceSize;
			// ignored msg.desc.size
			return true;
		}
		
		Ptr<const Display>	disp = _GetDisplayByCoord( msg.descr.position );

		if ( not disp )
			disp = &_display.GetDisplays().Front();

		HWND		wnd				= _wnd.Get<HWND>();

		DWORD		wnd_style		= 0;
		DWORD		wnd_ext_style	= WS_EX_APPWINDOW;
		RECT		win_rect		= { 0, 0, int(msg.descr.surfaceSize.x), int(msg.descr.surfaceSize.y) };
		uint2 const	scr_res			= disp->Resolution();

		// set window caption
		if ( msg.descr.caption != _windowDesc.caption )
		{
			_windowDesc.caption = msg.descr.caption;
			::SetWindowTextA( wnd, msg.descr.caption.cstr() );
		}
		
		if ( msg.descr.flags[ EWindowFlags::Fullscreen ] )
		{
			// setup for fullscreen
			wnd_style				|= WS_POPUP;
			_windowDesc.position	 = disp->FullArea().LeftBottom();
			_windowDesc.size		 = scr_res;
			_windowDesc.surfaceSize	 = scr_res;
			
			_windowDesc.flags[ EWindowFlags::Fullscreen ] = true;
		}
		else
		{
			// setup windowed
			if ( msg.descr.flags[ EWindowFlags::Resizable ] )
				wnd_style = WS_OVERLAPPEDWINDOW;
			else
				wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			::AdjustWindowRectEx( &win_rect, wnd_style, 0, wnd_ext_style );

			_windowDesc.size		= uint2( win_rect.right - win_rect.left, win_rect.bottom - win_rect.top );
			_windowDesc.surfaceSize	= msg.descr.surfaceSize;
			_windowDesc.position	= msg.descr.position;

			if ( msg.descr.flags[ EWindowFlags::Centered ] )
			{
				_windowDesc.position = disp->WorkArea().Center() - int2(_windowDesc.size) / 2;		
			}
			
			_windowDesc.flags[ EWindowFlags::Fullscreen ] = false;
		}

		::SetWindowPos( wnd, HWND_TOP,
					    _windowDesc.position.x, _windowDesc.position.y,
					    win_rect.right - win_rect.left, win_rect.bottom - win_rect.top,
					    SWP_FRAMECHANGED );

		_ShowWindow( msg.descr.visibility );
		_UpdateDescription();

		_SendEvent( OSMsg::WindowDescriptionChanged{ _windowDesc });
		return true;
	}

/*
=================================================
	_WindowGetDescription
=================================================
*/
	bool WinWindow::_WindowGetDescription (const OSMsg::WindowGetDescription &msg)
	{
		CHECK_ERR( _IsCreated() );

		msg.result.Set( _windowDesc );
		return true;
	}
	
/*
=================================================
	_WindowGetDescription
=================================================
*/
	bool WinWindow::_GetWinWindowHandle (const OSMsg::GetWinWindowHandle &msg)
	{
		msg.result.Set( _wnd );
		return true;
	}
	
/*
=================================================
	_GetDisplays
=================================================
*/
	bool WinWindow::_GetDisplays (const OSMsg::GetDisplays &msg)
	{
		msg.result.Set( _display.GetDisplays() );
		return true;
	}
	
/*
=================================================
	_GetOSModules
=================================================
*/
	bool WinWindow::_GetOSModules (const OSMsg::GetOSModules &msg)
	{
		msg.result.Set( WinObjectsConstructor::GetModuleIDs() );
		return true;
	}

/*
=================================================
	_Create
=================================================
*/
	bool WinWindow::_Create (const ModulePtr &, const OSMsg::OnWinPlatformCreated &platformInfo)
	{
		CHECK_ERR( _display.Update() );

		Ptr<const Display>	disp = _GetDisplayByCoord( _createInfo.position );

		if ( not disp )
			disp = &_display.GetDisplays().Front();

		// validate create info
		{
			if ( _createInfo.caption.Empty() )
				_createInfo.caption = ENGINE_NAME;

			if ( not _createInfo.flags[ EWindowFlags::Fullscreen ] )
				_createInfo.surfaceSize = Clamp( _createInfo.surfaceSize, uint2(0), uint2(disp->WorkArea().Size()) );

			_createInfo.orientation = EDisplayOrientation::Default;
		}

		
		// allow subscribers to change settings
		auto const					msg		= OSMsg::WindowBeforeCreate{ _createInfo };
		CreateInfo::Window const&	info	= *msg.editable;

		_SendEvent( msg );


		// crate window
		CHECK_ERR( _CreateWindow( *disp, info, platformInfo.className, platformInfo.instance ) );

		CHECK_ERR( _UpdateDescription() );


		// send event
		_SendEvent( OSMsg::WindowCreated{} );
		_SendEvent( OSMsg::WindowDescriptionChanged{ _windowDesc });


		// start window message loop
		_looping = true;
		_StartMessageProc();
		_ShowWindow( _createInfo.initialVisibility );

		return true;
	}
	
/*
=================================================
	_CreateWindow
=================================================
*/
	bool WinWindow::_CreateWindow (const Display &disp, const CreateInfo::Window &info, StringCRef className, HWND_t instance)
	{
		CHECK_ERR( not _IsCreated() );

		DWORD		wnd_style		= 0;
		DWORD		wnd_ext_style	= WS_EX_APPWINDOW;
		RECT		win_rect		= { 0, 0, int(info.surfaceSize.x), int(info.surfaceSize.y) };
		uint2 const	scr_res			= disp.Resolution();
		
		if ( info.flags[ EWindowFlags::Fullscreen ] )
		{
			// setup for fullscreen
			wnd_style				|= WS_POPUP;
			_windowDesc.position	 = disp.FullArea().LeftBottom();
			_windowDesc.size		 = scr_res;
			_windowDesc.surfaceSize	 = scr_res;
		}
		else
		{
			// setup windowed
			if ( info.flags[ EWindowFlags::Resizable ] )
				wnd_style = WS_OVERLAPPEDWINDOW;
			else
				wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			::AdjustWindowRectEx( &win_rect, wnd_style, 0, wnd_ext_style );

			_windowDesc.size		= uint2( win_rect.right - win_rect.left, win_rect.bottom - win_rect.top );
			_windowDesc.surfaceSize	= info.surfaceSize;
			_windowDesc.position	= info.position;

			if ( info.flags[ EWindowFlags::Centered ] )
			{
				_windowDesc.position = disp.WorkArea().Center() - int2(_windowDesc.size) / 2;	
			}
		}
		
		_windowDesc.caption = info.caption;
		
		_wnd = ::CreateWindowExA( wnd_ext_style,
								 className.cstr(),
								 _windowDesc.caption.cstr(),
								 wnd_style,
								 _windowDesc.position.x,
								 _windowDesc.position.y,
								 _windowDesc.size.x,
								 _windowDesc.size.y,
								 null,
								 null,
								 instance.Get<HMODULE>(),
								 null );
		CHECK_ERR( _IsCreated() );

		_windowDesc.caption		= info.caption;
		_windowDesc.flags		= info.flags;
		_windowDesc.orientation	= EDisplayOrientation::Default;
		return true;
	}
	
/*
=================================================
	_StartMessageProc
=================================================
*/
	void WinWindow::_StartMessageProc ()
	{
		struct Utils {
			static LRESULT CALLBACK MsgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				WinWindow*	wp = ReferenceCast<WinWindow *>(GetWindowLongPtrA( hWnd, GWLP_USERDATA ));
				return wp->_ProcessMessage( uMsg, wParam, lParam );
			}
		};

		CHECK_ERR( _IsCreated() and _looping, void() );

		::SetWindowLongPtrA( _wnd.Get<HWND>(), GWLP_USERDATA, ReferenceCast<LONG_PTR>( this ) );
		::SetWindowLongPtrA( _wnd.Get<HWND>(), GWLP_WNDPROC,  ReferenceCast<LONG_PTR>( &Utils::MsgProc ) );
	}
	
/*
=================================================
	_ShowWindow
=================================================
*/
	void WinWindow::_ShowWindow (EVisibility flags)
	{
		CHECK_ERR( _IsCreated(), void() );

		switch ( flags )
		{
			case EVisibility::VisibleFocused :
				::SetForegroundWindow( _wnd.Get<HWND>() );
				::ShowWindow( _wnd.Get<HWND>(), SW_SHOWNA );
				break;

			case EVisibility::VisibleUnfocused :
				::ShowWindow( _wnd.Get<HWND>(), SW_SHOWNOACTIVATE );
				break;

			case EVisibility::Invisible :
				::ShowWindow( _wnd.Get<HWND>(), SW_FORCEMINIMIZE );
				break;

			default :
				RETURN_ERR( "unknown visibility flags", void() );
		}
	}

/*
=================================================
	_WindowTick
=================================================
*/
	void WinWindow::_WindowTick ()
	{
		if ( not _looping or _requestQuit )
			return;
		
		MSG		msg	= {};

		while ( ::PeekMessageA( &msg, null, 0, 0, PM_REMOVE ) )
		{
			if ( WM_QUIT == msg.message )
			{
				_requestQuit = true;

				_Destroy();
			}
			else
				::DispatchMessageA( &msg );
		}
	}
	
/*
=================================================
	_ProcessMessage
=================================================
*/
	isize WinWindow::_ProcessMessage (uint uMsg, usize wParam, isize lParam)
	{
		OSMsg::OnWinWindowRawMessage	msg{ uMsg, wParam, lParam };
		
		// WM_PAINT //
		if ( uMsg == WM_PAINT and _windowDesc.visibility == EVisibility::VisibleFocused )
		{
			msg.wasProcessed = true;

			//TODO( "" );
			//_Update( true );
		}
		else

		// WM_MOVE //
		if ( uMsg == WM_MOVE )
		{
			msg.wasProcessed = true;

			_UpdateDescription();
			_SendEvent( OSMsg::WindowDescriptionChanged{ _windowDesc });
		}
		else

		// WM_KILLPOCUS //
		if ( uMsg == WM_KILLFOCUS )
		{
			msg.wasProcessed = true;

			_windowDesc.visibility = EVisibility::VisibleUnfocused;
			_SendEvent( OSMsg::WindowVisibilityChanged{ _windowDesc.visibility });
		}
		else

		// WM_SETFOCUS //
		if ( uMsg == WM_SETFOCUS )
		{
			msg.wasProcessed = true;
			
			_windowDesc.visibility = EVisibility::VisibleFocused;
			_SendEvent( OSMsg::WindowVisibilityChanged{ _windowDesc.visibility });
		}
		else
			
		// WM_SIZE //
		if ( uMsg == WM_SIZE )
		{
			msg.wasProcessed = true;

			if ( wParam == SIZE_MINIMIZED )
			{
				_windowDesc.visibility = EVisibility::Invisible;
				_SendEvent( OSMsg::WindowVisibilityChanged{ _windowDesc.visibility });
			}

			_UpdateDescription();
			_SendEvent( OSMsg::WindowDescriptionChanged{ _windowDesc });
		}
		else

		// WM_DISPLAYCHANGE
		if ( uMsg == WM_DISPLAYCHANGE )
		{
			_display.Update();

			TODO( "move window to primiry display if currently used display will be detached" );
		}
		else

		// WM_SYSCOMMAND //
		if ( uMsg == WM_SYSCOMMAND and (wParam == SC_SCREENSAVE or wParam == SC_MONITORPOWER) )
		{
			return 0;
		}
		else

		// WM_CLOSE //
		if ( uMsg == WM_CLOSE )
		{
			::PostMessageA( _wnd.Get<HWND>(), WM_QUIT, 0, 0 );
			return 1;
		}

		_SendEvent( msg );

		return ::DefWindowProcA( _wnd.Get<HWND>(), uMsg, wParam, lParam );
	}

/*
=================================================
	_UpdateDescription
=================================================
*/
	bool WinWindow::_UpdateDescription ()
	{
		CHECK_ERR( _IsCreated() );

		auto	hwnd	= _wnd.Get<HWND>();
		RECT	win_rect = {0,0,0,0};

		::GetWindowRect( hwnd, OUT &win_rect );
		_windowDesc.position	= int2( win_rect.left, win_rect.top );
		_windowDesc.size		= uint2( win_rect.right - win_rect.left, win_rect.bottom - win_rect.top );
		
		::GetClientRect( hwnd, OUT &win_rect );
		_windowDesc.surfaceSize	= uint2( win_rect.right - win_rect.left, win_rect.bottom - win_rect.top );

		_windowDesc.visibility	= 
				IsZero( _windowDesc.surfaceSize )	?	EVisibility::Invisible			:
				hwnd == ::GetForegroundWindow()		?	EVisibility::VisibleFocused		:
				::IsWindowVisible( hwnd )			?	EVisibility::VisibleUnfocused	:
														EVisibility::Invisible;
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool WinWindow::_IsCreated () const
	{
		return _wnd.IsNotNull<HWND>();
	}

/*
=================================================
	_Destroy
=================================================
*/
	void WinWindow::_Destroy ()
	{
		if ( _IsCreated() )
		{
			::SetWindowLongPtrA( _wnd.Get<HWND>(), GWLP_WNDPROC, ReferenceCast<LONG_PTR>(&::DefWindowProcA) );

			_SendEvent( OSMsg::WindowBeforeDestroy{} );

			::DestroyWindow( _wnd.Get<HWND>() );
			_wnd = null;

			LOG( "window destroyed", ELog::Debug );

			_SendEvent( OSMsg::WindowAfterDestroy{} );
		}
	}
	
/*
=================================================
	_GetDisplayByCoord
=================================================
*/
	Ptr<const Display> WinWindow::_GetDisplayByCoord (const int2 &point) const
	{
		for (auto& disp : _display.GetDisplays())
		{
			if ( disp.FullArea().IsInnerPoint( point ) )
			{
				return &disp;
			}
		}
		return null;
	}

}	// PlatformWin
//-----------------------------------------------------------------------------
	

namespace Platforms
{
/*
=================================================
	CreateWinWindow
=================================================
*/
	ModulePtr WinObjectsConstructor::CreateWinWindow (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Window &ci)
	{
		return New< PlatformWin::WinWindow >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
