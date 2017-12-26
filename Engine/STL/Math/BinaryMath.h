// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Trigonometry.h"

namespace GX_STL
{
namespace GXMath
{
	
/*
=================================================
	SafeLeftBitShift
=================================================
*/
	template <typename T>
	forceinline T  SafeLeftBitShift (const T& x, BitsU shift)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		//ASSUME( shift >= 0 );

		return x << ( usize(shift) & (CompileTime::SizeOf<T>::bits - 1) );
	}
	
	template <typename T, usize I, ulong U>
	inline Vec<T,I,U>  SafeLeftBitShift (const Vec<T,I,U> &x, const Vec<BitsU,I,U>& shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SafeLeftBitShift( x[i], shift[i] );
		return ret;
	}
	
	template <typename T, usize I, ulong U>
	inline Vec<T,I,U>  SafeLeftBitShift (const Vec<T,I,U> &x, BitsU shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SafeLeftBitShift( x[i], shift );
		return ret;
	}
	
/*
=================================================
	SafeRightBitShift
=================================================
*/
	template <typename T>
	forceinline T  SafeRightBitShift (const T& x, BitsU shift)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		//ASSUME( shift >= 0 );

		return x >> ( usize(shift) & (CompileTime::SizeOf<T>::bits - 1) );
	}
	
	template <typename T, usize I, ulong U>
	inline Vec<T,I,U>  SafeRightBitShift (const Vec<T,I,U> &x, const Vec<BitsU,I,U>& shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SafeRightBitShift( x[i], shift[i] );
		return ret;
	}
	
	template <typename T, usize I, ulong U>
	inline Vec<T,I,U>  SafeRightBitShift (const Vec<T,I,U> &x, BitsU shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SafeLeftBitShift( x[i], shift );
		return ret;
	}
	
/*
=================================================
	BitRotateLeft
----
	from https://en.wikipedia.org/wiki/Circular_shift#Implementing_circular_shifts
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		forceinline T _BitRotateLeft (T value, usize shift)
		{
			const usize	mask = (CompileTime::SizeOf<T>::bits - 1);

			shift &= mask;
			return (value << shift) | (value >> ( ~(shift-1) & mask ));
		}
	}
	
	template <typename T>
	forceinline T  BitRotateLeft (const T& x, BitsU shift)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		ASSUME( shift >= 0 );

		typedef CompileTime::NearUInt::FromType<T>	Unsigned_t;

		return (T) _math_hidden_::_BitRotateLeft( Unsigned_t(x), usize(shift) );
	}

	template <typename T, usize I, ulong U>
	inline T  BitRotateLeft (const Vec<T,I,U> &x, const Vec<BitsU,I,U>& shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = BitRotateLeft( x[i], shift[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	inline T  BitRotateLeft (const Vec<T,I,U> &x, BitsU shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = BitRotateLeft( x[i], shift );
		return ret;
	}
	
/*
=================================================
	BitRotateRight
----
	from https://en.wikipedia.org/wiki/Circular_shift#Implementing_circular_shifts
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		forceinline T _BitRotateRight (T value, usize shift)
		{
			const usize	mask = (CompileTime::SizeOf<T>::bits - 1);

			shift &= mask;
			return (value >> shift) | (value << ( ~(shift-1) & mask ));
		}
	}

	template <typename T>
	forceinline T  BitRotateRight (const T& x, BitsU shift)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		ASSUME( shift >= 0 );

		typedef CompileTime::NearUInt::FromType<T>	Unsigned_t;

		return (T) _math_hidden_::_BitRotateRight( Unsigned_t(x), int(shift) );
	}

	template <typename T, usize I, ulong U>
	inline T  BitRotateRight (const Vec<T,I,U> &x, const Vec<BitsU,I,U>& shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = BitRotateLeft( x[i], shift[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	inline T  BitRotateRight (const Vec<T,I,U> &x, BitsU shift)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = BitRotateLeft( x[i], shift );
		return ret;
	}

/*
=================================================
	ToBit
=================================================
*/
	template <typename T>
	forceinline T  ToBit (BitsU bitIndex)
	{
		return SafeLeftBitShift( T(1), (usize)bitIndex );
	}

	template <typename T, usize I, ulong U>
	inline Vec<T,I,U>  ToBit (const Vec<BitsU,I,U> &bitIndex)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = ToBit<T>( bitIndex[i] );
		return ret;
	}
	
/*
=================================================
	ToMask
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		struct _ToMask {
			forceinline static T Get (const BitsU lastBitIndex)
			{
				STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
				STATIC_ASSERT( CompileTime::IsInteger<T> /*and CompileTime::IsUnsigned<T>*/ );

				// Test:
				// (1 << lastBitIndex-1) & ToMask( lastBitIndex ) == (1 << lastBitIndex-1)

				return	lastBitIndex < 0 ? T(0) :
							lastBitIndex < CompileTime::SizeOf<T>::bits ?
								(T(1) << (usize)lastBitIndex) - 1 :
								T(-1);
			}
		};
		
		template <typename T, usize I, ulong U>
		struct _ToMask< Vec<T,I,U> > {
			forceinline static Vec<T,I,U> Get (const BitsU lastBitIndex)
			{
				Vec<T,I,U>		ret;
				FOR( i, ret )	ret[i] = _ToMask<T>::Get( lastBitIndex );
				return ret;
			}
		};
	}	// _math_hidden_

	template <typename T>
	forceinline T  ToMask (const BitsU lastBitIndex)
	{
		return _math_hidden_::_ToMask<T>::Get( lastBitIndex );
	}

	template <typename T, usize I, ulong U>
	inline Vec<T,I,U>  ToMask (const Vec<BitsU,I,U> &lastBitIndex)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = ToMask<T>( lastBitIndex[i] );
		return ret;
	}

/*
=================================================
	ToMask
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		struct _ToMask2 {
			forceinline static T Get (const BitsU first, const BitsU last)
			{
				ASSERT( first <= last );
				return _ToMask<T>::Get( first ) ^ _ToMask<T>::Get( last );
			}
		};
		
		template <typename T, usize I, ulong U>
		struct _ToMask2< Vec<T,I,U> > {
			forceinline static Vec<T,I,U> Get (const BitsU first, const BitsU last)
			{
				Vec<T,I,U>		ret;
				FOR( i, ret )	ret[i] = _ToMask2<T>::Get( first, last );
				return ret;
			}
		};
	}	// _math_hidden_

	template <typename T>
	forceinline T  ToMask (const BitsU first, const BitsU last)
	{
		return _math_hidden_::_ToMask2<T>::Get( first, last );
	}
	
	template <typename T, usize I, ulong U>
	inline Vec<T,I,U>  ToMask (const Vec<BitsU,I,U> first, const Vec<BitsU,I,U> last)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = ToMask<T>( first[i], last[i] );
		return ret;
	}

/*
=================================================
	GetMaskForType
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		struct _GetMaskForType {
			forceinline static T Get ()
			{
				STATIC_ASSERT( CompileTime::IsInteger<T> and CompileTime::IsUnsigned<T> );
				return UMax;
			}
		};
		
		template <typename T, usize I, ulong U>
		struct _GetMaskForType< Vec<T,I,U> > {
			forceinline static Vec<T,I,U> Get ()
			{
				Vec<T,I,U>		ret;
				FOR( i, ret )	ret[i] = _GetMaskForType<T>::Get();
				return ret;
			}
		};
	}	// _math_hidden_

	template <typename T>
	forceinline T  GetMaskForType (const T &)
	{
		return _math_hidden_::_GetMaskForType< T >::Get();
	}
	
/*
=================================================
	IntLog2
=================================================
*/
	namespace _math_hidden_
	{
		template <uint Bit>
		struct _RecursiveBitScanReverse
		{
			template <typename T>
			forceinline constexpr static uint Get (const T& x)
			{
				return uint( !!T(x >> Bit) ) + _RecursiveBitScanReverse< Bit-1 >::Get( x );
			}
		};

		template <>
		struct _RecursiveBitScanReverse<0>
		{
			template <typename T>
			forceinline constexpr static uint Get (const T& x)
			{
				return uint( !!x );
			}
		};

	}	// _math_hidden_


	template <typename T>
	forceinline uint IntLog2 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		
		typedef typename CompileTime::NearUInt::FromType<T>	utype;

		const utype	value = ReferenceCast<utype>(x);

		return _math_hidden_::_RecursiveBitScanReverse< CompileTime::SizeOf<utype>::bits-1 >::Get( value ) - 1;
	}

	template <typename T, usize I, ulong U>
	inline Vec<uint,I,U>  IntLog2 (const Vec<T,I,U> &x)
	{
		Vec<uint,I,U>		ret;
		FOR( i, ret )	ret[i] = IntLog2( x[i] );
		return ret;
	}

/*
=================================================
	BitScanReverse
=================================================
*/
	template <typename T>
	forceinline BitsU BitScanReverse (const T& x)
	{
		return BitsU( IntLog2( x ) );
	}

	template <typename T, usize I, ulong U>
	inline Vec<BitsU,I,U> BitScanReverse (const Vec<T,I,U> &x)
	{
		Vec<BitsU,I,U>	ret;
		FOR( i, x )		ret[i] = BitScanReverse( x[i] );
		return ret;
	}
	
/*
=================================================
	BitScanForward
=================================================
*/
	namespace _math_hidden_
	{
		template <uint Bit>
		struct _RecursiveBitScanForward
		{
			template <typename T>
			forceinline constexpr static uint Get (const T& x)
			{
				return uint( (!!T(x >> Bit)) & (!(x & ((T(1) << Bit) - 1))) ) + _RecursiveBitScanForward< Bit-1 >::Get( x );
			}
		};

		template <>
		struct _RecursiveBitScanForward<0>
		{
			template <typename T>
			forceinline constexpr static uint Get (const T& x)
			{
				return uint( !!x );
			}
		};
	}

	template <typename T>
	forceinline BitsU BitScanForward (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		typedef typename CompileTime::NearUInt::FromType<T>	utype;

		const utype	value = ReferenceCast<utype>(x);
		
		return BitsU( _math_hidden_::_RecursiveBitScanForward< CompileTime::SizeOf<utype>::bits-1 >::Get( value ) - 1 );
	}

	template <typename T, usize I, ulong U>
	inline Vec<BitsU,I,U> BitScanForward (const Vec<T,I,U> &x)
	{
		Vec<BitsU,I,U>	ret;
		FOR( i, x )		ret[i] = BitScanForward( x[i] );
		return ret;
	}
	
/*
=================================================
	ReverseBitOrder
=================================================
*/
	namespace _math_hidden_
	{
		// from http://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious

		template <uint Bit>
		struct _RecursiveReverseBitOrder
		{
			template <typename T>
			forceinline static T Get (const T &x, const T &mask)
			{
				const uint	s = Bit >> 1;
				const T		m = mask ^ (mask << s);
				const T		v = ((x >> s) & m) | ((x << s) & ~m);

				return _RecursiveReverseBitOrder< s >::Get( v, m );
			}
		};
		
		template <>
		struct _RecursiveReverseBitOrder<0>
		{
			template <typename T>
			forceinline static T Get (const T &x, const T &)
			{
				return x;
			}
		};
	}
	
	template <typename T>
	forceinline T ReverseBitOrder (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		typedef typename CompileTime::NearUInt::FromType<T>	utype;

		const utype	value = ReferenceCast<utype>(x);

		return _math_hidden_::_RecursiveReverseBitOrder< CompileTime::SizeOf<utype>::bits >::Get( x, ~T(0) );
	}

	template <typename T, usize I, ulong U>
	inline Vec<T,I,U> ReverseBitOrder (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>	ret;
		FOR( i, x )	ret[i] = ReverseBitOrder( x[i] );
		return ret;
	}


}	// GXMath
}	// GX_STL
