// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/BaseObject.h"

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

		enum class EPriority : uint
		{
			_Reserved	= 0,
			Highest,
			VeryHigh,
			High,
			AboveNormal,
			Normal,
			BelowNormal,
			Low,
			VeryLow,
			Lowest,
			Auto		= ~0u
		};

		using uint2				= GXMath::uint2;
		
		using Object_t			= StaticRefCountedObject;
		using ObjectPtr_t		= WP< Object_t >;
		using HandlerData_t		= GXMath::usize2;
		using Callback_t		= bool (*) (const ObjectPtr_t &, HandlerData_t &, VariantCRef);


		struct HandlerKey : CompileTime::FastCopyable
		{
			TypeId			id;
			EPriority		priority	= EPriority::Normal;

			HandlerKey () {}
			HandlerKey (TypeId id, EPriority prior) : id{id}, priority{prior} {}

			bool operator == (const HandlerKey &right) const	{ return id == right.id and priority == right.priority; }
			bool operator >  (const HandlerKey &right) const	{ return id != right.id ? id > right.id : priority > right.priority; }
		};

		struct HandlerSearch final
		{
			TypeId			id;
			
			explicit HandlerSearch (TypeId id) : id{id} {}

			bool operator == (const HandlerKey &right) const	{ return id == right.id; }
			bool operator >  (const HandlerKey &right) const	{ return id > right.id; }
		};

		struct Handler : CompileTime::FastCopyable
		{
			ObjectPtr_t		ptr;
			HandlerData_t	data	= {};
			Callback_t		func	= null;
		};

		using HandlersMap_t		= MultiMap< HandlerKey, Handler >;


	// variables
	private:
		HandlersMap_t		_handlers;


	// methods
	public:
		MessageHandler ();
		
		template <typename Class, typename Class2, typename MsgT>
		bool Subscribe (const SP<Class> &obj, bool (Class2::*) (const MsgT &), EPriority priority);
		
		template <typename Class, typename Class2, typename MsgT>
		bool Subscribe (const SP<Class> &obj, bool (Class2::*) (const MsgT &) const, EPriority priority);

		template <typename Class, typename Class2, typename MsgT>
		bool Subscribe (const WP<Class> &obj, bool (Class2::*) (const MsgT &), EPriority priority);
		
		template <typename Class, typename Class2, typename MsgT>
		bool Subscribe (const WP<Class> &obj, bool (Class2::*) (const MsgT &) const, EPriority priority);

		template <typename Class, typename Class2, typename MsgT>
		bool Subscribe (Class *obj, bool (Class2::*) (const MsgT &), EPriority priority);
		
		template <typename Class, typename Class2, typename MsgT>
		bool Subscribe (Class *obj, bool (Class2::*) (const MsgT &) const, EPriority priority);


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

		template <typename MsgT, typename Class>
		void Unsubscribe (Class *obj); 


		void UnsubscribeDeadHandlers ();


		template <typename Class, typename Class2>
		bool CopySubscriptions (const Class *selfObj, const SP<Class2> &otherObj, const MessageHandler &other,
								ArrayCRef<TypeId> msgIds, bool warnIfNotExist, EPriority priority);

		void Clear ();
		
		bool Validate (const TypeIdList &typelist) const;
		bool Validate (const TypeIdList &msgTypes, const TypeIdList &eventTypes) const;

	private:
		bool _Subscribe2 (TypeId id, EPriority priority, Handler &&handler);

		bool _CopySubscriptions (const Object_t *selfObj, const Object_t *otherObj, const MessageHandler &other,
								 ArrayCRef<TypeId> ids, bool warnIfNotExist, EPriority priority);

		void _UnsubscribeAll (const Object_t* obj);
		void _UnsubscribeAll (TypeId msgID);
		void _Unsubscribe2 (const Object_t* obj, const HandlerData_t &data);
		void _Unsubscribe2 (const Object_t* obj, TypeId msgID);

		template <typename Msg, typename Class, typename Func>
		bool _Subscribe (const WP<Class> &obj, Func func, EPriority priority);
		
		template <typename Func>
		void _Unsubscribe (const Object_t* obj, Func func);

		template <typename Class, typename Msg, typename Func>
		static bool _Call (const ObjectPtr_t &, HandlerData_t &, VariantCRef);
	};


/*
=================================================
	Subscribe
=================================================
*/
	template <typename Class, typename Class2, typename MsgT>
	forceinline bool MessageHandler::Subscribe (Class *obj, bool (Class2::* func) (const MsgT &), EPriority priority)
	{
		return Subscribe( WP<Class>(obj), func, priority );
	}
	
	template <typename Class, typename Class2, typename MsgT>
	forceinline bool MessageHandler::Subscribe (Class *obj, bool (Class2::* func) (const MsgT &) const, EPriority priority)
	{
		return Subscribe( WP<Class>(obj), func, priority );
	}
	
	template <typename Class, typename Class2, typename MsgT>
	forceinline bool MessageHandler::Subscribe (const SP<Class> &obj, bool (Class2::* func) (const MsgT &), EPriority priority)
	{
		return Subscribe( WP<Class>(obj), func, priority );
	}
	
	template <typename Class, typename Class2, typename MsgT>
	forceinline bool MessageHandler::Subscribe (const SP<Class> &obj, bool (Class2::* func) (const MsgT &) const, EPriority priority)
	{
		return Subscribe( WP<Class>(obj), func, priority );
	}

	template <typename Class, typename Class2, typename MsgT>
	forceinline bool MessageHandler::Subscribe (const WP<Class> &obj, bool (Class2::* func) (const MsgT &), EPriority priority)
	{
        STATIC_ASSERT( typename MsgT::__is_message(true) );
		STATIC_ASSERT(( CompileTime::IsSameTypes< Class, Class2 > or CompileTime::IsBaseOf< Class2, Class > ));
		return _Subscribe< MsgT >( obj, func, priority );
	}

	template <typename Class, typename Class2, typename MsgT>
	forceinline bool MessageHandler::Subscribe (const WP<Class> &obj, bool (Class2::* func) (const MsgT &) const, EPriority priority)
	{
        STATIC_ASSERT( typename MsgT::__is_message(true) );
		STATIC_ASSERT(( CompileTime::IsSameTypes< Class, Class2 > or CompileTime::IsBaseOf< Class2, Class > ));
		return _Subscribe< MsgT >( obj, func, priority );
	}

/*
=================================================
	_Subscribe
=================================================
*/
	template <typename Msg, typename Class, typename Func>
	forceinline bool MessageHandler::_Subscribe (const WP<Class> &obj, Func func, EPriority priority)
	{
		STATIC_ASSERT( sizeof(Handler::data) >= sizeof(func) );

		Handler		handler;
		UnsafeMem::MemCopy( OUT &handler.data, Cast<void *>(&func), BytesU::SizeOf(func) );
		handler.ptr		= obj;
		handler.func	= &_Call< Class, Msg, Func >;
		
		return _Subscribe2( TypeIdOf< Msg >(), priority, RVREF(handler) );
	}

/*
=================================================
	_Unsubscribe
=================================================
*/
	template <typename Func>
	forceinline void MessageHandler::_Unsubscribe (const Object_t *obj, Func func)
	{
		STATIC_ASSERT( sizeof(HandlerData_t) >= sizeof(func) );

		HandlerData_t	data;
		UnsafeMem::MemCopy( OUT &data, Cast<void *>(&func), BytesU::SizeOf(func) );

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
		return _Unsubscribe( obj.RawPtr(), func );
	}

	template <typename Class, typename Func>
	forceinline void MessageHandler::Unsubscribe (const WP<Class> &obj, Func func)
	{
		return _Unsubscribe( obj.RawPtr(), func );
	}

	template <typename Class, typename Func>
	forceinline void MessageHandler::Unsubscribe (Class *obj, Func func)
	{
		return _Unsubscribe( obj, func );
	}
	
/*
=================================================
	Unsubscribe
=================================================
*/
	template <typename MsgT, typename Class>
	forceinline void MessageHandler::Unsubscribe (Class *obj)
	{
		STATIC_ASSERT( MsgT::__is_message(true) );
		return _Unsubscribe2( obj, TypeIdOf<MsgT>() );
	}

/*
=================================================
	CopySubscriptions
=================================================
*/
	template <typename Class, typename Class2>
	forceinline bool MessageHandler::CopySubscriptions (const Class *selfObj, const SP<Class2> &otherObj, const MessageHandler &other,
														ArrayCRef<TypeId> msgIds, bool warnIfNotExist, EPriority priority)
	{
		return _CopySubscriptions( selfObj, otherObj.RawPtr(), other, msgIds, warnIfNotExist, priority );
	}
	
/*
=================================================
	UnsubscribeAll
=================================================
*/
	template <typename Class>
	forceinline void MessageHandler::UnsubscribeAll (const SP<Class> &obj)
	{
		return _UnsubscribeAll( obj.RawPtr() );
	}

	template <typename Class>
	forceinline void MessageHandler::UnsubscribeAll (const WP<Class> &obj)
	{
		return _UnsubscribeAll( obj.RawPtr() );
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

		return (cl->*fn)( msg );
	}

	
}	// Base
}	// Engine
