// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		_handlers.Reserve( 8 );
	}
	
/*
=================================================
	Send
=================================================
*/
	bool MessageHandler::Send (VariantCRef msgRef)
	{
		HandlersMap_t::iterator	iter;

		if ( _handlers.Find( msgRef.GetValueTypeId(), OUT iter ) )
		{
			//CHECK_ERR( not iter->second.locked );
			//SCOPE_SETTER( iter->second.locked = true, false );

			iter->second->Send( msgRef );
			return true;
		}

		return false;
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
	UnsubscribeAll
=================================================
*/
	void MessageHandler::UnsubscribeAll (const ModulePtr &ptr)
	{
		FOR( i, _handlers )
		{
			// TODO: may be it is not a good idea
			//CHECK_ERR( not _handlers[i].second.locked, void() );

			_handlers[i].second->UnsubscribeAll( ptr );
		}
	}

/*
=================================================
	Validate
----
	it is too slow
=================================================
*/
	bool MessageHandler::Validate (const Runtime::VirtualTypeList &typelist) const
	{
		// is all handlers presented in typelist?
		FOR( i, _handlers )
		{
			const TypeId	id = _handlers[i].first;

			if ( not typelist.HasType( id ) )
				RETURN_ERR( "Type '" << ToString( id ) << "' is not exist in typelist" );
		}

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
	bool MessageHandler::Validate (const Runtime::VirtualTypeList &msgTypes, const Runtime::VirtualTypeList &eventTypes) const
	{
		// is all handlers presented in typelist?
		FOR( i, _handlers )
		{
			const TypeId	id = _handlers[i].first;

			if ( not eventTypes.HasType( id ) and not msgTypes.HasType( id ) )
				RETURN_ERR( "Type '" << ToString( id ) << "' is not exist in typelists" );
		}

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
