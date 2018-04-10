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
	bool MessageHandler::_Subscribe2 (const TypeIdList& validTypes, TypeId id, Handler &&handler, bool checked)
	{
		#if not (defined(GX_ENABLE_DEBUGGING) or defined(GX_ENABLE_PROFILING))
			checked = true;
		#endif

		if ( checked and not validTypes.HasType( id ) )
			RETURN_ERR( "Can't subscribe for event '" << ToString( id ) << "'" );

		//SCOPELOCK( _lock );
		
		usize	first;
		if ( _handlers.FindFirstIndex( id, OUT first ) )
		{
			for (usize i = first; i < _handlers.Count() and _handlers[i].first == id; ++i)
			{
				if ( MemCmp( _handlers[i].second, handler ) == 0 )
				{
					_handlers.EraseByIndex( i );
					break;
				}
			}
		}

		_handlers.Add( id, RVREF(handler) );
		return true;
	}
	
/*
=================================================
	_CopySubscriptions
=================================================
*/
	bool MessageHandler::_CopySubscriptions (const TypeIdList& validTypes, const ObjectPtr_t &obj, const MessageHandler &other, ArrayCRef<TypeId> ids)
	{
		//SCOPELOCK( other._lock );

		FOR( j, ids )
		{
			uint	copied = 0;
			usize	first;

			if ( other._handlers.FindFirstIndex( ids[j], OUT first ) )
			{
				for (usize i = first; i < other._handlers.Count() and other._handlers[i].first == ids[j]; ++i)
				{
					if ( other._handlers[i].second.ptr == obj )
					{
						_Subscribe2( validTypes, ids[j], Handler(other._handlers[i].second), false );
						copied++;
					}
				}
			}

			if ( copied == 0 )
				RETURN_ERR( "Handler for message '" << ToString( ids[j] ) << "' is not exist" );
		}
		return true;
	}

/*
=================================================
	_UnsubscribeAll
=================================================
*/
	void MessageHandler::_UnsubscribeAll (const ObjectPtr_t &ptr)
	{
		//SCOPELOCK( _lock );

		FOR( i, _handlers )
		{
			if ( _handlers[i].second.ptr == ptr )
			{
				_handlers.EraseByIndex( i );
				--i;
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
		//SCOPELOCK( _lock );

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
		//SCOPELOCK( _lock );

		// is all handlers presented in typelist?
		#if not (defined(GX_ENABLE_DEBUGGING) or defined(GX_ENABLE_PROFILING))
		{
			FOR( i, _handlers )
			{
				const TypeId	id = _handlers[i].first;

				if ( not typelist.HasType( id ) )
					RETURN_ERR( "Type '" << ToString( id ) << "' is not exist in typelist" );
			}
		}
		#endif

		// is all types in typelist has handlers?
		for (usize i = 0, cnt = typelist.Count(); i < cnt; ++i)
		{
			const TypeId	id = typelist.Get(i);

			if ( not _handlers.IsExist( id ) )
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
		//SCOPELOCK( _lock );

		// is all handlers presented in typelist?
		#if not (defined(GX_ENABLE_DEBUGGING) or defined(GX_ENABLE_PROFILING))
		{
			FOR( i, _handlers )
			{
				const TypeId	id = _handlers[i].first;

				if ( not eventTypes.HasType( id ) and not msgTypes.HasType( id ) )
					RETURN_ERR( "Type '" << ToString( id ) << "' is not exist in typelists" );
			}
		}
		#endif

		// is all types in typelist has handlers?
		for (usize i = 0, cnt = msgTypes.Count(); i < cnt; ++i)
		{
			const TypeId	id = msgTypes.Get(i);

			if ( not _handlers.IsExist( id ) )
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
