// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"

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
		forceinline static void Delete(T *ptr)	{ delete ptr; }
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
		typedef T						value_t;
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
				S::Delete( (B*)_ptr );
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


		~UniquePtr()
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
			if ( (void*)_ptr == (void*)right._ptr ) {
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
		

		forceinline explicit operator bool () const
		{
			return IsNotNull();
		}


		forceinline T * operator -> () const
		{
			return ptr();
		}


		forceinline T & operator * () const
		{
			return *ptr();
		}


		forceinline T * ptr() const
		{
			ASSUME( _ptr != null );
			return const_cast< T* >( _ptr );
		}


		forceinline bool Empty() const
		{
			return ( _ptr == null );
		}


		forceinline bool IsNull() const
		{
			return ( _ptr == null );
		}


		forceinline bool IsNotNull() const
		{
			return not IsNull();
		}


		forceinline void Reset()
		{
			_ptr = null;
		}
		

		forceinline bool operator == (const T *right) const
		{
			return _ptr == right;
		}

		
		forceinline bool operator == (const Self &right) const
		{
			return _ptr == right._ptr;
		}

		
		forceinline bool operator != (const T *right) const
		{
			return _ptr != right;
		}
		

		forceinline bool operator != (const Self &right) const
		{
			return _ptr != right._ptr;
		}

		
		forceinline bool operator ! () const
		{
			return _ptr != null;
		}


		template <typename T2>
		forceinline static Self CreateCopy (const T2& value)
		{
			return Self( new T( value ) );
		}
	};

	
	template <typename T, typename B, typename S>
	struct Hash< UniquePtr< T, B, S > > :
		private Hash< typename UniquePtr<T,B,S>::value_t const * >
	{
		typedef UniquePtr< T, B, S >						key_t;
		typedef Hash< typename key_t::value_t const * >		base_t;
		typedef typename base_t::result_t					result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.ptr() );
		}
	};
	

	template <typename T, typename ...Args>
	inline UniquePtr<T>  MakeUnique (Args&& ...args)
	{
		return UniquePtr<T>(new T( FW<Args>(args)... ));
	}


}	// GXTypes
}	// GX_STL
