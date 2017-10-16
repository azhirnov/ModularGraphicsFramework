// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/OS/Input.h"
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/Windows/WinPlatform.h"

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
											ModuleMsg::WindowDescriptorChanged,
											ModuleMsg::WindowCreated,
											ModuleMsg::WindowBeforeDestroy,
											ModuleMsg::WindowRawMessage
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputMotion
										> >;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		uint2				_surfaceSize;
		Optional<float2>	_mouseDifference;
		float2				_mousePos;
		

	// methods
	public:
		WinMouseInput (const GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci);
		~WinMouseInput ();


	// message handlers
	private:
		bool _WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &);
		bool _WindowCreated (const Message< ModuleMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &);
		bool _WindowRawMessage (const Message< ModuleMsg::WindowRawMessage > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
	};
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	WinMouseInput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	WinMouseInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinMouseInput::WinMouseInput (const GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci) :
		Module( gs, ModuleConfig{ WinMouseInputModuleID, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "WinMouseInput" );
		
		_SubscribeOnMsg( this, &WinMouseInput::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &WinMouseInput::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &WinMouseInput::_Update );
		_SubscribeOnMsg( this, &WinMouseInput::_Link );
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
		LOG( "WinMouseInput finalized", ELog::Debug );
	}

/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool WinMouseInput::_WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &msg)
	{
		_surfaceSize = msg->desc.surfaceSize;
		return true;
	}
	
/*
=================================================
	_WindowCreated
=================================================
*/
	bool WinMouseInput::_WindowCreated (const Message< ModuleMsg::WindowCreated > &msg)
	{
		CHECK_ERR( msg->hwnd.IsNotNull<HWND>() );

		RAWINPUTDEVICE	Rid[1] = {};
		
		// mouse
		Rid[0].usUsagePage	= 0x01;
		Rid[0].usUsage		= 0x02;
		Rid[0].dwFlags		= 0;
		Rid[0].hwndTarget	= msg->hwnd.Get<HWND>();

		CHECK( RegisterRawInputDevices( &Rid[0], (UINT) CountOf(Rid), sizeof(Rid[0]) ) == TRUE );

		CHECK( _DefCompose( true ) );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool WinMouseInput::_WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &msg)
	{
		_SendMsg< ModuleMsg::Delete >({});
		return true;
	}
	
/*
=================================================
	_WindowRawMessage
=================================================
*/
	bool WinMouseInput::_WindowRawMessage (const Message< ModuleMsg::WindowRawMessage > &msg)
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
		return true;
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool WinMouseInput::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( Module::_Link_Impl( msg ) );
		
		// attach to manager
		{
			ModulePtr	input = GlobalSystems()->Get< ParallelThread >()->GetModuleByID( InputThreadModuleID );
			CHECK_ERR( input );
			
			_AttachSelfToManager( input, UntypedID_t(), true );
		}

		// subscribe on window events
		{
			// TODO: use SearchModule message
			// TODO: reset to initial state if window was detached
			ModulePtr	wnd = GlobalSystems()->Get< ParallelThread >()->GetModuleByID( WinWindowModuleID );
			CHECK_ATTACHMENT( wnd );

			if ( _IsComposedState( wnd->GetState() ) )
			{
				Message< ModuleMsg::WindowGetHandle >	request_hwnd;

				SendTo( wnd, request_hwnd );

				if ( request_hwnd->hwnd.IsDefined() and
					 request_hwnd->hwnd.Get().IsNotNull<HWND>() )
				{
					_SendMsg< ModuleMsg::WindowCreated >({ WindowDesc(), request_hwnd->hwnd.Get() });
				}
			}

			wnd->Subscribe( this, &WinMouseInput::_WindowDescriptorChanged );
			wnd->Subscribe( this, &WinMouseInput::_WindowCreated );
			wnd->Subscribe( this, &WinMouseInput::_WindowBeforeDestroy );
			wnd->Subscribe( this, &WinMouseInput::_WindowRawMessage );
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool WinMouseInput::_Update (const Message< ModuleMsg::Update > &msg)
	{
		// send events to InputThread

		if ( _mouseDifference )
		{
			float2	diff = _mouseDifference.Get( float2() );

			if ( IsNotZero( diff.x ) )
				_SendEvent< ModuleMsg::InputMotion >({ "mouse.x"_MotionID, diff.x, _mousePos.x });
			
			if ( IsNotZero( diff.y ) )
				_SendEvent< ModuleMsg::InputMotion >({ "mouse.y"_MotionID, diff.y, _mousePos.y });
		}
		
		_mouseDifference.Undefine();
		return true;
	}
//-----------------------------------------------------------------------------
	


	ModulePtr WinPlatform::_CreateWinMouseInput (const GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci)
	{
		return New< WinMouseInput >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
