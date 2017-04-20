// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Base/Modules/Module.h"
#include "Engine/Base/Tasks/TaskModule.h"

namespace Engine
{
namespace Base
{

/*
=================================================
	constructor
=================================================
*/
	Module::Module (const SubSystemsRef gs, UntypedID_t id,
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
	_SelfDetach
=================================================
*/
	void Module::_SelfDetach ()
	{
		if ( _parent )
		{
			CHECK( _parent->Send( Message< ModuleMsg::DetachModule >( this, this ) ) );
		}
	}
	
/*
=================================================
	_DeferredDelete
=================================================
*
	void Module::_DeferredDelete (const TaskModulePtr &)
	{
		ASSERT( GetState() == EState::Deleting );

		_SelfDetach();

		//_msgHandler.Clear();
		_msgHandler.UnsubscribeAll( this );
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
	void Module::_FindModule_Impl (const Message< ModuleMsg::FindModule > &msg)
	{
		Modules_t::iterator	iter;

		if ( _attachments.Find( msg->id, OUT iter ) )
		{
			msg->result.Set( iter->second );
		}
	}
	
/*
=================================================
	_AttachModule_Impl
=================================================
*/
	void Module::_AttachModule_Impl (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK( _Attach( msg->newModule ) );
	}
	
/*
=================================================
	_DetachModule_Impl
=================================================
*/
	void Module::_DetachModule_Impl (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );
	}
	
/*
=================================================
	_Link_Impl
=================================================
*/
	void Module::_Link_Impl (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( _IsMutableState( GetState() ), void() );

		_SendForEachAttachments( Message< ModuleMsg::Link >{ this, _attachments } );

		CHECK( _SetState( EState::Linked ) );
	}
	
/*
=================================================
	_Compose_Impl
=================================================
*/
	void Module::_Compose_Impl (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked, void() );

		_SendForEachAttachments( Message< ModuleMsg::Compose >{ this } );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
	}
	
/*
=================================================
	_Delete_Impl
=================================================
*/
	void Module::_Delete_Impl (const Message< ModuleMsg::Delete > &msg)
	{
		// TODO: unsubscribe

		_DetachAllAttachments();
		_SelfDetach();
		//_SetManager( null );

		// TODO: how to send Delete message to subscribers?
		_msgHandler.Clear();
		//_msgHandler.UnsubscribeAll( this );

		CHECK( _SetState( EState::Deleting ) );

		/*GlobalSystems()->Get< TaskModule >()->Send( Message< ModuleMsg::PushAsyncMessage >{
			this,
			AsyncMessage{ &Module::_DeferredDelete, this },
			GetThreadID(),
			GetThreadID()
		} );*/
	}

/*
=================================================
	_Update_Impl
=================================================
*/
	void Module::_Update_Impl (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ), void() );

		_SendForEachAttachments( msg );
	}
	
/*
=================================================
	_OnModuleAttached
=================================================
*/
	void Module::_OnModuleAttached (const Message< ModuleMsg::OnModuleAttached > &msg)
	{
		if ( msg->attachedModule == this )
		{
			CHECK( _parent == null );
			CHECK( _SetState( EState::Initial ) );

			_parent = msg->parent;
		}
	}
	
/*
=================================================
	_OnModuleDetached
=================================================
*/
	void Module::_OnModuleDetached (const Message< ModuleMsg::OnModuleDetached > &msg)
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
	}
	
/*
=================================================
	_FindModule_Empty
=================================================
*/
	void Module::_FindModule_Empty (const Message< ModuleMsg::FindModule > &msg)
	{
	}
	
/*
=================================================
	_AttachModule_Empty
=================================================
*/
	void Module::_AttachModule_Empty (const Message< ModuleMsg::AttachModule > &msg)
	{
	}
	
/*
=================================================
	_DetachModule_Empty
=================================================
*/
	void Module::_DetachModule_Empty (const Message< ModuleMsg::DetachModule > &msg)
	{
	}
	
/*
=================================================
	_Update_Empty
=================================================
*/
	void Module::_Update_Empty (const Message< ModuleMsg::Update > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ), void() );
	}
	
/*
=================================================
	_Compose_Empty
=================================================
*/
	void Module::_Compose_Empty (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked, void() );

		CHECK( _SetState( EState::ComposedMutable ) );
	}

/*
=================================================
	_Link_Empty
=================================================
*/
	void Module::_Link_Empty (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( _IsMutableState( GetState() ), void() );

		CHECK( _SetState( EState::Linked ) );
	}
	
/*
=================================================
	_Delete_Empty
=================================================
*/
	void Module::_Delete_Empty (const Message< ModuleMsg::Delete > &msg)
	{
		CHECK( _SetState( EState::Deleting ) );
	}


}	// Base
}	// Engine
