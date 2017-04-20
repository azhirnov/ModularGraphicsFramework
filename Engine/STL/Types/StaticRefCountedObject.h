// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	StaticRefCountedObject class may be created as static or
	dynamic object and cast to shared pointer always available and
	safe in most cases.
*/

#pragma once

#include "RefCountedObject.h"

namespace GX_STL
{
namespace GXTypes
{

	template <typename T, typename ...Args>
	forceinline SHARED_POINTER_TYPE(T) New (Args ...args) noexcept;


	
	//
	// Static Reference Counted Object
	//

	class StaticRefCountedObject : protected RefCountedObject
	{
	// types
	private:
		typedef StaticRefCountedObject			Self;
		typedef SHARED_POINTER_TYPE( Self )		SelfPtr_t;


	// variables
	private:
		bool	_isDynamicObj;


	// methods
	public:
		StaticRefCountedObject () : _isDynamicObj(true)
		{
			_SetDynamicObj( false );
		}
		

		explicit
		StaticRefCountedObject (const Self &other) :
			RefCountedObject(other),
			_isDynamicObj(true)
		{
			_SetDynamicObj( false );
		}


		StaticRefCountedObject (Self &&other) :
			RefCountedObject( other ),
			_isDynamicObj(true)
		{
			_SetDynamicObj( false );
		}


		~StaticRefCountedObject ()
		{
			ASSERT( (_GetRefCount() != 0) == not _isDynamicObj );
		}

		
		// use New<T>( T(args) ) for private/protected constructors
		template <typename T, typename ...Args>
		friend forceinline SHARED_POINTER_TYPE(T)  New (Args ...args) noexcept
		{
			SHARED_POINTER_TYPE( T ) tmp = new T( FW<Args>(args)... );
			tmp->_SetDynamicObj( true );
			return tmp;
		}


	private:
		// use 'New' function
		forceinline void * operator new (usize size)
		{
			return ::operator new( size );
		}


		// can't use delete for shared pointer
		forceinline void operator delete (void* p)
		{
			return ::operator delete( p );
		}


		forceinline void _SetDynamicObj (bool isDynamic)
		{
			if ( _isDynamicObj == isDynamic )
				return;

			_isDynamicObj = isDynamic;

			if ( not _isDynamicObj )
			{
				_AddRef();
				_DebugRemoveRef();
			}
			else
			{
				_ReleaseRef();
				_DebugAddRef();
			}
		}


		// RefCountedObject //
		virtual void _Release () override final
		{
			if ( _isDynamicObj )
			{
				delete this;
			}
		}
	};


}	// GXTypes
}	// GX_STL
