// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "WinPlatform.h"
#include "WinWindow.h"

#if defined( PLATFORM_WINDOWS )

#include <Windows.h>

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	WinPlatform::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	WinPlatform::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinPlatform::WinPlatform (const SubSystemsRef gs, const CreateInfo::Platform &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes ),
		_instance( UninitializedT< HMODULE >() ),
		_createInfo( ci )
	{
		SetDebugName( "WinPlatform" );

		_SubscribeOnMsg( this, &WinPlatform::_OnModuleAttached );
		_SubscribeOnMsg( this, &WinPlatform::_OnModuleDetached );
		_SubscribeOnMsg( this, &WinPlatform::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_Update_Empty );
		_SubscribeOnMsg( this, &WinPlatform::_Link_Empty );
		_SubscribeOnMsg( this, &WinPlatform::_Compose );
		_SubscribeOnMsg( this, &WinPlatform::_Delete );
		//_SubscribeOnMsg( this, &WinPlatform::_OnRegistered );
		//_SubscribeOnMsg( this, &WinPlatform::_OnUnregistered );
		_SubscribeOnMsg( this, &WinPlatform::_RequestDisplayParams );
		_SubscribeOnMsg( this, &WinPlatform::_AddToManager );
		_SubscribeOnMsg( this, &WinPlatform::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	WinPlatform::~WinPlatform ()
	{
		LOG( "WinPlatform finalized", ELog::Debug );

		_windows.Clear();

		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void WinPlatform::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		// TODO: delete windows
		_Destroy();

		Module::_Delete_Impl( msg );
	}

/*
=================================================
	_Compose
=================================================
*/
	void WinPlatform::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked, void() );

		// can't recteate platform
		CHECK_ERR( not _IsCreated(), void() );

		CHECK( _Create() );

		CHECK( _SetState( EState::ComposedImmutable ) );

		LOG( "platform created", ELog::Debug );

		_SendEvent( Message< ModuleMsg::PlatformCreated >{ this, _instance, _className } );
	}
	
/*
=================================================
	_Update
=================================================
*
	void WinPlatform::_Update (const Message< ModuleMsg::Update > &msg)
	{
		// do nothing
	}

/*
=================================================
	_RequestDisplayParams
=================================================
*/
	void WinPlatform::_RequestDisplayParams (const Message< ModuleMsg::RequestDisplayParams > &msg)
	{
		msg->result.Set( &_display );
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	void WinPlatform::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		//SCOPELOCK( _lock );

		CHECK_ERR( msg->module, void() );
		CHECK_ERR( msg->module->GetModuleID() == WinWindow::GetStaticID(), void() );
		ASSERT( not _windows.IsExist( msg->module ) );

		_windows.Add( msg->module );

		if ( _IsCreated() )
		{
			msg->module->Send( Message< ModuleMsg::PlatformCreated >{ this, _instance, _className } );
		}
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	void WinPlatform::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		//SCOPELOCK( _lock );

		CHECK_ERR( msg->module, void() );
		CHECK_ERR( msg->module->GetModuleID() == WinWindow::GetStaticID(), void() );
		ASSERT( _windows.IsExist( msg->module ) );

		_windows.Erase( msg->module );
	}

/*
=================================================
	_OnRegistered
=================================================
*
	void WinPlatform::_OnRegistered (const Message< ModuleMsg::OnRegistered > &msg)
	{
		CHECK( msg->factory->Register( WinWindow::GetStaticID(), null, &_CreateWinWindow ) );
		CHECK( msg->factory->Register( WinWindow::GetStaticID(), null, &_CreateWinKeyInput ) );
		CHECK( msg->factory->Register( WinWindow::GetStaticID(), null, &_CreateWinMouseInput ) );
	}
	
/*
=================================================
	_OnUnregistered
=================================================
*
	void WinPlatform::_OnUnregistered (const Message< ModuleMsg::OnUnregistered > &msg)
	{
		msg->factory->Unregister< WinWindow >();
		msg->factory->Unregister< WinKeyInput >();
		msg->factory->Unregister< WinMouseInput >();
	}
	
/*
=================================================
	_RegisterAll
=================================================
*/
	void WinPlatform::_RegisterAll (const SubSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( WinPlatform::GetStaticID(), null, &_CreateWinPlatform ) );
		CHECK( mf->Register( WinWindow::GetStaticID(), null, &_CreateWinWindow ) );
		CHECK( mf->Register( WinKeyInput::GetStaticID(), null, &_CreateWinKeyInput ) );
		CHECK( mf->Register( WinMouseInput::GetStaticID(), null, &_CreateWinMouseInput ) );
	}
	
/*
=================================================
	_CreateWinPlatform
=================================================
*/
	ModulePtr WinPlatform::_CreateWinPlatform (const SubSystemsRef gs, const CreateInfo::Platform &ci)
	{
		return GXTypes::New< WinPlatform >( gs, ci );
	}

/*
=================================================
	_CreateWinWindow
=================================================
*/
	ModulePtr WinPlatform::_CreateWinWindow (const SubSystemsRef gs, const CreateInfo::Window &ci)
	{
		return GXTypes::New< WinWindow >( gs, ci );
	}
	
/*
=================================================
	_CreateWinKeyInput
=================================================
*/
	ModulePtr WinPlatform::_CreateWinKeyInput (const SubSystemsRef gs, const CreateInfo::RawInputHandler &ci)
	{
		return GXTypes::New< WinKeyInput >( gs, ci );
	}
	
/*
=================================================
	_CreateWinMouseInput
=================================================
*/
	ModulePtr WinPlatform::_CreateWinMouseInput (const SubSystemsRef gs, const CreateInfo::RawInputHandler &ci)
	{
		return GXTypes::New< WinMouseInput >( gs, ci );
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool WinPlatform::_IsCreated () const
	{
		return _instance.IsNotNull< HMODULE >();
	}

/*
=================================================
	_Create
=================================================
*/
	bool WinPlatform::_Create ()
	{
		_Destroy();

		_instance = ::GetModuleHandle( (LPCSTR) null );

		CHECK_ERR( _GetDirectories() );
		CHECK_ERR( _RegisterClass() );

		if ( _createInfo.flags[ CreateInfo::Platform::EPlatformFlags::ChangeDisplayMode ] )
		{
			CHECK_ERR( _SetScreenResolution( _createInfo.resolution, _createInfo.frequency ) );
		}

		_UpdateDisplayParams();
		return true;
	}
	
/*
=================================================
	_Destroy
=================================================
*/
	bool WinPlatform::_GetDirectories ()
	{
		char	buf[ MAX_PATH<<2 ] = {0};
		CHECK_ERR( ::GetModuleFileName( _instance.Get<HMODULE>(), buf, (DWORD) CountOf(buf) ) != 0 );

		_directories[ EDirectory::Internal ]		= "internal";
		_directories[ EDirectory::InternalCache ]	= "internal_cache";
		_directories[ EDirectory::External ]		= "external";
		_directories[ EDirectory::ExternalCache ]	= "external_cache";
		_directories[ EDirectory::SDCard ]			= "sdcard";
		_directories[ EDirectory::Library ]			= FileAddress::GetPath( StringCRef( buf ) );
		
		return true;
	}
	
/*
=================================================
	_RegisterClass
=================================================
*/
	bool WinPlatform::_RegisterClass ()
	{
		_className =  ENGINE_NAME;
		_className += "Class";

		HMODULE		instance	= _instance.Get<HMODULE>();
		WNDCLASSA	tmp			= {0};
		bool		ret			= GetClassInfo( instance, _className.cstr(), &tmp ) == TRUE;
		
		if ( ret or ( _className == tmp.lpszClassName and instance == tmp.hInstance ) )
			return true;

		WNDCLASSEXA		window_class = {0};
		window_class.cbSize			= sizeof(window_class);
		window_class.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		window_class.lpfnWndProc	= &DefWindowProcA;
		window_class.cbClsExtra		= 0;
		window_class.cbWndExtra		= 0;
		window_class.hInstance		= instance;
		window_class.hbrBackground	= (HBRUSH) COLOR_WINDOW+1; //(HBRUSH) COLOR_WINDOWFRAME;
		window_class.hIcon			= (HICON) null;
		window_class.hCursor		= LoadCursor( null, IDC_ARROW );
		window_class.lpszMenuName	= null;
		window_class.lpszClassName	= _className.cstr();
		window_class.hIconSm		= (HICON) null;

		CHECK_ERR( ::RegisterClassExA( &window_class ) != 0 );

		return true;
	}
	
/*
=================================================
	_UpdateDisplayParams
=================================================
*/
	void WinPlatform::_UpdateDisplayParams ()
	{
		_display.SetResolution( _GetScreenResolution() );
		_display.SetPhysicsSize( _ScreenPhysicalSize() );
		_display.SetPixelsPerInch( _display.CalcPixelsPerInch().Max() );
		_display.SetOrientation( _display.IsHorizontal() ? EDisplayOrientation::Landscape : EDisplayOrientation::Portrait );
		_display.SetFrequency( _GetDisplayFrequency() );
	}
	
/*
=================================================
	_GetScreenResolution
=================================================
*/
	uint2 WinPlatform::_GetScreenResolution () const
	{
		return uint2( ::GetSystemMetrics( SM_CXSCREEN ),
					  ::GetSystemMetrics( SM_CYSCREEN ) );
	}
	
/*
=================================================
	_ScreenPhysicalSize
=================================================
*/
	float2 WinPlatform::_ScreenPhysicalSize () const
	{
		float2	size;
		HDC		hDCScreen	= ::GetDC( null );
		
		size.x = (float) ::GetDeviceCaps( hDCScreen, HORZSIZE );
		size.y = (float) ::GetDeviceCaps( hDCScreen, VERTSIZE );
		
		::ReleaseDC( null, hDCScreen );
		return size * 0.001f;
	}
	
/*
=================================================
	_GetDisplayFrequency
=================================================
*/
	uint WinPlatform::_GetDisplayFrequency () const
	{
		HDC		hDCScreen	= ::GetDC( null );
		
		uint freq = ::GetDeviceCaps( hDCScreen, VREFRESH );
		
		::ReleaseDC( null, hDCScreen );
		return freq;
	}

/*
=================================================
	_Destroy
=================================================
*/
	void WinPlatform::_Destroy ()
	{
		if ( _createInfo.flags[ CreateInfo::Platform::EPlatformFlags::ChangeDisplayMode ] )
		{
			_ReturnToDefaultResolution();
		}

		if ( not _className.Empty() )
		{
			::UnregisterClass( _className.cstr(), _instance.Get<HMODULE>() );
			_className.Clear();
		}

		_instance = HMODULE(null);
	}

/*
=================================================
	_SetScreenResolution
=================================================
*/
	bool WinPlatform::_SetScreenResolution (const uint2 &size, uint freq)
	{
		DEVMODEA	mode = {0};
		mode.dmSize				= sizeof( mode );
		mode.dmPelsWidth		= (DWORD) size.x;
		mode.dmPelsHeight		= (DWORD) size.y;
		mode.dmBitsPerPel		= (DWORD) 8;
		mode.dmDisplayFrequency	= (DWORD) freq;
		mode.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH |
								  DM_PELSHEIGHT | (freq ? DM_DISPLAYFREQUENCY : 0);

		if ( ::ChangeDisplaySettings( &mode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			mode.dmFields &= ~DM_DISPLAYFREQUENCY;

			CHECK_ERR( ::ChangeDisplaySettings( &mode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL );
		}

		return true;
	}
	
/*
=================================================
	_ReturnToDefaultResolution
=================================================
*/
	bool WinPlatform::_ReturnToDefaultResolution ()
	{
		CHECK_ERR( ::ChangeDisplaySettings( (DEVMODEA *)null, CDS_RESET ) != DISP_CHANGE_SUCCESSFUL );
		return true;
	}


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
