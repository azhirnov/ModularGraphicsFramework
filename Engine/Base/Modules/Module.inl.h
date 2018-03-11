// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

namespace Engine
{
namespace Base
{

/*
=================================================
	Send
=================================================
*/
	template <typename T>
	forceinline bool Module::Send (const Message<T> &msg) noexcept
	{
		CHECK_ERR( msg.IsAsync() or _ownThread == ThreadID::GetCurrent() );
		
		GX_PROFILE_MSG( _SendUncheckedEvent(Message< ProfilingMsg::OnSendMsg >{ this, msg }.Async()) );

		return _msgHandler.Send( msg );
	}

/*
=================================================
	Subscribe
=================================================
*/
	template <typename ...Types>
	forceinline bool Module::Subscribe (Types&& ...args)
	{
		return _SubscribeOnEvent( FW<Types>( args )... );
	}
	
/*
=================================================
	_SubscribeReceiver_Func
=================================================
*/
	struct Module::_SubscribeReceiver_Func
	{
		ModulePtr	_self;
		ModulePtr	_other;
		bool		ok		= true;

		template <typename T, usize Index>
		void Process ()
		{
			bool is_ok = _self->Subscribe( _other, &Module::_SendEvent< typename T::Data_t > );
			ASSERT( is_ok );
			ok &= is_ok;
		}
	};

/*
=================================================
	ReceiveEvents
=================================================
*/
	template <typename MsgList>
	forceinline bool Module::ReceiveEvents (const ModulePtr &other)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_SubscribeReceiver_Func		func{ this, other };

		MsgList::RuntimeForEach( func );
		CHECK_ERR( func.ok );

		return true;
	}

/*
=================================================
	_SubscribeDbg
=================================================
*/
	GX_PROFILE_MSG(
	template <typename ...Types>
	forceinline bool Module::_SubscribeDbg (Types&& ...args)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Subscribe( TypeIdList{}, FW<Types>( args )..., /*checked*/false );
		return true;
	})

/*
=================================================
	Unsubscribe
=================================================
*
	template <typename ...Types>
	forceinline bool Module::Unsubscribe (Types&& ...args)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Unsubscribe( FW<Types>( args )... );
		return true;
	}
	
/*
=================================================
	UnsubscribeAll
=================================================
*/
	template <typename T>
	forceinline void Module::UnsubscribeAll (const T &unit)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent(), void() );

		_msgHandler.UnsubscribeAll( unit );
	}
		
/*
=================================================
	_SubscribeOnEvent
----
	subscribe external handlers for messages from
	self module.
=================================================
*/
	template <typename ...Types>
	forceinline bool Module::_SubscribeOnEvent (Types&& ...args)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Subscribe( GetSupportedEvents(), FW<Types>( args )... );
		return true;
	}

/*
=================================================
	_SendMsg
----
	send message and in debug check if this message is supported,
	it is recomended to use this method instead of 'Send'
	when you send message to oneself.
=================================================
*/
	template <typename T>
	forceinline bool Module::_SendMsg (const Message<T> &msg)
	{
		CHECK_ERR( msg.IsAsync() or _ownThread == ThreadID::GetCurrent() );
		
		if ( not GetSupportedMessages().HasType( TypeIdOf< Message<T> >() ) )
			RETURN_ERR( "Unsupported message type '" << ToString( TypeIdOf<T>() ) << "'" );

		return _msgHandler.Send( msg.From( this ) );
	}
		
/*
=================================================
	_SendEvent
----
	send message to subscribers (and to oneself if subscribed)
=================================================
*/
	template <typename T>
	forceinline bool Module::_SendEvent (const Message<T> &msg)
	{
		CHECK_ERR( msg.IsAsync() or _ownThread == ThreadID::GetCurrent() );
		
		if ( not GetSupportedEvents().HasType( TypeIdOf< Message<T> >() ) )
			RETURN_ERR( "Unsupported event type '" << ToString( TypeIdOf<T>() ) << "'" );

		return _msgHandler.Send( msg.From( this ) );
	}
	
	template <typename T>
	forceinline bool Module::_SendUncheckedEvent (const Message<T> &msg)
	{
		CHECK_ERR( msg.IsAsync() or _ownThread == ThreadID::GetCurrent() );

		return _msgHandler.Send( msg.From( this ) );
	}

/*
=================================================
	_SubscribeOnMsg
----
	subscribe to messages from oneself.
=================================================
*/
	template <typename ...Types>
	forceinline bool Module::_SubscribeOnMsg (Types&& ...args)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Subscribe( GetSupportedMessages(), FW<Types>( args )... );
		return true;
	}

/*
=================================================
	_SendForEachAttachments
=================================================
*/
	template <typename DataType>
	forceinline bool Module::_SendForEachAttachments (const Message<DataType> &msg)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		AttachedModules_t	temp = _attachments;		// TODO: optimize

		FOR( i, temp )
		{
			temp[i].second->Send( msg.From( this ) );
		}
		return true;
	}

/*
=================================================
	GetModuleByMsg
=================================================
*/
	template <typename Typelist>
	inline ModulePtr Module::GetModuleByMsg ()
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		ModulePtr	result;
		_FindAttachment( TypeIdList{UninitializedT<Typelist>()}, TypeIdList{}, OUT result );
		return result;
	}
	
/*
=================================================
	GetModuleByEvent
=================================================
*/
	template <typename Typelist>
	inline ModulePtr Module::GetModuleByEvent ()
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		ModulePtr	result;
		_FindAttachment( TypeIdList{}, TypeIdList{UninitializedT<Typelist>()}, OUT result );
		return result;
	}
	
/*
=================================================
	GetModuleByMsgEvent
=================================================
*/
	template <typename MsgList, typename EventsList>
	inline ModulePtr Module::GetModuleByMsgEvent ()
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		ModulePtr	result;
		_FindAttachment( TypeIdList{UninitializedT<MsgList>()}, TypeIdList{UninitializedT<EventsList>()}, OUT result );
		return result;
	}

/*
=================================================
	GetParentByMsg
=================================================
*/
	template <typename Typelist>
	inline ModulePtr Module::GetParentByMsg ()
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		ModulePtr	result;
		_FindParent( TypeIdList{UninitializedT<Typelist>()}, TypeIdList{}, OUT result );
		return result;
	}
	
/*
=================================================
	GetParentByEvent
=================================================
*/
	template <typename Typelist>
	inline ModulePtr Module::GetParentByEvent ()
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		ModulePtr	result;
		_FindParent( TypeIdList{}, TypeIdList{UninitializedT<Typelist>()}, OUT result );
		return result;
	}
	
/*
=================================================
	GetParentByMsgEvent
=================================================
*/
	template <typename MsgList, typename EventsList>
	inline ModulePtr Module::GetParentByMsgEvent ()
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		ModulePtr	result;
		_FindParent( TypeIdList{UninitializedT<MsgList>()}, TypeIdList{UninitializedT<EventsList>()}, OUT result );
		return result;
	}
	
/*
=================================================
	_CopySubscriptions
----
	will copy message handlers from 'other' to self
=================================================
*/
	template <typename MsgList>
	inline bool Module::_CopySubscriptions (const ModulePtr &other, bool removeUnsupported)
	{
		CHECK_ERR( other );

		TypeIdList	tlist{ UninitializedT<MsgList>() };

		FOR( i, tlist )
		{
			if ( other->GetSupportedMessages().HasType( tlist.Get(i) ) )
				continue;

			if ( removeUnsupported )
			{
				tlist.EraseByIndex( i );
				--i;
			}
			else
			{
				RETURN_ERR( "Message with type '" << ToString( tlist.Get(i) ) << "' is not supported" );
			}
		}

		return _msgHandler.CopySubscriptions( GetSupportedMessages(), other, other->_msgHandler, tlist );
	}

/*
=================================================
	SendTo
=================================================
*/
	template <typename T>
	forceinline bool BaseObject::SendTo (const ModulePtr &target, const Message<T> &msg) const
	{
		return target->Send( msg.From( WeakPointerType< BaseObject >( const_cast<BaseObject *>(this) ) ) );
	}


}	// Base
}	// Engine
