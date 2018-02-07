// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/Math/MathFunc.h"
#include "Engine/STL/Defines/OperatorHelpers.h"

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
		using Self		= Bitfield< B, IndexType >;
		using T			= typename CompileTime::NearUInt::FromBits< CompileTime::Max< uint, B, 32 > >;
		using Value_t	= T;
		using index_t	= IndexType;


	// variables
	private:
		T	_bits	: B;

		//  ||||||||||||
		// B-1         0
		
		static constexpr T	_MAX_VALUE = CompileTime::ToMask< T, B >;


	// methods
	public:
		constexpr Bitfield (GX_DEFCTOR): _bits( T(0) )		{}

		explicit
		constexpr Bitfield (const T bits): _bits(bits)		{}
		
		constexpr Bitfield (Self &&other) = default;
		constexpr Bitfield (const Self &other) = default;
		constexpr Self &	Set (index_t i)						{ ASSUME(i<B);  _bits |= (T(1) << i);  return *this; }
		constexpr Self &	Reset (index_t i)					{ ASSUME(i<B);  _bits &= ~(T(1) << i);  return *this; }
		constexpr Self &	SetAt (index_t i, bool bit)			{ ASSUME(i<B);  _bits &= ~(T(1) << i);  _bits |= (T(bit) << i);  return *this; }

		constexpr Self &	And (index_t i, bool bit)			{ ASSUME(i<B);  _bits &= (T(bit) << i);  return *this; }
		constexpr Self &	Or (index_t i, bool bit)			{ ASSUME(i<B);  _bits |= (T(bit) << i);  return *this; }
		constexpr Self &	Xor (index_t i, bool bit)			{ ASSUME(i<B);  _bits ^= (T(bit) << i);  return *this; }

		constexpr Self &	SetAll (bool bit = true)			{ _bits = bit ? _MAX_VALUE : T(0);  return *this; }

		CHECKRES constexpr bool		Get (index_t i) const		{ ASSUME(i<B);  return !!( _bits & (T(1) << i) ); }

		CHECKRES constexpr bool		IsZero ()	const			{ return _bits == 0; }
		CHECKRES constexpr bool		IsNotZero () const			{ return _bits != 0; }
		CHECKRES constexpr bool		Empty ()	const			{ return IsZero(); }

		CHECKRES constexpr index_t	MinIndex () const			{ return (index_t) GXMath::BitScanForward( _bits ); }
		CHECKRES constexpr index_t	MaxIndex () const			{ return (index_t) GXMath::BitScanReverse( _bits ); }

		CHECKRES explicit constexpr operator uint () const		{ return Cast<uint>(_bits); }
		CHECKRES explicit constexpr operator ulong () const		{ return Cast<usize>(_bits); }

		CHECKRES static constexpr usize Count ()				{ return B; }

		CHECKRES constexpr Self	operator ~  ()	const			{ return Self( ~_bits ); }

		Self &	operator =  (Self &&right) = default;
		Self &	operator =  (const Self &right) = default;
		
		// add bit
							Self &	operator |= (index_t i)			{ return Set( i ); }
		CHECKRES constexpr	Self	operator |  (index_t i) const	{ return Self(*this).Set( i ); }
		
		// remove bit
							Self &	operator ^= (index_t i)			{ return Reset( i ); }
		CHECKRES constexpr	Self	operator ^  (index_t i) const	{ return Self(*this).Reset( i ); }

		CHECKRES constexpr	bool 	operator [] (index_t i) const	{ return Get( i ); }
		CHECKRES			BitRef	operator [] (index_t i)			{ ASSUME(i < B);  return BitRef( *this, i ); }
		

		_GX_DIM_CMP_OPERATORS_SELF( _bits );
		_GX_DIM_OPERATORS_SELF( |,	_bits );
		_GX_DIM_OPERATORS_SELF( &,	_bits );
		_GX_DIM_OPERATORS_SELF( ^,	_bits );


		forceinline constexpr void SetInterval (index_t first, index_t last)
		{
			ASSUME( first <= last and last < B );
			_bits |= GXMath::ToMask<T>( BitsU(first), BitsU(last+1) );
		}

		forceinline constexpr void ResetInterval (index_t first, index_t last)
		{
			ASSUME( first <= last and last < B );
			_bits &= ~GXMath::ToMask<T>( BitsU(first), BitsU(last+1) );
		}

		CHECKRES forceinline constexpr bool HasInterval (index_t first, index_t last) const
		{
			ASSUME( first <= last and last < B );
			const T	mask = GXMath::ToMask<T>( BitsU(first), BitsU(last+1) );
			return (_bits & mask) == mask;
		}

		CHECKRES forceinline constexpr bool HasAll () const
		{
			return HasInterval( 0, B-1 );
		}

		CHECKRES forceinline constexpr bool BitsEq (const Self &right) const
		{
			// Compare only setted bits!
			// Warning: not same result as right.BitsEq( *this )

			return (_bits & right._bits) == right._bits;
		}

		CHECKRES forceinline constexpr bool AnyBitEq (const Self &right) const
		{
			return (_bits & right._bits) != 0;
		}


	public:
		struct BitRef
		{
		private:
			Self &		_bitfield;
			index_t		_index;

			void _Reset ()			{ _bitfield.Reset( _index ); }
			void _Set (bool bit)	{ _bitfield.SetAt( _index, bit ); }
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
			explicit operator C () const
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
	struct Hash < Bitfield<B,IT> >
	{
		CHECKRES HashResult  operator () (const Bitfield<B,IT> &x) const
		{
			return HashOf( T(x) );
		}
	};

}	// GXTypes
}	// GX_STL
