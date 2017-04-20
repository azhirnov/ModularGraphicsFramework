// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/CompileTime/PlainOldDataType.h"
#include "Engine/STL/Algorithms/Hash.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Simple Ptr
	//

	template <typename T>
	struct Ptr : public CompileTime::FastCopyable
	{
	// types
	public:
		typedef Ptr<T>		Self;
		typedef T			value_t;
		typedef T *			value_ptr_t;
		typedef bool		_is_simple_ptr;

		
	// variables
	private:
		T *	_ptr;


	// methods
	private:
		template <typename ToType, typename FromType>
		static void _CheckCast (FromType *p)
		{
			ASSERT( CompileTime::TypeDescriptor::CanCastTo< ToType >( p ) );
		}


	public:
		Ptr (GX_DEFCTOR): _ptr(null) {}
		
		Ptr (T *p): _ptr(p) {}
		

		forceinline Self & operator = (T *right)
		{
			_ptr = right;
			return *this;
		}

		forceinline Self & operator = (Self &right)
		{
			_ptr = right._ptr;
			return *this;
		}


		forceinline operator bool () const
		{
			return IsNotNull();
		}
		

		forceinline operator T * () const
		{
			return _ptr;
		}
		

		forceinline operator T *& ()
		{
			return _ptr;
		}


		forceinline T * operator -> () const
		{
			ASSUME( _ptr != null );
			return _ptr;
		}


		forceinline T & operator * () const
		{
			ASSUME( _ptr != null );
			return *_ptr;
		}

		forceinline T ** operator & ()
		{
			return &_ptr;
		}


		forceinline T * const * operator & () const
		{
			return &_ptr;
		}


		forceinline T *& ref ()
		{
			return _ptr;
		}

		forceinline T* const & ref () const
		{
			return _ptr;
		}


		forceinline T * ptr ()
		{
			ASSERT( _ptr != null );
			return _ptr;
		}

		forceinline const T * const ptr () const
		{
			ASSERT( _ptr != null );
			return _ptr;
		}


		forceinline bool IsNull () const
		{
			return ( _ptr == null );
		}

		forceinline bool IsNotNull () const
		{
			return ( _ptr != null );
		}


		template <typename T2>
		forceinline T2 To ()
		{
			STATIC_ASSERT( typename T2::_is_simple_ptr(true) );
			_CheckCast< typename T2::value_t, value_t >( _ptr );
			return T2( (typename T2::value_t *) _ptr );
		}
		

		forceinline bool operator > (const Self &right) const
		{
			return _ptr > right._ptr;
		}
		
		forceinline bool operator < (const Self &right) const
		{
			return _ptr < right._ptr;
		}


		template <typename R>
		forceinline R *			ToPtr ()	const
		{
			_CheckCast< R, value_t >( _ptr );
			return (R *)ptr();
		}
		
		template <typename R>
		forceinline R const *	ToCPtr ()	const
		{
			_CheckCast< R const, value_t >( _ptr );
			return (R const *)ptr();
		}
	};
	

	template <typename T>
	struct Hash< Ptr<T> > : public Hash< T* >
	{};

}	// GXTypes
}	// GX_STL
