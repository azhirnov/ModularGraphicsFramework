// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Modules/ModulesFactory.h"

namespace Engine
{
namespace Base
{
	
	const TypeIdList	TaskManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	TaskManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	TaskManager::TaskManager (GlobalSystemsRef gs, const CreateInfo::TaskManager &) :
		Module( gs, ModuleConfig{ TaskManagerModuleID, 1 }, &_msgTypes, &_eventTypes )
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
	_Delete
=================================================
*/
	bool TaskManager::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool TaskManager::_AddToManager (const Message< ModuleMsg::AddToManager > &)
	{
		RETURN_ERR( "use 'AddTaskSchedulerToManager' message instead of 'AddToManager'" );
	}
	
/*
=================================================
	_AddTaskSchedulerToManager
=================================================
*/
	bool TaskManager::_AddTaskSchedulerToManager (const Message< ModuleMsg::AddTaskSchedulerToManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module );
		ASSERT( not _threads.IsExist( msg->module->GetThreadID() ) );

		_threads.Add( msg->module->GetThreadID(), { msg->module, RVREF(msg->asyncPushMsg.Get()) } );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool TaskManager::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module );
		ASSERT( _threads.IsExist( msg->module->GetThreadID() ) );

		usize	idx;
		if ( _threads.FindIndex( msg->module->GetThreadID(), OUT idx ) )
		{
			ASSERT( _threads[idx].second.module == msg->module );

			_threads.EraseByIndex( idx );
		}
		return true;
	}

/*
=================================================
	_PushAsyncMessage
=================================================
*/
	bool TaskManager::_PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &msg) noexcept
	{
		SCOPELOCK( _lock );
		ASSERT( msg.IsAsync() );

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
		if ( not iter and
			 not _threads.Empty() and
			 (msg->target == ThreadID() or msg->altTarget == ThreadID()) )
		{
			TODO("");
			iter = &_threads.Front();
		}

		if ( iter )
		{
			iter->second.asyncPushMsg( RVREF( msg->asyncMsg.Get() ) );
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
		auto	mf = gs->Get< ModulesFactory >();

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
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll( TaskModuleModuleID );
		mf->UnregisterAll( TaskManagerModuleID );
	}
	
/*
=================================================
	_CreateTaskManager
=================================================
*/
	ModulePtr TaskManager::_CreateTaskManager (GlobalSystemsRef gs, const CreateInfo::TaskManager &ci)
	{
		return New< TaskManager >( gs, ci );
	}


}	// Base
}	// Engine
