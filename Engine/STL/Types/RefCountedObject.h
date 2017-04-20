// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "ReferenceCounter.h"
#include "Engine/STL/ThreadSafe/Atomic.h"
#include "Engine/STL/OS/OSLowLevel.h"
#include "Engine/STL/Containers/Set.h"

namespace GX_STL
{
namespace GXTypes
{

#	define SHARED_POINTER_TYPE( _name_ ) \
		::GX_STL::GXTypes::ReferenceCounter< _name_, \
			::GX_STL::GXTypes::RefCountedObject, \
			::GX_STL::GXTypes::SharedPointerStrategy< ::GX_STL::GXTypes::RefCountedObject > >

#	define SHARED_POINTER( _name_ ) \
		typedef SHARED_POINTER_TYPE( _name_ )	_name_##Ptr;



	//
	// Reference Counted Object
	//

	class RefCountedObject : public CompileTime::TypeQualifier< CompileTime::ETypeQualifier::Def_Noncopyable >
	{
		friend class  StaticRefCountedObject;
		friend struct SharedPointerStrategy< RefCountedObject >;

	// types
	private:
		typedef RefCountedObject	Self;


	// variables
	private:
		Atomic< int >		_atomicCounter;

		DEBUG_ONLY(
			static Set< Self *>	_objectRefs;
			static OS::Mutex	_mutex;
		)


	// methods
	private:
		void operator = (Self &&) = delete;
		void operator = (const Self &) = delete;

	private:
		forceinline void _DebugAddRef ()
		{
			DEBUG_ONLY(
				SCOPELOCK( _mutex );
				_objectRefs.Add( this );
			)
		}

		forceinline void _DebugRemoveRef ()
		{
			DEBUG_ONLY(
				SCOPELOCK( _mutex );

				usize	idx;
				if ( _objectRefs.FindIndex( this, idx ) ) {
					_objectRefs.EraseFromIndex( idx );
				}
			)
		}

	protected:
		RefCountedObject (): _atomicCounter(0)
		{
			_DebugAddRef();
		}
		
		explicit
		RefCountedObject (const Self &) : _atomicCounter(0)
		{
			_DebugAddRef();
		}
		
		RefCountedObject (Self &&other) : _atomicCounter( other._atomicCounter )
		{
			other._atomicCounter = 0;
		}

		virtual ~RefCountedObject ()
		{
			_DebugRemoveRef();
		}

		forceinline  void _AddRef ()				{ _atomicCounter.Inc(); }
		forceinline  uint _GetRefCount ()	const	{ return _atomicCounter; }
		forceinline  void _ReleaseRef ()			{ ASSERT( int(_atomicCounter) > 0 );  if ( _atomicCounter.Dec() == 0 ) _Release(); }
		virtual		 void _Release ()				{ delete this; }
		

	public:
		DEBUG_ONLY(
		static void s_ChenckNotReleasedObjects (uint refValue = 0)
		{
			SCOPELOCK( _mutex );

			if ( _objectRefs.Count() != refValue )
			{
				GX_BREAK_POINT();
			}
		})
	};
	
	
		
	template <>
	struct SharedPointerStrategy< RefCountedObject >
	{
		forceinline static void IncRef (RefCountedObject *ptr)	{ ptr->_AddRef(); }
		forceinline static void DecRef (RefCountedObject *ptr)	{ ptr->_ReleaseRef(); }
		forceinline static int  Count  (RefCountedObject *ptr)	{ return ptr->_GetRefCount(); }
	};


}	// GXTypes
}	// GX_STL
