// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Platforms/Input/InputManager.h"

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	InputManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	InputManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputManager::InputManager (const SubSystemsRef gs, const CreateInfo::InputManager &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		SetDebugName( "InputManager" );

		_SubscribeOnMsg( this, &InputManager::_OnModuleAttached );
		_SubscribeOnMsg( this, &InputManager::_OnModuleDetached );
		_SubscribeOnMsg( this, &InputManager::_AttachModule_Impl );
		_SubscribeOnMsg( this, &InputManager::_DetachModule_Impl );
		_SubscribeOnMsg( this, &InputManager::_FindModule_Impl );
		_SubscribeOnMsg( this, &InputManager::_Update_Impl );
		_SubscribeOnMsg( this, &InputManager::_Link_Impl );
		_SubscribeOnMsg( this, &InputManager::_Compose_Impl );
		_SubscribeOnMsg( this, &InputManager::_Delete_Impl );
		_SubscribeOnMsg( this, &InputManager::_AddToManager );
		_SubscribeOnMsg( this, &InputManager::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	InputManager::~InputManager ()
	{
		LOG( "InputManager finalized", ELog::Debug );

		_threads.Clear();
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	void InputManager::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module, void() );
		CHECK_ERR( msg->module->GetModuleID() == InputThread::GetStaticID(), void() );
		ASSERT( not _threads.IsExist( msg->module ) );

		_threads.Add( msg->module );
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	void InputManager::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module, void() );
		CHECK_ERR( msg->module->GetModuleID() == InputThread::GetStaticID(), void() );
		ASSERT( _threads.IsExist( msg->module ) );

		_threads.Erase( msg->module );
	}
	
/*
=================================================
	_CreateInputThread
=================================================
*/
	ModulePtr InputManager::_CreateInputThread (const SubSystemsRef gs, const CreateInfo::InputThread &ci)
	{
		return GXTypes::New< InputThread >( gs, ci );
	}
	
/*
=================================================
	_CreateInputManager
=================================================
*/
	ModulePtr InputManager::_CreateInputManager (const SubSystemsRef gs, const CreateInfo::InputManager &ci)
	{
		return GXTypes::New< InputManager >( gs, ci );
	}
	
/*
=================================================
	_RegisterAll
=================================================
*/
	void InputManager::_RegisterAll (const SubSystemsRef gs)
	{
		CHECK( gs->Get< ModulesFactory >()->Register( InputManager::GetStaticID(), null, &_CreateInputManager ) );
		CHECK( gs->Get< ModulesFactory >()->Register( InputThread::GetStaticID(), null, &_CreateInputThread ) );
	}


}	// Platforms
}	// Engine
