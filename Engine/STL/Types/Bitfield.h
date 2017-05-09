﻿// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/Math/MathFunc.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Bitfield
	//
	
	template <usize B, typename IndexType = usize>
	struct Bitfield : public CompileTime::PODType
	{
	// types
	public:
		struct BitRef;
		typedef Bitfield< B, IndexType >						Self;
		typedef typename CompileTime::NearUInt::FromBits< B >	T;
		typedef T												value_t;
		typedef IndexType										index_t;


	// variables
	private:
		T	_bits	: B;

		//  ||||||||||||
		// B-1         0
		
		static const T	_MAX_VALUE = CompileTime::ToMask< T, B >;


	// methods
	public:
		Bitfield (GX_DEFCTOR): _bits( T(0) )		{}

		explicit
		Bitfield (const T bits): _bits(bits)		{}
		
		Bitfield (Self &&other) = default;
		Bitfield (const Self &other) = default;

		bool	Get (index_t i) const				{ ASSUME(i<B);  return !!( _bits & (T(1) << i) ); }
		Self &	Set (index_t i)						{ ASSUME(i<B);  _bits |= T(1) << i;  return *this; }
		Self &	Reset (index_t i)					{ ASSUME(i<B);  _bits ^= T(1) << i;  return *this; }
		Self &	SetAt (index_t i, bool bit)			{ ASSUME(i<B); (_bits &= ~(T(1) << i)) |= T(bit) << i;  return *this; }

		Self &	And (index_t i, bool bit)			{ ASSUME(i<B);  _bits &= T(bit) << i;  return *this; }
		Self &	Or (index_t i, bool bit)			{ ASSUME(i<B);  _bits |= T(bit) << i;  return *this; }
		Self &	Xor (index_t i, bool bit)			{ ASSUME(i<B);  _bits ^= T(bit) << i;  return *this; }

		Self &	SetAll (bool bit = true)			{ _bits = bit ? _MAX_VALUE : T(0);  return *this; }

		bool	IsZero ()	const					{ return _bits == 0; }
		bool	IsNotZero () const					{ return _bits != 0; }
		bool	Empty ()	const					{ return IsZero(); }

		index_t	MinIndex () const					{ return (index_t) GXMath::BitScanForward( _bits ); }
		index_t	MaxIndex () const					{ return (index_t) GXMath::BitScanReverse( _bits ); }

		operator T () const							{ return _bits; }

		static usize Count ()						{ return B; }

		Self	operator ~  ()	const				{ return Self( ~_bits ); }

		Self &	operator =  (Self &&right) = default;
		Self &	operator =  (const Self &right) = default;
		
		// add bit
		Self &	operator |= (index_t i)				{ return Set( i ); }
		Self	operator |  (index_t i) const		{ return Self(*this).Set( i ); }
		
		// remove bit
		Self &	operator ^= (index_t i)				{ return Reset( i ); }
		Self	operator ^  (index_t i) const		{ return Self(*this).Reset( i ); }

		bool 	operator [] (index_t i) const		{ return Get( i ); }
		BitRef	operator [] (index_t i)				{ ASSUME(i < B);  return BitRef( *this, i ); }
		

		forceinline void SetInterval (index_t first, index_t last)
		{
			ASSUME( first <= last and last < B );
			_bits |= GXMath::ToMask<T>( BitsU(first), BitsU(last+1) );
		}

		forceinline void ResetInterval (index_t first, index_t last)
		{
			ASSUME( first <= last and last < B );
			_bits ^= GXMath::ToMask<T>( BitsU(first), BitsU(last+1) );
		}

		forceinline bool HasInterval (index_t first, index_t last) const
		{
			ASSUME( first <= last and last < B );
			const T	mask = GXMath::ToMask<T>( BitsU(first), BitsU(last+1) );
			return (_bits & mask) == mask;
		}

		forceinline bool HasAll () const
		{
			return HasInterval( 0, B-1 );
		}

		forceinline bool BitsEq (const Self &right) const
		{
			// Compare only setted bits!
			// Warning: not same result as right.BitsEq( *this )

			return (_bits & right._bits) == right._bits;
		}

		forceinline bool AnyBitEq (const Self &right) const
		{
			return (_bits & right._bits) != 0;
		}


	public:
		struct BitRef
		{
		private:
			Self &		_bitfield;
			index_t		_index;

			void _Set ()			{ _bitfield.Set( _index ); }
			void _Reset ()			{ _bitfield.Reset( _index ); }
			void _SetAt (bool bit)	{ _bitfield.SetAt( _index, bit ); }
			void _And (bool bit)	{ _bitfield.And( _index, bit ); }
			void _Or (bool bit)		{ _bitfield.Or( _index, bit ); }
			void _Xor (bool bit)	{ _bitfield.Xor( _index, bit ); }
			bool _Get () const		{ return _bitfield.Get( _index ); }

		public:
			BitRef (Self &bitfield, index_t index):
				_bitfield( bitfield ), _index(index)
			{}

			BitRef & operator = (const BitRef &bit)
			{
				_Set( bit._Get() );
				return *this;
			}

			template <typename C>
			BitRef & operator = (const C &right)
			{
				_Set( (bool)right );
				return *this;
			}

			operator bool () const
			{
				return _Get();
			}

			template <typename C>
			operator C () const
			{
				return C( _Get() );
			}

			template <typename C>
			BitRef &	operator |= (const C right)
			{
				_Or( (bool)right );
				return *this;
			}

			template <typename C>
			BitRef &	operator &= (const C right)
			{
				_And( (bool)right );
				return *this;
			}

			template <typename C>
			BitRef &	operator ^= (const C right)
			{
				_Xor( (bool)right );
				return *this;
			}
		};


		STATIC_ASSERT( CompileTime::SizeOf<T>::bits >= B );
	};


	template <typename EnumType>
	using EnumBitfield = Bitfield< EnumType::_Count, typename EnumType::type >;

	
	template <usize B, typename IT>
	struct Hash < Bitfield< B, IT > > :
		private Hash< typename Bitfield< B, IT >::value_t >
	{
		typedef Bitfield<B>						key_t;
		typedef Hash< typename key_t::value_t >	base_t;
		typedef typename base_t::result_t		result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL