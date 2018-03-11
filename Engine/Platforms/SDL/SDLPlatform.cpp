// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/Platforms/Public/OS/Display.h"
#include "Engine/Platforms/Public/OS/Platform.h"
#include "Engine/Platforms/Public/OS/OSEnums.h"
#include "Engine/Platforms/Public/OS/Input.h"
#include "Engine/Platforms/SDL/SDLMessages.h"
#include "Engine/Platforms/SDL/SDLObjectsConstructor.h"
#include "Engine/Platforms/SDL/SDLDisplay.h"

namespace Engine
{
namespace PlatformSDL
{

	//
	// SDL Platform
	//

	class SDLPlatform final : public Module
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
											OSMsg::OnSDLPlatformCreated
										>;

		using SDLWindowMsgList_t	= MessageListFrom<
											OSMsg::OnSDLPlatformCreated,
											OSMsg::GetSDLWindowHandle
										>;

		using ModArray_t			= Set< ModulePtr >;
		using Directories_t			= StaticArray< String, EDirectory::_Count >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModArray_t				_windows;
		SDLDisplay				_display;
		
		CreateInfo::Platform	_createInfo;

		Directories_t			_directories;

		bool					_isCreated;


	// methods
	public:
		SDLPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci);
		~SDLPlatform ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _GetDisplays (const Message< OSMsg::GetDisplays > &);
		bool _GetOSModules (const Message< OSMsg::GetOSModules > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

	private:
		bool _IsCreated () const;
		bool _Create ();
		void _Destroy ();

		bool _GetDirectories ();

		bool _SetScreenResolution (const uint2 &size, uint freq);
		bool _ReturnToDefaultResolution ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SDLPlatform::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SDLPlatform::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SDLPlatform::SDLPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci) :
		Module( gs, ModuleConfig{ SDLPlatformModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_createInfo( ci ), _isCreated( false )
	{
		SetDebugName( "SDLPlatform" );

		_SubscribeOnMsg( this, &SDLPlatform::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SDLPlatform::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SDLPlatform::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SDLPlatform::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SDLPlatform::_FindModule_Impl );
		_SubscribeOnMsg( this, &SDLPlatform::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SDLPlatform::_Update_Empty );
		_SubscribeOnMsg( this, &SDLPlatform::_Link_Empty );
		_SubscribeOnMsg( this, &SDLPlatform::_Compose );
		_SubscribeOnMsg( this, &SDLPlatform::_Delete );
		_SubscribeOnMsg( this, &SDLPlatform::_AddToManager );
		_SubscribeOnMsg( this, &SDLPlatform::_RemoveFromManager );
		_SubscribeOnMsg( this, &SDLPlatform::_GetDisplays );
		_SubscribeOnMsg( this, &SDLPlatform::_GetOSModules );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SDLPlatform::~SDLPlatform ()
	{
		LOG( "SDLPlatform finalized", ELog::Debug );

		_windows.Clear();

		ASSERT( not _IsCreated() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool SDLPlatform::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_Destroy();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_Compose
=================================================
*/
	bool SDLPlatform::_Compose (const Message< ModuleMsg::Compose > &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _Create() );

		CHECK( _ValidateAllSubscriptions() );
		CHECK( _SetState( EState::ComposedImmutable ) );

		LOG( "platform created", ELog::Debug );

		_SendEvent< OSMsg::OnSDLPlatformCreated >({});

		// async message
		FOR( i, _windows )
		{
			CHECK( GlobalSystems()->taskModule->Send( Message< ModuleMsg::PushAsyncMessage >{
						AsyncMessage{ LAMBDA(
							target = _windows[i],
							msg = Message< OSMsg::OnSDLPlatformCreated >{} ) (GlobalSystemsRef)
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
	bool SDLPlatform::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetSupportedMessages().HasAllTypes< SDLWindowMsgList_t >() );
		ASSERT( not _windows.IsExist( msg->module ) );

		_windows.Add( msg->module );

		if ( _IsCreated() )
		{
			CHECK( GlobalSystems()->taskModule->Send( Message< ModuleMsg::PushAsyncMessage >{
						AsyncMessage{ LAMBDA(
							target = msg->module,
							msg = Message< OSMsg::OnSDLPlatformCreated >{} ) (GlobalSystemsRef)
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
	bool SDLPlatform::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );

		ModulePtr	module = msg->module.Lock();

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
	bool SDLPlatform::_IsCreated () const
	{
		return _isCreated;
	}

/*
=================================================
	_Create
=================================================
*/
	bool SDLPlatform::_Create ()
	{
		CHECK_ERR( not _IsCreated() );

		SDL_CALL( SDL_Init( SDL_INIT_EVERYTHING ) );

		CHECK_ERR( _GetDirectories() );

		if ( _createInfo.flags[ CreateInfo::Platform::EPlatformFlags::ChangeDisplayMode ] )
		{
			CHECK_ERR( _SetScreenResolution( _createInfo.resolution, _createInfo.frequency ) );
		}

		_isCreated = true;
		return true;
	}
	
/*
=================================================
	_GetDisplays
=================================================
*/
	bool SDLPlatform::_GetDisplays (const Message< OSMsg::GetDisplays > &msg)
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
	bool SDLPlatform::_GetDirectories ()
	{
		_directories[ EDirectory::Internal ]		= "internal";
		_directories[ EDirectory::InternalCache ]	= "internal_cache";
		_directories[ EDirectory::External ]		= "external";
		_directories[ EDirectory::ExternalCache ]	= "external_cache";
		_directories[ EDirectory::SDCard ]			= "sdcard";
		_directories[ EDirectory::Library ]			= SDL_GetBasePath();
		
		return true;
	}
	
/*
=================================================
	_GetOSModules
=================================================
*/
	bool SDLPlatform::_GetOSModules (const Message< OSMsg::GetOSModules > &msg)
	{
		msg->result.Set( SDLObjectsConstructor::GetModuleIDs() );
		return true;
	}

/*
=================================================
	_Destroy
=================================================
*/
	void SDLPlatform::_Destroy ()
	{
		if ( _createInfo.flags[ CreateInfo::Platform::EPlatformFlags::ChangeDisplayMode ] )
		{
			_ReturnToDefaultResolution();
		}
		
		SDL_Quit();

		_isCreated = false;
	}

/*
=================================================
	_SetScreenResolution
=================================================
*/
	bool SDLPlatform::_SetScreenResolution (const uint2 &size, uint freq)
	{
		TODO( "_SetScreenResolution" );
		return true;
	}
	
/*
=================================================
	_ReturnToDefaultResolution
=================================================
*/
	bool SDLPlatform::_ReturnToDefaultResolution ()
	{
		TODO( "_ReturnToDefaultResolution" );
		return true;
	}

}	// PlatformSDL
//-----------------------------------------------------------------------------
	

namespace Platforms
{
/*
=================================================
	Register
=================================================
*/
	void SDLObjectsConstructor::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( SDLPlatformModuleID, &CreateSDLPlatform ) );
		CHECK( mf->Register( SDLWindowModuleID, &CreateSDLWindow ) );
		CHECK( mf->Register( SDLKeyInputModuleID, &CreateSDLKeyInput ) );
		CHECK( mf->Register( SDLMouseInputModuleID, &CreateSDLMouseInput ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void SDLObjectsConstructor::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( SDLPlatformModuleID );
		mf->UnregisterAll( SDLWindowModuleID );
		mf->UnregisterAll( SDLKeyInputModuleID );
		mf->UnregisterAll( SDLMouseInputModuleID );
	}
	
/*
=================================================
	GetModuleIDs
=================================================
*/
	OSModuleIDs SDLObjectsConstructor::GetModuleIDs ()
	{
		OSModuleIDs	ids;
		ids.platform	= SDLPlatformModuleID;
		ids.window		= SDLWindowModuleID;
		ids.keyInput	= SDLKeyInputModuleID;
		ids.mouseInput	= SDLMouseInputModuleID;
		return ids;
	}

/*
=================================================
	CreateSDLPlatform
=================================================
*/
	ModulePtr SDLObjectsConstructor::CreateSDLPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci)
	{
		return New< PlatformSDL::SDLPlatform >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_SDL