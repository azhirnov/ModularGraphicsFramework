// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Platforms/Input/InputThread.h"
#include "Engine/Platforms/Input/InputManager.h"

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	InputThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	InputThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputThread::InputThread (const SubSystemsRef gs, const CreateInfo::InputThread &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		SetDebugName( "InputThread" );

		_SubscribeOnMsg( this, &InputThread::_OnModuleAttached );
		_SubscribeOnMsg( this, &InputThread::_OnModuleDetached );
		_SubscribeOnMsg( this, &InputThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &InputThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &InputThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &InputThread::_Update_Impl );
		_SubscribeOnMsg( this, &InputThread::_Link_Impl );
		_SubscribeOnMsg( this, &InputThread::_Compose_Impl );
		_SubscribeOnMsg( this, &InputThread::_Delete );
		
		CHECK( _ValidateMsgSubscriptions() );

		GXTypes::New< AttachModuleToManagerAsyncTask >( this, InputManager::GetStaticID() )->Execute();
	}
	
/*
=================================================
	destructor
=================================================
*/
	InputThread::~InputThread ()
	{
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void InputThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		Module::_Delete_Impl( msg );
		
		GXTypes::New< DetachModuleFromManagerAsyncTask >( this, InputManager::GetStaticID() )->Execute();
	}
	
/*
=================================================
	_InputKey
=================================================
*/
	void InputThread::_InputKey (const Message< ModuleMsg::InputKey > &msg)
	{
	}
	
/*
=================================================
	_InputMotion
=================================================
*/
	void InputThread::_InputMotion (const Message< ModuleMsg::InputMotion > &msg)
	{
	}


}	// Platforms
}	// Engine
