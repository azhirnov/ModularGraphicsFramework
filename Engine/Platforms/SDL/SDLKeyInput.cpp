// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/Platforms/Public/OS/Input.h"
#include "Engine/Platforms/SDL/SDLMessages.h"
#include "Engine/Platforms/SDL/SDLObjectsConstructor.h"

namespace Engine
{
namespace PlatformSDL
{
	using namespace Platforms;


	//
	// SDL Key Input
	//

	class SDLKeyInput final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											OSMsg::WindowDescriptorChanged,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											OSMsg::OnSDLWindowRawMessage
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputKey
										> >;

		using WindowMsgList_t		= MessageListFrom< OSMsg::GetSDLWindowHandle >;
		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy, OSMsg::OnSDLWindowRawMessage >;

		using Keys_t				= Array< ModuleMsg::InputKey >;	// TODO: use static array


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		ModulePtr	_window;
		Keys_t		_pendingKeys;	// store keys and send in Update


	// methods
	public:
		SDLKeyInput (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci);
		~SDLKeyInput ();


	// message handlers
	private:
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _OnSDLWindowRawMessage (const Message< OSMsg::OnSDLWindowRawMessage > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

	private:
		void _Destroy ();
	};
//-----------------------------------------------------------------------------


	
	static KeyID::type _MapKey (SDL_Scancode);

	const TypeIdList	SDLKeyInput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SDLKeyInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SDLKeyInput::SDLKeyInput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RawInputHandler &) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "SDLKeyInput" );

		_SubscribeOnMsg( this, &SDLKeyInput::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SDLKeyInput::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SDLKeyInput::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SDLKeyInput::_DetachModule );
		_SubscribeOnMsg( this, &SDLKeyInput::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &SDLKeyInput::_FindModule_Impl );
		_SubscribeOnMsg( this, &SDLKeyInput::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SDLKeyInput::_Update );
		_SubscribeOnMsg( this, &SDLKeyInput::_Link );
		_SubscribeOnMsg( this, &SDLKeyInput::_Delete_Impl );
		_SubscribeOnMsg( this, &SDLKeyInput::_WindowDescriptorChanged );
		_SubscribeOnMsg( this, &SDLKeyInput::_WindowCreated );
		_SubscribeOnMsg( this, &SDLKeyInput::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &SDLKeyInput::_OnSDLWindowRawMessage );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, InputThreadModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SDLKeyInput::~SDLKeyInput ()
	{
		LOG( "SDLKeyInput finalized", ELog::Debug );
	}
	
/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool SDLKeyInput::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &)
	{
		return true;
	}
	
/*
=================================================
	_WindowCreated
=================================================
*/
	bool SDLKeyInput::_WindowCreated (const Message< OSMsg::WindowCreated > &)
	{
		CHECK( _DefCompose( false ) );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool SDLKeyInput::_WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &)
	{
		_Destroy();

		_SendMsg< ModuleMsg::Delete >({});
		return true;
	}
	
/*
=================================================
	_OnSDLWindowRawMessage
=================================================
*/
	bool SDLKeyInput::_OnSDLWindowRawMessage (const Message< OSMsg::OnSDLWindowRawMessage > &msg)
	{
		switch ( msg->event.type )
		{
			// key //
			case SDL_KEYDOWN :
			case SDL_KEYUP :
			{
				const KeyID::type	key		= _MapKey( msg->event.key.keysym.scancode );
				const bool			down	= msg->event.type == SDL_KEYDOWN;
					
				if ( not (key == KeyID::Unknown ) )
					_pendingKeys.PushBack( ModuleMsg::InputKey( key, down ? 1.0f : -1.0f ) );

				break;
			}
			// mouse button //
			case SDL_MOUSEBUTTONDOWN :
			case SDL_MOUSEBUTTONUP :
			{
				const float		pressure = (msg->event.button.state == SDL_PRESSED ? 1.0f : -1.0f);

				switch ( msg->event.button.button )
				{
					case SDL_BUTTON_LEFT :		_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 0"_KeyID, pressure ) );	break;
					case SDL_BUTTON_RIGHT :		_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 1"_KeyID, pressure ) );	break;
					case SDL_BUTTON_MIDDLE :	_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 2"_KeyID, pressure ) );	break;
					case SDL_BUTTON_X1 :		_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 3"_KeyID, pressure ) );	break;
					case SDL_BUTTON_X2 :		_pendingKeys.PushBack( ModuleMsg::InputKey( "mouse 4"_KeyID, pressure ) );	break;
				}
				break;
			}

			// mouse wheel //
			case SDL_MOUSEWHEEL :
			{
				const int			wheel_delta = msg->event.wheel.y;
				const KeyID::type	key			= (wheel_delta > 0 ? "mouse wheel+"_KeyID : "mouse wheel-"_KeyID);
				
				for (int i = Min( 3, Abs(wheel_delta) ); i > 0; --i)
				{
					_pendingKeys.PushBack( ModuleMsg::InputKey( key, 1.0f ) );
					_pendingKeys.PushBack( ModuleMsg::InputKey( key, -1.0f ) );
				}
				break;
			}
		}

		return true;
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SDLKeyInput::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( Module::_Link_Impl( msg ) );

		// subscribe on window events
		{
			CHECK_ATTACHMENT(( _window = GetParentByMsgEvent< WindowMsgList_t, WindowEventList_t >() ));

			if ( _IsComposedState( _window->GetState() ) )
			{
				_SendMsg< OSMsg::WindowCreated >({});
			}

			_window->Subscribe( this, &SDLKeyInput::_WindowDescriptorChanged );
			_window->Subscribe( this, &SDLKeyInput::_WindowCreated );
			_window->Subscribe( this, &SDLKeyInput::_WindowBeforeDestroy );
			_window->Subscribe( this, &SDLKeyInput::_OnSDLWindowRawMessage );
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool SDLKeyInput::_Update (const Message< ModuleMsg::Update > &)
	{
		// send events to InputThread
		FOR( i, _pendingKeys )
		{
			_SendEvent< ModuleMsg::InputKey >({ _pendingKeys[i].key, _pendingKeys[i].pressure });
		}

		_pendingKeys.Clear();
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool SDLKeyInput::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		if ( _Detach( msg->oldModule ) and msg->oldModule == _window )
		{
			CHECK( _SetState( EState::Initial ) );
			_Destroy();
		}
		return true;
	}
	
/*
=================================================
	_Destroy
=================================================
*/
	void SDLKeyInput::_Destroy ()
	{
		if ( _window )
			_window->UnsubscribeAll( this );

		_window = null;
	}

/*
=================================================
	_MapKey
=================================================
*/
	static KeyID::type  _MapKey (SDL_Scancode code)
	{
		switch ( code )
		{
			case SDL_SCANCODE_BACKSPACE		: return "backspace"_KeyID;
			case SDL_SCANCODE_TAB			: return "tab"_KeyID;
			case SDL_SCANCODE_CLEAR			: return "clear"_KeyID;
			case SDL_SCANCODE_RETURN		: return "enter"_KeyID;
			case SDL_SCANCODE_LCTRL			: return "l-ctrl"_KeyID;
			case SDL_SCANCODE_RCTRL			: return "r-ctrl"_KeyID;
			case SDL_SCANCODE_LALT			: return "l-alt"_KeyID;
			case SDL_SCANCODE_RALT			: return "r-alt"_KeyID;
			case SDL_SCANCODE_PAUSE			: return "pause"_KeyID;
			case SDL_SCANCODE_CAPSLOCK		: return "caps lock"_KeyID;
			case SDL_SCANCODE_ESCAPE		: return "escape"_KeyID;
			case SDL_SCANCODE_SPACE			: return "space"_KeyID;
			case SDL_SCANCODE_PAGEUP		: return "page up"_KeyID;
			case SDL_SCANCODE_PAGEDOWN		: return "page down"_KeyID;
			case SDL_SCANCODE_END			: return "end"_KeyID;
			case SDL_SCANCODE_HOME			: return "home"_KeyID;
			case SDL_SCANCODE_LEFT			: return "arrow left"_KeyID;
			case SDL_SCANCODE_UP			: return "arrow up"_KeyID;
			case SDL_SCANCODE_RIGHT			: return "arrow right"_KeyID;
			case SDL_SCANCODE_DOWN			: return "arrow down"_KeyID;
			case SDL_SCANCODE_SELECT		: return "select"_KeyID;
			case SDL_SCANCODE_EXECUTE		: return "execute"_KeyID;
			case SDL_SCANCODE_PRINTSCREEN	: return "print screen"_KeyID;
			case SDL_SCANCODE_INSERT		: return "insert"_KeyID;
			case SDL_SCANCODE_DELETE		: return "delete"_KeyID;
			case SDL_SCANCODE_HELP			: return "help"_KeyID;
			case SDL_SCANCODE_STOP			: return "stop"_KeyID;
			case SDL_SCANCODE_AGAIN			: return "again"_KeyID;
			case SDL_SCANCODE_UNDO			: return "undo"_KeyID;
			case SDL_SCANCODE_CUT			: return "cut"_KeyID;
			case SDL_SCANCODE_COPY			: return "copy"_KeyID;
			case SDL_SCANCODE_PASTE			: return "paste"_KeyID;
			case SDL_SCANCODE_FIND			: return "find"_KeyID;

			case SDL_SCANCODE_0				: return "0"_KeyID;
			case SDL_SCANCODE_1				: return "1"_KeyID;
			case SDL_SCANCODE_2				: return "2"_KeyID;
			case SDL_SCANCODE_3				: return "3"_KeyID;
			case SDL_SCANCODE_4				: return "4"_KeyID;
			case SDL_SCANCODE_5				: return "5"_KeyID;
			case SDL_SCANCODE_6				: return "6"_KeyID;
			case SDL_SCANCODE_7				: return "7"_KeyID;
			case SDL_SCANCODE_8				: return "8"_KeyID;
			case SDL_SCANCODE_9				: return "9"_KeyID;

			case SDL_SCANCODE_A				: return "A"_KeyID;
			case SDL_SCANCODE_B				: return "B"_KeyID;
			case SDL_SCANCODE_C				: return "C"_KeyID;
			case SDL_SCANCODE_D				: return "D"_KeyID;
			case SDL_SCANCODE_E				: return "E"_KeyID;
			case SDL_SCANCODE_F				: return "F"_KeyID;
			case SDL_SCANCODE_G				: return "G"_KeyID;
			case SDL_SCANCODE_H				: return "H"_KeyID;
			case SDL_SCANCODE_I				: return "I"_KeyID;
			case SDL_SCANCODE_J				: return "J"_KeyID;
			case SDL_SCANCODE_K				: return "K"_KeyID;
			case SDL_SCANCODE_L				: return "L"_KeyID;
			case SDL_SCANCODE_M				: return "M"_KeyID;
			case SDL_SCANCODE_N				: return "N"_KeyID;
			case SDL_SCANCODE_O				: return "O"_KeyID;
			case SDL_SCANCODE_P				: return "P"_KeyID;
			case SDL_SCANCODE_Q				: return "Q"_KeyID;
			case SDL_SCANCODE_R				: return "R"_KeyID;
			case SDL_SCANCODE_S				: return "S"_KeyID;
			case SDL_SCANCODE_T				: return "T"_KeyID;
			case SDL_SCANCODE_U				: return "U"_KeyID;
			case SDL_SCANCODE_V				: return "V"_KeyID;
			case SDL_SCANCODE_W				: return "W"_KeyID;
			case SDL_SCANCODE_X				: return "X"_KeyID;
			case SDL_SCANCODE_Y				: return "Y"_KeyID;
			case SDL_SCANCODE_Z				: return "Z"_KeyID;

			case SDL_SCANCODE_LGUI			: return "l-win"_KeyID;
			case SDL_SCANCODE_RGUI			: return "r-win"_KeyID;
			case SDL_SCANCODE_APPLICATION	: return "l-apps"_KeyID;
			case SDL_SCANCODE_SLEEP			: return "sleep"_KeyID;
				
			case SDL_SCANCODE_KP_ENTER		: return "numpad enter"_KeyID;
			case SDL_SCANCODE_KP_0			: return "numpad 0"_KeyID;
			case SDL_SCANCODE_KP_1			: return "numpad 1"_KeyID;
			case SDL_SCANCODE_KP_2			: return "numpad 2"_KeyID;
			case SDL_SCANCODE_KP_3			: return "numpad 3"_KeyID;
			case SDL_SCANCODE_KP_4			: return "numpad 4"_KeyID;
			case SDL_SCANCODE_KP_5			: return "numpad 5"_KeyID;
			case SDL_SCANCODE_KP_6			: return "numpad 6"_KeyID;
			case SDL_SCANCODE_KP_7			: return "numpad 7"_KeyID;
			case SDL_SCANCODE_KP_8			: return "numpad 8"_KeyID;
			case SDL_SCANCODE_KP_9			: return "numpad 9"_KeyID;
			case SDL_SCANCODE_KP_MULTIPLY	: return "numpad *"_KeyID;
			case SDL_SCANCODE_KP_PLUS		: return "numpad +"_KeyID;
			case SDL_SCANCODE_SEPARATOR		: return "numpad sep"_KeyID;
			case SDL_SCANCODE_KP_MINUS		: return "numpad -"_KeyID;
			case SDL_SCANCODE_KP_PERIOD		: return "numpad ."_KeyID;
			case SDL_SCANCODE_KP_DIVIDE		: return "numpad /"_KeyID;
			case SDL_SCANCODE_KP_EQUALS		: return "numpad ="_KeyID;
			case SDL_SCANCODE_KP_COMMA		: return "numpad ,"_KeyID;

			case SDL_SCANCODE_F1			: return "F1"_KeyID;
			case SDL_SCANCODE_F2			: return "F2"_KeyID;
			case SDL_SCANCODE_F3			: return "F3"_KeyID;
			case SDL_SCANCODE_F4			: return "F4"_KeyID;
			case SDL_SCANCODE_F5			: return "F5"_KeyID;
			case SDL_SCANCODE_F6			: return "F6"_KeyID;
			case SDL_SCANCODE_F7			: return "F7"_KeyID;
			case SDL_SCANCODE_F8			: return "F8"_KeyID;
			case SDL_SCANCODE_F9			: return "F9"_KeyID;
			case SDL_SCANCODE_F10			: return "F10"_KeyID;
			case SDL_SCANCODE_F11			: return "F11"_KeyID;
			case SDL_SCANCODE_F12			: return "F12"_KeyID;
			case SDL_SCANCODE_F13			: return "F13"_KeyID;
			case SDL_SCANCODE_F14			: return "F14"_KeyID;
			case SDL_SCANCODE_F15			: return "F15"_KeyID;
			case SDL_SCANCODE_F16			: return "F16"_KeyID;
			case SDL_SCANCODE_F17			: return "F17"_KeyID;
			case SDL_SCANCODE_F18			: return "F18"_KeyID;
			case SDL_SCANCODE_F19			: return "F19"_KeyID;
			case SDL_SCANCODE_F20			: return "F20"_KeyID;
			case SDL_SCANCODE_F21			: return "F21"_KeyID;
			case SDL_SCANCODE_F22			: return "F22"_KeyID;
			case SDL_SCANCODE_F23			: return "F23"_KeyID;
			case SDL_SCANCODE_F24			: return "F24"_KeyID;
			
			case SDL_SCANCODE_NUMLOCKCLEAR	: return "num lock"_KeyID;
			case SDL_SCANCODE_SCROLLLOCK	: return "scroll lock"_KeyID;
			case SDL_SCANCODE_LSHIFT		: return "l-shift"_KeyID;
			case SDL_SCANCODE_RSHIFT		: return "r-shift"_KeyID;
			case SDL_SCANCODE_MENU			: return "l-menu"_KeyID;

			case SDL_SCANCODE_SEMICOLON		: return ";"_KeyID;
			case SDL_SCANCODE_EQUALS		: return "="_KeyID;
			case SDL_SCANCODE_COMMA			: return ","_KeyID;
			case SDL_SCANCODE_MINUS			: return "-"_KeyID;
			case SDL_SCANCODE_PERIOD		: return "."_KeyID;
			case SDL_SCANCODE_BACKSLASH		: return "/"_KeyID;
			case SDL_SCANCODE_GRAVE			: return "~"_KeyID;
			case SDL_SCANCODE_LEFTBRACKET	: return "["_KeyID;
			case SDL_SCANCODE_SLASH			: return "\\"_KeyID;
			case SDL_SCANCODE_RIGHTBRACKET	: return "]"_KeyID;
			case SDL_SCANCODE_APOSTROPHE	: return "'"_KeyID;

			case SDL_SCANCODE_AC_BACK		: return "brow back"_KeyID;
			case SDL_SCANCODE_AC_FORWARD	: return "brow forward"_KeyID;
			case SDL_SCANCODE_AC_REFRESH	: return "brow refresh"_KeyID;
			case SDL_SCANCODE_AC_STOP		: return "brow stop"_KeyID;
			case SDL_SCANCODE_AC_SEARCH		: return "brow search"_KeyID;
			case SDL_SCANCODE_AC_BOOKMARKS	: return "brow favor"_KeyID;
			case SDL_SCANCODE_AC_HOME		: return "brow home"_KeyID;

			case SDL_SCANCODE_MUTE			: return "vol mute"_KeyID;
			case SDL_SCANCODE_VOLUMEDOWN	: return "vol -"_KeyID;
			case SDL_SCANCODE_VOLUMEUP		: return "vol +"_KeyID;
			case SDL_SCANCODE_AUDIONEXT		: return "media next"_KeyID;
			case SDL_SCANCODE_AUDIOPREV		: return "media prev"_KeyID;
			case SDL_SCANCODE_AUDIOSTOP		: return "media stop"_KeyID;
			case SDL_SCANCODE_AUDIOPLAY		: return "media play"_KeyID;
			case SDL_SCANCODE_AUDIOMUTE		: return "media mute"_KeyID;
			case SDL_SCANCODE_MEDIASELECT	: return "media select"_KeyID;

			case SDL_SCANCODE_MAIL			: return "mail"_KeyID;
			case SDL_SCANCODE_APP1			: return "launch app1"_KeyID;
			case SDL_SCANCODE_APP2			: return "launch app2"_KeyID;
			case SDL_SCANCODE_CRSEL			: return "crsel"_KeyID;
			case SDL_SCANCODE_EXSEL			: return "exsel"_KeyID;
			case SDL_SCANCODE_ALTERASE		: return "alterase"_KeyID;
			case SDL_SCANCODE_SYSREQ		: return "sys req"_KeyID;
			case SDL_SCANCODE_CANCEL		: return "cancel"_KeyID;
			case SDL_SCANCODE_PRIOR			: return "prior"_KeyID;
			case SDL_SCANCODE_RETURN2		: return "enter 2"_KeyID;
			case SDL_SCANCODE_OUT			: return "out"_KeyID;
			//case SDL_SCANCODE_OPER			: return "oper"_KeyID;
			//case SDL_SCANCODE_CLEARAGAIN
			//case SDL_SCANCODE_CALCULATOR
			//case SDL_SCANCODE_COMPUTER
			//case SDL_SCANCODE_WWW
			//case SDL_SCANCODE_BRIGHTNESSDOWN
			//case SDL_SCANCODE_BRIGHTNESSUP
			//case SDL_SCANCODE_DISPLAYSWITCH
			//case SDL_SCANCODE_KBDILLUMTOGGLE
			//case SDL_SCANCODE_KBDILLUMDOWN
			//case SDL_SCANCODE_KBDILLUMUP
			//case SDL_SCANCODE_EJECT
			//case SDL_SCANCODE_AUDIOREWIND
			//case SDL_SCANCODE_AUDIOFASTFORWARD

			// SDL_SCANCODE_KP_00 .. SDL_SCANCODE_KP_HEXADECIMAL


		}

		return KeyID::Unknown;
	}

}	// PlatformSDL
//-----------------------------------------------------------------------------


namespace Platforms
{
/*
=================================================
	CreateSDLKeyInput
=================================================
*/
	ModulePtr SDLObjectsConstructor::CreateSDLKeyInput (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci)
	{
		return New< PlatformSDL::SDLKeyInput >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_SDL
