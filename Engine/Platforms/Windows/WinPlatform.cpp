// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/Platform.h"
#include "Engine/Platforms/Shared/OS/OSEnums.h"
#include "Engine/Platforms/Shared/OS/Input.h"
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/Windows/WinDisplay.h"
#include "Engine/Platforms/Windows/WinObjectsConstructor.h"

#if defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"

namespace Engine
{
namespace Platforms
{


	//
	// Windows Platform
	//

	class WinPlatform final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											OSMsg::GetDisplays,
											OSMsg::GetOSModules
										> >;
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Delete,
											OSMsg::OnWinPlatformCreated
										>;

		using ModArray_t			= Set< ModulePtr >;
		using Directories_t			= StaticArray< String, EDirectory::_Count >;

		using HMODULE_t				= OS::HiddenOSTypeFrom<void*>;
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModArray_t				_windows;
		WinDisplay				_display;
		
		CreateInfo::Platform	_createInfo;

		String					_className;
		HMODULE_t				_instance;

		Directories_t			_directories;


	// methods
	public:
		WinPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci);
		~WinPlatform ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		//bool _Update (const Message< ModuleMsg::Update > &);
		bool _GetDisplays (const Message< OSMsg::GetDisplays > &);
		bool _GetOSModules (const Message< OSMsg::GetOSModules > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

	private:
		bool _IsCreated () const;
		bool _Create ();
		void _Destroy ();

		bool _GetDirectories ();
		bool _RegisterClass ();

		bool _SetScreenResolution (const uint2 &size, uint freq);
		bool _ReturnToDefaultResolution ();
	};
//-----------------------------------------------------------------------------



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
		_SubscribeOnMsg( this, &WinPlatform::_GetOSModules );
		
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
			CHECK( GlobalSystems()->taskModule->Send( Message< ModuleMsg::PushAsyncMessage >{
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
			CHECK( GlobalSystems()->taskModule->Send( Message< ModuleMsg::PushAsyncMessage >{
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

		ModulePtr	module = msg->module.Lock();

		if ( not module )
			return false;

		CHECK_ERR( module->GetModuleID() == WinWindowModuleID );
		ASSERT( _windows.IsExist( module ) );

		_windows.Erase( module );
		return true;
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
	_GetOSModules
=================================================
*/
	bool WinPlatform::_GetOSModules (const Message< OSMsg::GetOSModules > &msg)
	{
		msg->result.Set( WinObjectsConstructor::GetModuleIDs() );
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
		WNDCLASSA	tmp			= {};
		bool		ret			= GetClassInfo( instance, _className.cstr(), &tmp ) == TRUE;
		
		if ( ret or ( _className == tmp.lpszClassName and instance == tmp.hInstance ) )
			return true;

		WNDCLASSEXA		window_class = {};
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
		DEVMODEA	mode = {};
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
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	Register
=================================================
*/
	void WinObjectsConstructor::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( WinPlatformModuleID, &CreateWinPlatform ) );
		CHECK( mf->Register( WinWindowModuleID, &CreateWinWindow ) );
		CHECK( mf->Register( WinKeyInputModuleID, &CreateWinKeyInput ) );
		CHECK( mf->Register( WinMouseInputModuleID, &CreateWinMouseInput ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void WinObjectsConstructor::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( WinPlatformModuleID );
		mf->UnregisterAll( WinWindowModuleID );
		mf->UnregisterAll( WinKeyInputModuleID );
		mf->UnregisterAll( WinMouseInputModuleID );
	}
	
/*
=================================================
	GetModuleIDs
=================================================
*/
	OSModuleIDs WinObjectsConstructor::GetModuleIDs ()
	{
		OSModuleIDs	ids;
		ids.platform	= WinPlatformModuleID;
		ids.window		= WinWindowModuleID;
		ids.keyInput	= WinKeyInputModuleID;
		ids.mouseInput	= WinMouseInputModuleID;
		return ids;
	}

/*
=================================================
	CreateWinPlatform
=================================================
*/
	ModulePtr WinObjectsConstructor::CreateWinPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci)
	{
		return New< WinPlatform >( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
