// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Threads/ThreadManager.h"
#include "Engine/Base/Modules/ModulesFactory.h"
#include "Engine/Base/Modules/ModuleUtils.h"
#include "Engine/Base/Main/MainSystem.h"

namespace Engine
{
namespace Base
{
	
	const TypeIdList	ThreadManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	ThreadManager::ThreadManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ThreadManager &) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_currentThread{New<ParallelThreadImpl>( ParallelThreadModuleID, gs, CreateInfo::Thread{ "MainThread", null } )}
	{
		SetDebugName( "ThreadManager" );

		_SubscribeOnMsg( this, &ThreadManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &ThreadManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &ThreadManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &ThreadManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &ThreadManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &ThreadManager::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &ThreadManager::_Update );
		_SubscribeOnMsg( this, &ThreadManager::_Link );
		_SubscribeOnMsg( this, &ThreadManager::_Compose );
		_SubscribeOnMsg( this, &ThreadManager::_Delete );
		//_SubscribeOnMsg( this, &ThreadManager::_FindThread );
		_SubscribeOnMsg( this, &ThreadManager::_AddToManager );
		_SubscribeOnMsg( this, &ThreadManager::_AddThreadToManager );
		_SubscribeOnMsg( this, &ThreadManager::_RemoveFromManager );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );
		
		_currentThread->_SetManager( this );
		CHECK( Send( ModuleMsg::AddThreadToManager{ _currentThread, DelegateBuilder( _currentThread, &ParallelThreadImpl::_NoWait )} ));
	}
	
/*
=================================================
	destructor
=================================================
*/
	ThreadManager::~ThreadManager ()
	{
		//LOG( "ThreadManager finalized", ELog::Debug );

		ASSERT( _threads.Empty() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool ThreadManager::_Delete (const ModuleMsg::Delete &msg)
	{
		_SendToAllThreads(
			LAMBDA() (const ThreadManagerPtr &mngr, const ModulePtr &thread, GlobalSystemsRef) {
				thread->Send( ModuleMsg::Delete{} );
			},
			true // exceptMain
		);
		
		// all secondary threads must be finished before deleting main thread
		{
			SCOPELOCK( _lock );

			for (auto& thread : _threads)
			{
				thread.second.wait();
				
				_SendForEachAttachments( ModuleMsg::Update{ TimeL::FromNanoSeconds(1) });
			}
		}

		_currentThread->Send( msg );
		_currentThread->_OnExit();

		CHECK_ERR( Module::_Delete_Impl( msg ) );

		_threads.Clear();
		
		_currentThread = null;
		return true;
	}
	
/*
=================================================
	_SendToAllThreads
=================================================
*/
	bool ThreadManager::_SendToAllThreads (CallInThreadFunc_t func, bool exceptMain)
	{
		using TaskMngrMsgList = MessageListFrom< ModuleMsg::AddTaskSchedulerToManager >;

		SCOPELOCK( _lock );

		auto	task_mngr = GlobalSystems()->mainSystem->GetModuleByMsg< TaskMngrMsgList >();
		CHECK_ERR( task_mngr );
		
		for (auto& thread : _threads)
		{
			if ( exceptMain and thread.second.thread == _currentThread )
				continue;

			CHECK( task_mngr->SendAsync( ModuleMsg::PushAsyncMessage{
						thread.first,
						LAMBDA( func, mngr = ModulePtr(this), thread = thread.second.thread ) (GlobalSystemsRef gs) {
							func( mngr, thread, gs );
						}}
					));
		}
		return true;
	}

/*
=================================================
	_Link
=================================================
*/
	bool ThreadManager::_Link (const ModuleMsg::Link &msg)
	{
		CHECK_ERR( Module::_Link_Impl( msg ) );
		
		_currentThread->Send( msg );
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool ThreadManager::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( Module::_DefCompose( false ) );
		
		_currentThread->Send( msg );
		_currentThread->_OnEnter();

		return true;
	}

/*
=================================================
	_Update
=================================================
*/
	bool ThreadManager::_Update (const ModuleMsg::Update &)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		SCOPELOCK( _lock );

		_currentThread->_SyncUpdate();
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool ThreadManager::_AddToManager (const ModuleMsg::AddToManager &)
	{
		RETURN_ERR( "use 'AddThreadToManager' instead of 'AddToManager'" );
	}
	
/*
=================================================
	_AddThreadToManager
=================================================
*/
	bool ThreadManager::_AddThreadToManager (const ModuleMsg::AddThreadToManager &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg.module );
		ASSERT( not _threads.IsExist( msg.module->GetThreadID() ) );

		_threads.Add( msg.module->GetThreadID(), { msg.module, RVREF(msg.wait.Get()) } );
		return true;
	}

/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool ThreadManager::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
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
			ASSERT( _threads[idx].second.thread == module );

			_threads.EraseByIndex( idx );
		}
		return true;
	}

/*
=================================================
	_FindThread
=================================================
*
	bool ThreadManager::_FindThread (const ModuleMsg::FindThread &msg)
	{
		return true;
	}
	
/*
=================================================
	CreateParallelThreadData
=================================================
*/
	struct ThreadManager::CreateParallelThreadData
	{
	// variables
		UntypedID_t					id;
		CreateInfo::Thread			info;
		OS::Thread					thread;
		SyncEvent					dataInitialized;
		SyncEvent					dataCopied;
		ModulePtr					result;			// out
		BlockingWaitThread_t		waitFunc;		// out
		Ptr< Module >				main;
		Ptr< ModulesFactory >		factory;

	// methods
		CreateParallelThreadData (UntypedID_t id, GlobalSystemsRef gs, CreateInfo::Thread &&info) :
			id{ id },
			info( RVREF( info ) ),
			dataInitialized( SyncEvent::MANUAL_RESET ),
			dataCopied( SyncEvent::MANUAL_RESET ),
			main( gs->mainSystem.ptr() ),
			factory( gs->modulesFactory.ptr() )
		{}
	};
	
/*
=================================================
	_CreateParallelThread
=================================================
*/
	ModulePtr ThreadManager::_CreateParallelThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Thread &ci)
	{
		// find manager for thread
		ModulePtr	mngr = ci.manager;

		// get manager from current thread
		/*if ( not mngr and gs->parallelThread )
		{
			mngr = gs->parallelThread->_GetManager();	// TODO
		}*/

		// get manager from global variable
		if ( not mngr and
			 gs->mainSystem and
			 gs->mainSystem->GetThreadID() == ThreadID::GetCurrent() )
		{
			mngr = gs->mainSystem->GetModuleByID( ThreadManagerModuleID );
		}

		CHECK_ERR( mngr );


		CreateParallelThreadData	data{ id, gs, CreateInfo::Thread{ ci.name, mngr, RVREF( ci.onStarted.Get() ) } };

		// start thread and set 'data' to new thread
		data.thread.Create( &_RunAsync, &data );

		// allow to copy 'data' in second thread.
		data.dataInitialized.Signal();
		
		// 'data' will changed in other thread, so we can't modify anything, you should wait for signal.
		CHECK_ERR( data.dataCopied.Wait( 10_sec ) );
		
		CHECK( mngr->Send( ModuleMsg::AddThreadToManager{ data.result, RVREF(data.waitFunc) }) );
		
		return data.result;
	}

/*
=================================================
	_CreateThreadManager
=================================================
*/
	ModulePtr ThreadManager::_CreateThreadManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ThreadManager &ci)
	{
		return New< ThreadManager >( id, gs, ci );
	}
	
/*
=================================================
	Register
=================================================
*/
	void ThreadManager::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		CHECK( mf->Register( ParallelThreadModuleID, &_CreateParallelThread ) );
		CHECK( mf->Register( ThreadManagerModuleID, &_CreateThreadManager ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void ThreadManager::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		mf->UnregisterAll( ParallelThreadModuleID );
		mf->UnregisterAll( ThreadManagerModuleID );
	}

/*
=================================================
	_RunAsync
=================================================
*/
	void ThreadManager::_RunAsync (void *d)
	{
		GlobalSubSystems		global_sys;
		ParallelThreadImplPtr	pt;

		// in this scope 'data' is valid,
		// after call data.dataCopied.Signal() 'data' may be invalidated
		{
			CreateParallelThreadData&	data = *Cast<CreateParallelThreadData *>(d);

			data.dataInitialized.Wait();

			global_sys.mainSystem._Set( data.main );
			global_sys.modulesFactory._Set( data.factory );

			pt = New< ParallelThreadImpl >( data.id, GlobalSystemsRef(global_sys), data.info );

			pt->_thread		= RVREF( data.thread );

			data.result		= pt;
			data.waitFunc	= DelegateBuilder( pt, &ParallelThreadImpl::_Wait );

			pt->_SetManager( data.info.manager );

			data.dataCopied.Signal();
		}
		
		pt->_OnEnter();

		ModuleUtils::Initialize({ pt });

		pt->_Loop();

		volatile ModulePtr	task_mod = pt->GetModuleByID( TaskModuleModuleID );

		pt->_OnExit();

		pt = null;

		GX_UNUSED( task_mod );
	}


}	// Base
}	// Engine
