// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#ifdef PLATFORM_WINDOWS

#include "Engine/Platforms/Public/OS/Display.h"
#include "Engine/Platforms/Public/OS/Platform.h"
#include "Engine/Platforms/Public/OS/OSEnums.h"
#include "Engine/Platforms/Public/OS/Input.h"
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/Windows/WinDisplay.h"
#include "Engine/Platforms/Windows/WinObjectsConstructor.h"
#include "Core/STL/OS/Windows/WinHeader.h"

namespace Engine
{
namespace PlatformWin
{


	//
	// Windows Platform
	//

	class WinPlatform final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											OSMsg::GetDisplays,
											OSMsg::GetOSModules,
											OSMsg::GetProccessorInfo,
											OSMsg::GetMemoryInfo
										>;
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Delete,
											OSMsg::OnWinPlatformCreated
										>;

		using WinWindowMsgList_t	= MessageListFrom<
											OSMsg::OnWinPlatformCreated,
											OSMsg::GetWinWindowHandle
										>;

		using ModArray_t			= Set< ModulePtr >;
		using Directories_t			= StaticArray< String, EDirectory::_Count >;

		using HMODULE_t				= DeferredTypeFrom<void*>;
		

	// constants
	private:
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
		WinPlatform (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::Platform &ci);
		~WinPlatform ();


	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);

		bool _GetDisplays (const OSMsg::GetDisplays &);
		bool _GetOSModules (const OSMsg::GetOSModules &);
		bool _GetProccessorInfo (const OSMsg::GetProccessorInfo &);
		bool _GetMemoryInfo (const OSMsg::GetMemoryInfo &);

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



	const TypeIdList	WinPlatform::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	WinPlatform::WinPlatform (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Platform &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
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
		_SubscribeOnMsg( this, &WinPlatform::_GetProccessorInfo );
		_SubscribeOnMsg( this, &WinPlatform::_GetMemoryInfo );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	WinPlatform::~WinPlatform ()
	{
		//LOG( "WinPlatform finalized", ELog::Debug );

		_windows.Clear();

		ASSERT( not _IsCreated() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool WinPlatform::_Delete (const ModuleMsg::Delete &msg)
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
	bool WinPlatform::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _Create() );

		CHECK( _ValidateAllSubscriptions() );
		CHECK( _SetState( EState::ComposedImmutable ) );

		LOG( "platform created", ELog::Debug );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		_SendEvent( OSMsg::OnWinPlatformCreated{ _instance, _className });

		// async message
		for (auto& wnd : _windows)
		{
			CHECK( GlobalSystems()->taskModule->SendAsync( ModuleMsg::PushAsyncMessage{
						wnd->GetThreadID(),
						LAMBDA( target = wnd, msg = OSMsg::OnWinPlatformCreated{ _instance, _className } ) (GlobalSystemsRef) {
							target->Send( msg );
						}}
					));
		}
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool WinPlatform::_AddToManager (const ModuleMsg::AddToManager &msg)
	{
		CHECK_ERR( msg.module );
		CHECK_ERR( msg.module->SupportsAllMessages< WinWindowMsgList_t >() );
		ASSERT( not _windows.IsExist( msg.module ) );

		_windows.Add( msg.module );

		if ( _IsCreated() )
		{
			CHECK( GlobalSystems()->taskModule->SendAsync( ModuleMsg::PushAsyncMessage{
						msg.module->GetThreadID(),
						LAMBDA( target = msg.module, msg = OSMsg::OnWinPlatformCreated{ _instance, _className } ) (GlobalSystemsRef) {
							target->Send( msg );
						}}
					));
		}
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool WinPlatform::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		CHECK_ERR( msg.module );

		ModulePtr	module = msg.module.Lock();

		if ( not module )
			return false;

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

		_instance = ::GetModuleHandle( LPCSTR(null) );

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
	bool WinPlatform::_GetDisplays (const OSMsg::GetDisplays &msg)
	{
		_display.Update();

		msg.result.Set( _display.GetDisplays() );
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
		CHECK_ERR( ::GetModuleFileName( _instance.Get<HMODULE>(), buf, DWORD(CountOf( buf )) ) != 0 );

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
	bool WinPlatform::_GetOSModules (const OSMsg::GetOSModules &msg)
	{
		msg.result.Set( WinObjectsConstructor::GetModuleIDs() );
		return true;
	}
	
/*
=================================================
	_GetProccessorInfo
=================================================
*/
	bool WinPlatform::_GetProccessorInfo (const OSMsg::GetProccessorInfo &msg)
	{
		using EProcessorArch = OSMsg::GetProccessorInfo::EProcessorArch;

		SYSTEM_INFO		sys_info = {};
		::GetSystemInfo( OUT &sys_info );

		OSMsg::GetProccessorInfo::Info	info;
		info.coresCount	= sys_info.dwNumberOfProcessors;

		switch ( sys_info.wProcessorArchitecture )
		{
			case PROCESSOR_ARCHITECTURE_AMD64 :			info.arch = EProcessorArch::X64;	break;
			case PROCESSOR_ARCHITECTURE_INTEL :			info.arch = EProcessorArch::X64;	break;
			case PROCESSOR_ARCHITECTURE_ARM :			info.arch = EProcessorArch::ARM;	break;
			case 12/*PROCESSOR_ARCHITECTURE_ARM64*/ :	info.arch = EProcessorArch::ARM64;	break;
		}

		msg.result.Set( info );
		return true;
	}
	
/*
=================================================
	_GetMemoryInfo
=================================================
*/
	bool WinPlatform::_GetMemoryInfo (const OSMsg::GetMemoryInfo &msg)
	{
		MEMORYSTATUSEX	status = {};
		CHECK_ERR( ::GlobalMemoryStatusEx( OUT &status ) != FALSE );

		OSMsg::GetMemoryInfo::Info	info;
		info.total				= BytesU::FromBytes( status.ullTotalPhys );
		info.available			= BytesU::FromBytes( status.ullAvailPhys );
		info.totalVirtual		= BytesU::FromBytes( status.ullTotalVirtual );
		info.availableVirtual	= BytesU::FromBytes( status.ullAvailVirtual );	// TODO: ullAvailExtendedVirtual ?

		msg.result.Set( info );
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
		bool		ret			= ::GetClassInfo( instance, _className.cstr(), &tmp ) != FALSE;
		
		if ( ret or ( _className == tmp.lpszClassName and instance == tmp.hInstance ) )
			return true;

		WNDCLASSEXA		window_class = {};
		window_class.cbSize			= sizeof(window_class);
		window_class.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		window_class.lpfnWndProc	= &::DefWindowProcA;
		window_class.cbClsExtra		= 0;
		window_class.cbWndExtra		= 0;
		window_class.hInstance		= instance;
		window_class.hbrBackground	= HBRUSH(COLOR_WINDOW+1); //(HBRUSH) COLOR_WINDOWFRAME;
		window_class.hIcon			= HICON(null);
		window_class.hCursor		= ::LoadCursor( null, IDC_ARROW );
		window_class.lpszMenuName	= null;
		window_class.lpszClassName	= _className.cstr();
		window_class.hIconSm		= HICON(null);

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
		mode.dmPelsWidth		= DWORD(size.x);
		mode.dmPelsHeight		= DWORD(size.y);
		mode.dmBitsPerPel		= DWORD(8);
		mode.dmDisplayFrequency	= DWORD(freq);
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
		CHECK_ERR( ::ChangeDisplaySettings( Cast<DEVMODEA *>(null), CDS_RESET ) != DISP_CHANGE_SUCCESSFUL );
		return true;
	}

}	// PlatformWin
//-----------------------------------------------------------------------------
	

namespace Platforms
{
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
	ModulePtr WinObjectsConstructor::CreateWinPlatform (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Platform &ci)
	{
		return New< PlatformWin::WinPlatform >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
