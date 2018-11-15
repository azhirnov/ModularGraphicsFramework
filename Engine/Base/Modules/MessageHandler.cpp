// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Modules/MessageHandler.h"

namespace Engine
{
namespace Base
{
	
/*
=================================================
	constructor
=================================================
*/
	MessageHandler::MessageHandler ()
	{
		_handlers.Reserve( 64 );
	}
	
/*
=================================================
	_Subscribe2
=================================================
*/
	bool MessageHandler::_Subscribe2 (TypeId id, EPriority priority, Handler &&handler)
	{
		ASSERT( priority != EPriority::Auto );
		
		// remove existing handler
		usize	first;
		if ( _handlers.CustomSearch().FindFirstIndex( HandlerSearch{id}, OUT first ) )
		{
			for (usize i = first; i < _handlers.Count() and _handlers[i].first.id == id; ++i)
			{
				if ( MemCmp( _handlers[i].second, handler ) == 0 )
				{
					_handlers.EraseByIndex( i );
					break;
				}
			}
		}

		_handlers.Add( HandlerKey{ id, priority }, RVREF(handler) );
		return true;
	}
	
/*
=================================================
	_CopySubscriptions
=================================================
*/
	bool MessageHandler::_CopySubscriptions (const Object_t *selfObj, const Object_t *, const MessageHandler &other,
											 ArrayCRef<TypeId> msgIds, bool warnIfNotExist, EPriority priority)
	{
		for (auto id : msgIds)
		{
			uint	copied = 0;
			usize	first;

			if ( other._handlers.CustomSearch().FindFirstIndex( HandlerSearch{id}, OUT first ) )
			{
				for (usize i = first; i < other._handlers.Count() and other._handlers[i].first.id == id; ++i)
				{
					auto*	ptr = other._handlers[i].second.ptr.Lock().RawPtr();

					if ( ptr != selfObj )
					{
						EPriority	prior = (priority == EPriority::Auto ? other._handlers[i].first.priority : priority);

						_Subscribe2( id, prior, Handler(other._handlers[i].second) );
						copied++;
					}
				}
			}

			if ( warnIfNotExist and copied == 0 )
				RETURN_ERR( "Handler for message '" << ToString( id ) << "' is not exist" );
		}
		return true;
	}
	
/*
=================================================
	UnsubscribeDeadHandlers
=================================================
*/
	void MessageHandler::UnsubscribeDeadHandlers ()
	{
		FOR( i, _handlers )
		{
			if ( _handlers[i].second.ptr.Lock() == null )
			{
				_handlers.EraseByIndex( i );
				--i;
			}
		}
	}

/*
=================================================
	_UnsubscribeAll
=================================================
*/
	void MessageHandler::_UnsubscribeAll (const Object_t *obj)
	{
		FOR( i, _handlers )
		{
			if ( _handlers[i].second.ptr.Lock() == null  or
				 _handlers[i].second.ptr.RawPtr() == obj )
			{
				_handlers.EraseByIndex( i );
				--i;
			}
		}
	}
	
/*
=================================================
	_UnsubscribeAll
=================================================
*/
	void MessageHandler::_UnsubscribeAll (TypeId msgID)
	{
		usize	first;
		if ( _handlers.CustomSearch().FindFirstIndex( HandlerSearch{msgID}, OUT first ) )
		{
			for (usize i = first; i < _handlers.Count() and _handlers[i].first.id == msgID; ++i)
			{
				_handlers.EraseByIndex( i );
				--i;
			}
		}
	}

/*
=================================================
	_Unsubscribe2
=================================================
*/
	void MessageHandler::_Unsubscribe2 (const Object_t* obj, const HandlerData_t &data)
	{
		FOR( i, _handlers )
		{
			if ( _handlers[i].second.ptr.Lock() == null		or
				 ( _handlers[i].second.ptr.RawPtr() == obj	and
				  All( _handlers[i].second.data == data ))	)
			{
				_handlers.EraseByIndex( i );
				--i;
			}
		}
	}
	
/*
=================================================
	_Unsubscribe2
=================================================
*/
	void MessageHandler::_Unsubscribe2 (const Object_t* obj, TypeId msgID)
	{
		usize	first;
		if ( _handlers.CustomSearch().FindFirstIndex( HandlerSearch{msgID}, OUT first ) )
		{
			for (usize i = first; i < _handlers.Count() and _handlers[i].first.id == msgID; ++i)
			{
				if ( _handlers[i].second.ptr.Lock() == null  or
					 _handlers[i].second.ptr.RawPtr() == obj )
				{
					_handlers.EraseByIndex( i );
					--i;
				}
			}
		}
	}

/*
=================================================
	Clear
=================================================
*/
	void MessageHandler::Clear ()
	{
		_handlers.Clear();
	}

/*
=================================================
	Validate
----
	it is too slow
=================================================
*/
	bool MessageHandler::Validate (const TypeIdList &typelist) const
	{
		// is all handlers presented in typelist?
		/*#if not (defined(GX_ENABLE_DEBUGGING) or defined(GX_ENABLE_PROFILING))
		{
			FOR( i, _handlers )
			{
				const TypeId	id = _handlers[i].first.id;

				if ( not typelist.HasType( id ) )
					RETURN_ERR( "Type '" << ToString( id ) << "' is not exist in typelist" );
			}
		}
		#endif*/

		// is all types in typelist has handlers?
		for (usize i = 0, cnt = typelist.Count(); i < cnt; ++i)
		{
			const TypeId	id = typelist.Get(i);

			if ( not _handlers.CustomSearch().IsExist( HandlerSearch{id} ) )
				RETURN_ERR( "Type '" << ToString( id ) << "' is not exist in message handlers" );
		}
		return true;
	}
	
/*
=================================================
	Validate
----
	it is too slow
=================================================
*/
	bool MessageHandler::Validate (const TypeIdList &msgTypes, const TypeIdList &eventTypes) const
	{
		// is all handlers presented in typelist?
		#if not (defined(GX_ENABLE_DEBUGGING) or defined(GX_ENABLE_PROFILING))
		{
			FOR( i, _handlers )
			{
				const TypeId	id = _handlers[i].first.id;

				if ( not eventTypes.HasType( id ) and not msgTypes.HasType( id ) )
					RETURN_ERR( "Type '" << ToString( id ) << "' is not exist in typelists" );
			}
		}
		#endif

		// is all types in typelist has handlers?
		for (usize i = 0, cnt = msgTypes.Count(); i < cnt; ++i)
		{
			const TypeId	id = msgTypes.Get(i);

			if ( not _handlers.CustomSearch().IsExist( HandlerSearch{id} ) )
				RETURN_ERR( "Message type '" << ToString( id ) << "' is not exist in message handlers" );
		}

		// check is someone subscribed to all events
		/*for (usize i = 0, cnt = eventTypes.Count(); i < cnt; ++i)
		{
			const TypeId	id = eventTypes.Get(i);
			
			if ( not _handlers.IsExist( id ) )
				RETURN_ERR( "Event type '" << ToString( id ) << "' is not exist in message handlers" );
		}*/
		return true;
	}

}	// Base
}	// Engine
