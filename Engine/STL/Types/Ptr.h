// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/NewTypeInfo.h"
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
		typedef T*			Value_t;
		typedef bool		_is_simple_ptr;

		
	// variables
	private:
		mutable T *		_ptr;


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

		forceinline Self & operator = (const Self &right)
		{
			_ptr = right._ptr;
			return *this;
		}


		ND_ forceinline explicit operator bool () const
		{
			return IsNotNull();
		}
		

		ND_ forceinline operator T * () const
		{
			return ptr();
		}
		

		ND_ forceinline explicit operator T *& ()
		{
			return _ptr;
		}


		ND_ forceinline T * operator -> () const
		{
			return ptr();
		}


		ND_ forceinline T & operator * () const
		{
			return *ptr();
		}


		ND_ forceinline T * const * operator & () const
		{
			return &_ptr;
		}


		ND_ forceinline T * ptr () const
		{
			ASSUME( _ptr != null );
			return RawPtr();
		}


		ND_ forceinline T * RawPtr () const
		{
			return _ptr;
		}


		ND_ forceinline bool IsNull () const
		{
			return ( _ptr == null );
		}

		ND_ forceinline bool IsNotNull () const
		{
			return ( _ptr != null );
		}


		template <typename T2>
		ND_ forceinline T2  To () const
		{
			STATIC_ASSERT( typename T2::_is_simple_ptr(true) );
			_CheckCast< TypeTraits::RemovePointer<typename T2::Value_t>, T >( _ptr );
			return T2( static_cast<typename T2::Value_t>(RawPtr()) );
		}

		template <typename R>
		ND_ forceinline R *  ToPtr () const
		{
			_CheckCast< R, Value_t >( _ptr );
			return ptr();
		}


		ND_ forceinline bool operator == (NullPtr_t) const
		{
			return IsNull();
		}
		
		
		_GX_DIM_CMP_OPERATORS_SELF( RawPtr() );
		_GX_DIM_CMP_OPERATORS_TYPE( RawPtr(), Value_t const, );
	};
	

	template <typename T>
	struct Hash< Ptr<T> > : public Hash< T* >
	{};

}	// GXTypes
}	// GX_STL
