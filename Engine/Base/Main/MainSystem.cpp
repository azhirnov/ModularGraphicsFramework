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
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes ),
		_factory( GlobalSystems() ),
		_taskMngr( GlobalSystems(), CreateInfo::TaskManager() ),
		_threadMngr( GlobalSystems(), CreateInfo::ThreadManager() ),
		_fileMngr( GlobalSystems() )
	{
		SetDebugName( "MainSystem" );

		GlobalSystems()->GetSetter< MainSystem >().Set( this );

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

		_Attach( &_taskMngr );
		_Attach( &_threadMngr );
		
		GlobalSystems()->Get< ParallelThread >()->
			AddModule( TaskModule::GetStaticID(), CreateInfo::TaskModule() );
	}

/*
=================================================
	destructor
=================================================
*/
	MainSystem::~MainSystem ()
	{
		StreamManager::Unregister( GlobalSystems() );
		ThreadManager::Unregister( GlobalSystems() );
		TaskManager::Unregister( GlobalSystems() );

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
	_Delete
=================================================
*/
	bool MainSystem::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_threadMngr.Send( msg );

		_SendForEachAttachments( msg );

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_CreateThreadManager
=================================================
*/
	ModulePtr MainSystem::_CreateThreadManager (GlobalSystemsRef gs, const CreateInfo::ThreadManager &info)
	{
		return GXTypes::New<ThreadManager>( gs, info );
	}
	
/*
=================================================
	_CreateTaskManager
=================================================
*/
	ModulePtr MainSystem::_CreateTaskManager (GlobalSystemsRef gs, const CreateInfo::TaskManager &info)
	{
		return GXTypes::New<TaskManager>( gs, info );
	}

	
}	// Base
}	// Engine
