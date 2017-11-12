// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Windows/WinPlatform.h"

#if defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"

namespace Engine
{
namespace Platforms
{
	
	const TypeIdList	WinPlatform::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	WinPlatform::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinPlatform::WinPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci) :
		Module( gs, ModuleConfig{ WinPlatformModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_createInfo( ci ),
		_instance( UninitializedT< HMODULE >() )
	{
		SetDebugName( "WinPlatform" );

		_SubscribeOnMsg( this, &WinPlatform::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_AttachModule_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_DetachModule_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_FindModule_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &WinPlatform::_Update_Empty );
		_SubscribeOnMsg( this, &WinPlatform::_Link_Empty );
		_SubscribeOnMsg( this, &WinPlatform::_Compose );
		_SubscribeOnMsg( this, &WinPlatform::_Delete );
		_SubscribeOnMsg( this, &WinPlatform::_AddToManager );
		_SubscribeOnMsg( this, &WinPlatform::_RemoveFromManager );
		_SubscribeOnMsg( this, &WinPlatform::_GetDisplays );
		
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
	bool WinPlatform::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		// TODO: delete windows
		_Destroy();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_Compose
=================================================
*/
	bool WinPlatform::_Compose (const Message< ModuleMsg::Compose > &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _Create() );

		CHECK( _ValidateAllSubscriptions() );
		CHECK( _SetState( EState::ComposedImmutable ) );

		LOG( "platform created", ELog::Debug );

		_SendEvent< OSMsg::OnWinPlatformCreated >({ _instance, _className });

		// async message
		FOR( i, _windows )
		{
			CHECK( GlobalSystems()->Get< TaskModule >()->Send( Message< ModuleMsg::PushAsyncMessage >{
						AsyncMessage{ LAMBDA(
							target = _windows[i],
							msg = Message< OSMsg::OnWinPlatformCreated >{ _instance, _className } ) (GlobalSystemsRef)
						{
							target->Send( msg );
						}},
						_windows[i]->GetThreadID()
					}.Async())
			);
		}
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool WinPlatform::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == WinWindowModuleID );
		ASSERT( not _windows.IsExist( msg->module ) );

		_windows.Add( msg->module );

		if ( _IsCreated() )
		{
			CHECK( GlobalSystems()->Get< TaskModule >()->Send( Message< ModuleMsg::PushAsyncMessage >{
						AsyncMessage{ LAMBDA(
							target = msg->module,
							msg = Message< OSMsg::OnWinPlatformCreated >{ _instance, _className } ) (GlobalSystemsRef)
						{
							target->Send( msg );
						}},
						msg->module->GetThreadID()
					}.Async())
			);
		}
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool WinPlatform::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == WinWindowModuleID );
		ASSERT( _windows.IsExist( msg->module ) );

		_windows.Erase( msg->module );
		return true;
	}
	
/*
=================================================
	Register
=================================================
*/
	void WinPlatform::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( WinPlatformModuleID, &_CreateWinPlatform ) );
		CHECK( mf->Register( WinWindowModuleID, &_CreateWinWindow ) );
		CHECK( mf->Register( WinKeyInputModuleID, &_CreateWinKeyInput ) );
		CHECK( mf->Register( WinMouseInputModuleID, &_CreateWinMouseInput ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void WinPlatform::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll( WinPlatformModuleID );
		mf->UnregisterAll( WinWindowModuleID );
		mf->UnregisterAll( WinKeyInputModuleID );
		mf->UnregisterAll( WinMouseInputModuleID );
	}

/*
=================================================
	_CreateWinPlatform
=================================================
*/
	ModulePtr WinPlatform::_CreateWinPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci)
	{
		return New< WinPlatform >( gs, ci );
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
		CHECK_ERR( not _IsCreated() );

		_instance = ::GetModuleHandle( (LPCSTR) null );

		CHECK_ERR( _GetDirectories() );
		CHECK_ERR( _RegisterClass() );

		if ( _createInfo.flags[ CreateInfo::Platform::EPlatformFlags::ChangeDisplayMode ] )
		{
			CHECK_ERR( _SetScreenResolution( _createInfo.resolution, _createInfo.frequency ) );
		}
		return true;
	}
	
/*
=================================================
	_GetDisplays
=================================================
*/
	bool WinPlatform::_GetDisplays (const Message< OSMsg::GetDisplays > &msg)
	{
		_display.Update();

		msg->result.Set( _display.GetDisplays() );
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
