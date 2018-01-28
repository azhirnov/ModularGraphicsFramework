// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/OS/Input.h"
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/Windows/WinObjectsConstructor.h"

#if defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"

namespace Engine
{
namespace Platforms
{

	//
	// Windows Mouse Input
	//

	class WinMouseInput final : public Module
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
											OSMsg::OnWinWindowRawMessage
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputMotion
										> >;

		using WindowMsgList_t		= MessageListFrom< OSMsg::GetWinWindowHandle >;
		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy, OSMsg::OnWinWindowRawMessage >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		ModulePtr			_window;
		uint2				_surfaceSize;
		Optional<float2>	_mouseDifference;
		Optional<float>		_wheelDelta;
		float2				_mousePos;
		bool				_lbPressed;
		

	// methods
	public:
		WinMouseInput (GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci);
		~WinMouseInput ();


	// message handlers
	private:
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _OnWinWindowRawMessage (const Message< OSMsg::OnWinWindowRawMessage > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

	private:
		void _Destroy ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	WinMouseInput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	WinMouseInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinMouseInput::WinMouseInput (GlobalSystemsRef gs, const CreateInfo::RawInputHandler &) :
		Module( gs, ModuleConfig{ WinMouseInputModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_lbPressed{ false }
	{
		SetDebugName( "WinMouseInput" );
		
		_SubscribeOnMsg( this, &WinMouseInput::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_DetachModule );
		_SubscribeOnMsg( this, &WinMouseInput::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &WinMouseInput::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_Update );
		_SubscribeOnMsg( this, &WinMouseInput::_Link );
		_SubscribeOnMsg( this, &WinMouseInput::_Delete_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_WindowDescriptorChanged );
		_SubscribeOnMsg( this, &WinMouseInput::_WindowCreated );
		_SubscribeOnMsg( this, &WinMouseInput::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &WinMouseInput::_OnWinWindowRawMessage );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, InputThreadModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	WinMouseInput::~WinMouseInput ()
	{
		LOG( "WinMouseInput finalized", ELog::Debug );
	}

/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool WinMouseInput::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &msg)
	{
		_surfaceSize = msg->desc.surfaceSize;
		return true;
	}
	
/*
=================================================
	_WindowCreated
=================================================
*/
	bool WinMouseInput::_WindowCreated (const Message< OSMsg::WindowCreated > &)
	{
		Message< OSMsg::GetWinWindowHandle >	req_hwnd;
		SendTo( _window, req_hwnd );

		RAWINPUTDEVICE	Rid[1] = {};
		
		// mouse
		Rid[0].usUsagePage	= 0x01;
		Rid[0].usUsage		= 0x02;
		Rid[0].dwFlags		= 0;
		Rid[0].hwndTarget	= req_hwnd->result->Get<HWND>();

		CHECK( RegisterRawInputDevices( &Rid[0], (UINT) CountOf(Rid), sizeof(Rid[0]) ) != FALSE );

		CHECK( _DefCompose( false ) );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool WinMouseInput::_WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &)
	{
		_Destroy();

		_SendMsg< ModuleMsg::Delete >({});
		return true;
	}
	
/*
=================================================
	_OnWinWindowRawMessage
=================================================
*/
	bool WinMouseInput::_OnWinWindowRawMessage (const Message< OSMsg::OnWinWindowRawMessage > &msg)
	{
		// WM_INPUT //
		if ( msg->uMsg == WM_INPUT )
		{
			ubyte	input_data[60];
			uint	data_size = sizeof(input_data);

			if ( ::GetRawInputData( (HRAWINPUT) msg->lParam, RID_INPUT, input_data, &data_size, sizeof(RAWINPUTHEADER) ) != UMax )
			{
				RAWINPUT  *	p_data = (RAWINPUT *)input_data;
				
				// MOUSE //
				if ( p_data->header.dwType == RIM_TYPEMOUSE )
				{
					ASSERT( p_data->data.mouse.usFlags == MOUSE_MOVE_RELATIVE );

					// relative position
					const float2	diff = float2(int2( p_data->data.mouse.lLastX, p_data->data.mouse.lLastY ));

					if ( not _mouseDifference )
						_mouseDifference = diff;
					else
						(*_mouseDifference) += diff;

					// Mouse Wheel
					if ( p_data->data.mouse.usButtonFlags & RI_MOUSE_WHEEL )
					{
						if ( not _wheelDelta )
							_wheelDelta = 0;

						*_wheelDelta += float(p_data->data.mouse.usButtonData) / WHEEL_DELTA;
					}

					// Mouse Button
					if ( p_data->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN )
						_lbPressed = true;
					
					if ( p_data->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP )
						_lbPressed = false;
				}
			}
		}
		else

		// WM_MOUSEMOVE //
		if ( msg->uMsg == WM_MOUSEMOVE )
		{
			_mousePos = float2(int2( LOWORD( msg->lParam ), _surfaceSize.y - HIWORD( msg->lParam ) ));
		}
		return true;
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool WinMouseInput::_Link (const Message< ModuleMsg::Link > &msg)
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

			_window->Subscribe( this, &WinMouseInput::_WindowDescriptorChanged );
			_window->Subscribe( this, &WinMouseInput::_WindowCreated );
			_window->Subscribe( this, &WinMouseInput::_WindowBeforeDestroy );
			_window->Subscribe( this, &WinMouseInput::_OnWinWindowRawMessage );
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool WinMouseInput::_Update (const Message< ModuleMsg::Update > &)
	{
		// send events to InputThread

		if ( _mouseDifference )
		{
			const float2	diff = _mouseDifference.Get();

			if ( IsNotZero( diff.x ) )
				_SendEvent< ModuleMsg::InputMotion >({ "mouse.x"_MotionID, diff.x, _mousePos.x });
			
			if ( IsNotZero( diff.y ) )
				_SendEvent< ModuleMsg::InputMotion >({ "mouse.y"_MotionID, diff.y, _mousePos.y });
			
			if ( _lbPressed and IsNotZero( diff.x ) )
				_SendEvent< ModuleMsg::InputMotion >({ "touch.x"_MotionID, diff.x, _mousePos.x });
			
			if ( _lbPressed and IsNotZero( diff.y ) )
				_SendEvent< ModuleMsg::InputMotion >({ "touch.y"_MotionID, diff.y, _mousePos.y });
		}

		if ( _wheelDelta )
		{
			const float	delta = _wheelDelta.Get();

			if ( IsNotZero( delta ) )
				_SendEvent< ModuleMsg::InputMotion >({ "mouse.wheel"_MotionID, delta, 0.0f });
		}
		
		_mouseDifference.Undefine();
		_wheelDelta.Undefine();
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool WinMouseInput::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
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
	void WinMouseInput::_Destroy ()
	{
		if ( _window )
			_window->UnsubscribeAll( this );

		_window = null;
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	CreateWinMouseInput
=================================================
*/
	ModulePtr WinObjectsConstructor::CreateWinMouseInput (GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci)
	{
		return New< WinMouseInput >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
