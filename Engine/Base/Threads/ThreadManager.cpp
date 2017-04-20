// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Base/Threads/ThreadManager.h"
#include "Engine/Base/Modules/ModulesFactory.h"
#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Main/MainSystem.h"

namespace Engine
{
namespace Base
{
	
	const Runtime::VirtualTypeList	ThreadManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	ThreadManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	ThreadManager::ThreadManager (const SubSystemsRef gs, const CreateInfo::ThreadManager &info) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes ),
		_currentThread( gs, CreateInfo::Thread{ "MainThread", this } )
	{
		SetDebugName( "ThreadManager" );

		_SubscribeOnMsg( this, &ThreadManager::_OnModuleAttached );
		_SubscribeOnMsg( this, &ThreadManager::_OnModuleDetached );
		_SubscribeOnMsg( this, &ThreadManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &ThreadManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &ThreadManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &ThreadManager::_Update );
		_SubscribeOnMsg( this, &ThreadManager::_Link );
		_SubscribeOnMsg( this, &ThreadManager::_Compose );
		_SubscribeOnMsg( this, &ThreadManager::_Delete );
		_SubscribeOnMsg( this, &ThreadManager::_OnRegistered );
		_SubscribeOnMsg( this, &ThreadManager::_OnUnregistered );
		//_SubscribeOnMsg( this, &ThreadManager::_FindThread );
		_SubscribeOnMsg( this, &ThreadManager::_AddToManager );
		_SubscribeOnMsg( this, &ThreadManager::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );

		_threads.Add( _currentThread.GetThreadID(), &_currentThread );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ThreadManager::~ThreadManager ()
	{
		LOG( "ThreadManager finalized", ELog::Debug );

		ASSERT( _threads.Empty() );

		_threads.Clear();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void ThreadManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		SCOPELOCK( _lock );

		// TODO: sender is not RC
		SendToAllThreads( AsyncMessage{ LAMBDA( sender = this ) (const TaskModulePtr &task) {
								task->GlobalSystems()->Get< ParallelThread >()->Send( Message< ModuleMsg::Delete >( sender ) );
							}} );

		//_currentThread._SyncUpdate();
		_currentThread._OnExit();

		Module::_Delete_Impl( msg );
	}

/*
=================================================
	_OnRegistered
=================================================
*/
	void ThreadManager::_OnRegistered (const Message< ModuleMsg::OnRegistered > &msg)
	{
		CHECK( msg->factory->Register( ParallelThread::GetStaticID(), null, &ThreadManager::_CreateParallelThread ) );
	}
	
/*
=================================================
	_OnUnregistered
=================================================
*/
	void ThreadManager::_OnUnregistered (const Message< ModuleMsg::OnUnregistered > &msg)
	{
		msg->factory->Unregister( ParallelThread::GetStaticID() );
	}
	
/*
=================================================
	SendToAllThreads
=================================================
*/
	bool ThreadManager::SendToAllThreads (AsyncMessage &&msg)
	{
		SCOPELOCK( _lock );

		auto	task_mngr = GlobalSystems()->Get< MainSystem >()->GetModule( TaskManager::GetStaticID() ).ToPtr< TaskManager >();
		CHECK_ERR( task_mngr );

		FOR( i, _threads )
		{
			CHECK( task_mngr->PushAsyncMessage( Message< ModuleMsg::PushAsyncMessage >{
				this,
				RVREF( msg ),
				_threads[i].first,	// target
				_threads[i].first	// alt target is same, becouse it is necessery
			} ) );
		}
		return true;
	}

/*
=================================================
	_Link
=================================================
*/
	void ThreadManager::_Link (const Message< ModuleMsg::Link > &msg)
	{
		Module::_Link_Impl( msg );
		
		// TODO: sender is not RC
		SendToAllThreads( AsyncMessage{ LAMBDA( sender = this ) (const TaskModulePtr &task) {
								task->GlobalSystems()->Get< ParallelThread >()->Send( Message< ModuleMsg::Link >( sender ) );
							}} );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	void ThreadManager::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		Module::_Compose_Impl( msg );
		
		// TODO: sender is not RC
		SendToAllThreads( AsyncMessage{ LAMBDA( sender = this ) (const TaskModulePtr &task) {
								task->GlobalSystems()->Get< ParallelThread >()->Send( Message< ModuleMsg::Compose >( sender ) );
							}} );

		_currentThread._OnEnter();
	}

/*
=================================================
	_Update
=================================================
*/
	void ThreadManager::_Update (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ), void() );

		SCOPELOCK( _lock );

		_currentThread._SyncUpdate();
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	void ThreadManager::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module, void() );
		ASSERT( not _threads.IsExist( msg->module->GetThreadID() ) );
		CHECK_ERR( msg->module->GetModuleID() == ParallelThread::GetStaticID(), void() );

		_threads.Add( msg->module->GetThreadID(), msg->module );
	}

/*
=================================================
	_RemoveFromManager
=================================================
*/
	void ThreadManager::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module, void() );
		ASSERT( _threads.IsExist( msg->module->GetThreadID() ) );
		CHECK_ERR( msg->module->GetModuleID() == ParallelThread::GetStaticID(), void() );

		_threads.Erase( msg->module->GetThreadID() );
	}

/*
=================================================
	_FindThread
=================================================
*
	void ThreadManager::_FindThread (const Message< ModuleMsg::FindThread > &msg)
	{
	}

/*
=================================================
	CreateParallelThreadData
=================================================
*/
	struct CreateParallelThreadData
	{
	// variables
		CreateInfo::Thread			info;
		OS::Thread					thread;
		OS::SyncEvent				sync;
		ModulePtr					result;
		Ptr< MainSystem >			main;
		Ptr< ModulesFactory >		factory;

	// methods
		CreateParallelThreadData (SubSystemsRef gs, const CreateInfo::Thread &info) :
			info( info ),
			sync( OS::SyncEvent::MANUAL_RESET ),
			main( gs->Get<MainSystem>() ),
			factory( gs->Get<ModulesFactory>() )
		{}
	};
	
/*
=================================================
	_CreateParallelThread
=================================================
*/
	ModulePtr ThreadManager::_CreateParallelThread (SubSystemsRef gs, const CreateInfo::Thread &info)
	{
		CreateParallelThreadData	data( gs, info );

		// start thread and set 'data' to new thread
		data.thread.Create( &_RunAsync, &data );

		
		// data will changed in other thread, so we can't modify anything, you should wait for signal.
		CHECK_ERR( data.sync.Wait( TimeU::FromSeconds( 10 ) ) );

		return data.result;
	}
	
/*
=================================================
	_RunAsync
=================================================
*/
	void ThreadManager::_RunAsync (void *d)
	{
		EngineSubSystems			global_sys;

		CreateParallelThreadData&	data = *Cast<CreateParallelThreadData *>(d);

		global_sys.GetSetter< MainSystem >().Set( data.main );
		global_sys.GetSetter< ModulesFactory >().Set( data.factory );

		auto pt = GXTypes::New< ParallelThread >( SubSystemsRef(global_sys), data.info );

		pt->_thread = RVREF( data.thread );

		data.result = pt;

		data.sync.Signal();

		pt->_OnEnter();
		pt->_Loop();
		pt->_OnExit();
	}


}	// Base
}	// Engine
