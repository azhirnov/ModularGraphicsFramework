// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Ptr.h"
#include "Core/STL/Common/Cast.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Shared Pointer Strategy
	//

	template <typename T>
	struct SharedPointerStrategy
	{
		forceinline static void IncRef (T *ptr);
		forceinline static void DecRef (T *ptr);
		forceinline static int  Count (T *ptr);
	};



	//
	// Reference Counter
	//

	template <typename T, typename B, typename S = SharedPointerStrategy<B> >
	class SharedPointer final : public CompileTime::FastCopyable
	{
		template <typename T2, typename B2, typename S2>
		friend class SharedPointer;
		
		template <typename T2, typename B2, typename WS2, typename SPS2>
		friend class WeakPointer;

	// types
	public:
		using Self		= SharedPointer<T,B,S>;
		using Parent_t	= B;
		using Value_t	= T*;

	private:
		using _is_shared_pointer	= bool;


	// variables
	private:
		T *		_ptr	= null;
		

	// methods
	private:
		forceinline void _Inc ()
		{
			if ( _ptr != null )
				S::IncRef( Cast<B *>(_ptr) );
		}

		forceinline void _Dec ()
		{
			if ( _ptr != null )
				S::DecRef( Cast<B *>(_ptr) );
		}

		template <typename T2>
		forceinline bool _CmpPtr (const T2 *p) const
		{
			return Cast<const void *>(_ptr) == Cast<const void *>(p);
		}

		template <typename T2, typename B2, typename S2>
		forceinline bool _CmpPtr (const SharedPointer<T2,B2,S2> &sRC) const
		{
			return _CmpPtr( sRC.RawPtr() );
		}

		template <typename ToType, typename FromType>
		static void _CheckCast (FromType *p)
		{
			STATIC_ASSERT(	sizeof(FromType) > 0 and sizeof(ToType) > 0 );
			STATIC_ASSERT(( CompileTime::IsBaseOf< ToType, FromType > or
							CompileTime::IsBaseOf< FromType, ToType > or
						    CompileTime::IsSameTypes< ToType, FromType > ));

			ASSERT( CompileTime::TypeDescriptor::CanCastTo< ToType >( p ) );
		}
		
		template <typename WS>
		SharedPointer (const WeakPointer<T,B,WS,S> &);


	public:
		forceinline SharedPointer (GX_DEFCTOR)
		{
		}

		forceinline SharedPointer (NullPtr_t)
		{}

		forceinline SharedPointer (T *ptr) : _ptr(ptr)
		{
			STATIC_ASSERT(( CompileTime::IsBaseOf< B, T > ));
			_Inc();
		}

		forceinline SharedPointer (Ptr<T> ptr) : SharedPointer( ptr.RawPtr() )
		{}

		forceinline SharedPointer (Self &&other) : _ptr(other._ptr)
		{
			other._ptr = null;
		}

		forceinline SharedPointer (const Self &other) : _ptr(other._ptr)
		{
			_Inc();
		}


		template <typename T2>
		forceinline SharedPointer (const SharedPointer<T2,B,S> &right)
		{
			_CheckCast<T>( right.RawPtr() );

			_ptr = Cast<T *>( right.RawPtr() );
			_Inc();
		}


		forceinline ~SharedPointer ()
		{
			_Dec();
		}


		template <typename T2>
		forceinline Self & operator = (const SharedPointer<T2,B,S> &right)
		{
			if ( _CmpPtr( right ) )
				return *this;

			_CheckCast<T>( right.RawPtr() );

			_Dec();
			_ptr = Cast<T *>( right.RawPtr() );
			_Inc();
			return *this;
		}

		
		forceinline Self & operator = (const Self &right)
		{
			return Self::operator = <T>( right );
		}

		forceinline Self & operator = (Self &&right)
		{
			_Dec();
			_ptr = right._ptr;
			right._ptr = null;
			return *this;
		}

		template <typename T2>
		forceinline Self & operator = (T2 *right)
		{
			if ( _CmpPtr( right ) )
				return *this;
			
			_CheckCast<T>( right );

			_Dec();
			_ptr = Cast<T *>( right );
			_Inc();
			return *this;
		}
		

		template <typename T2>
		forceinline Self & operator = (Ptr<T2> right)
		{
			return Self::operator= ( right.RawPtr() );
		}

		
		ND_ forceinline explicit operator bool () const
		{
			return IsNotNull();
		}


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
			ASSUME( _ptr != null );
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


		template <typename T2>
		ND_ forceinline const T2 To () const
		{
			STATIC_ASSERT( typename T2::_is_shared_pointer(true) );
			return T2( *this );
		}

		template <typename T2>
		ND_ forceinline Ptr<T2> ToPtr () const
		{
			_CheckCast<T2>( _ptr );
			return Cast< T2 *>( RawPtr() );
		}

		forceinline int GetRefCount () const
		{
			STATIC_ASSERT( sizeof(T) > 0 and sizeof(B) > 0 );

			if ( _ptr != null )	return S::Count( Cast<B *>(_ptr) );
			else				{ WARNING(""); return -1; }
		}

		
		_GX_DIM_CMP_OPERATORS_SELF( RawPtr() );
		_GX_DIM_CMP_OPERATORS_TYPE( RawPtr(), Value_t const, );
	};
	

}	// GXTypes
}	// GX_STL
