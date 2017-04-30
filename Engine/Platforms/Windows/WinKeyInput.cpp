// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Windows/WinKeyInput.h"
#include "Engine/Platforms/Input/InputThread.h"

#if defined( PLATFORM_WINDOWS )

#include <Windows.h>

namespace Engine
{
namespace Platforms
{
	
	static KeyID::type _MapKey (const RAWKEYBOARD &);

	const Runtime::VirtualTypeList	WinKeyInput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	WinKeyInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinKeyInput::WinKeyInput (const GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		SetDebugName( "WinKeyInput" );

		_SubscribeOnMsg( this, &WinKeyInput::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &WinKeyInput::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &WinKeyInput::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinKeyInput::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WinKeyInput::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &WinKeyInput::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinKeyInput::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &WinKeyInput::_Update );
		_SubscribeOnMsg( this, &WinKeyInput::_Link );
		_SubscribeOnMsg( this, &WinKeyInput::_Delete_Impl );
		_SubscribeOnMsg( this, &WinKeyInput::_WindowDescriptorChanged );
		_SubscribeOnMsg( this, &WinKeyInput::_WindowCreated );
		_SubscribeOnMsg( this, &WinKeyInput::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &WinKeyInput::_WindowRawMessage );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	WinKeyInput::~WinKeyInput ()
	{
		LOG( "WinKeyInput finalized", ELog::Debug );
	}
	
/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool WinKeyInput::_WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &msg)
	{
		return true;
	}
	
/*
=================================================
	_WindowCreated
=================================================
*/
	bool WinKeyInput::_WindowCreated (const Message< ModuleMsg::WindowCreated > &msg)
	{
		CHECK_ERR( msg->hwnd.IsNotNull<HWND>() );

		RAWINPUTDEVICE	Rid[1] = {};
		
		// keyboard
		Rid[0].usUsagePage	= 0x01;
		Rid[0].usUsage		= 0x06;
		Rid[0].dwFlags		= 0;	// RIDEV_INPUTSINK | RIDEV_NOHOTKEYS | RIDEV_NOLEGACY | RIDEV_REMOVE;
		Rid[0].hwndTarget	= msg->hwnd.Get<HWND>();

		CHECK( RegisterRawInputDevices( &Rid[0], CountOf(Rid), sizeof(Rid[0]) ) == TRUE );

		CHECK( _Compose( true ) );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool WinKeyInput::_WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &msg)
	{
		_SendMsg( Message< ModuleMsg::Delete >( this ) );
		return true;
	}
	
/*
=================================================
	_WindowRawMessage
=================================================
*/
	bool WinKeyInput::_WindowRawMessage (const Message< ModuleMsg::WindowRawMessage > &msg)
	{
		// WM_INPUT //
		if ( msg->uMsg == WM_INPUT )
		{
			ubyte	input_data[60];
			uint	data_size = sizeof(input_data);

			if ( ::GetRawInputData( (HRAWINPUT) msg->lParam, RID_INPUT, input_data, &data_size, sizeof(RAWINPUTHEADER) ) != -1 )
			{
				RAWINPUT  *	p_data = (RAWINPUT *)input_data;

				// KEYBOARD //
				if ( p_data->header.dwType == RIM_TYPEKEYBOARD )
				{
					const KeyID::type	key		= _MapKey( p_data->data.keyboard );
					const bool			down	= (p_data->data.keyboard.Flags & RI_KEY_BREAK) == 0;

					if ( not (key == KeyID::Unknown and p_data->data.keyboard.VKey == 0xFF) )
						_pendingKeys.PushBack( ModuleMsg::InputKey( key, down ? 1.0f : -1.0f ) );
				}
				else

				// MOUSE //
				if ( p_data->header.dwType == RIM_TYPEMOUSE )
				{
					const int			wheel_delta = short( p_data->data.mouse.usButtonData ) / WHEEL_DELTA;
					const KeyID::type	key			= (wheel_delta > 0 ? "mouse wheel+"_KeyID : "mouse wheel-"_KeyID);

					for (int i = Min( 3, Abs(wheel_delta) ); i > 0; --i)
					{
						_pendingKeys.PushBack( ModuleMsg::InputKey( key, 1.0f ) );
						_pendingKeys.PushBack( ModuleMsg::InputKey( key, -1.0f ) );
					}
				}
			}
		}
		else

		// WM_LBUTTONUP //
		// WM_LBUTTONDOWN //
		if ( msg->uMsg == WM_LBUTTONDOWN or msg->uMsg == WM_LBUTTONUP or
			 msg->uMsg == WM_RBUTTONDOWN or msg->uMsg == WM_RBUTTONUP or
			 msg->uMsg == WM_MBUTTONDOWN or msg->uMsg == WM_MBUTTONUP or
			 msg->uMsg == WM_XBUTTONDOWN or msg->uMsg == WM_XBUTTONUP )
		{
			if ( msg->uMsg == WM_LBUTTONDOWN or msg->uMsg == WM_LBUTTONUP )
				_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 0"_KeyID, msg->uMsg == WM_LBUTTONDOWN ? 1.0f : -1.0f ) );
			else
			if ( msg->uMsg == WM_RBUTTONDOWN or msg->uMsg == WM_RBUTTONUP )
				_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 1"_KeyID, msg->uMsg == WM_RBUTTONDOWN ? 1.0f : -1.0f ) );
			else
			if ( msg->uMsg == WM_MBUTTONDOWN or msg->uMsg == WM_MBUTTONUP )
				_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 2"_KeyID, msg->uMsg == WM_MBUTTONDOWN ? 1.0f : -1.0f ) );
			else
			if ( msg->uMsg == WM_XBUTTONDOWN or msg->uMsg == WM_XBUTTONUP )
			{
				const uint	btn = GET_XBUTTON_WPARAM( msg->wParam );

				if ( btn & XBUTTON1 )
					_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 3"_KeyID, msg->uMsg == WM_XBUTTONDOWN ? 1.0f : -1.0f ) );
				else
				if ( btn & XBUTTON2 )
					_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 4"_KeyID, msg->uMsg == WM_XBUTTONDOWN ? 1.0f : -1.0f ) );
				else
					WARNING( "unknown mouse button" );
			}
		}
		return true;
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool WinKeyInput::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( Module::_Link_Impl( msg ) );

		// attach to manager
		{
			ModulePtr	input = GlobalSystems()->Get< ParallelThread >()->GetModule( InputThread::GetStaticID() );
			CHECK_ERR( input );

			_AttachSelfToManager( input, UntypedID_t(), true );
		}

		// subscribe on window events
		{
			ModulePtr	wnd = GlobalSystems()->Get< ParallelThread >()->GetModule( WinWindow::GetStaticID() );
			CHECK_ERR( wnd );

			if ( _IsComposedState( wnd->GetState() ) )
			{
				Message< ModuleMsg::WindowGetHandle >	request_hwnd;

				wnd->Send( request_hwnd );

				if ( request_hwnd->hwnd.Get().IsDefined() and
					 request_hwnd->hwnd.Get().Get().IsNotNull<HWND>() )
				{
					_SendMsg( Message< ModuleMsg::WindowCreated >{ this, WindowDesc(), request_hwnd->hwnd.Get().Get() } );
				}
			}

			wnd->Subscribe( this, &WinKeyInput::_WindowDescriptorChanged );
			wnd->Subscribe( this, &WinKeyInput::_WindowCreated );
			wnd->Subscribe( this, &WinKeyInput::_WindowBeforeDestroy );
			wnd->Subscribe( this, &WinKeyInput::_WindowRawMessage );
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool WinKeyInput::_Update (const Message< ModuleMsg::Update > &msg)
	{
		// send events to InputThread

		FOR( i, _pendingKeys )
		{
			_SendEvent( Message< ModuleMsg::InputKey >( this, _pendingKeys[i].key, _pendingKeys[i].pressure ) );
		}

		_pendingKeys.Clear();
		return true;
	}
	
/*
=================================================
	_MapKey
=================================================
*/
	static KeyID::type  _MapKey (const RAWKEYBOARD &kb)
	{
		ASSERT( kb.VKey < 256 );

		// TODO: change 8 and 16 to constant

		const uint	code = kb.VKey << (kb.Flags & RI_KEY_E0 ? 8 : (kb.Flags & RI_KEY_E1 ? 16 : 0));

		switch ( code )
		{
			case VK_LBUTTON				: return "mouse 0"_KeyID;
			case VK_RBUTTON				: return "mouse 1"_KeyID;
			case VK_CANCEL				: return "break"_KeyID;
			case VK_MBUTTON				: return "mouse 2"_KeyID;
			case VK_XBUTTON1			: return "mouse 3"_KeyID;
			case VK_XBUTTON2			: return "mouse 4"_KeyID;

			case VK_BACK				: return "backspace"_KeyID;
			case VK_TAB					: return "tab"_KeyID;
			case VK_CLEAR				: return "clear"_KeyID;
			case VK_RETURN				: return "enter"_KeyID;
			case VK_SHIFT				: return "shift"_KeyID;		// left and right shift		// TODO: fix it
			case VK_CONTROL				: return "l-ctrl"_KeyID;
			case (VK_CONTROL << 8)		: return "r-ctrl"_KeyID;
			case VK_MENU				: return "l-alt"_KeyID;
			case (VK_MENU << 8)			: return "r-alt"_KeyID;
			case VK_PAUSE				: return "pause"_KeyID;
			case VK_CAPITAL				: return "caps lock"_KeyID;
			//case VK_KANA				: return "kana"_KeyID;
			//case VK_HANGUL			: return "hangul"_KeyID;
			case VK_ESCAPE				: return "escape"_KeyID;
			case VK_SPACE				: return "space"_KeyID;
			case (VK_PRIOR << 8)		: return "page up"_KeyID;
			case (VK_NEXT << 8)			: return "page down"_KeyID;
			case (VK_END << 8)			: return "end"_KeyID;
			case (VK_HOME << 8)			: return "home"_KeyID;
			case (VK_LEFT << 8)			: return "arrow left"_KeyID;
			case (VK_UP << 8)			: return "arrow up"_KeyID;
			case (VK_RIGHT << 8)		: return "arrow right"_KeyID;
			case (VK_DOWN << 8)			: return "arrow down"_KeyID;
			case VK_SELECT				: return "select"_KeyID;
			case VK_PRINT				: return "print"_KeyID;
			case VK_EXECUTE				: return "execute"_KeyID;
			case VK_SNAPSHOT			: return "print screen"_KeyID;
			case (VK_INSERT << 8)		: return "insert"_KeyID;
			case (VK_DELETE << 8)		: return "delete"_KeyID;
			case VK_HELP				: return "help"_KeyID;
			case 0x30					: return "0"_KeyID;
			case 0x31					: return "1"_KeyID;
			case 0x32					: return "2"_KeyID;
			case 0x33					: return "3"_KeyID;
			case 0x34					: return "4"_KeyID;
			case 0x35					: return "5"_KeyID;
			case 0x36					: return "6"_KeyID;
			case 0x37					: return "7"_KeyID;
			case 0x38					: return "8"_KeyID;
			case 0x39					: return "9"_KeyID;
			case 0x41					: return "A"_KeyID;
			case 0x42					: return "B"_KeyID;
			case 0x43					: return "C"_KeyID;
			case 0x44					: return "D"_KeyID;
			case 0x45					: return "E"_KeyID;
			case 0x46					: return "F"_KeyID;
			case 0x47					: return "G"_KeyID;
			case 0x48					: return "H"_KeyID;
			case 0x49					: return "I"_KeyID;
			case 0x4A					: return "J"_KeyID;
			case 0x4B					: return "K"_KeyID;
			case 0x4C					: return "L"_KeyID;
			case 0x4D					: return "M"_KeyID;
			case 0x4E					: return "N"_KeyID;
			case 0x4F					: return "O"_KeyID;
			case 0x50					: return "P"_KeyID;
			case 0x51					: return "Q"_KeyID;
			case 0x52					: return "R"_KeyID;
			case 0x53					: return "S"_KeyID;
			case 0x54					: return "T"_KeyID;
			case 0x55					: return "U"_KeyID;
			case 0x56					: return "V"_KeyID;
			case 0x57					: return "W"_KeyID;
			case 0x58					: return "X"_KeyID;
			case 0x59					: return "Y"_KeyID;
			case 0x5A					: return "Z"_KeyID;
			case VK_LWIN				: return "l-win"_KeyID;
			case (VK_RWIN << 8)			: return "r-win"_KeyID;
			case VK_APPS				: return "l-apps"_KeyID;
			case (VK_APPS << 8)			: return "r-apps"_KeyID;
			case VK_SLEEP				: return "sleep"_KeyID;
				
			case (VK_RETURN << 8)		: return "numpad enter"_KeyID;
			case VK_NUMPAD0				: return "numpad 0"_KeyID;
			case VK_NUMPAD1				: return "numpad 1"_KeyID;
			case VK_NUMPAD2				: return "numpad 2"_KeyID;
			case VK_NUMPAD3				: return "numpad 3"_KeyID;
			case VK_NUMPAD4				: return "numpad 4"_KeyID;
			case VK_NUMPAD5				: return "numpad 5"_KeyID;
			case VK_NUMPAD6				: return "numpad 6"_KeyID;
			case VK_NUMPAD7				: return "numpad 7"_KeyID;
			case VK_NUMPAD8				: return "numpad 8"_KeyID;
			case VK_NUMPAD9				: return "numpad 9"_KeyID;
			case VK_MULTIPLY			: return "numpad *"_KeyID;
			case VK_ADD					: return "numpad +"_KeyID;
			case VK_SEPARATOR			: return "numpad sep"_KeyID;
			case VK_SUBTRACT			: return "numpad -"_KeyID;
			case VK_DECIMAL				: return "numpad ."_KeyID;
			case (VK_DIVIDE << 8)		:
			case VK_DIVIDE				: return "numpad /"_KeyID;
			case VK_END					: return "numpad end"_KeyID;
			case VK_LEFT				: return "numpad left"_KeyID;
			case VK_UP					: return "numpad up"_KeyID;
			case VK_RIGHT				: return "numpad right"_KeyID;
			case VK_DOWN				: return "numpad down"_KeyID;
			case VK_HOME				: return "numpad home"_KeyID;
			case VK_PRIOR				: return "numpad pg.up"_KeyID;
			case VK_NEXT				: return "numpad pg.dn"_KeyID;
			case VK_INSERT				: return "numpad ins"_KeyID;
			case VK_DELETE				: return "numpad del"_KeyID;

			case VK_F1					: return "F1"_KeyID;
			case VK_F2					: return "F2"_KeyID;
			case VK_F3					: return "F3"_KeyID;
			case VK_F4					: return "F4"_KeyID;
			case VK_F5					: return "F5"_KeyID;
			case VK_F6					: return "F6"_KeyID;
			case VK_F7					: return "F7"_KeyID;
			case VK_F8					: return "F8"_KeyID;
			case VK_F9					: return "F9"_KeyID;
			case VK_F10					: return "F10"_KeyID;
			case VK_F11					: return "F11"_KeyID;
			case VK_F12					: return "F12"_KeyID;
			case VK_F13					: return "F13"_KeyID;
			case VK_F14					: return "F14"_KeyID;
			case VK_F15					: return "F15"_KeyID;
			case VK_F16					: return "F16"_KeyID;
			case VK_F17					: return "F17"_KeyID;
			case VK_F18					: return "F18"_KeyID;
			case VK_F19					: return "F19"_KeyID;
			case VK_F20					: return "F20"_KeyID;
			case VK_F21					: return "F21"_KeyID;
			case VK_F22					: return "F22"_KeyID;
			case VK_F23					: return "F23"_KeyID;
			case VK_F24					: return "F24"_KeyID;
			case VK_NUMLOCK				: return "num lock"_KeyID;
			case VK_SCROLL				: return "scroll lock"_KeyID;
			case VK_LSHIFT				: return "l-shift"_KeyID;
			case VK_RSHIFT				: return "r-shift"_KeyID;
			//case VK_LCONTROL			: return "l-ctrl"_KeyID;
			//case VK_RCONTROL			: return "r-ctrl"_KeyID;
			case VK_LMENU				: return "l-menu"_KeyID;
			case VK_RMENU				: return "r-menu"_KeyID;

			case 186					: return ";"_KeyID;
			case 187					: return "="_KeyID;
			case 188					: return ","_KeyID;
			case 189					: return "-"_KeyID;
			case 190					: return "."_KeyID;
			case 191					: return "/"_KeyID;
			case 192					: return "~"_KeyID;
			case 219					: return "["_KeyID;
			case 220					: return "\\"_KeyID;
			case 221					: return "]"_KeyID;
			case 222					: return "'"_KeyID;

			case VK_BROWSER_BACK		: return "brow back"_KeyID;
			case VK_BROWSER_FORWARD		: return "brow forward"_KeyID;
			case VK_BROWSER_REFRESH		: return "brow refresh"_KeyID;
			case VK_BROWSER_STOP		: return "brow stop"_KeyID;
			case VK_BROWSER_SEARCH		: return "brow search"_KeyID;
			case VK_BROWSER_FAVORITES	: return "brow favor"_KeyID;
			case VK_BROWSER_HOME		: return "brow home"_KeyID;
			case VK_VOLUME_MUTE			: return "vol mute"_KeyID;
			case VK_VOLUME_DOWN			: return "vol -"_KeyID;
			case VK_VOLUME_UP			: return "vol +"_KeyID;
			case VK_MEDIA_NEXT_TRACK	: return "media next"_KeyID;
			case VK_MEDIA_PREV_TRACK	: return "media prev"_KeyID;
			case VK_MEDIA_STOP			: return "media stop"_KeyID;
			case VK_MEDIA_PLAY_PAUSE	: return "media play"_KeyID;
			case VK_LAUNCH_MAIL			: return "mail"_KeyID;
			case VK_LAUNCH_MEDIA_SELECT	: return "media select"_KeyID;
			case VK_LAUNCH_APP1			: return "launch app1"_KeyID;
			case VK_LAUNCH_APP2			: return "launch app2"_KeyID;
			case VK_ATTN				: return "attn"_KeyID;
			case VK_CRSEL				: return "crsel"_KeyID;
			case VK_EXSEL				: return "exsel"_KeyID;
			case VK_EREOF				: return "erase eof"_KeyID;
			case VK_PLAY				: return "play"_KeyID;
			case VK_ZOOM				: return "zoom"_KeyID;
		}

		return KeyID::Unknown;
	}


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
