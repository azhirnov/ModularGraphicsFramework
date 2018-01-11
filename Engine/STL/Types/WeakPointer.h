// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "SharedPointer.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Weak Pointer Strategy
	//
	template <typename T>
	struct WeakPointerStrategy
	{
		using Counter_t = typename T::RefCounter_t;
		
		static Counter_t& Create (T *ptr);
		static void	IncRef (Counter_t &rc);
		static void	DecRef (Counter_t &rc);
		static int  Count  (Counter_t &rc);
		static bool IncShared (Counter_t &rc);
		static void DecShared (Counter_t &rc);
	};



	//
	// Weak Pointer
	//

	template <typename T, typename B = T, typename WS = WeakPointerStrategy<B>, typename SPS = SharedPointerStrategy<B> >
	class WeakPointer final : public CompileTime::FastCopyable
	{
		template <typename T2, typename B2, typename WS2, typename SPS2>
		friend class WeakPointer;

	// types
	public:
		using Self			= WeakPointer< T, B, WS, SPS >;
		using Value_t		= T*;

	private:
		using RC			= SharedPointer< T, B, SPS >;
		using Counter_t		= typename WS::Counter_t;
		using Strategy_t	= WS;
		using _is_weak_ptr	= bool;


	// variables
	private:
		T *					_ptr		= null;
		mutable Counter_t	_counter	= null;


	// methods
	private:
		forceinline void _Inc ()
		{
			if ( _ptr )
				Strategy_t::IncRef( _counter );
		}

		forceinline void _Dec ()
		{
			if ( _ptr )
				Strategy_t::DecRef( _counter );
		}
		
		template <typename T2>
		forceinline bool _CmpPtr (const T2 *p) const
		{
			return ((const void *) _ptr) == ((const void *) p);
		}

		template <typename ToType, typename FromType>
		static constexpr void _CheckCast (FromType *)
		{
			STATIC_ASSERT(( CompileTime::IsBaseOf< ToType, FromType > or
							CompileTime::IsBaseOf< FromType, ToType > or
						    CompileTime::IsSameTypes< ToType, FromType > ));
		}


	public:
		forceinline WeakPointer (GX_DEFCTOR)
		{
			STATIC_ASSERT(( CompileTime::IsBaseOf< B, T > ));
		}

		forceinline WeakPointer (NullPtr_t)
		{}

		template <typename T2>
		forceinline WeakPointer (T2 *ptr)
		{
			_CheckCast<T>( ptr );
			_ptr = PointerCast<T>( ptr );

			if ( _ptr ) {
				_counter = Strategy_t::Create( PointerCast<B>( _ptr ) );
				_Inc();
			}
		}

		template <typename T2>
		forceinline WeakPointer (const SharedPointer<T2,B,SPS> &other) : WeakPointer( other.RawPtr() )
		{}

		template <typename T2>
		forceinline WeakPointer (Ptr<T2> other) : WeakPointer( other.RawPtr() )
		{}

		template <typename T2>
		forceinline WeakPointer (const WeakPointer<T2,B,WS,SPS> &other) : _counter(other._counter)
		{
			_CheckCast<T>( other._ptr );

			_ptr = PointerCast<T>(other._ptr);
			_Inc();
		}

		forceinline WeakPointer (const Self &other) : _ptr(other._ptr), _counter(other._counter)
		{
			_Inc();
		}

		forceinline WeakPointer (Self &&other) : _ptr(other._ptr), _counter(other._counter)
		{
			other._ptr = null;
		}

		forceinline ~WeakPointer ()
		{
			_Dec();
		}


		forceinline RC Lock () const
		{
			if ( _ptr and Strategy_t::IncShared( _counter ) )
			{
				RC	result( _ptr );
				Strategy_t::DecShared( _counter );	// TODO: optimize
				return result;
			}
			return null;
		}


		forceinline explicit operator bool () const		{ return IsNotNull(); }

		forceinline bool IsNotNull ()	const			{ return _ptr != null; }
		forceinline bool IsNull ()		const			{ return _ptr; }

		forceinline T * RawPtr ()		const			{ return const_cast<T*>( _ptr ); }

		
		template <typename T2>
		forceinline Self& operator = (T2 *ptr)
		{
			if ( _CmpPtr( ptr ) )
				return *this;

			_CheckCast<T>( ptr );
			_Dec();
			_ptr = PointerCast<T>( ptr );
			if ( _ptr ) {
				_counter = Strategy_t::Create( PointerCast<B>( _ptr ) );
			}
			_Inc();
			return *this;
		}


		template <typename T2>
		forceinline Self& operator = (const WeakPointer<T2,B,WS,SPS> &right)
		{
			if ( _CmpPtr( right._ptr ) and _counter == right._counter )
				return *this;

			_CheckCast<T>( right._ptr );
			_Dec();
			_ptr		= PointerCast<T>( right._ptr );
			_counter	= right._counter;
			_Inc();
			return *this;
		}


		template <typename T2>
		forceinline Self& operator = (const SharedPointer<T2,B,SPS> &right)
		{
			return Self::operator = ( right.RawPtr() );
		}

		template <typename T2>
		forceinline Self& operator = (Ptr<T2> right)
		{
			return Self::operator = ( right.RawPtr() );
		}

		forceinline Self& operator = (Self &&right)
		{
			_Dec();
			_counter = right._counter;
			_ptr	 = right._ptr;
			return *this;
		}

		forceinline Self& operator = (const Self &right)
		{
			return Self::operator = <T>( right );
		}

		template <typename T2>
		forceinline T2 To ()
		{
			STATIC_ASSERT( typename T2::_is_weak_ptr(true) );
			return T2( *this );
		}
		
		_GX_DIM_CMP_OPERATORS_SELF( RawPtr() );
		_GX_DIM_CMP_OPERATORS_TYPE( RawPtr(), Value_t const, );
	};


}	// GXTypes
}	// GX_STL
