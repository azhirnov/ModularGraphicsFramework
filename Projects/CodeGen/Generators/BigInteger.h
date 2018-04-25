// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	//
	// Dynamic Integer Value
	//

	template <typename T, usize Size>
	class BigInteger
	{
		friend struct BigIntegerTest;

	// types
	public:
		using value_t	= T;
		using Array_t	= StaticArray< value_t, Size >;
		using Self		= BigInteger< T, Size >;


	// variables
	private:
		Array_t		_value;
		BitsU		_lastBit;


	// methods
	public:
		BigInteger (GX_DEFCTOR)
		{}

		BigInteger (Self &&) = default;
		BigInteger (const Self &) = default;

		forceinline explicit BigInteger (value_t val)
		{
			_value[0]	= val;
			_lastBit	= _CalcBits( 0 );
		}

		forceinline Self&	operator = (Self &&) = default;
		forceinline Self&	operator = (const Self &) = default;

		forceinline Self&	operator ++ ()
		{
			_Add( 1 );
			return *this;
		}

		forceinline Self&	operator += (value_t val)
		{
			_Add( val );
			return *this;
		}

		forceinline BitsU Count () const
		{
			return _lastBit;
		}

		forceinline static BitsU MaxSize ()
		{
			return BitsU::SizeOf<value_t>() * Array_t::STATIC_COUNT;
		}

		
		template <typename B, usize S>
		void	Assign (const BigInteger<B,S> &other);

		value_t	Read (BitsU pos, BitsU count) const;
		void	Write (value_t value, BitsU pos, BitsU count);
		bool	IsZero (BitsU pos) const;

		String	ToString () const;

		ArrayCRef<T> ToArray () const	{ return _value; }


	private:
		void _Add (value_t val);

		BitsU _CalcBits (usize i) const;
	};

	
/*
=================================================
	Read
=================================================
*/
	template <typename T, usize Size>
	forceinline typename BigInteger<T,Size>::value_t  BigInteger<T,Size>::Read (BitsU pos, BitsU count) const
	{
		const BitsU		maxBits = BitsU::SizeOf<value_t>();
		const usize		i		= usize(pos / maxBits);

		ASSERT( count <= maxBits );
		ASSERT( pos + count <= MaxSize() );

		pos -= maxBits * i;
		
		value_t	result		= (_value[i] >> (usize)pos) & ToMask<value_t>( count );
		BitsU	max_count	= maxBits - pos;

		if ( count > max_count )
		{
			result |= ( _value[i+1] & ToMask<value_t>( count - max_count ) ) << usize(max_count);
		}
		return result;
	}
	
/*
=================================================
	Write
=================================================
*/
	template <typename T, usize Size>
	forceinline void BigInteger<T,Size>::Write (value_t value, BitsU pos, BitsU count)
	{
		const BitsU		maxBits = BitsU::SizeOf<value_t>();

		ASSERT( count <= maxBits );
		ASSERT( pos + count <= MaxSize() );
		
		usize	i	= usize(pos / maxBits);

		pos		-= maxBits * i;
		value	&= ToMask<value_t>( count );
		
		BitsU	max_count = maxBits - pos;

		_value[i] |= (value << usize(pos));

		if ( count > max_count )
		{
			_value[++i] |= (value >> usize(max_count));
		}
		
		_lastBit = _CalcBits( i );	// TODO: check
	}

/*
=================================================
	IsZero
=================================================
*/
	template <typename T, usize Size>
	forceinline bool BigInteger<T,Size>::IsZero (BitsU pos) const
	{
		return pos >= _lastBit;
	}
	
/*
=================================================
	_Add
=================================================
*/
	template <typename T, usize Size>
	forceinline void BigInteger<T,Size>::_Add (value_t val)
	{
		const value_t	maxVal	= MaxValue<value_t>();

		if ( val == 0 )
			return;

		FOR( i, _value )
		{
			value_t	prev = _value[i];

			_value[i] += val;

			if ( _value[i] >= prev )
			{
				_lastBit = Max( _lastBit, _CalcBits( i ) );
				return;
			}
	
			val = 1;
		}
	}
	
/*
=================================================
	_CalcBits
=================================================
*/
	template <typename T, usize Size>
	forceinline BitsU  BigInteger<T,Size>::_CalcBits (usize i) const
	{
		return BitScanReverse( _value[i] ) + 1 + (i * BitsU::SizeOf<value_t>());
	}

/*
=================================================
	ToString
=================================================
*/
	template <typename T, usize Size>
	inline String  BigInteger<T,Size>::ToString () const
	{
		static const uint	radix	= 16;
		static const uint	align	= CompileTime::SizeOf<value_t>::bits / CompileTime::IntLog2< uint, radix >;

		String	str;	str << "{ ";

		FOR( i, _value ) {
			str << (i > 0 ? ", " : "") << String().FormatAlignedI( _value[i], align, '0', radix );
		}
		str << " }";

		return str;
	}
	
/*
=================================================
	Assign
=================================================
*/
	template <typename T, usize Size>
	template <typename B, usize S>
	void BigInteger<T,Size>::Assign (const BigInteger<B,S> &other)
	{
		BitsU	pos;
		BitsU	step	= Min( BitsU::SizeOf<T>(), BitsU::SizeOf<B>() );
		BitsU	max		= Min( MaxSize(), other.MaxSize(), other.Count() );

		_value.Clear();
		_lastBit = BitsU();

		for (; pos < max; pos += step)
		{
			T	r = (T) other.Read( pos, step );

			Write( r, pos, step );
		}
	}


}	// CodeGen
