// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Platforms/Windows/WinMouseInput.h"

#if defined( PLATFORM_WINDOWS )

#include <Windows.h>

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	WinMouseInput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	WinMouseInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinMouseInput::WinMouseInput (const SubSystemsRef gs, const CreateInfo::RawInputHandler &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		SetDebugName( "WinMouseInput" );
		
		_SubscribeOnMsg( this, &WinMouseInput::_OnModuleAttached );
		_SubscribeOnMsg( this, &WinMouseInput::_OnModuleDetached );
		_SubscribeOnMsg( this, &WinMouseInput::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_Update );
		_SubscribeOnMsg( this, &WinMouseInput::_Link );
		_SubscribeOnMsg( this, &WinMouseInput::_Compose_Empty );
		_SubscribeOnMsg( this, &WinMouseInput::_Delete_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_WindowDescriptorChanged );
		_SubscribeOnMsg( this, &WinMouseInput::_WindowCreated );
		_SubscribeOnMsg( this, &WinMouseInput::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &WinMouseInput::_WindowRawMessage );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	WinMouseInput::~WinMouseInput ()
	{
	}

/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	void WinMouseInput::_WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &msg)
	{
		_surfaceSize = msg->desc.surfaceSize;
	}
	
/*
=================================================
	_WindowCreated
=================================================
*/
	void WinMouseInput::_WindowCreated (const Message< ModuleMsg::WindowCreated > &msg)
	{
		CHECK_ERR( msg->hwnd.IsNotNull<HWND>(), void() );

		RAWINPUTDEVICE	Rid[1] = {};
		
		// mouse
		Rid[0].usUsagePage	= 0x01;
		Rid[0].usUsage		= 0x02;
		Rid[0].dwFlags		= 0;
		Rid[0].hwndTarget	= msg->hwnd.Get<HWND>();

		CHECK( RegisterRawInputDevices( &Rid[0], CountOf(Rid), sizeof(Rid[0]) ) == TRUE );

		CHECK( _SetState( EState::ComposedImmutable ) );
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	void WinMouseInput::_WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &msg)
	{
		Send( Message< ModuleMsg::Delete >( this ) );
	}
	
/*
=================================================
	_WindowRawMessage
=================================================
*/
	void WinMouseInput::_WindowRawMessage (const Message< ModuleMsg::WindowRawMessage > &msg)
	{
		// WM_INPUT //
		if ( msg->uMsg == WM_INPUT )
		{
			ubyte	input_data[60];
			uint	data_size = sizeof(input_data);

			if ( ::GetRawInputData( (HRAWINPUT) msg->lParam, RID_INPUT, input_data, &data_size, sizeof(RAWINPUTHEADER) ) != -1 )
			{
				RAWINPUT  *	p_data = (RAWINPUT *)input_data;
				
				// MOUSE //
				if ( p_data->header.dwType == RIM_TYPEMOUSE )
				{
					const float2	diff = float2( int2( p_data->data.mouse.lLastX, p_data->data.mouse.lLastY ) );

					if ( not _mouseDifference )
						_mouseDifference = diff;
					else
						(*_mouseDifference) += diff;
				}
			}
		}
		else

		// WM_MOUSEMOVE //
		if ( msg->uMsg == WM_MOUSEMOVE )
		{
			_mousePos = float2(int2( LOWORD( msg->lParam ), _surfaceSize.y - HIWORD( msg->lParam ) ));
		}
	}
	
/*
=================================================
	_Link
=================================================
*/
	void WinMouseInput::_Link (const Message< ModuleMsg::Link > &msg)
	{
		Module::_Link_Impl( msg );

		ModulePtr	wnd = _GetParent()->GetModule( WinWindow::GetStaticID() );
		CHECK_ERR( wnd, void() );

		if ( _IsComposedState( wnd->GetState() ) )
		{
			Message< ModuleMsg::WindowGetHandle >	request_hwnd;

			wnd->Send( request_hwnd );

			if ( request_hwnd->hwnd.Get().IsDefined() and
				 request_hwnd->hwnd.Get().Get().IsNotNull<HWND>() )
			{
				Send( Message< ModuleMsg::WindowCreated >{ this, WindowDesc(), request_hwnd->hwnd.Get().Get() } );
			}
		}

		wnd->Subscribe( this, &WinMouseInput::_WindowDescriptorChanged );
		wnd->Subscribe( this, &WinMouseInput::_WindowCreated );
		wnd->Subscribe( this, &WinMouseInput::_WindowBeforeDestroy );
		wnd->Subscribe( this, &WinMouseInput::_WindowRawMessage );
	}
	
/*
=================================================
	_Update
=================================================
*/
	void WinMouseInput::_Update (const Message< ModuleMsg::Update > &msg)
	{
		// send events to InputThread

		if ( _mouseDifference )
		{
			float2	diff = _mouseDifference.Get( float2() );

			if ( IsNotZero( diff.x ) )
				_SendEvent( Message< ModuleMsg::InputMotion >( this, "mouse.x"_MotionID, diff.x, _mousePos.x ) );
			
			if ( IsNotZero( diff.y ) )
				_SendEvent( Message< ModuleMsg::InputMotion >( this, "mouse.y"_MotionID, diff.y, _mousePos.y ) );
		}
		
		_mouseDifference.Undefine();
	}
	

}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
