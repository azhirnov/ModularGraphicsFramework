// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "WinWindow.h"
#include "WinPlatform.h"

#if defined( PLATFORM_WINDOWS )

#include <Windows.h>

namespace Engine
{
namespace Platforms
{

	const Runtime::VirtualTypeList	WinWindow::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	WinWindow::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinWindow::WinWindow (const SubSystemsRef gs, const CreateInfo::Window &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes ),
		_createInfo( ci ),
		_wnd( UninitializedT< HWND >() ),
		_requestQuit( false ),
		_looping( false )
	{
		SetDebugName( "WinWindow" );

		_SubscribeOnMsg( this, &WinWindow::_OnModuleAttached );
		_SubscribeOnMsg( this, &WinWindow::_OnModuleDetached );
		_SubscribeOnMsg( this, &WinWindow::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinWindow::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WinWindow::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinWindow::_Delete );
		_SubscribeOnMsg( this, &WinWindow::_Update );
		_SubscribeOnMsg( this, &WinWindow::_WindowSetDescriptor );
		_SubscribeOnMsg( this, &WinWindow::_WindowGetDescriptor );
		_SubscribeOnMsg( this, &WinWindow::_PlatformCreated );
		_SubscribeOnMsg( this, &WinWindow::_WindowGetHandle );
		
		CHECK( _ValidateMsgSubscriptions() );

		GXTypes::New< AttachModuleToManagerAsyncTask >( this, WinPlatform::GetStaticID() )->Execute();

		if ( _GetManager() )
		{
			CHECK( _GetManager()->Subscribe( this, &WinWindow::_PlatformCreated ) );
			CHECK( _GetManager()->Subscribe( this, &WinWindow::_PlatformDeleted ) );

			CHECK( _SetState( EState::Linked ) );
		}
	}
	
/*
=================================================
	destructor
=================================================
*/
	WinWindow::~WinWindow ()
	{
		LOG( "WinWindow finalized", ELog::Debug );

		CHECK( not _IsCreated() );
		ASSERT( not _looping );
	}
	
/*
=================================================
	_Update
=================================================
*/
	void WinWindow::_Update (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ), void() );
		
		_WindowTick();

		_SendForEachAttachments( msg );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void WinWindow::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_requestQuit	= true;
		_looping		= false;

		_Destroy();

		Module::_Delete_Impl( msg );
		
		GXTypes::New< DetachModuleFromManagerAsyncTask >( this, WinPlatform::GetStaticID() )->Execute();
	}

/*
=================================================
	_PlatformCreated
=================================================
*/
	void WinWindow::_PlatformCreated (const Message< ModuleMsg::PlatformCreated > &msg)
	{
		CHECK_ERR( not _IsComposedState( GetState() ), void() );

		CHECK( _Create( msg.Sender(), msg.Data() ) );

		CHECK( _SetState( _IsCreated() and _looping ? EState::ComposedMutable : EState::ComposingFailed ) );

		// compose attachments
		if ( _IsComposedState( GetState() ) )
		{
			LOG( "window created", ELog::Debug );
			
			_SendForEachAttachments( Message< ModuleMsg::Link >{ this, _GetAttachments() } );
			_SendForEachAttachments( Message< ModuleMsg::Compose >{ this } );
		}
	}
		
/*
=================================================
	_PlatformDeleted
=================================================
*/
	void WinWindow::_PlatformDeleted (const Message< ModuleMsg::Delete > &msg)
	{
		_Delete( msg );
	}

/*
=================================================
	_WindowSetDescriptor
=================================================
*/
	void WinWindow::_WindowSetDescriptor (const Message< ModuleMsg::WindowSetDescriptor > &msg)
	{
		TODO( "" );
	}

/*
=================================================
	_WindowGetDescriptor
=================================================
*/
	void WinWindow::_WindowGetDescriptor (const Message< ModuleMsg::WindowGetDescriptor > &msg)
	{
		CHECK_ERR( _IsCreated(), void() );

		msg->result.Set( _windowDesc );
	}
	
/*
=================================================
	_WindowGetDescriptor
=================================================
*/
	void WinWindow::_WindowGetHandle (const Message< ModuleMsg::WindowGetHandle > &msg)
	{
		msg->hwnd.Set( _wnd );
	}

/*
=================================================
	_Create
=================================================
*/
	bool WinWindow::_Create (const ModulePtr &sender, const ModuleMsg::PlatformCreated &platformInfo)
	{
		// request display info
		auto	request = Message<ModuleMsg::RequestDisplayParams>( this );

		sender->Send( request );
		CHECK_ERR( request->result.Get().IsDefined() );

		Display const&	disp = *request->result.Get().Get();


		// validate create info
		{
			if ( _createInfo.caption.Empty() )
				_createInfo.caption = ENGINE_NAME;

			_createInfo.surfaceSize = Clamp( _createInfo.surfaceSize, uint2(0), disp.Resolution() );

			_createInfo.orientation = EDisplayOrientation::Default;
		}

		
		// allow subscribers to change settings
		auto const					msg		= Message< ModuleMsg::WindowBeforeCreate >( this, _createInfo );
		CreateInfo::Window const&	info	= msg->editable;

		_SendEvent( msg );


		// crate window
		CHECK_ERR( _CreateWindow( disp, info, platformInfo.className, platformInfo.inst ) );

		CHECK_ERR( _UpdateDescriptor() );


		// send event
		_SendEvent( Message< ModuleMsg::WindowCreated >{ this, _windowDesc, _wnd } );

		_SendEvent( Message< ModuleMsg::WindowDescriptorChanged >{ this, _windowDesc } );


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
	bool WinWindow::_CreateWindow (const Display &disp, const CreateInfo::Window &info, StringCRef className, Handle_t instance)
	{
		CHECK_ERR( not _IsCreated() );

		DWORD		wndStyle	= 0;
		DWORD		wndExtStyle	= WS_EX_APPWINDOW;
		RECT		winRect		= { 0, 0, info.surfaceSize.x, info.surfaceSize.y };
		uint2 const	scr_res		= disp.Resolution();
		
		if ( info.flags[ CreateInfo::Window::EWindowFlags::Fullscreen ] )
		{
			// setup for fullscreen
			wndStyle				|= WS_POPUP;
			_windowDesc.position	 = int2();
			_windowDesc.size		 = scr_res;
			_windowDesc.surfaceSize	 = scr_res;
		}
		else
		{
			// setup windowed
			if ( info.flags[ CreateInfo::Window::EWindowFlags::Resizable ] )
				wndStyle = WS_OVERLAPPEDWINDOW;
			else
				wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			::AdjustWindowRectEx( &winRect, wndStyle, 0, wndExtStyle );

			_windowDesc.size		= uint2( winRect.right - winRect.left, winRect.bottom - winRect.top );
			_windowDesc.surfaceSize	= info.surfaceSize;
			_windowDesc.position	= info.position;

			if ( All( info.position < -int2(disp.Resolution()) ) )
			{
				_windowDesc.position.x = Max( 0, (int)scr_res.x - (int)_windowDesc.size.x ) / 2;
				_windowDesc.position.y = Max( 0, (int)scr_res.y - (int)_windowDesc.size.y ) / 2;		
			}
		}
		
		_wnd = ::CreateWindowEx( wndExtStyle,
								 className.cstr(),
								 info.caption.cstr(),
								 wndStyle,
								 _windowDesc.position.x,
								 _windowDesc.position.y,
								 _windowDesc.size.x,
								 _windowDesc.size.y,
								 null,
								 null,
								 instance.Get<HMODULE>(),
								 null );
		CHECK_ERR( _IsCreated() );

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
				WinWindow*	wp = (WinWindow *)GetWindowLongPtrA( hWnd, GWLP_USERDATA );
				return wp->_ProcessMessage( uMsg, wParam, lParam );
			}
		};

		CHECK_ERR( _IsCreated() and _looping, void() );

		::SetWindowLongPtrA( _wnd.Get<HWND>(), GWLP_USERDATA, (LONG_PTR) this );
		::SetWindowLongPtrA( _wnd.Get<HWND>(), GWLP_WNDPROC,  (LONG_PTR) &Utils::MsgProc );
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
			case EVisibility::Visible :
				::SetForegroundWindow( _wnd.Get<HWND>() );
				::ShowWindow( _wnd.Get<HWND>(), SW_SHOWNA );
				break;

			case EVisibility::Unfocused :
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
		
		MSG		msg	= {0};

		while ( ::PeekMessageA( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			if ( WM_QUIT == msg.message )
			{
				_requestQuit = true;

				Send( Message< ModuleMsg::Delete >() );
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
		Message< ModuleMsg::WindowRawMessage >	msg{ this, uMsg, wParam, lParam };
		
		// WM_PAINT //
		if ( uMsg == WM_PAINT and _windowDesc.visibility == EVisibility::Visible )
		{
			msg->wasProcessed = true;

			TODO( "" );
			//_Update( true );
		}
		else

		// WM_MOVE //
		if ( uMsg == WM_MOVE )
		{
			msg->wasProcessed = true;

			_UpdateDescriptor();
			_SendEvent( Message< ModuleMsg::WindowDescriptorChanged >{ this, _windowDesc } );
		}
		else

		// WM_KILLPOCUS //
		if ( uMsg == WM_KILLFOCUS )
		{
			msg->wasProcessed = true;

			_windowDesc.visibility = EVisibility::Unfocused;
			_SendEvent( Message< ModuleMsg::WindowVisibilityChanged >{ this, _windowDesc.visibility } );
		}
		else

		// WM_SETFOCUS //
		if ( uMsg == WM_SETFOCUS )
		{
			msg->wasProcessed = true;
			
			_windowDesc.visibility = EVisibility::Visible;
			_SendEvent( Message< ModuleMsg::WindowVisibilityChanged >{ this, _windowDesc.visibility } );
		}
		else
			
		// WM_SIZE //
		if ( uMsg == WM_SIZE )
		{
			msg->wasProcessed = true;

			if ( wParam != SIZE_MINIMIZED )
			{
				_windowDesc.visibility = EVisibility::Invisible;
				_SendEvent( Message< ModuleMsg::WindowVisibilityChanged >{ this, _windowDesc.visibility } );
			}
			else
			{
				_UpdateDescriptor();
				_SendEvent( Message< ModuleMsg::WindowDescriptorChanged >{ this, _windowDesc } );
			}
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
	_UpdateDescriptor
=================================================
*/
	bool WinWindow::_UpdateDescriptor ()
	{
		CHECK_ERR( _IsCreated() );

		RECT	win_rect = {0,0,0,0};
		::GetWindowRect( _wnd.Get<HWND>(), &win_rect );

		_windowDesc.position	= int2( win_rect.left, win_rect.top );
		_windowDesc.size		= uint2( win_rect.right - win_rect.left, win_rect.bottom - win_rect.top );
		
		::GetClientRect( _wnd.Get<HWND>(), &win_rect );

		_windowDesc.surfaceSize	= uint2( win_rect.right - win_rect.left, win_rect.bottom - win_rect.top );

		return true;
	}
	
/*
=================================================
	_Resize
=================================================
*
	void WinWindow::_Resize ()
	{
		CHECK_ERR( _IsCreated(), void() );

		/*uint2 const	res = _GetScreenResolution();
		int2		pos;

		size = Clamp( size, uint2(12), res );

		DWORD dwStyle	= (DWORD) ::GetWindowLongA( _wnd.Get<HWND>(), GWL_STYLE );
		DWORD dwExStyle	= (DWORD) ::GetWindowLongA( _wnd.Get<HWND>(), GWL_EXSTYLE );

		RECT	winRect = { 0, 0, size.x, size.y };
		AdjustWindowRectEx( &winRect, dwStyle, FALSE, dwExStyle );
		
		TODO( "is window size setted?" );

		if ( alignCenter )
		{
			pos.x = Max( 0, (int)res.x - int(winRect.right  - winRect.left) ) / 2;
			pos.y = Max( 0, (int)res.y - int(winRect.bottom - winRect.top) ) / 2;

			::SetWindowPos( _wnd.Get<HWND>(), HWND_TOP, pos.x, pos.y, winRect.right - winRect.left,
							winRect.bottom - winRect.top, SWP_FRAMECHANGED );
		}* /
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
			::SetWindowLongPtrA( _wnd.Get<HWND>(), GWLP_WNDPROC, (LONG_PTR) ::DefWindowProcA );

			_SendEvent( Message< ModuleMsg::WindowBeforeDestroy >{ this } );

			::DestroyWindow( _wnd.Get<HWND>() );
			_wnd = null;

			LOG( "window destroyed", ELog::Debug );

			_SendEvent( Message< ModuleMsg::WindowAfterDestroy >{ this } );
		}
	}


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
