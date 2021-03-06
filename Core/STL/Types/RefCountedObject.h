// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Types/SharedPointer.h"
#include "Core/STL/Types/WeakPointer.h"
#include "Core/STL/Types/RefCounter.h"
#include "Core/STL/OS/OSLowLevel.h"
#include "Core/STL/Containers/Set.h"

namespace GX_STL
{
namespace GXTypes
{

#	define SHARED_POINTER( _name_ )		using _name_##Ptr = ::GX_STL::GXTypes::SharedPointerType< _name_ >;
#	define WEAK_POINTER( _name_ )		using _name_##WPtr = ::GX_STL::GXTypes::WeakPointerType< _name_ >;



	//
	// Reference Counted Debug Object
	//
	class DbgRefCountedObject : public CompileTime::TypeQualifier< CompileTime::ETypeQualifier::Def_Noncopyable >
	{
	protected:
		DbgRefCountedObject ()
		{
			_DebugAddRef();
		}
		
		DbgRefCountedObject (DbgRefCountedObject &&) = delete;
		DbgRefCountedObject (const DbgRefCountedObject &) = delete;

		virtual ~DbgRefCountedObject ()
		{
			_DebugRemoveRef();
		}


	private:
		forceinline void _DebugAddRef ()
		{
			DEBUG_ONLY(
				SCOPELOCK( _GetMutex() );
				_GetObjectRefs().Add( this );
			)
		}

		forceinline void _DebugRemoveRef ()
		{
			DEBUG_ONLY(
				SCOPELOCK( _GetMutex() );

				usize	idx;
				if ( _GetObjectRefs().FindIndex( this, idx ) ) {
					_GetObjectRefs().EraseByIndex( idx );
				}
			)
		}

		DEBUG_ONLY(
		static Set< DbgRefCountedObject *>& _GetObjectRefs () noexcept
		{
			static Set< DbgRefCountedObject *>		s_instance;
			return s_instance;
		}

		static Mutex& _GetMutex () noexcept
		{
			static Mutex	s_mutex;
			return s_mutex;
		})


	public:
		DEBUG_ONLY(
		static void s_ChenckNotReleasedObjects (uint refValue = 0)
		{
			SCOPELOCK( _GetMutex() );

			auto& objects = _GetObjectRefs();

			if ( objects.Count() != refValue )
			{
				GX_BREAK_POINT();
			}
		})
	};



	//
	// Reference Counted Object
	//

	template <typename CounterType = RefCounter2>
	class RefCountedObject : public DbgRefCountedObject
	{
		friend class  StaticRefCountedObject;
		friend struct SharedPointerStrategy< RefCountedObject< CounterType > >;
		friend struct WeakPointerStrategy< RefCountedObject< CounterType > >;

	// types
	protected:
		using Self			= RefCountedObject< CounterType >;
		using RefCounter_t	= CounterType;


	// variables
	private:
		RefCounter_t		_counter;


	// methods
	private:
		void operator = (Self &&) = delete;
		void operator = (const Self &) = delete;
		
		forceinline	RefCounter_t&	_GetCounter ()		{ return _counter; }

	protected:
		RefCountedObject ()
		{}
		
		explicit RefCountedObject (const Self &other) : _counter( other._counter )
		{}
		
		explicit RefCountedObject (Self &&other) : _counter( RVREF(other._counter) )
		{}

		virtual ~RefCountedObject ()
		{}

		virtual void _Release (INOUT RefCounter_t &)	{ delete this; }
	};
	
	
		
	template <>
	struct SharedPointerStrategy< RefCountedObject< RefCounter > >
	{
		using T = RefCountedObject< RefCounter >;

		forceinline static void IncRef (T *ptr)		{ ptr->_GetCounter().Inc(); }
		forceinline static void DecRef (T *ptr)		{ if ( ptr->_GetCounter().DecAndTest() ) { ptr->_Release( INOUT ptr->_GetCounter() ); } }
		forceinline static int  Count  (T *ptr)		{ return ptr->_GetCounter().Count(); }
	};
	
	template <>
	struct SharedPointerStrategy< RefCountedObject< RefCounter2 > >
	{
		using T = RefCountedObject< RefCounter2 >;

		forceinline static void IncRef (T *ptr)		{ ptr->_GetCounter().Inc(); }
		forceinline static void DecRef (T *ptr)		{ if ( ptr->_GetCounter().DecAndTest() ) { RefCounter2 rc{ ptr->_GetCounter() };  ptr->_Release( INOUT ptr->_GetCounter() );  rc.DecWeak(); } }
		forceinline static int  Count  (T *ptr)		{ return ptr->_GetCounter().Count(); }
	};

	template <>
	struct WeakPointerStrategy< RefCountedObject< RefCounter2 > >
	{
		using T			= RefCountedObject< RefCounter2 >;
		using Counter_t	= RefCounter2;

		forceinline static Counter_t& Create (T *ptr)		{ return ptr->_GetCounter(); }

		forceinline static void	IncRef (Counter_t &rc)		{ rc.IncWeak(); }
		forceinline static void	DecRef (Counter_t &rc)		{ rc.DecWeak(); }
		forceinline static int  Count  (Counter_t &rc)		{ return rc.CountWeak(); }
		forceinline static bool IncShared (Counter_t &rc)	{ return rc.TryInc(); }
	};


	template <typename T>
	using SharedPointerType = SharedPointer< T, RefCountedObject< RefCounter2 >, SharedPointerStrategy< RefCountedObject< RefCounter2 > > >;

	template <typename T>
	using WeakPointerType = WeakPointer< T, RefCountedObject< RefCounter2 >, WeakPointerStrategy< RefCountedObject< RefCounter2 > >, SharedPointerStrategy< RefCountedObject< RefCounter2 > > >;

	
	
	template <typename R, typename T>
	ND_ forceinline constexpr R  Cast (const SharedPointerType<T> &value)
	{
		return Cast<R>( value.RawPtr() );
	}
	
	template <typename R, typename T>
	ND_ forceinline constexpr R  Cast (const WeakPointerType<T> &value)
	{
		return Cast<R>( value.Lock() );
	}


	template <typename R, typename T>
	ND_ forceinline constexpr R  DynCast (const SharedPointerType<T> &value)
	{
		return DynCast<R>( value.RawPtr() );
	}

	template <typename R, typename T>
	ND_ forceinline constexpr R  DynCast (const WeakPointerType<T> &value)
	{
		return DynCast<R>( value.Lock() );
	}

}	// GXTypes
}	// GX_STL
