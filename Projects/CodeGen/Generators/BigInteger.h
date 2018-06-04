// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	//
	// Dynamic Integer Value
	//

	template <typename T, usize Size>
	class BigInteger final : CompileTime::PODStruct
	{
		friend struct BigIntegerTest;

	// types
	public:
		using Value_t	= T;
		using Array_t	= StaticArray< Value_t, Size >;
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

		explicit BigInteger (Value_t val)
		{
			_value[0]	= val;
			_lastBit	= _CalcBits( 0 );
		}

		Self&	operator = (Self &&) = default;
		Self&	operator = (const Self &) = default;

		Self&	operator ++ ()
		{
			_Add( 1 );
			return *this;
		}

		Self&	operator += (Value_t val)
		{
			_Add( val );
			return *this;
		}
		
		template <typename B, usize S>
		Self&	operator += (const BigInteger<B,S> &val);
		
		bool	operator == (const Self &right) const;
		bool	operator != (const Self &right) const	{ return not (*this == right); }

		bool	operator >  (const Self &right) const;
		bool	operator <  (const Self &right) const	{ return (right > *this); }

		bool	operator >= (const Self &right) const	{ return not (*this < right); }
		bool	operator <= (const Self &right) const	{ return not (*this > right); }

		BitsU Count () const
		{
			return _lastBit;
		}

		static BitsU MaxSize ()
		{
			return BitsU::SizeOf<Value_t>() * Array_t::STATIC_COUNT;
		}

		
		template <typename B, usize S>
		void	Assign (const BigInteger<B,S> &other);

		void	Fill (BitsU count);
		void	Fill (BitsU pos, BitsU count);

		Value_t	Read (BitsU pos, BitsU count) const;
		void	Write (Value_t value, BitsU pos, BitsU count);
		
		bool	IsZero (BitsU pos) const;
		bool	IsZero () const			{ return IsZero( 0_bit ); }

		String	ToString () const;

		ArrayCRef<T> ToArray () const	{ return _value; }


	private:
		void _Add (Value_t val);

		BitsU _CalcBits (usize i) const;
	};

	
/*
=================================================
	Read
=================================================
*/
	template <typename T, usize Size>
	inline T  BigInteger<T,Size>::Read (BitsU pos, BitsU count) const
	{
		const BitsU		maxBits = BitsU::SizeOf<Value_t>();
		const usize		i		= usize(pos / maxBits);

		ASSERT( count <= maxBits );
		ASSERT( pos + count <= MaxSize() );

		pos -= maxBits * i;
		
		Value_t	result		= (_value[i] >> (usize)pos) & ToMask<Value_t>( count );
		BitsU	max_count	= maxBits - pos;

		if ( count > max_count )
		{
			result |= ( _value[i+1] & ToMask<Value_t>( count - max_count ) ) << usize(max_count);
		}
		return result;
	}
	
/*
=================================================
	Write
=================================================
*/
	template <typename T, usize Size>
	inline void BigInteger<T,Size>::Write (Value_t value, BitsU pos, BitsU count)
	{
		const BitsU		maxBits = BitsU::SizeOf<Value_t>();

		ASSERT( count <= maxBits );
		ASSERT( pos + count <= MaxSize() );
		
		usize	i	= usize(pos / maxBits);
		BitsU	p	= pos - i * maxBits;

		pos		-= maxBits * i;
		value	&= ToMask<Value_t>( count );
		
		BitsU	max_count = maxBits - pos;
		
		_value[i] &= ~ToMask<Value_t>( p, p + count );
		_value[i] |= (value << usize(pos));

		if ( count > max_count )
		{
			++i;
			_value[i] &= ~ToMask<Value_t>( max_count );
			_value[i] |= (value >> usize(max_count));
		}
		
		_lastBit = _CalcBits( i );	// TODO: check
	}

/*
=================================================
	IsZero
=================================================
*/
	template <typename T, usize Size>
	inline bool BigInteger<T,Size>::IsZero (BitsU afterPos) const
	{
		return afterPos >= _lastBit;
	}
	
/*
=================================================
	_Add
=================================================
*/
	template <typename T, usize Size>
	inline void BigInteger<T,Size>::_Add (Value_t val)
	{
		if ( val == 0 )
			return;

		FOR( i, _value )
		{
			Value_t	prev = _value[i];

			_value[i] += val;

			if ( _value[i] >= prev )
			{
				_lastBit = Clamp( _CalcBits( i ), _lastBit, MaxSize() );
				break;
			}
	
			val = 1;
		}
	}
	
/*
=================================================
	operator +=
=================================================
*/
	template <typename T, usize Size>
	template <typename B, usize S>
	inline BigInteger<T,Size>&  BigInteger<T,Size>::operator += (const BigInteger<B,S> &right)
	{
		FOR( j, right._value )
		{
			Value_t	val = right._value[j];

			if ( val == 0 )
				continue;

			FORv( i, j, _value )
			{
				Value_t	prev = _value[i];

				_value[i] += val;

				if ( _value[i] >= prev )
				{
					_lastBit = Clamp( _CalcBits( i ), _lastBit, MaxSize() );
					break;
				}
	
				val = 1;
			}
		}
		return *this;
	}
	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T, usize Size>
	inline bool BigInteger<T,Size>::operator == (const Self &right) const
	{
		if ( _lastBit != right._lastBit )
			return false;

		FOR( i, _value )
		{
			const BitsU		offset		= i * BitsU::SizeOf<Value_t>();
			const BitsU		last_bit	= Min( _lastBit - offset, BitsU::SizeOf<Value_t>() );
			const Value_t	mask		= ToMask<Value_t>( 0_bit, last_bit );
			const Value_t	lhs			= _value[i] & mask;
			const Value_t	rhs			= right._value[i] & mask;

			if ( lhs != rhs )
				return false;

			if ( last_bit < BitsU::SizeOf<Value_t>() )
				break;
		}

		return true;
	}
	
/*
=================================================
	operator >
=================================================
*/
	template <typename T, usize Size>
	inline bool BigInteger<T,Size>::operator >  (const Self &right) const
	{
		if ( _lastBit > right._lastBit )
			return true;

		FOR( i, _value )
		{
			const BitsU		offset		= i * BitsU::SizeOf<Value_t>();
			const BitsU		last_bit	= Min( _lastBit - offset, BitsU::SizeOf<Value_t>() );
			const Value_t	mask		= ToMask<Value_t>( 0_bit, last_bit );
			const Value_t	lhs			= _value[i] & mask;
			const Value_t	rhs			= right._value[i] & mask;
			
			if ( lhs > rhs )
				return true;

			if ( last_bit < BitsU::SizeOf<Value_t>() )
				break;
		}

		return false;
	}

/*
=================================================
	_CalcBits
=================================================
*/
	template <typename T, usize Size>
	inline BitsU  BigInteger<T,Size>::_CalcBits (usize i) const
	{
		return BitScanReverse( _value[i] ) + 1 + (i * BitsU::SizeOf<Value_t>());
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
		static const uint	align	= CompileTime::SizeOf<Value_t>::bits / CompileTime::IntLog2< uint, radix >;

		String	str;	str << "{ ";

		FOR( i, _value )
		{
			const BitsU		offset		= i * BitsU::SizeOf<Value_t>();
			const BitsU		last_bit	= _lastBit - offset;
			const Value_t	mask		= ToMask<Value_t>( 0_bit, last_bit );
			const Value_t	val			= _value[i] & mask;

			str << (i > 0 ? ", " : "") << String().FormatAlignedI( val, align, '0', radix );
			
			if ( last_bit < BitsU::SizeOf<Value_t>() )
				break;
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
	inline void BigInteger<T,Size>::Assign (const BigInteger<B,S> &other)
	{
		const BitsU		step = Min( BitsU::SizeOf<T>(), BitsU::SizeOf<B>() );
		const BitsU		max	 = Min( MaxSize(), other.MaxSize(), other.Count() );

		_value.Clear();
		_lastBit = BitsU();

		for (BitsU pos; pos < max; pos += step)
		{
			const Value_t	r = Value_t(other.Read( pos, step ));

			Write( r, pos, step );
		}
	}
	
/*
=================================================
	Fill
=================================================
*/
	template <typename T, usize Size>
	inline void BigInteger<T,Size>::Fill (BitsU count)
	{
		return Fill( 0_bit, count );
	}
	
	template <typename T, usize Size>
	inline void BigInteger<T,Size>::Fill (BitsU pos, BitsU count)
	{
		if ( pos >= MaxSize() )
			return;

		_lastBit = Clamp( pos + count, _lastBit, MaxSize() );

		const BitsU	last_fill_bit	= Min( pos + count, _lastBit );
		const usize	start			= usize(pos / BitsU::SizeOf<Value_t>());

		for (usize i = start; i < _value.Count(); ++i)
		{
			const BitsU		offset		= i * BitsU::SizeOf<Value_t>();
			const BitsU		first_bit	= pos > offset ? pos - offset : 0_bit;
			const BitsU		last_bit	= Min( last_fill_bit - offset, BitsU::SizeOf<Value_t>() );
			const Value_t	mask		= ToMask<Value_t>( first_bit, last_bit );

			_value[i] |= mask;

			if ( last_bit < BitsU::SizeOf<Value_t>() )
				break;
		}
	}


}	// CodeGen
