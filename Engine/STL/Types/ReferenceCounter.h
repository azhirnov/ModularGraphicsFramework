// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Ptr.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Reference Counter Strategy
	//

	template <typename T>
	struct SharedPointerStrategy
	{
		forceinline static void IncRef (T *ptr)		{}
		forceinline static void DecRef (T *ptr)		{}
		forceinline static int  Count (T *ptr)		{ return 0; }
	};



	//
	// Reference Counter
	//

	template <typename T, typename B, typename S = SharedPointerStrategy<B> >
	class ReferenceCounter : public CompileTime::FastCopyable
	{
		template <typename T2, typename B2, typename S2>
		friend class ReferenceCounter;

	// types
	public:
		typedef ReferenceCounter<T,B,S>		Self;
		typedef T							Value_t;
		typedef B							parent_t;

	private:
		typedef bool						_is_ref_counter;


	// variables
	private:
		T *		_ptr;
		

	// methods
	private:
		forceinline void _Inc ()
		{
			if ( _ptr != null )
				S::IncRef( (B *)_ptr );
		}

		forceinline void _Dec ()
		{
			if ( _ptr != null )
				S::DecRef( (B *)_ptr );
		}

		template <typename T2>
		forceinline bool _CmpPtr (const T2 *p) const
		{
			return ((const void *) _ptr) == ((const void *) p);
		}

		template <typename T2, typename B2, typename S2>
		forceinline bool _CmpPtr (const ReferenceCounter<T2,B2,S2> &sRC) const
		{
			return _CmpPtr( sRC.RawPtr() );
		}

		template <typename ToType, typename FromType>
		static void _CheckCast (FromType *p)
		{
			STATIC_ASSERT(( CompileTime::IsBaseOf< ToType, FromType > or
							CompileTime::IsBaseOf< FromType, ToType > ));

			ASSERT( CompileTime::TypeDescriptor::CanCastTo< ToType >( p ) );
		}


	public:
		ReferenceCounter (GX_DEFCTOR) : _ptr(null)
		{}


		ReferenceCounter (T *ptr) : _ptr(ptr)
		{
			STATIC_ASSERT(( CompileTime::IsBaseOf< B, T > ));

			_Inc();
		}


		ReferenceCounter (Ptr<T> ptr) :
			ReferenceCounter( ptr.RawPtr() )
		{}


		ReferenceCounter (Self &&sRC) : _ptr(sRC._ptr)
		{
			sRC._ptr = null;
		}


		ReferenceCounter (const Self &sRC) : _ptr(sRC._ptr)
		{
			_Inc();
		}


		template <typename T2>
		ReferenceCounter (const ReferenceCounter<T2,B,S> &right) : _ptr(null)
		{
			_CheckCast<T>( right.RawPtr() );

			_ptr = Cast<T *>( right.RawPtr() );
			_Inc();
		}


		~ReferenceCounter ()
		{
			_Dec();
		}


		template <typename T2>
		forceinline Self & operator = (const ReferenceCounter<T2,B,S> &right)
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
			if ( _CmpPtr( right ) )
				return *this;

			_Dec();
			_ptr = right._ptr;
			_Inc();
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
		forceinline Self & operator = (Ptr<T> right)
		{
			return Self::operator= ( right.RawPtr() );
		}

		
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


		forceinline T * ptr () const
		{
			ASSUME( _ptr != null );
			return RawPtr();
		}

		forceinline T * RawPtr () const
		{
			return const_cast< T* >( _ptr );
		}


		forceinline bool Empty () const
		{
			return _ptr == null;
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
		forceinline const T2 To () const
		{
			typedef typename T2::Value_t	type_t;
			STATIC_ASSERT( typename T2::_is_ref_counter(true) );

			_CheckCast< type_t >( _ptr );
			return T2( static_cast< type_t *>( RawPtr() ) );
		}

		template <typename T2>
		forceinline Ptr<T2> ToPtr () const
		{
			_CheckCast<T2>( _ptr );
			return static_cast< T2 *>( RawPtr() );
		}

		forceinline int GetRefCount () const
		{
			if ( _ptr != null )	return S::Count( (B *)_ptr );
			else				{ WARNING(""); return -1; }
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
		

		forceinline bool operator > (const Self &right) const
		{
			return _ptr > right._ptr;
		}
		
		forceinline bool operator < (const Self &right) const
		{
			return _ptr < right._ptr;
		}
	};
	

}	// GXTypes
}	// GX_STL
