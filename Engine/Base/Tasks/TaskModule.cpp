// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Tasks/TaskModule.h"
#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Threads/ParallelThread.h"
#include "Engine/Base/Modules/ModuleAsyncTasks.h"

namespace Engine
{
namespace Base
{

	const Runtime::VirtualTypeList	TaskModule::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	TaskModule::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	TaskModule::TaskModule (const GlobalSystemsRef gs, const CreateInfo::TaskModule &info) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		SetDebugName( GlobalSystems()->Get< ParallelThread >()->GetDebugName() + "_Tasks"_str );

		GlobalSystems()->GetSetter< TaskModule >().Set( this );

		_SubscribeOnMsg( this, &TaskModule::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &TaskModule::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &TaskModule::_AttachModule_Empty );
		_SubscribeOnMsg( this, &TaskModule::_DetachModule_Empty );
		_SubscribeOnMsg( this, &TaskModule::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &TaskModule::_FindModule_Empty );
		_SubscribeOnMsg( this, &TaskModule::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &TaskModule::_Update );
		_SubscribeOnMsg( this, &TaskModule::_Delete );
		_SubscribeOnMsg( this, &TaskModule::_PushAsyncMessage );
		
		CHECK( _ValidateMsgSubscriptions() );

		_msgQueue.ReserveCurrent( 256 );
		_msgQueue.ReservePending( 128 );

		_AttachSelfToManager( info.manager, TaskManager::GetStaticID(), true );

		CHECK( _SetState( EState::ComposedImmutable ) );
	}
	
/*
=================================================
	destructor
=================================================
*/
	TaskModule::~TaskModule ()
	{
		LOG( "TaskModule finalized", ELog::Debug );

		GlobalSystems()->GetSetter< TaskModule >().Set( null );

		ASSERT( _msgQueue.GetCurrentQueueCount() == 0 );
		ASSERT( _msgQueue.GetPendingQueueCount() == 0 );
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool TaskModule::_Update (const Message< ModuleMsg::Update > &msg)
	{
		//CHECK_ERR( _IsComposedState( GetState() ), void() );

		CHECK_ERR( msg.Sender() and msg.Sender() == _GetParent() );

		_Flush();
		_ProcessMessages();
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool TaskModule::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetParent() );

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_PushAsyncMessage
=================================================
*/
	bool TaskModule::_PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &msg)
	{
		CHECK_ERR( _GetManager() );

		CHECK( _GetManager().ToPtr< TaskManager >()->PushAsyncMessage( msg ) );
		return true;
	}

/*
=================================================
	_Push
----
	returns the size of pending queue
=================================================
*/
	usize TaskModule::_Push (AsyncMessage &&op)
	{
		const usize	res = _msgQueue.Push( RVREF( op ) );

		if ( res > _MaxPendingQueueSize() )
		{
			LOG( "pending queue is too big, queue will be force flushed!", ELog::Debug );
			_Flush();
			return 0;
		}
		return res;
	}
	
/*
=================================================
	_Flush
----
	returns the new size of current queue
=================================================
*/
	usize TaskModule::_Flush ()
	{
		return _msgQueue.Flush();
	}
	
/*
=================================================
	_ProcessMessages
----
	returns the number of processed messages
=================================================
*/
	usize TaskModule::_ProcessMessages ()
	{
		return _msgQueue.ProcessAll( LAMBDA(this) (const AsyncMessage &op) {{ op.Process( this ); }} );
	}

	
}	// Base
}	// Engine
