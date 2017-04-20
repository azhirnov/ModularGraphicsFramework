// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

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

		//
		// Handler interface
		//
		struct IHandler
		{
		// interface
			virtual ~IHandler () {}
			virtual void Send (const VariantCRef &data) = 0;
			virtual void Subscribe (const VariantRef &callback) = 0;
			virtual void Unsubscribe (const VariantRef &callback) = 0;
			virtual void UnsubscribeAll (const ModulePtr &ptr) = 0;
		};

		//
		// Handler implementation
		//
		template <typename T>
		struct HandlerImpl final : public IHandler
		{
		// types
			using Delegate_t	= Delegate< void (const T&) >;
			using Event_t		= Event< Delegate_t >;

		// variables
			Event_t		_event;

		// methods
			HandlerImpl ()											{ _event.Reserve( 8 ); }
			HandlerImpl (HandlerImpl &&other) = default;
			HandlerImpl (const HandlerImpl &) = default;

			void Subscribe (const VariantRef &cb)		override	{ _event.Add( RVREF( cb.Get<Delegate_t>() ) ); }
			void Unsubscribe (const VariantRef &cb)		override	{ _event.Remove( RVREF( cb.Get<Delegate_t>() ) ); }
			void UnsubscribeAll (const ModulePtr &ptr)	override	{ _event.RemoveAllFor( ptr ); }

			void Send (const VariantCRef &data) override
			{
				auto const&	msg = data.Get<T>();

				Event_t		tmp = _event;

				FOR( i, tmp ) {
					++msg._numOfSends;
					tmp.Get(i).Call( msg );
				}
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
		HandlersMap_t	_handlers;


	// methods
	public:
		MessageHandler ();

		
		template <typename T>
		bool Send (const Message<T> &msg);
		
		bool Send (VariantCRef msgRef);
		
		template <typename ...Types>
		bool Subscribe (const Runtime::VirtualTypeList& validTypes, Types&& ...args);

		template <typename T>
		bool Subscribe (const Runtime::VirtualTypeList& validTypes, Delegate< void (const Message<T> &)> &&cb);
		
		template <typename ...Types>
		bool Unsubscribe (const Runtime::VirtualTypeList& validTypes, Types&& ...args);

		template <typename T>
		bool Unsubscribe (const Runtime::VirtualTypeList& validTypes, Delegate< void (const Message<T> &)> &&cb);

		void UnsubscribeAll (const ModulePtr &unit);

		void Clear ();
		
		bool Validate (const Runtime::VirtualTypeList &typelist) const;
		bool Validate (const Runtime::VirtualTypeList &msgTypes, const Runtime::VirtualTypeList &eventTypes) const;


	private:
		template <typename C, typename Class, typename T>
		Delegate< void (const Message<T> &) >  _CreateDelegate (C *ptr, void (Class:: *fn)(const Message<T> &));
		
		template <typename C, typename Class, typename T>
		Delegate< void (const Message<T> &) >  _CreateDelegate (C *ptr, void (Class:: *fn)(const Message<T> &) const);

		template <typename C, typename Class, typename T>
		Delegate< void (const Message<T> &) >  _CreateDelegate (const RC<C> &rc, void (Class:: *fn)(const Message<T> &));
		
		template <typename C, typename Class, typename T>
		Delegate< void (const Message<T> &) >  _CreateDelegate (const RC<C> &rc, void (Class:: *fn)(const Message<T> &) const);
		
		template <typename T>
		Delegate< void (const Message<T> &) >  _CreateDelegate (void (*fn)(const Message<T> &));
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
	inline bool MessageHandler::Subscribe (const Runtime::VirtualTypeList& validTypes, Delegate< void (const Message<T> &)> &&cb)
	{
		const TypeId	id		= TypeIdOf< Message<T> >();
		usize			index	= -1;

		CHECK_ERR( validTypes.HasType( id ) );

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
	inline bool MessageHandler::Unsubscribe (const Runtime::VirtualTypeList& validTypes, Delegate< void (const Message<T> &)> &&cb)
	{
		const TypeId			id = TypeIdOf< Message<T> >();
		HandlersMap_t::iterator	iter;
		
		CHECK_ERR( validTypes.HasType( id ) );

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
	_CreateDelegate
=================================================
*/
	template <typename C, typename Class, typename T>
	forceinline Delegate<void (const Message<T> &)>  MessageHandler::_CreateDelegate (C *ptr, void (Class:: *fn)(const Message<T> &))
	{
		return DelegateBuilder( RC<C>( ptr ), fn );
	}
		
	template <typename C, typename Class, typename T>
	forceinline Delegate<void (const Message<T> &)>  MessageHandler::_CreateDelegate (C *ptr, void (Class:: *fn)(const Message<T> &) const)
	{
		return DelegateBuilder( RC<C>( ptr ), fn );
	}

	template <typename C, typename Class, typename T>
	forceinline Delegate<void (const Message<T> &)>  MessageHandler::_CreateDelegate (const RC<C> &rc, void (Class:: *fn)(const Message<T> &))
	{
		return DelegateBuilder( rc, fn );
	}
		
	template <typename C, typename Class, typename T>
	forceinline Delegate<void (const Message<T> &)>  MessageHandler::_CreateDelegate (const RC<C> &rc, void (Class:: *fn)(const Message<T> &) const)
	{
		return DelegateBuilder( rc, fn );
	}
		
	template <typename T>
	forceinline Delegate<void (const Message<T> &)>  MessageHandler::_CreateDelegate (void (*fn)(const Message<T> &))
	{
		return DelegateBuilder( fn );
	}

	
}	// Base
}	// Engine
