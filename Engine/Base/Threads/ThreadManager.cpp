// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	ThreadManager::ThreadManager (const GlobalSystemsRef gs, const CreateInfo::ThreadManager &info) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
		_currentThread( gs, CreateInfo::Thread{ "MainThread", null } )
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
		_SubscribeOnMsg( this, &ThreadManager::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
		
		_currentThread._SetManager( this );
		CHECK( SendTo( this, Message< ModuleMsg::AddToManager >{ &_currentThread } ) );
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
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool ThreadManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		SCOPELOCK( _lock );

		SendToAllThreads(
			LAMBDA() (const ThreadManagerPtr &mngr, const ModulePtr &thread, const ModulePtr &task) {
				thread->Send( Message< ModuleMsg::Delete >{}.From( mngr ) );
			}
		);

		_currentThread._OnExit();

		CHECK_ERR( Module::_Delete_Impl( msg ) );

		FOR( i, _threads )
		{
			_threads[i].second.ToPtr< ParallelThread >()->_thread.Wait();
		}

		_threads.Clear();
		return true;
	}
	
/*
=================================================
	SendToAllThreads
=================================================
*/
	bool ThreadManager::SendToAllThreads (CallInThreadFunc_t func)
	{
		SCOPELOCK( _lock );

		auto	task_mngr = GlobalSystems()->Get< MainSystem >()->GetModule( TaskManager::GetStaticID() ).ToPtr< TaskManager >();
		CHECK_ERR( task_mngr );

		FOR( i, _threads )
		{
			task_mngr->PushAsyncMessage( Message< ModuleMsg::PushAsyncMessage >{
					AsyncMessage{
						LAMBDA( func, mngr = ModulePtr(this), thread = _threads[i].second ) (const TaskModulePtr &task) {
							func( mngr, thread, task );
					} },
					_threads[i].first
				}.From( this )
			);
		}
		return true;
	}

/*
=================================================
	_Link
=================================================
*/
	bool ThreadManager::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( Module::_Link_Impl( msg ) );
		
		SendTo( &_currentThread, msg );
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool ThreadManager::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( Module::_Compose_Impl( msg ) );
		
		SendTo( &_currentThread, msg );

		_currentThread._OnEnter();
		return true;
	}

/*
=================================================
	_Update
=================================================
*/
	bool ThreadManager::_Update (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		SCOPELOCK( _lock );

		_currentThread._SyncUpdate();
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool ThreadManager::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module );
		ASSERT( not _threads.IsExist( msg->module->GetThreadID() ) );
		CHECK_ERR( msg->module->GetModuleID() == ParallelThread::GetStaticID() );

		_threads.Add( msg->module->GetThreadID(), msg->module );
		return true;
	}

/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool ThreadManager::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		SCOPELOCK( _lock );

		CHECK_ERR( msg->module );
		ASSERT( _threads.IsExist( msg->module->GetThreadID() ) );
		CHECK_ERR( msg->module->GetModuleID() == ParallelThread::GetStaticID() );

		_threads.Erase( msg->module->GetThreadID() );
		return true;
	}

/*
=================================================
	_FindThread
=================================================
*
	bool ThreadManager::_FindThread (const Message< ModuleMsg::FindThread > &msg)
	{
		return true;
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
		Ptr< FileManager >			fileMngr;

	// methods
		CreateParallelThreadData (GlobalSystemsRef gs, CreateInfo::Thread &&info) :
			info( RVREF( info ) ),
			sync( OS::SyncEvent::MANUAL_RESET ),
			main( gs->Get<MainSystem>() ),
			factory( gs->Get<ModulesFactory>() ),
			fileMngr( gs->Get<FileManager>() )
		{}
	};
	
/*
=================================================
	_CreateParallelThread
=================================================
*/
	ModulePtr ThreadManager::_CreateParallelThread (const GlobalSystemsRef gs, const CreateInfo::Thread &ci)
	{
		// find manager for thread
		ModulePtr	mngr = ci.manager;

		// get manager from current thread
		if ( not mngr and gs->Get< ParallelThread >() )
		{
			mngr = gs->Get< ParallelThread >()->_GetManager();
		}

		// get manager from global variable
		if ( not mngr and
			 gs->Get< MainSystem >() and
			 gs->Get< MainSystem >()->GetThreadID() == ThreadID::GetCurrent() )
		{
			mngr = gs->Get< MainSystem >()->GetModule( ThreadManager::GetStaticID() );
		}

		CHECK_ERR( mngr );


		CreateParallelThreadData	data{ gs, CreateInfo::Thread{ ci.name, mngr, RVREF( ci.onStarted.Get() ) } };

		// start thread and set 'data' to new thread
		data.thread.Create( &_RunAsync, &data );

		
		// data will changed in other thread, so we can't modify anything, you should wait for signal.
		CHECK_ERR( data.sync.Wait( TimeL::FromSeconds( 10 ) ) );
		
		CHECK( mngr->Send( Message< ModuleMsg::AddToManager >{ data.result } ) );
		
		return data.result;
	}
	
/*
=================================================
	_CreateThreadManager
=================================================
*/
	ModulePtr ThreadManager::_CreateThreadManager (const GlobalSystemsRef gs, const CreateInfo::ThreadManager &ci)
	{
		return New< ThreadManager >( gs, ci );
	}
	
/*
=================================================
	Register
=================================================
*/
	void ThreadManager::Register (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( ParallelThread::GetStaticID(), &_CreateParallelThread ) );
		CHECK( mf->Register( ThreadManager::GetStaticID(), &_CreateThreadManager ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void ThreadManager::Unregister (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll< ParallelThread >();
		mf->UnregisterAll< ThreadManager >();
	}

/*
=================================================
	_RunAsync
=================================================
*/
	void ThreadManager::_RunAsync (void *d)
	{
		GlobalSubSystems	global_sys;
		ParallelThreadPtr	pt;

		// in this scope 'data' is valid, after call data.sync.Signal() 'data' may be invalidated
		{
			CreateParallelThreadData&	data = *Cast<CreateParallelThreadData *>(d);

			global_sys.GetSetter< MainSystem >().Set( data.main );
			global_sys.GetSetter< ModulesFactory >().Set( data.factory );
			global_sys.GetSetter< FileManager >().Set( data.fileMngr );

			pt = New< ParallelThread >( GlobalSystemsRef(global_sys), data.info );

			pt->_thread = RVREF( data.thread );

			data.result = pt;
			pt->_SetManager( data.info.manager );

			data.sync.Signal();
		}
		
		pt->_OnEnter();

		pt->Send( Message< ModuleMsg::Link >{}.From( pt->_GetManager() ) );
		pt->Send( Message< ModuleMsg::Compose >{}.From( pt->_GetManager() ) );

		pt->_Loop();

		ModulePtr	task_mod = pt->GetModule( TaskModule::GetStaticID() );

		pt->_OnExit();

		pt = null;
		task_mod = null;
	}


}	// Base
}	// Engine
