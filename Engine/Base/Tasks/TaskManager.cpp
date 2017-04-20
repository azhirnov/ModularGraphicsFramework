// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Modules/ModulesFactory.h"

namespace Engine
{
namespace Base
{
	
	const Runtime::VirtualTypeList	TaskManager::_msgTypes{ UninitializedT< TaskManager::SupportedMessages_t >() };
	const Runtime::VirtualTypeList	TaskManager::_eventTypes{ UninitializedT< TaskManager::SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	TaskManager::TaskManager (const SubSystemsRef gs, const CreateInfo::TaskManager &info) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		SetDebugName( "TaskManager" );

		_SubscribeOnMsg( this, &TaskManager::_OnModuleAttached );
		_SubscribeOnMsg( this, &TaskManager::_OnModuleDetached );
		_SubscribeOnMsg( this, &TaskManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &TaskManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &TaskManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Update_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Link_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Compose_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Delete );
		_SubscribeOnMsg( this, &TaskManager::_OnRegistered );
		_SubscribeOnMsg( this, &TaskManager::_OnUnregistered );
		_SubscribeOnMsg( this, &TaskManager::_AddToManager );
		_SubscribeOnMsg( this, &TaskManager::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	_OnRegistered
=================================================
*/
	TaskManager::~TaskManager ()
	{
		LOG( "TaskManager finalized", ELog::Debug );

		ASSERT( _threads.Empty() );
	}

/*
=================================================
	_OnRegistered
=================================================
*/
	void TaskManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_SendForEachAttachments( msg );

		Module::_Delete_Impl( msg );
	}

/*
=================================================
	_OnRegistered
=================================================
*/
	void TaskManager::_OnRegistered (const Message< ModuleMsg::OnRegistered > &msg)
	{
		CHECK( msg->factory->Register( TaskModule::GetStaticID(), null, &_CreateTaskModule ) );
	}
	
/*
=================================================
	_OnUnregistered
=================================================
*/
	void TaskManager::_OnUnregistered (const Message< ModuleMsg::OnUnregistered > &msg)
	{
		CHECK( msg->factory->Unregister( TaskModule::GetStaticID() ) );
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	void TaskManager::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module, void() );
		ASSERT( not _threads.IsExist( msg->module->GetThreadID() ) );
		CHECK_ERR( msg->module->GetModuleID() == TaskModule::GetStaticID(), void() );

		_threads.Add( msg->module->GetThreadID(), msg->module );
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	void TaskManager::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module, void() );
		ASSERT( _threads.IsExist( msg->module->GetThreadID() ) );
		CHECK_ERR( msg->module->GetModuleID() == TaskModule::GetStaticID(), void() );

		_threads.Erase( msg->module->GetThreadID() );
	}

/*
=================================================
	PushAsyncMessage
=================================================
*/
	bool TaskManager::PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &msg)
	{
		SCOPELOCK( _lock );

		TaskThreads_t::iterator	iter;

		// find main target thread
		if ( msg->target != ThreadID() )
		{
			_threads.Find( msg->target, OUT iter );
		}
		
		// find alternative target thread
		if ( not iter and msg->altTarget != ThreadID() )
		{
			_threads.Find( msg->altTarget, OUT iter );
		}

		// find low load thread
		if ( not iter and not _threads.Empty() )
		{
			TODO("");
			iter = &_threads.Front();
		}

		if ( iter )
		{
			iter->second->_Push( RVREF( msg->asyncMsg.Get() ) );
			return true;
		}

		return false;
	}
	
/*
=================================================
	_CreateTaskModule
=================================================
*/
	ModulePtr TaskManager::_CreateTaskModule (const SubSystemsRef gs, const CreateInfo::TaskModule &info)
	{
		return GXTypes::New<TaskModule>( gs, info );
	}


}	// Base
}	// Engine
