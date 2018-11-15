// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Input/InputManager.h"

namespace Engine
{
namespace Platforms
{
	
	const TypeIdList	InputManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputManager::InputManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::InputManager &) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes )
	{
		SetDebugName( "InputManager" );

		_SubscribeOnMsg( this, &InputManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &InputManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &InputManager::_AttachModule_Impl );
		_SubscribeOnMsg( this, &InputManager::_DetachModule_Impl );
		_SubscribeOnMsg( this, &InputManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &InputManager::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &InputManager::_Update_Empty );
		_SubscribeOnMsg( this, &InputManager::_Link_Impl );
		_SubscribeOnMsg( this, &InputManager::_Compose_Impl );
		_SubscribeOnMsg( this, &InputManager::_Delete_Impl );
		_SubscribeOnMsg( this, &InputManager::_AddToManager );
		_SubscribeOnMsg( this, &InputManager::_RemoveFromManager );
	}
	
/*
=================================================
	destructor
=================================================
*/
	InputManager::~InputManager ()
	{
		//LOG( "InputManager finalized", ELog::Debug );

		_threads.Clear();
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool InputManager::_AddToManager (const ModuleMsg::AddToManager &msg)
	{
		CHECK_ERR( msg.module );
		CHECK_ERR( msg.module->GetModuleID() == InputThreadModuleID );
		ASSERT( not _threads.IsExist( msg.module ) );

		_threads.Add( msg.module );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool InputManager::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		CHECK_ERR( msg.module );

		ModulePtr	module = msg.module.Lock();

		if ( not module )
			return false;

		CHECK_ERR( module->GetModuleID() == InputThreadModuleID );
		ASSERT( _threads.IsExist( module ) );

		_threads.Erase( module );
		return true;
	}
	
/*
=================================================
	_CreateInputManager
=================================================
*/
	ModulePtr InputManager::_CreateInputManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::InputManager &ci)
	{
		return New< InputManager >( id, gs, ci );
	}
	
/*
=================================================
	Register
=================================================
*/
	void InputManager::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( InputManagerModuleID, &_CreateInputManager ) );
		CHECK( mf->Register( InputThreadModuleID, &_CreateInputThread ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void InputManager::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( InputManagerModuleID );
		mf->UnregisterAll( InputThreadModuleID );
	}


}	// Platforms
}	// Engine
