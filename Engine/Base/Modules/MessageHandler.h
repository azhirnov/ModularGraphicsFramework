// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		friend class Module;

	// types
	private:
		template <typename T> using SP = SharedPointerType<T>;
		template <typename T> using WP = WeakPointerType<T>;

		using uint2				= GXMath::uint2;
		
		using ObjectPtr_t		= WP< StaticRefCountedObject >;
		using HandlerData_t		= GXMath::usize2;
		using Callback_t		= bool (*) (const ObjectPtr_t &, HandlerData_t &, VariantCRef);

		struct Handler final : CompileTime::FastCopyable
		{
			ObjectPtr_t		ptr;
			HandlerData_t	data	= {};
			Callback_t		func	= null;
		};

		using HandlersMap_t		= MultiMap< TypeId, Handler >;


	// variables
	private:
		HandlersMap_t		_handlers;


	// methods
	public:
		MessageHandler ();
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, const SP<Class> &obj, bool (Class2::*) (const Message<T> &), bool checked = true);
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, const SP<Class> &obj, bool (Class2::*) (const Message<T> &) const, bool checked = true);

		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, bool (Class2::*) (const Message<T> &), bool checked = true);
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, bool (Class2::*) (const Message<T> &) const, bool checked = true);

		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, Class *obj, bool (Class2::*) (const Message<T> &), bool checked = true);
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, Class *obj, bool (Class2::*) (const Message<T> &) const, bool checked = true);


		template <typename Class>
		void UnsubscribeAll (const SP<Class> &obj);

		template <typename Class>
		void UnsubscribeAll (const WP<Class> &obj);

		template <typename Class>
		void UnsubscribeAll (Class *obj);
		

		template <typename Class, typename Func>
		void Unsubscribe (const SP<Class> &obj, Func func);

		template <typename Class, typename Func>
		void Unsubscribe (const WP<Class> &obj, Func func);

		template <typename Class, typename Func>
		void Unsubscribe (Class *obj, Func func);


		template <typename Class>
		bool CopySubscriptions (const TypeIdList& validTypes, const SP<Class> &obj, const MessageHandler &other, ArrayCRef<TypeId> msgIds);

		void Clear ();
		
		bool Validate (const TypeIdList &typelist) const;
		bool Validate (const TypeIdList &msgTypes, const TypeIdList &eventTypes) const;

	private:
		bool _Subscribe2 (const TypeIdList& validTypes, TypeId id, Handler &&handler, bool checked);
		bool _CopySubscriptions (const TypeIdList& validTypes, const ObjectPtr_t &otherObj, const MessageHandler &other, ArrayCRef<TypeId> ids);
		void _UnsubscribeAll (const ObjectPtr_t &obj);
		void _Unsubscribe2 (const ObjectPtr_t &obj, const HandlerData_t &data);
		
		template <typename Msg, typename Class, typename Func>
		bool _Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, Func func, bool checked = true);
		
		template <typename Class, typename Func>
		void _Unsubscribe (const WP<Class> &obj, Func func);

		template <typename Class, typename Msg, typename Func>
		static bool _Call (const ObjectPtr_t &, HandlerData_t &, VariantCRef);
	};


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
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, Class *obj, bool (Class2::* func) (const Message<T> &) const, bool checked)
	{
		return Subscribe( validTypes, WP<Class>(obj), func, checked );
	}
	
	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, const SP<Class> &obj, bool (Class2::* func) (const Message<T> &), bool checked)
	{
		return Subscribe( validTypes, WP<Class>(obj), func, checked );
	}
	
	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, const SP<Class> &obj, bool (Class2::* func) (const Message<T> &) const, bool checked)
	{
		return Subscribe( validTypes, WP<Class>(obj), func, checked );
	}

	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, bool (Class2::* func) (const Message<T> &), bool checked)
	{
		STATIC_ASSERT(( CompileTime::IsSameTypes< Class, Class2 > or CompileTime::IsBaseOf< Class2, Class > ));
		return _Subscribe< Message<T> >( validTypes, obj, func, checked );
	}

	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, bool (Class2::* func) (const Message<T> &) const, bool checked)
	{
		STATIC_ASSERT(( CompileTime::IsSameTypes< Class, Class2 > or CompileTime::IsBaseOf< Class2, Class > ));
		return _Subscribe< Message<T> >( validTypes, obj, func, checked );
	}

/*
=================================================
	_Subscribe
=================================================
*/
	template <typename Msg, typename Class, typename Func>
	forceinline bool MessageHandler::_Subscribe (const TypeIdList& validTypes, const WP<Class> &obj, Func func, bool checked)
	{
		STATIC_ASSERT( sizeof(Handler::data) >= sizeof(func) );

		Handler		handler;
		UnsafeMem::MemCopy( OUT &handler.data, (void *)&func, BytesU::SizeOf(func) );
		handler.ptr		= obj;
		handler.func	= &_Call< Class, Msg, Func >;
		
		return _Subscribe2( validTypes, TypeIdOf< Msg >(), RVREF(handler), checked );
	}

/*
=================================================
	_Unsubscribe
=================================================
*/
	template <typename Class, typename Func>
	forceinline void MessageHandler::_Unsubscribe (const WP<Class> &obj, Func func)
	{
		STATIC_ASSERT( sizeof(HandlerData_t) >= sizeof(func) );

		HandlerData_t	data;
		UnsafeMem::MemCopy( OUT &data, (void *)&func, BytesU::SizeOf(func) );

		return _Unsubscribe2( obj, data );
	}
	
/*
=================================================
	Unsubscribe
=================================================
*/
	template <typename Class, typename Func>
	forceinline void MessageHandler::Unsubscribe (const SP<Class> &obj, Func func)
	{
		return _Unsubscribe( WP<Class>(obj), func );
	}

	template <typename Class, typename Func>
	forceinline void MessageHandler::Unsubscribe (const WP<Class> &obj, Func func)
	{
		return _Unsubscribe( obj, func );
	}

	template <typename Class, typename Func>
	forceinline void MessageHandler::Unsubscribe (Class *obj, Func func)
	{
		return _Unsubscribe( WP<Class>(obj), func );
	}

/*
=================================================
	CopySubscriptions
=================================================
*/
	template <typename Class>
	forceinline bool MessageHandler::CopySubscriptions (const TypeIdList& validTypes, const SP<Class> &obj, const MessageHandler &other, ArrayCRef<TypeId> msgIds)
	{
		return _CopySubscriptions( validTypes, ObjectPtr_t(obj), other, msgIds );
	}
	
/*
=================================================
	UnsubscribeAll
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
	template <typename Class, typename Msg, typename Func>
	inline bool MessageHandler::_Call (const ObjectPtr_t &ptr, HandlerData_t &data, VariantCRef msgRef)
	{
		auto	sp	= ptr.Lock();
		if ( not sp )
			return false;

		Class*	cl	= sp.ToPtr< Class >();
		auto	fn	= ReferenceCast< Func >(data);
		auto&	msg	= msgRef.Get< Msg >();
	
		++msg._numOfSends;

		return (cl->*fn)( msg );
	}

	
}	// Base
}	// Engine
