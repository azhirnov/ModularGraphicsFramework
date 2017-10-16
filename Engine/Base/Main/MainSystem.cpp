// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Main/MainSystem.h"
#include "Engine/Base/Stream/StreamManager.h"

namespace Engine
{
namespace Base
{
	
	const Runtime::VirtualTypeList	MainSystem::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	MainSystem::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	GetMainSystemInstace
=================================================
*/
	Ptr<Module>  GetMainSystemInstace ()
	{
		static GlobalSubSystems		global_systems;
		static MainSystem			main_system{ GlobalSystemsRef(global_systems) };
		return &main_system;
	}

/*
=================================================
	constructor
=================================================
*/
	MainSystem::MainSystem (const GlobalSystemsRef gs) :
		Module( gs, ModuleConfig{ MainSystemModuleID, 0 }, &_msgTypes, &_eventTypes ),
		_factory( GlobalSystems() ),
		_fileMngr( GlobalSystems() )
	{
		SetDebugName( "MainSystem" );

		GlobalSystems()->GetSetter< MainSystem >().Set( this );

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

		GlobalSystems()->GetSetter< MainSystem >().Set( null );
		
		LOG( "MainSystem finalized", ELog::Debug );
	}
	
/*
=================================================
	destructor
=================================================
*/
	MainSystem::_FinalChecks::~_FinalChecks ()
	{
		Logger::GetInstance()->Close();

		DEBUG_ONLY( RefCountedObject::s_ChenckNotReleasedObjects() );
	}
	
/*
=================================================
	_Create
=================================================
*/
	void MainSystem::_Create ()
	{
		_taskMngr	= New<TaskManager>( GlobalSystems(), CreateInfo::TaskManager() );
		_threadMngr = New<ThreadManager>( GlobalSystems(), CreateInfo::ThreadManager() );

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
		StreamManager::Register( GlobalSystems() );

		_SendMsg< ModuleMsg::AttachModule >({ _taskMngr });
		_SendMsg< ModuleMsg::AttachModule >({ _threadMngr });
		
		GlobalSystems()->Get< ParallelThread >()->
			AddModule( StringCRef(), TaskModuleModuleID, CreateInfo::TaskModule{ _taskMngr } );
	}
	
/*
=================================================
	_Destroy
=================================================
*/
	void MainSystem::_Destroy ()
	{
		// delete thread manager at first
		_threadMngr->Send< ModuleMsg::Delete >({});

		_SendForEachAttachments< ModuleMsg::Delete >({});

		_DetachAllAttachments();
		_ClearMessageHandlers();

		//StreamManager::Unregister( GlobalSystems() );
		//ThreadManager::Unregister( GlobalSystems() );
		//TaskManager::Unregister( GlobalSystems() );

		_factory.Clear();

		_taskMngr	= null;
		_threadMngr	= null;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool MainSystem::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		msg.From( null );	// free sender

		_Destroy();
		_Create();
		return true;
	}

/*
=================================================
	_CreateThreadManager
=================================================
*/
	ModulePtr MainSystem::_CreateThreadManager (GlobalSystemsRef gs, const CreateInfo::ThreadManager &info)
	{
		return New<ThreadManager>( gs, info );
	}
	
/*
=================================================
	_CreateTaskManager
=================================================
*/
	ModulePtr MainSystem::_CreateTaskManager (GlobalSystemsRef gs, const CreateInfo::TaskManager &info)
	{
		return New<TaskManager>( gs, info );
	}

	
}	// Base
}	// Engine
