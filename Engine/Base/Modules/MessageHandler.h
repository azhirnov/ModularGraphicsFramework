// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Message.h"
#include "Engine/Base/Common/TypeIdList.h"

namespace Engine
{
namespace Base
{

	//
	// Message Handler
	//

	class _ENGINE_BASE_EXPORT_ MessageHandler final
	{
	// types
	private:
		using uint2				= GXMath::uint2;
		
		template <typename T> using RC = SharedPointerType<T>;

		using ObjectPtr_t		= RC< StaticRefCountedObject >;
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

		mutable OS::Mutex	_lock;


	// methods
	public:
		MessageHandler ();
		
		template <typename T>
		bool Send (const Message<T> &msg);
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, const RC<Class> &obj, bool (Class2::*) (const Message<T> &));
		
		template <typename Class, typename Class2, typename T>
		bool Subscribe (const TypeIdList& validTypes, Class *obj, bool (Class2::*) (const Message<T> &));

		template <typename MsgList, typename Class>
		bool CopySubscriptions (const TypeIdList& validTypes, const RC<Class> &obj, const MessageHandler &other);
		
		void UnsubscribeAll (const ObjectPtr_t &unit);

		void Clear ();
		
		bool Validate (const TypeIdList &typelist) const;
		bool Validate (const TypeIdList &msgTypes, const TypeIdList &eventTypes) const;

	private:
		bool _Send (VariantCRef);
		bool _Subscribe (const TypeIdList& validTypes, TypeId id, Handler &&handler);
		bool _CopySubscriptions (const TypeIdList& validTypes, const ObjectPtr_t &obj, const MessageHandler &other, ArrayCRef<TypeId> ids);

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
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, const RC<Class> &obj, bool (Class2::* func) (const Message<T> &))
	{
		return Subscribe( validTypes, obj.ptr(), func );
	}

/*
=================================================
	Subscribe
=================================================
*/
	template <typename Class, typename Class2, typename T>
	forceinline bool MessageHandler::Subscribe (const TypeIdList& validTypes, Class *obj, bool (Class2::* func) (const Message<T> &))
	{
		STATIC_ASSERT( sizeof(Handler::data) >= sizeof(func) );
		STATIC_ASSERT(( CompileTime::IsSameTypes< Class, Class2 > or CompileTime::IsBaseOf< Class2, Class > ));

		Handler		handler;
		UnsafeMem::MemCopy( handler.data, (void *)&func, SizeOf(func) );
		handler.ptr		= RC<Class>( obj );
		handler.func	= &_Call< Class, T >;
		
		return _Subscribe( validTypes, TypeIdOf< Message<T> >(), RVREF(handler) );
	}
	
/*
=================================================
	CopySubscriptions
=================================================
*/
	template <typename MsgList, typename Class>
	forceinline bool MessageHandler::CopySubscriptions (const TypeIdList& validTypes, const RC<Class> &obj, const MessageHandler &other)
	{
		return _CopySubscriptions( validTypes, ObjectPtr_t(obj), other, TypeIdList(UninitializedT<MsgList>{}) );
	}

/*
=================================================
	_Call
=================================================
*/
	template <typename Class, typename T>
	inline bool MessageHandler::_Call (const ObjectPtr_t &ptr, HandlerData_t &data, VariantCRef msgRef)
	{
		Class*	cl	= ptr.ToPtr< Class >();
		auto	fn	= ReferenceCast< bool (Class::*) (const Message<T>&) >(data);
		auto&	msg	= msgRef.Get< Message<T> >();
	
		++msg._numOfSends;

		return (cl->*fn)( msg );
	}

	
}	// Base
}	// Engine
