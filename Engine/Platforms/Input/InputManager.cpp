// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	InputManager::InputManager (const GlobalSystemsRef gs, const CreateInfo::InputManager &ci) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes )
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
	bool InputManager::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == InputThread::GetStaticID() );
		ASSERT( not _threads.IsExist( msg->module ) );

		_threads.Add( msg->module );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool InputManager::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == InputThread::GetStaticID() );
		ASSERT( _threads.IsExist( msg->module ) );

		_threads.Erase( msg->module );
		return true;
	}
	
/*
=================================================
	_CreateInputThread
=================================================
*/
	ModulePtr InputManager::_CreateInputThread (const GlobalSystemsRef gs, const CreateInfo::InputThread &ci)
	{
		return GXTypes::New< InputThread >( gs, ci );
	}
	
/*
=================================================
	_CreateInputManager
=================================================
*/
	ModulePtr InputManager::_CreateInputManager (const GlobalSystemsRef gs, const CreateInfo::InputManager &ci)
	{
		return GXTypes::New< InputManager >( gs, ci );
	}
	
/*
=================================================
	Register
=================================================
*/
	void InputManager::Register (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( InputManager::GetStaticID(), &_CreateInputManager ) );
		CHECK( mf->Register( InputThread::GetStaticID(), &_CreateInputThread ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void InputManager::Unregister (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll< InputManager >();
		mf->UnregisterAll< InputThread >();
	}


}	// Platforms
}	// Engine
