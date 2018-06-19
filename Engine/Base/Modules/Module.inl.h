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
	template <typename MsgT>
	forceinline bool Module::Send (const MsgT &msg) noexcept
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
	
		return SendAsync( msg );
	}
	
/*
=================================================
	SendAsync
=================================================
*/
	template <typename MsgT>
	GX_NO_INLINE bool Module::SendAsync (const MsgT &msg) noexcept
	{
		STATIC_ASSERT( MsgT::__is_message(true) );
		
		GX_PROFILE_MSG( _SendUncheckedEvent( ProfilingMsg::OnSendMsg{ this, msg }) );
		
		VariantCRef		var_msg = VariantCRef::FromConst( msg );

		#include "Module.Send.inl.h"
	}
	
/*
=================================================
	Request
=================================================
*/
	template <typename MsgT>
	forceinline auto  Module::Request (MsgT &&msg) noexcept
	{
		CHECK( Send( msg ) and bool(msg.result) );
		return *msg.result;
	}

/*
=================================================
	Subscribe
=================================================
*/
	template <typename Class, typename Func>
	forceinline bool Module::Subscribe (const Class &obj, Func func, EHandlerPriority priority)
	{
		return _SubscribeOnEvent( obj, func, priority );
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
			bool is_ok = _self->Subscribe( _other, &Module::_SendEvent<T> );
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
	template <typename Class, typename Func>
	forceinline bool Module::_SubscribeDbg (const Class &obj, Func func)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Subscribe( TypeIdList{}, obj, func, EHandlerPriority::_Reserved, /*checked*/false );
		return true;
	})

/*
=================================================
	UnsubscribeAll
=================================================
*/
	template <typename Class>
	forceinline void Module::UnsubscribeAll (const Class &unit)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent(), void() );

		_msgHandler.UnsubscribeAll( unit );
	}
		
/*
=================================================
	Unsubscribe
=================================================
*/
	template <typename Class, typename Func>
	forceinline void Module::Unsubscribe (const Class &obj, Func func)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent(), void() );

		return _msgHandler.Unsubscribe( obj, func );
	}

/*
=================================================
	_SubscribeOnEvent
----
	subscribe external handlers for messages from
	self module.
=================================================
*/
	template <typename Class, typename Func>
	forceinline bool Module::_SubscribeOnEvent (const Class &obj, Func func, EHandlerPriority priority)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		if ( priority == EHandlerPriority::Auto )
			priority = (Cast<void const*>(obj) == Cast<void const*>(this) ? EHandlerPriority::High : EHandlerPriority::Normal);

		_msgHandler.Subscribe( GetSupportedEvents(), obj, func, priority );
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
	template <typename MsgT>
	GX_NO_INLINE bool Module::_SendMsg (const MsgT &msg)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		if ( not GetSupportedMessages().HasType( TypeIdOf<MsgT>() ) )
			RETURN_ERR( "Unsupported message type '" << ToString( TypeIdOf<MsgT>() ) << "'" );

		VariantCRef		var_msg = VariantCRef::FromConst( msg );
		
		#include "Module.Send.inl.h"
	}
		
/*
=================================================
	_SendEvent
----
	send message to subscribers (and to oneself if subscribed)
=================================================
*/
	template <typename MsgT>
	GX_NO_INLINE bool Module::_SendEvent (const MsgT &msg)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		if ( not GetSupportedEvents().HasType( TypeIdOf<MsgT>() ) )
			RETURN_ERR( "Unsupported event type '" << ToString( TypeIdOf<MsgT>() ) << "'" );
		
		VariantCRef		var_msg = VariantCRef::FromConst( msg );
		
		#include "Module.Send.inl.h"
	}
	
	template <typename MsgT>
	GX_NO_INLINE bool Module::_SendUncheckedEvent (const MsgT &msg)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		VariantCRef		var_msg = VariantCRef::FromConst( msg );
		
		#include "Module.Send.inl.h"
	}

/*
=================================================
	_SubscribeOnMsg
----
	subscribe to messages from oneself.
=================================================
*/
	template <typename Class, typename Func>
	forceinline bool Module::_SubscribeOnMsg (const Class &obj, Func func, EHandlerPriority priority)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		if ( priority == EHandlerPriority::Auto )
			priority = (Cast<void const*>(obj) == Cast<void const*>(this) ? EHandlerPriority::High : EHandlerPriority::Normal);

		_msgHandler.Subscribe( GetSupportedMessages(), obj, func, priority );
		return true;
	}

/*
=================================================
	_SendForEachAttachments
=================================================
*/
	template <typename DataType>
	forceinline bool Module::_SendForEachAttachments (const DataType &msg)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		AttachedModules_t	temp = _attachments;		// TODO: optimize

		for (auto& attachment : temp)
		{
			attachment.second->Send( msg );
		}
		return true;
	}

/*
=================================================
	GetModuleByMsg
=================================================
*/
	template <typename Typelist>
	forceinline ModulePtr Module::GetModuleByMsg ()
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
	forceinline ModulePtr Module::GetModuleByEvent ()
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
	forceinline ModulePtr Module::GetModuleByMsgEvent ()
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
	forceinline ModulePtr Module::GetParentByMsg ()
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
	forceinline ModulePtr Module::GetParentByEvent ()
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
	forceinline ModulePtr Module::GetParentByMsgEvent ()
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
	forceinline bool Module::_CopySubscriptions (const ModulePtr &other, bool removeUnsupported)
	{
		return _CopySubscriptions<MsgList>( other, removeUnsupported, EHandlerPriority::Auto );
	}

	template <typename MsgList>
	inline bool Module::_CopySubscriptions (const ModulePtr &other, bool removeUnsupported, EHandlerPriority priority)
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

		return _msgHandler.CopySubscriptions( GetSupportedMessages(), other, other->_msgHandler, tlist, priority );
	}

	
}	// Base
}	// Engine
