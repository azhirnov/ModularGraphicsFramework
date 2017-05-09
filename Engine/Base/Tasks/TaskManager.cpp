// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Modules/ModulesFactory.h"

namespace Engine
{
namespace Base
{
	
	const Runtime::VirtualTypeList	TaskManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	TaskManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	TaskManager::TaskManager (const GlobalSystemsRef gs, const CreateInfo::TaskManager &info) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes )
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
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool TaskManager::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module );
		ASSERT( not _threads.IsExist( msg->module->GetThreadID() ) );
		CHECK_ERR( msg->module->GetModuleID() == TaskModule::GetStaticID() );

		_threads.Add( msg->module->GetThreadID(), msg->module );
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
		CHECK_ERR( msg->module->GetModuleID() == TaskModule::GetStaticID() );

		_threads.Erase( msg->module->GetThreadID() );
		return true;
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
	Register
=================================================
*/
	void TaskManager::Register (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( TaskModule::GetStaticID(), &_CreateTaskModule ) );
		CHECK( mf->Register( TaskManager::GetStaticID(), &_CreateTaskManager ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void TaskManager::Unregister (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll< TaskModule >();
		mf->UnregisterAll< TaskManager >();
	}

/*
=================================================
	_CreateTaskModule
=================================================
*/
	ModulePtr TaskManager::_CreateTaskModule (const GlobalSystemsRef gs, const CreateInfo::TaskModule &ci)
	{
		return GXTypes::New< TaskModule >( gs, ci );
	}
	
/*
=================================================
	_CreateTaskManager
=================================================
*/
	ModulePtr TaskManager::_CreateTaskManager (const GlobalSystemsRef gs, const CreateInfo::TaskManager &ci)
	{
		return GXTypes::New< TaskManager >( gs, ci );
	}


}	// Base
}	// Engine
