// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Modules/ModulesFactory.h"

namespace Engine
{
namespace Base
{
	
	const TypeIdList	TaskManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	TaskManager::TaskManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::TaskManager &) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes )
	{
		SetDebugName( "TaskManager" );

		_SubscribeOnMsg( this, &TaskManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &TaskManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &TaskManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &TaskManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &TaskManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &TaskManager::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Update_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Link_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Compose_Empty );
		_SubscribeOnMsg( this, &TaskManager::_Delete );
		_SubscribeOnMsg( this, &TaskManager::_AddToManager );
		_SubscribeOnMsg( this, &TaskManager::_RemoveFromManager );
		_SubscribeOnMsg( this, &TaskManager::_AddTaskSchedulerToManager );
		_SubscribeOnMsg( this, &TaskManager::_PushAsyncMessage );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );
	}
	
/*
=================================================
	_OnRegistered
=================================================
*/
	TaskManager::~TaskManager ()
	{
		//LOG( "TaskManager finalized", ELog::Debug );

		ASSERT( _threads.Empty() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool TaskManager::_Delete (const ModuleMsg::Delete &msg)
	{
		_SendForEachAttachments( msg );

		CHECK_ERR( Module::_Delete_Impl( msg ) );

		SCOPELOCK( _lock );
		_threads.Clear();

		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool TaskManager::_AddToManager (const ModuleMsg::AddToManager &)
	{
		RETURN_ERR( "use 'AddTaskSchedulerToManager' message instead of 'AddToManager'" );
	}
	
/*
=================================================
	_AddTaskSchedulerToManager
=================================================
*/
	bool TaskManager::_AddTaskSchedulerToManager (const ModuleMsg::AddTaskSchedulerToManager &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg.module );
		ASSERT( not _threads.IsExist( msg.module->GetThreadID() ) );

		_threads.Add( msg.module->GetThreadID(), { msg.module, RVREF(msg.asyncPushMsg.Get()) } );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool TaskManager::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		SCOPELOCK( _lock );
		CHECK_ERR( msg.module );

		ModulePtr	module = msg.module.Lock();

		if ( not module )
			return false;

		ASSERT( _threads.IsExist( module->GetThreadID() ) );

		usize	idx;
		if ( _threads.FindIndex( module->GetThreadID(), OUT idx ) )
		{
			ASSERT( _threads[idx].second.module == module );

			_threads.EraseByIndex( idx );
		}
		return true;
	}

/*
=================================================
	_PushAsyncMessage
=================================================
*/
	bool TaskManager::_PushAsyncMessage (const ModuleMsg::PushAsyncMessage &msg) noexcept
	{
		SCOPELOCK( _lock );

		TaskThreads_t::iterator	iter;

		// find main target thread
		if ( msg.target != ThreadID() )
		{
			_threads.Find( msg.target, OUT iter );
		}
		
		// find alternative target thread
		if ( not iter and msg.altTarget != ThreadID() )
		{
			_threads.Find( msg.altTarget, OUT iter );
		}

		// find low load thread
		if ( not iter and
			 not _threads.Empty() and
			 (msg.target == ThreadID() or msg.altTarget == ThreadID()) )
		{
			TODO("");
			iter = &_threads.Front();
		}

		if ( iter )
		{
			iter->second.asyncPushMsg( RVREF( msg.asyncMsg.Get() ) );
			return true;
		}

		RETURN_ERR( "can't find any thread to process message" );
	}
	
/*
=================================================
	Register
=================================================
*/
	void TaskManager::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		CHECK( mf->Register( TaskModuleModuleID, &_CreateTaskModule ) );
		CHECK( mf->Register( TaskManagerModuleID, &_CreateTaskManager ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void TaskManager::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		mf->UnregisterAll( TaskModuleModuleID );
		mf->UnregisterAll( TaskManagerModuleID );
	}
	
/*
=================================================
	_CreateTaskManager
=================================================
*/
	ModulePtr TaskManager::_CreateTaskManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::TaskManager &ci)
	{
		return New< TaskManager >( id, gs, ci );
	}


}	// Base
}	// Engine
