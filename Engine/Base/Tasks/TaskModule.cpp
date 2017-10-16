// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Modules/IDs.h"
#include "Engine/Base/Modules/Module.h"
#include "Engine/Base/Tasks/AsyncMessage.h"
#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Threads/ParallelThread.h"
#include "Engine/Base/Modules/ModuleAsyncTasks.h"

namespace Engine
{
namespace Base
{

/*
=================================================
	constructor
=================================================
*/
	TaskModule::TaskModule (const GlobalSystemsRef gs,
							const ModuleConfig &config,
							const Runtime::VirtualTypeList *msgTypes,
							const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes )
	{
		GlobalSystems()->GetSetter< TaskModule >().Set( this );
	}
	
/*
=================================================
	destructor
=================================================
*/
	TaskModule::~TaskModule ()
	{

		GlobalSystems()->GetSetter< TaskModule >().Set( null );
	}
//-----------------------------------------------------------------------------



	//
	// Async Task Module
	//

	class TaskModuleImpl final : public TaskModule
	{
	// types
	private:
		SHARED_POINTER( TaskModuleImpl );

		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::PushAsyncMessage,
											ModuleMsg::OnManagerChanged
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;
		using MsgQueue_t			= MtQueue< CircularQueue< AsyncMessage > >;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		MsgQueue_t		_msgQueue;


	// methods
	public:
		TaskModuleImpl (const GlobalSystemsRef gs, const CreateInfo::TaskModule &);
		~TaskModuleImpl ();


	// message handlers
	private:
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &);


	private:
		usize _Push (AsyncMessage &&op);
		usize _Flush ();
		usize _ProcessMessages ();

		static constexpr usize _MaxPendingQueueSize ()	{ return 1024; }
	};
//-----------------------------------------------------------------------------



	const Runtime::VirtualTypeList	TaskModuleImpl::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	TaskModuleImpl::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	TaskModuleImpl::TaskModuleImpl (const GlobalSystemsRef gs, const CreateInfo::TaskModule &info) :
		TaskModule( gs, ModuleConfig{ TaskModuleModuleID, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( GlobalSystems()->Get< ParallelThread >()->GetDebugName() + "_Tasks"_str );

		_SubscribeOnMsg( this, &TaskModuleImpl::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &TaskModuleImpl::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &TaskModuleImpl::_AttachModule_Empty );
		_SubscribeOnMsg( this, &TaskModuleImpl::_DetachModule_Empty );
		_SubscribeOnMsg( this, &TaskModuleImpl::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &TaskModuleImpl::_FindModule_Empty );
		_SubscribeOnMsg( this, &TaskModuleImpl::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &TaskModuleImpl::_Update );
		_SubscribeOnMsg( this, &TaskModuleImpl::_Delete );
		_SubscribeOnMsg( this, &TaskModuleImpl::_PushAsyncMessage );
		
		CHECK( _ValidateMsgSubscriptions() );

		_msgQueue.ReserveCurrent( 256 );
		_msgQueue.ReservePending( 128 );

		// attach to manager
		_SetManager( info.manager );

		if ( _GetManager()->GetThreadID() == GetThreadID() )
		{
			_GetManager()->Send< ModuleMsg::AddTaskSchedulerToManager >({ this, DelegateBuilder(this, &TaskModuleImpl::_Push) });
		}
		else
		{
			CHECK( _PushAsyncMessage( Message< ModuleMsg::PushAsyncMessage >{
						AsyncMessage{
							LAMBDA( mngr = _GetManager(), task = TaskModuleImplPtr(this) ) (const TaskModulePtr &) {
								mngr->Send< ModuleMsg::AddTaskSchedulerToManager >({ task, DelegateBuilder(task, &TaskModuleImpl::_Push) });
							}
						}, _GetManager()->GetThreadID()
			}) );
		}

		CHECK( _SetState( EState::ComposedImmutable ) );
	}
	
/*
=================================================
	destructor
=================================================
*/
	TaskModuleImpl::~TaskModuleImpl ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		LOG( "TaskModule finalized", ELog::Debug );

		ASSERT( _msgQueue.GetCurrentQueueCount() == 0 );
		ASSERT( _msgQueue.GetPendingQueueCount() == 0 );
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool TaskModuleImpl::_Update (const Message< ModuleMsg::Update > &msg)
	{
		//ASSERT( _IsComposedState( GetState() ), void() );
		ASSERT( msg.Sender() and _GetParents().IsExist( msg.Sender() ) );

		_Flush();
		_ProcessMessages();
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool TaskModuleImpl::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		ASSERT( msg.Sender() and _GetParents().IsExist( msg.Sender() ) );
		
		_DetachSelfFromManager();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		
		ASSERT( _msgQueue.GetCurrentQueueCount() == 0 );
		ASSERT( _msgQueue.GetPendingQueueCount() == 0 );

		_msgQueue.ClearAll();
		return true;
	}

/*
=================================================
	_PushAsyncMessage
=================================================
*/
	bool TaskModuleImpl::_PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &msg)
	{
		CHECK_ERR( GetState() != EState::Deleting );
		CHECK_ERR( _GetManager() );

		CHECK( _GetManager()->Send< ModuleMsg::PushAsyncMessage >( RVREF(msg) ) );
		return true;
	}

/*
=================================================
	_Push
----
	returns the size of pending queue
=================================================
*/
	usize TaskModuleImpl::_Push (AsyncMessage &&op)
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
	usize TaskModuleImpl::_Flush ()
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
	usize TaskModuleImpl::_ProcessMessages ()
	{
		return _msgQueue.ProcessAll( LAMBDA(this) (const AsyncMessage &op) {{ op.Process( this ); }} );
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	_CreateTaskModule
=================================================
*/
	ModulePtr TaskManager::_CreateTaskModule (const GlobalSystemsRef gs, const CreateInfo::TaskModule &ci)
	{
		CHECK_ERR( ci.manager );
		return New< TaskModuleImpl >( gs, ci );
	}
	
}	// Base
}	// Engine
