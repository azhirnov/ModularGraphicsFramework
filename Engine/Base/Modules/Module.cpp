// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Modules/Module.h"
#include "Engine/Base/Tasks/TaskModule.h"
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
	Module::Module (const GlobalSystemsRef gs, UntypedID_t id,
					const Runtime::VirtualTypeList *msgTypes,
					const Runtime::VirtualTypeList *eventTypes) :
		BaseObject( gs ),
		_compId( id ),
		_supportedMessages( *msgTypes ),
		_supportedEvents( *eventTypes ),
		_state( EState::Initial ),
		_ownThread( ThreadID::GetCurrent() )
		//_lockAttachments( false )
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	Module::~Module ()
	{
		ASSERT( GetState() == EState::Deleting );

		CHECK( _attachments.Empty() );
		CHECK( _parent.IsNull() );
		CHECK( _manager.IsNull() );
	}
	
/*
=================================================
	UnsubscribeAll
=================================================
*/
	void Module::UnsubscribeAll (const ModulePtr &unit)
	{
		_msgHandler.UnsubscribeAll( unit );
	}

/*
=================================================
	_Attach
=================================================
*/
	bool Module::_Attach (const ModulePtr &unit)
	{
		CHECK_ERR( unit );
		CHECK_ERR( _IsMutableState( GetState() ) );
		//CHECK_ERR( not _lockAttachments );

		Modules_t::iterator	iter;
		
		if ( _attachments.Find( unit->GetModuleID(), OUT iter ) )
		{
			// TODO: perfomance warning
			if ( iter->second == unit )
			{
				LOG( "module already attached!", ELog::Debug );
				return true;
			}

			RETURN_ERR( "module with ID '" << ToString( (GModID::type) unit->GetModuleID() ) << "' is alredy attached" );
		}

		_attachments.Add( unit->GetModuleID(), unit );

		_SendForEachAttachments( Message< ModuleMsg::OnModuleAttached >{ this, this, unit } );
		return true;
	}
	
/*
=================================================
	_Detach
=================================================
*/
	bool Module::_Detach (const ModulePtr &unit)
	{
		CHECK_ERR( unit );
		CHECK_ERR( GetState() != EState::ComposedImmutable );
		//CHECK_ERR( not _lockAttachments );
		
		Modules_t::iterator	iter;
		
		if ( _attachments.Find( unit->GetModuleID(), OUT iter ) )
		{
			CHECK_ERR( iter->second == unit );
		}
		else
			RETURN_ERR( "module is not attached" );

		_SendForEachAttachments( Message< ModuleMsg::OnModuleDetached >{ this, this, unit } );

		_attachments.Erase( unit->GetModuleID() );
		return true;
	}
	
/*
=================================================
	_DetachAllAttachments
=================================================
*/
	void Module::_DetachAllAttachments ()
	{
		//CHECK_ERR( not _lockAttachments, void() );

		for (; not _attachments.Empty();)
		{
			_Detach( _attachments.Back().second );
		}
	}
	
/*
=================================================
	_DetachSelfFromParent
=================================================
*/
	void Module::_DetachSelfFromParent ()
	{
		if ( _parent )
		{
			CHECK( _parent->Send( Message< ModuleMsg::DetachModule >( this, this ) ) );
		}
	}
	
/*
=================================================
	_DetachSelfFromManager
=================================================
*/
	void Module::_DetachSelfFromManager ()
	{
		if ( not _manager )
			return;

		if ( _manager->GetThreadID() == this->GetThreadID() )
		{
			_manager->Send( Message< ModuleMsg::RemoveFromManager >{ this, this } );

			_SetManager( null );
		}
		else
			GXTypes::New< DetachModuleFromManagerAsyncTask >( this, _manager )->Execute();
	}
	
/*
=================================================
	_AttachSelfToManager
----
	helper function for per-thread modules that need to
	attach to manager in other thread, also works
	for other modules (with optimized version).
=================================================
*/
	bool Module::_AttachSelfToManager (const ModulePtr &mngr, UntypedID_t id, bool wait)
	{
		SHARED_POINTER_TYPE( AttachModuleToManagerAsyncTask )	task;

		// attach to known manager
		if ( mngr )
		{
			ASSERT( id == UntypedID_t(0) or id == mngr->GetModuleID() );

			if ( mngr->GetThreadID() == this->GetThreadID() )
			{
				// single-thread optimization
				mngr->Send( Message< ModuleMsg::AddToManager >{ this, this } );

				_SetManager( mngr );
				return true;
			}

			task = GXTypes::New< AttachModuleToManagerAsyncTask >( this, mngr );
		}
		else
		// find manager by moduleID and attach
		{
			ASSERT( id != UntypedID_t(0) or id != UntypedID_t(-1) );

			task = GXTypes::New< AttachModuleToManagerAsyncTask >( this, id );
		}

		task->Execute();

		if ( wait )
		{
			CHECK_ERR( task->Wait() );
		}
		return true;
	}

/*
=================================================
	_SetManager
=================================================
*/
	void Module::_SetManager (const ModulePtr &mngr)
	{
		// don't forget to remove self from previous manager
		ASSERT( _manager == null or mngr == null );

		_manager = mngr;

		if ( _manager )
		{
			const UntypedID_t	mask = GModID::_IDMask;

			CHECK( (_manager->GetModuleID() & mask) < (this->GetModuleID() & mask) );
		}

		// TODO: add before/after messages?
		_SendMsg( Message< ModuleMsg::OnManagerChanged >{ this, _manager } );
	}

/*
=================================================
	_SetState
=================================================
*/
	bool Module::_SetState (EState newState)
	{
		if ( newState == _state )
			return true;

		// keep deleting state
		if ( _state == EState::Deleting )
			RETURN_ERR( "can't change 'deleteing' state" );

		// change to initial, deleting or error state
		if ( newState == EState::Initial or
			 newState == EState::Deleting or
			_IsErrorState( newState ) )
		{
			_state = newState;
			return true;
		}

		// change error state to non-error state
		if ( _IsErrorState( _state ) )
		{
			if ( newState == EState::Initial or
				 newState == EState::Deleting )
			{
				_state = newState;
				return true;
			}

			RETURN_ERR( "can't change error state to non-error state, excepts 'initial' or 'deleting'" );
		}

		// Initial -> Linked -> ComposedMutable -> ComposedImmutable -> Deleting
		if ( newState > _state )
		{
			_state = newState;
			return true;
		}

		RETURN_ERR( "unknown or unsupported state combination used" );
	}
	
/*
=================================================
	_ValidateMsgSubscriptions
=================================================
*/
	bool Module::_ValidateMsgSubscriptions ()
	{
		return _msgHandler.Validate( GetSupportedMessages() );
	}
	
/*
=================================================
	_ValidateAllSubscriptions
=================================================
*/
	bool Module::_ValidateAllSubscriptions ()
	{
		return _msgHandler.Validate( GetSupportedMessages(), GetSupportedEvents() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Module::_Compose (bool immutable)
	{
		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( Message< ModuleMsg::Compose >{ this } );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( immutable ? EState::ComposedImmutable : EState::ComposedMutable ) );
		return true;
	}

/*
=================================================
	_IsMutableState
=================================================
*/
	bool Module::_IsMutableState (EState state)
	{
		return	state == EState::Initial		or
				state == EState::Linked			or
				state == EState::ComposedMutable;
	}
	
/*
=================================================
	_IsComposedState
=================================================
*/
	bool Module::_IsComposedState (EState state)
	{
		return	state == EState::ComposedMutable	or
				state == EState::ComposedImmutable;
	}
	
/*
=================================================
	_IsErrorState
=================================================
*/
	bool Module::_IsErrorState (EState state)
	{
		return	state == EState::LinkageFailed			or
				state == EState::ComposingFailed		or
				state == EState::IncompleteAttachment;
	}

/*
=================================================
	GetModule
=================================================
*/
	ModulePtr Module::GetModule (UntypedID_t id)
	{
		Message< ModuleMsg::FindModule >	request{ this, id };
		_SendMsg( request );

		return request->result.Get().Get( null );
	}

/*
=================================================
	_FindModule_Impl
=================================================
*/
	bool Module::_FindModule_Impl (const Message< ModuleMsg::FindModule > &msg)
	{
		Modules_t::iterator	iter;

		if ( _attachments.Find( msg->id, OUT iter ) )
		{
			msg->result.Set( iter->second );
		}
		return true;
	}
	
/*
=================================================
	_ModulesDeepSearch_Impl
=================================================
*/
	bool Module::_ModulesDeepSearch_Impl (const Message< ModuleMsg::ModulesDeepSearch > &msg)
	{
		TODO( "" );
		return false;
	}

/*
=================================================
	_AttachModule_Impl
=================================================
*/
	bool Module::_AttachModule_Impl (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK( _Attach( msg->newModule ) );
		return true;
	}
	
/*
=================================================
	_DetachModule_Impl
=================================================
*/
	bool Module::_DetachModule_Impl (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );
		return true;
	}
	
/*
=================================================
	_Link_Impl
=================================================
*/
	bool Module::_Link_Impl (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( _IsMutableState( GetState() ) );

		_SendForEachAttachments( Message< ModuleMsg::Link >{ this, _attachments } );

		CHECK( _SetState( EState::Linked ) );
		return true;
	}
	
/*
=================================================
	_Compose_Impl
=================================================
*/
	bool Module::_Compose_Impl (const Message< ModuleMsg::Compose > &msg)
	{
		return _Compose( false );
	}
	
/*
=================================================
	_Delete_Impl
=================================================
*/
	bool Module::_Delete_Impl (const Message< ModuleMsg::Delete > &msg)
	{
		// TODO: unsubscribe

		_DetachAllAttachments();
		_DetachSelfFromParent();

		// TODO: how to send Delete message to subscribers?
		_msgHandler.Clear();
		//_msgHandler.UnsubscribeAll( this );

		CHECK( _SetState( EState::Deleting ) );

		_DetachSelfFromManager();
		return true;
	}

/*
=================================================
	_Update_Impl
=================================================
*/
	bool Module::_Update_Impl (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_SendForEachAttachments( msg );
		return true;
	}
	
/*
=================================================
	_OnModuleAttached_Impl
=================================================
*/
	bool Module::_OnModuleAttached_Impl (const Message< ModuleMsg::OnModuleAttached > &msg)
	{
		if ( msg->attachedModule == this )
		{
			CHECK( _parent == null );
			CHECK( _SetState( EState::Initial ) );

			_parent = msg->parent;
		}
		return true;
	}
	
/*
=================================================
	_OnModuleDetached_Impl
=================================================
*/
	bool Module::_OnModuleDetached_Impl (const Message< ModuleMsg::OnModuleDetached > &msg)
	{
		if ( msg->detachedModule == this )
		{
			CHECK( _parent == msg->parent );
			CHECK( _SetState( EState::Initial ) );

			_parent	= null;
		}
		else
		{
			_msgHandler.UnsubscribeAll( msg->detachedModule );
		}
		return true;
	}
	
/*
=================================================
	_FindModule_Empty
=================================================
*/
	bool Module::_FindModule_Empty (const Message< ModuleMsg::FindModule > &)
	{
		return false;
	}
	
/*
=================================================
	_ModulesDeepSearch_Empty
=================================================
*/
	bool Module::_ModulesDeepSearch_Empty (const Message< ModuleMsg::ModulesDeepSearch > &)
	{
		return false;
	}

/*
=================================================
	_AttachModule_Empty
=================================================
*/
	bool Module::_AttachModule_Empty (const Message< ModuleMsg::AttachModule > &)
	{
		return false;
	}
	
/*
=================================================
	_DetachModule_Empty
=================================================
*/
	bool Module::_DetachModule_Empty (const Message< ModuleMsg::DetachModule > &)
	{
		return false;
	}
	
/*
=================================================
	_OnManagerChanged_Empty
=================================================
*/
	bool Module::_OnManagerChanged_Empty (const Message< ModuleMsg::OnManagerChanged > &)
	{
		return true;
	}

/*
=================================================
	_Update_Empty
=================================================
*/
	bool Module::_Update_Empty (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		return true;
	}
	
/*
=================================================
	_Compose_Empty
=================================================
*/
	bool Module::_Compose_Empty (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK( _SetState( EState::ComposedMutable ) );
		return true;
	}

/*
=================================================
	_Link_Empty
=================================================
*/
	bool Module::_Link_Empty (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( _IsMutableState( GetState() ) );

		CHECK( _SetState( EState::Linked ) );
		return true;
	}
	
/*
=================================================
	_Delete_Empty
=================================================
*/
	bool Module::_Delete_Empty (const Message< ModuleMsg::Delete > &msg)
	{
		CHECK( _SetState( EState::Deleting ) );

		_msgHandler.Clear();
		return true;
	}


}	// Base
}	// Engine
