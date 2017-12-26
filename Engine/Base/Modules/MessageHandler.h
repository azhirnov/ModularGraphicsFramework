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
		using uint2				= GXMath::uint2;
		
		template <typename T> using SP = SharedPointerType<T>;
		template <typename T> using WP = WeakPointerType<T>;

		using ObjectPtr_t		= WP< StaticRefCountedObject >;
		using HandlerData_t		= usize[2];
		using Callback_t		= bool (*) (const ObjectPtr_t &, HandlerData_t &, VariantCRef);

		struct Handler : CompileTime::FastCopyable
		{
			ObjectPtr_t		ptr;
			HandlerData_t	data	= {};
			Callback_t		func	= null;
		};

		using HandlersMap_t		= MultiMap< TypeId, Handler >;
		using Dependencies_t	= Set< ObjectPtr_t >;


	// variables
	private:
		HandlersMap_t		_handlers;

		mutable OS::Mutex	_lock;		// TODO: is it needed?


	// methods
	public:
		MessageHandler ();
		
		template <typename T>
		bool Send (const Message<T> &msg);
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, const SP<Class> &obj, bool (Class2::*) (const Message<T> &), bool checked = true);
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, bool (Class2::*) (const Message<T> &), bool checked = true);

		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, Class *obj, bool (Class2::*) (const Message<T> &), bool checked = true);

		template <typename MsgList, typename Class>
		bool CopySubscriptions (const TypeIdList& validTypes, const SP<Class> &obj, const MessageHandler &other);
		
		template <typename MsgList, typename Class>
		bool CopySubscriptions (const TypeIdList& validTypes, const WP<Class> &obj, const MessageHandler &other);

		template <typename MsgList, typename Class>
		bool CopySubscriptions (const TypeIdList& validTypes, Class *obj, const MessageHandler &other);
		
		template <typename Class>
		void UnsubscribeAll (const SP<Class> &obj);

		template <typename Class>
		void UnsubscribeAll (const WP<Class> &obj);

		template <typename Class>
		void UnsubscribeAll (Class *obj);

		void Clear ();
		
		bool Validate (const TypeIdList &typelist) const;
		bool Validate (const TypeIdList &msgTypes, const TypeIdList &eventTypes) const;

	private:
		bool _Send (VariantCRef);
		bool _Subscribe (const TypeIdList& validTypes, TypeId id, Handler &&handler, bool checked);
		bool _CopySubscriptions (const TypeIdList& validTypes, const ObjectPtr_t &otherObj, const MessageHandler &other, ArrayCRef<TypeId> ids);
		void _UnsubscribeAll (const ObjectPtr_t &obj);

		template <typename Class, typename T>
		static bool _Call (const ObjectPtr_t &, HandlerData_t &, VariantCRef);
	};


	
/*
=================================================
	Send
=================================================
*/
	template <typename T>
	forceinline bool MessageHandler::Send (const Message<T> &msg)
	{
		return _Send( VariantCRef::FromConst( msg ) );
	}

/*
=================================================
	Subscribe
=================================================
*/
	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, Class *obj, bool (Class2::* func) (const Message<T> &), bool checked)
	{
		return Subscribe( validTypes, WP<Class>(obj), func, checked );
	}
	
	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, const SP<Class> &obj, bool (Class2::* func) (const Message<T> &), bool checked)
	{
		return Subscribe( validTypes, WP<Class>(obj), func, checked );
	}

/*
=================================================
	Subscribe
=================================================
*/
	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, bool (Class2::* func) (const Message<T> &), bool checked)
	{
		STATIC_ASSERT( sizeof(Handler::data) >= sizeof(func) );
		STATIC_ASSERT(( CompileTime::IsSameTypes< Class, Class2 > or CompileTime::IsBaseOf< Class2, Class > ));

		Handler		handler;
		UnsafeMem::MemCopy( handler.data, (void *)&func, SizeOf(func) );
		handler.ptr		= obj;
		handler.func	= &_Call< Class, T >;
		
		return _Subscribe( validTypes, TypeIdOf< Message<T> >(), RVREF(handler), checked );
	}
	
/*
=================================================
	CopySubscriptions
=================================================
*/
	template <typename MsgList, typename Class>
	forceinline bool MessageHandler::CopySubscriptions (const TypeIdList& validTypes, const SP<Class> &obj, const MessageHandler &other)
	{
		return _CopySubscriptions( validTypes, ObjectPtr_t(obj), other, TypeIdList(UninitializedT<MsgList>{}) );
	}

	template <typename MsgList, typename Class>
	forceinline bool MessageHandler::CopySubscriptions (const TypeIdList& validTypes, const WP<Class> &obj, const MessageHandler &other)
	{
		return _CopySubscriptions( validTypes, ObjectPtr_t(obj), other, TypeIdList(UninitializedT<MsgList>{}) );
	}
	
	template <typename MsgList, typename Class>
	forceinline bool MessageHandler::CopySubscriptions (const TypeIdList& validTypes, Class *obj, const MessageHandler &other)
	{
		return CopySubscriptions< MsgList >( validTypes, WP<Class>(obj), other );
	}
	
/*
=================================================
	CopySubscriptions
=================================================
*/
	template <typename Class>
	forceinline void MessageHandler::UnsubscribeAll (const SP<Class> &obj)
	{
		return _UnsubscribeAll( obj );
	}

	template <typename Class>
	forceinline void MessageHandler::UnsubscribeAll (const WP<Class> &obj)
	{
		return _UnsubscribeAll( obj );
	}

	template <typename Class>
	forceinline void MessageHandler::UnsubscribeAll (Class *obj)
	{
		return _UnsubscribeAll( obj );
	}

/*
=================================================
	_Call
=================================================
*/
	template <typename Class, typename T>
	inline bool MessageHandler::_Call (const ObjectPtr_t &ptr, HandlerData_t &data, VariantCRef msgRef)
	{
		auto	sp	= ptr.Lock();
		if ( not sp )
			return false;

		Class*	cl	= sp.ToPtr< Class >();
		auto	fn	= ReferenceCast< bool (Class::*) (const Message<T>&) >(data);
		auto&	msg	= msgRef.Get< Message<T> >();
	
		++msg._numOfSends;

		return (cl->*fn)( msg );
	}

	
}	// Base
}	// Engine
