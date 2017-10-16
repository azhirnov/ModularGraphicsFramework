// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Message.h"

namespace Engine
{
namespace Base
{

	//
	// Message Handler
	//

	class MessageHandler final
	{
	// types
	private:
		using uint2		= GXMath::uint2;

		//
		// Handler interface
		//
		struct IHandler
		{
		// interface
			virtual ~IHandler () {}
			virtual uint2 Send (const VariantCRef &data) = 0;
			virtual void  Subscribe (const VariantRef &callback) = 0;
			virtual void  Unsubscribe (const VariantRef &callback) = 0;
			virtual void  UnsubscribeAll (const void *ptr) = 0;
		};

		//
		// Handler implementation
		//
		template <typename T>
		struct HandlerImpl final : public IHandler
		{
		// types
			using Delegate_t	= Delegate< bool (const T&) >;
			using Event_t		= Event< Delegate_t >;

		// variables
			Event_t		_event;

		// methods
			HandlerImpl ()											{ _event.Reserve( 8 ); }
			HandlerImpl (HandlerImpl &&other) = default;
			HandlerImpl (const HandlerImpl &) = default;

			void Subscribe (const VariantRef &cb)		override	{ _event.Add( RVREF( cb.Get<Delegate_t>() ) ); }
			void Unsubscribe (const VariantRef &cb)		override	{ _event.Remove( RVREF( cb.Get<Delegate_t>() ) ); }
			void UnsubscribeAll (const void *ptr)		override	{ _event.RemoveAllFor( ptr ); }

			uint2 Send (const VariantCRef &data) override
			{
				auto const&	msg = data.Get<T>();
				Event_t		tmp = _event;	// TODO: optimize
				uint2		count;

				FOR( i, tmp )
				{
					++msg._numOfSends;
					++count[ tmp.Get(i).Call( msg ) ];
				}
				return count;	// returns count of successfull and unsuccessfull handler calls
			}
		};

		template <typename T>
		using RC				= SHARED_POINTER_TYPE(T);
		using Interface_t		= VariantInterface< IHandler, sizeof(HandlerImpl<Message<void*>>) >;


		//
		// Handler Info
		//
		struct HandlerInfo
		{
		// variables
			Interface_t		data;
			//bool			locked { false };
			
		// methods
			template <typename T>
			explicit HandlerInfo (UninitializedT<T>) : data( HandlerImpl< Message<T> >() ) {}

			HandlerInfo (HandlerInfo &&) = default;
			HandlerInfo (const HandlerInfo &) = default;

			HandlerInfo& operator = (HandlerInfo &&) = default;
			HandlerInfo& operator = (const HandlerInfo &) = default;

			Ptr<IHandler> operator -> () const	{ return data.GetInterface(); }
		};

		using HandlersMap_t		= Map< TypeId, HandlerInfo >;


	// variables
	private:
		HandlersMap_t		_handlers;

		mutable OS::Mutex	_lock;


	// methods
	public:
		MessageHandler ();

		
		template <typename T>
		bool Send (const Message<T> &msg);
		
		bool Send (VariantCRef msgRef);
		
		template <typename ...Types>
		bool Subscribe (const Runtime::VirtualTypeList& validTypes, Types&& ...args);

		template <typename T>
		bool Subscribe (const Runtime::VirtualTypeList& validTypes, Delegate< bool (const Message<T> &)> &&cb);
		
		template <typename ...Types>
		bool Unsubscribe (const Runtime::VirtualTypeList& validTypes, Types&& ...args);

		template <typename T>
		bool Unsubscribe (const Runtime::VirtualTypeList& validTypes, Delegate< bool (const Message<T> &)> &&cb);
		
		template <typename T>
		void UnsubscribeAll (const T &unit);
		void UnsubscribeAll (const void *ptr);

		void Clear ();
		
		bool Validate (const Runtime::VirtualTypeList &typelist) const;
		bool Validate (const Runtime::VirtualTypeList &msgTypes, const Runtime::VirtualTypeList &eventTypes) const;


	private:
		template <typename C, typename Class, typename T>
		Delegate< bool (const Message<T> &) >  _CreateDelegate (C *ptr, bool (Class:: *fn)(const Message<T> &));
		
		template <typename C, typename Class, typename T>
		Delegate< bool (const Message<T> &) >  _CreateDelegate (C *ptr, bool (Class:: *fn)(const Message<T> &) const);

		template <typename C, typename Class, typename T>
		Delegate< bool (const Message<T> &) >  _CreateDelegate (const RC<C> &rc, bool (Class:: *fn)(const Message<T> &));
		
		template <typename C, typename Class, typename T>
		Delegate< bool (const Message<T> &) >  _CreateDelegate (const RC<C> &rc, bool (Class:: *fn)(const Message<T> &) const);
		
		template <typename T>
		Delegate< bool (const Message<T> &) >  _CreateDelegate (bool (*fn)(const Message<T> &));
	};


	
/*
=================================================
	Send
=================================================
*/
	template <typename T>
	forceinline bool MessageHandler::Send (const Message<T> &msg)
	{
		return Send( VariantCRef::FromConst( msg ) );
	}

/*
=================================================
	Subscribe
=================================================
*/
	template <typename ...Types>
	forceinline bool MessageHandler::Subscribe (const Runtime::VirtualTypeList& validTypes, Types&& ...args)
	{
		STATIC_ASSERT( not CompileTime::IsDelegate< typename CompileTime::TypeListFrom<Types...>::Front > );

		return Subscribe( validTypes, RVREF( _CreateDelegate( FW<Types>( args )... ) ) );
	}
	
/*
=================================================
	Subscribe
=================================================
*/
	template <typename T>
	inline bool MessageHandler::Subscribe (const Runtime::VirtualTypeList& validTypes, Delegate< bool (const Message<T> &)> &&cb)
	{
		const TypeId	id		= TypeIdOf< Message<T> >();
		usize			index	= -1;

		if ( not validTypes.HasType( id ) )
			RETURN_ERR( "Can't subscribe for event '" << ToString( id ) << "'" );

		SCOPELOCK( _lock );

		// create handler
		if ( not _handlers.FindIndex( id, OUT index ) )
		{
			index = _handlers.Add( id, HandlerInfo{ UninitializedT<T>() } );
		}
			
		// TODO: may be it is not a good idea
		//CHECK_ERR( not _handlers[index].second.locked );

		_handlers[ index ].second->Subscribe( cb );
		return true;
	}

/*
=================================================
	Unsubscribe
=================================================
*/
	template <typename ...Types>
	forceinline bool MessageHandler::Unsubscribe (const Runtime::VirtualTypeList& validTypes, Types&& ...args)
	{
		STATIC_ASSERT( not CompileTime::IsDelegate< typename CompileTime::TypeListFrom<Types...>::Front > );

		return Unsubscribe( validTypes, RVREF( _CreateDelegate( FW<Types>( args )... ) ) );
	}
	
/*
=================================================
	Unsubscribe
=================================================
*/
	template <typename T>
	inline bool MessageHandler::Unsubscribe (const Runtime::VirtualTypeList& validTypes, Delegate< bool (const Message<T> &)> &&cb)
	{
		const TypeId			id = TypeIdOf< Message<T> >();
		HandlersMap_t::iterator	iter;
		
		if ( not validTypes.HasType( id ) )
			RETURN_ERR( "Can't unsubscribe for event '" << ToString( id ) << "'" );
		
		SCOPELOCK( _lock );

		if ( _handlers.Find( id, OUT iter ) )
		{
			// TODO: may be it is not a good idea
			//CHECK_ERR( not iter->second.locked );
			
			iter->second->Unsubscribe( cb );
			return true;
		}

		return false;
	}
	
/*
=================================================
	UnsubscribeAll
=================================================
*/
	template <typename T>
	inline void MessageHandler::UnsubscribeAll (const T &ptr)
	{
		SCOPELOCK( _lock );

		FOR( i, _handlers )
		{
			// TODO: may be it is not a good idea
			//CHECK_ERR( not _handlers[i].second.locked, void() );

			_handlers[i].second->UnsubscribeAll( (const void *) &(*ptr) );
		}
	}

	inline void MessageHandler::UnsubscribeAll (const void *ptr)
	{
		SCOPELOCK( _lock );

		FOR( i, _handlers ) {
			_handlers[i].second->UnsubscribeAll( ptr );
		}
	}
	
/*
=================================================
	_CreateDelegate
=================================================
*/
	template <typename C, typename Class, typename T>
	forceinline Delegate<bool (const Message<T> &)>  MessageHandler::_CreateDelegate (C *ptr, bool (Class:: *fn)(const Message<T> &))
	{
		return DelegateBuilder( RC<C>( ptr ), fn );		// only Reference Countable types are supported yet
	}
		
	template <typename C, typename Class, typename T>
	forceinline Delegate<bool (const Message<T> &)>  MessageHandler::_CreateDelegate (C *ptr, bool (Class:: *fn)(const Message<T> &) const)
	{
		return DelegateBuilder( RC<C>( ptr ), fn );		// only Reference Countable types are supported yet
	}

	template <typename C, typename Class, typename T>
	forceinline Delegate<bool (const Message<T> &)>  MessageHandler::_CreateDelegate (const RC<C> &rc, bool (Class:: *fn)(const Message<T> &))
	{
		return DelegateBuilder( rc, fn );
	}
		
	template <typename C, typename Class, typename T>
	forceinline Delegate<bool (const Message<T> &)>  MessageHandler::_CreateDelegate (const RC<C> &rc, bool (Class:: *fn)(const Message<T> &) const)
	{
		return DelegateBuilder( rc, fn );
	}
		
	template <typename T>
	forceinline Delegate<bool (const Message<T> &)>  MessageHandler::_CreateDelegate (bool (*fn)(const Message<T> &))
	{
		return DelegateBuilder( fn );
	}

	
}	// Base
}	// Engine
