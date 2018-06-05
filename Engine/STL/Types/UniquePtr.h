// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/Algorithms/Swap.h"
#include "Engine/STL/Types/Ptr.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Unique Ptr Strategy
	//

	template <typename T>
	struct TUniquePtrStrategy
	{
		forceinline static void Delete (T *ptr)		{ STATIC_ASSERT( sizeof(T) > 0 );  delete ptr; }
	};



	//
	// Unique Ptr
	//

	template <typename T, typename B = T, typename S = TUniquePtrStrategy<B> >
	struct UniquePtr : public CompileTime::FastCopyable
	{
	// types
	public:
		typedef UniquePtr< T, B, S >	Self;
		typedef T						Value_t;
		typedef bool					_is_uniqueptr;
		

	// variables
	private:
		T *		_ptr;


	// methods
	private:
		forceinline void _Delete()
		{
			if ( _ptr != null )
			{
				S::Delete( Cast<B*>(_ptr) );
				_ptr = null;
			}
		}


	public:
		UniquePtr (GX_DEFCTOR) : _ptr(null)
		{}


		explicit UniquePtr (T *p) : _ptr(p)
		{}


		explicit UniquePtr (Ptr<T> p) : _ptr( p.RawPtr() )
		{}
		

		UniquePtr (const Self &) = delete;


		UniquePtr (Self &&other) : _ptr(other._ptr)
		{
			other._ptr = null;
		}


		~UniquePtr ()
		{
			_Delete();
		}


		template <typename T2>
		forceinline Self & operator = (T2 *right)
		{
			if ( _ptr == right ) {
				WARNING( "" );
				return *this;
			}

			_Delete();
			_ptr = right;
			return *this;
		}


		forceinline Self & operator = (T *right)
		{
			if ( right != null and _ptr == right ) {
				WARNING( "" );
				return *this;
			}

			_Delete();
			_ptr = right;
			return *this;
		}


		forceinline Self & operator = (Ptr<T> right)
		{
			return Self::operator= ( right.RawPtr() );
		}


		template <typename T2>
		forceinline Self & operator = (UniquePtr<T2,B,S> &&right)
		{
			if ( Cast<void*>(_ptr) == Cast<void*>(right._ptr) ) {
				WARNING( "" );
				return *this;
			}

			_Delete();
			_ptr = right._ptr;
			right._ptr = null;
			return *this;
		}


		forceinline Self & operator = (Self &&right)
		{
			if ( _ptr == right._ptr ) {
				WARNING( "" );
				return *this;
			}

			_Delete();
			_ptr = right._ptr;
			right._ptr = null;
			return *this;
		}
		

		Self& operator = (const Self &right) = delete;


		ND_ forceinline T * operator -> () const
		{
			return ptr();
		}


		ND_ forceinline T & operator * () const
		{
			return *ptr();
		}


		ND_ forceinline T * ptr () const
		{
			ASSUME( IsNotNull() );
			return RawPtr();
		}


		ND_ forceinline T * RawPtr () const
		{
			return const_cast< T* >( _ptr );
		}


		ND_ forceinline bool IsNull () const
		{
			return ( _ptr == null );
		}


		ND_ forceinline bool IsNotNull () const
		{
			return ( _ptr != null );
		}


		ND_ inline T* Release ()
		{
			T*	res = _ptr;
			_ptr = null;
			return res;
		}
		

		ND_ forceinline explicit operator bool () const
		{
			return IsNotNull();
		}

		
		ND_ forceinline bool operator == (const Self &right) const
		{
			return _ptr == right._ptr;
		}

		
		ND_ forceinline bool operator != (const T *right) const
		{
			return _ptr != right;
		}
		

		ND_ forceinline bool operator != (const Self &right) const
		{
			return _ptr != right._ptr;
		}


		template <typename T2>
		ND_ forceinline static Self CreateCopy (const T2& value)
		{
			return Self( new T( value ) );
		}


		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			return GXTypes::SwapValues( INOUT left._ptr, INOUT right._ptr );
		}
	};

	
	template <typename T, typename B, typename S>
	struct Hash< UniquePtr< T, B, S > >
	{
		ND_ HashResult  operator () (const UniquePtr< T, B, S > &x) const noexcept
		{
			return HashOf( Cast<T const *>( x.ptr() ) );
		}
	};
	

	template <typename T, typename ...Args>
	ND_ forceinline UniquePtr<T>  MakeUnique (Args&& ...args)
	{
		return UniquePtr<T>(new T( FW<Args>(args)... ));
	}


}	// GXTypes
}	// GX_STL
