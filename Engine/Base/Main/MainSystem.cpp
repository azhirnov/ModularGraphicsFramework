// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Main/MainSystem.h"
#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"

namespace Engine
{
namespace Base
{
	
	const TypeIdList	MainSystem::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	MainSystem::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	GetMainSystemInstance
=================================================
*/
	struct _FinalChecks {
		~_FinalChecks ();
	};

	Ptr<Module>  GetMainSystemInstance ()
	{
		static _FinalChecks			final_checks;
		static GlobalSubSystems		global_systems;
		static MainSystem			main_system{ GlobalSystemsRef(global_systems) };
		return &main_system;
	}

/*
=================================================
	constructor
=================================================
*/
	MainSystem::MainSystem (GlobalSystemsRef gs) :
		Module( gs, ModuleConfig{ MainSystemModuleID, 0 }, &_msgTypes, &_eventTypes ),
		_factory( GlobalSystems() )
	{
		SetDebugName( "MainSystem" );

		GlobalSystems()->mainSystem._Set( this );

		_Create();
	}

/*
=================================================
	destructor
=================================================
*/
	MainSystem::~MainSystem ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		_Destroy();

		CHECK( _SetState( EState::Deleting ) );

		GlobalSystems()->mainSystem._Set( null );
		
		LOG( "MainSystem finalized", ELog::Debug );
	}
	
/*
=================================================
	_FinalChecks
=================================================
*/
	_FinalChecks::~_FinalChecks ()
	{
		Logger::GetInstance()->Close();

		// compare with 1 becouse MainSystem is referenced object too
		DEBUG_ONLY( DbgRefCountedObject::s_ChenckNotReleasedObjects() );
		DEBUG_ONLY( RefCounter2::s_CheckAllocations() );
	}
	
/*
=================================================
	_Create
=================================================
*/
	void MainSystem::_Create () noexcept
	{
		_taskMngr	= New<TaskManager>( TaskManagerModuleID, GlobalSystems(), CreateInfo::TaskManager() );
		_threadMngr = New<ThreadManager>( ThreadManagerModuleID, GlobalSystems(), CreateInfo::ThreadManager() );

		CHECK( _SetState( EState::Initial ) );

		_SubscribeOnMsg( this, &MainSystem::_AttachModule_Impl );
		_SubscribeOnMsg( this, &MainSystem::_DetachModule_Impl );
		_SubscribeOnMsg( this, &MainSystem::_FindModule_Impl );
		_SubscribeOnMsg( this, &MainSystem::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &MainSystem::_Update_Impl );
		_SubscribeOnMsg( this, &MainSystem::_Link_Impl );
		_SubscribeOnMsg( this, &MainSystem::_Compose_Impl );
		_SubscribeOnMsg( this, &MainSystem::_Delete );
		
		CHECK( _ValidateMsgSubscriptions() );

		TaskManager::Register( GlobalSystems() );
		ThreadManager::Register( GlobalSystems() );
		DataProviderObjectsConstructor::Register( GlobalSystems() );

		_SendMsg( ModuleMsg::AttachModule{ _taskMngr });
		_SendMsg( ModuleMsg::AttachModule{ _threadMngr });
		
		GlobalSystems()->parallelThread->AddModule( StringCRef(), TaskModuleModuleID, CreateInfo::TaskModule{ _taskMngr } );
	}
	
/*
=================================================
	_Destroy
=================================================
*/
	void MainSystem::_Destroy () noexcept
	{
		// delete thread manager at first
		_threadMngr->Send( ModuleMsg::Delete{} );

		_SendForEachAttachments( ModuleMsg::Delete{} );

		_DetachAllAttachments();
		_ClearMessageHandlers();

		_factory.Clear();

		_taskMngr	= null;
		_threadMngr	= null;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool MainSystem::_Delete (const ModuleMsg::Delete &)
	{
		_Destroy();
		_Create();
		return true;
	}

/*
=================================================
	_CreateThreadManager
=================================================
*/
	ModulePtr MainSystem::_CreateThreadManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ThreadManager &info)
	{
		return New<ThreadManager>( id, gs, info );
	}
	
/*
=================================================
	_CreateTaskManager
=================================================
*/
	ModulePtr MainSystem::_CreateTaskManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::TaskManager &info)
	{
		return New<TaskManager>( id, gs, info );
	}

	
}	// Base
}	// Engine
