// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Types/SharedPointer.h"
#include "Engine/STL/Types/WeakPointer.h"
#include "Engine/STL/Types/RefCounter.h"
#include "Engine/STL/OS/OSLowLevel.h"
#include "Engine/STL/Containers/Set.h"

namespace GX_STL
{
namespace GXTypes
{

#	define SHARED_POINTER( _name_ )		using _name_##Ptr = ::GX_STL::GXTypes::SharedPointerType< _name_ >;
#	define WEAK_POINTER( _name_ )		using _name_##WPtr = ::GX_STL::GXTypes::WeakPointerType< _name_ >;



	//
	// Reference Counted Debug Object
	//
	class _STL_EXPORT_ DbgRefCountedObject : public CompileTime::TypeQualifier< CompileTime::ETypeQualifier::Def_Noncopyable >
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

		static OS::Mutex& _GetMutex () noexcept
		{
			static OS::Mutex	s_mutex;
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
	class _STL_EXPORT_ RefCountedObject : public DbgRefCountedObject
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

		virtual void _Release (RefCounter_t &)	{ delete this; }
	};
	
	
		
	template <>
	struct SharedPointerStrategy< RefCountedObject< RefCounter > >
	{
		using T = RefCountedObject< RefCounter >;

		forceinline static void IncRef (T *ptr)		{ ptr->_GetCounter().Inc(); }
		forceinline static void DecRef (T *ptr)		{ if ( ptr->_GetCounter().Dec() == 0 ) { ptr->_Release( ptr->_GetCounter() ); } }
		forceinline static int  Count  (T *ptr)		{ return ptr->_GetCounter().Count(); }
	};
	
	template <>
	struct SharedPointerStrategy< RefCountedObject< RefCounter2 > >
	{
		using T = RefCountedObject< RefCounter2 >;

		forceinline static void IncRef (T *ptr)		{ ptr->_GetCounter().Inc(); }
		forceinline static void DecRef (T *ptr)		{ if ( ptr->_GetCounter().Dec() == 0 ) { RefCounter2 rc{ ptr->_GetCounter() };  ptr->_Release( ptr->_GetCounter() );  rc.DecWeak(); } }
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
		forceinline static void DecShared (Counter_t &rc)	{ rc.Dec(); }
	};

	template <typename T>
	using SharedPointerType = SharedPointer< T, RefCountedObject< RefCounter2 >, SharedPointerStrategy< RefCountedObject< RefCounter2 > > >;

	template <typename T>
	using WeakPointerType = WeakPointer< T, RefCountedObject< RefCounter2 >, WeakPointerStrategy< RefCountedObject< RefCounter2 > >, SharedPointerStrategy< RefCountedObject< RefCounter2 > > >;

}	// GXTypes
}	// GX_STL
