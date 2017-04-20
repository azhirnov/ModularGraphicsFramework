// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Trigonometry.h"
#include "Engine/STL/Dimensions/ByteAndBit.h"
#include "Engine/STL/Experimental/FastMath.h"

namespace GX_STL
{
namespace GXMath
{

//-------------------------------------------------------------------
//	Operation with sign

	
/*
=================================================
	ReverseBitOrder
=================================================
*/
	namespace _math_hidden_
	{
		template <bool S, bool I>
		struct _Abs_impl {};

		template <>
		struct _Abs_impl<true,true>
		{
			template <typename T>	forceinline static T abs (const T& val)
			{
			#if 1
				typedef typename CompileTime::NearUInt::FromType<T>	uint_t;
				const uint_t mask = ((uint_t(1) << (CompileTime::SizeOf<T>::bits-1))-1);

				// if val == MinValue result is undefined, remove sign bit anyway
				return ::abs( val ) & mask;

			#elif 0
				// from http://graphics.stanford.edu/~seander/bithacks.html#IntegerAbs
				typedef typename CompileTime::NearUInt::FromType<T>	uint_t;

				const T	mask = val >> (CompileTime::SizeOf<T>::bits-1);
				return (val + mask) ^ mask;

			#elif 0
				return (val < T(0)) ? (val == MinValue<T>() ? -(val+1) : -val) : val;
			#endif
			}
		};

		template <>
		struct _Abs_impl<false,true>
		{
			template <typename T>	forceinline static T abs (const T& val)		{ return val; }
		};

		template <>
		struct _Abs_impl<false,false>
		{
			template <typename T>	forceinline static T abs (const T& val)		{ return val; }
		};

		template <>
		struct _Abs_impl<true,false>
		{
			template <typename T>
			forceinline static T abs (const T& val)
			{
			#if 1
				typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

				return (T) ::abs( _float_t(val) );

			#else
				typedef typename CompileTime::NearUInt::FromType<T>	int_t;
			
				static const int_t	mask = ~( int_t(1) << CompileTime::TypeDescriptor::template GetTypeInfo<T>::SignBit() );

				T	ret( val );
				ReferenceCast< int_t >( ret ) &= mask;
				return ret;
			#endif
			}
		};
		
		template <typename T>
		struct _Abs
		{
			forceinline static T abs (const T& val)
			{
				return _Abs_impl< CompileTime::IsSigned<T>, CompileTime::IsInteger<T> >::template abs( val );
			}
		};
	}
	
	template <typename T>
	forceinline T Abs (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_Abs<T>::abs( x );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Abs (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Abs( x[i] );
		return ret;
	}

/*
=================================================
	Sign
----
	returns 1 or -1
=================================================
*/
	template <typename T>
	forceinline T Sign (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return ( x < T(0) ? T(-1) : T(1) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Sign (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Sign<T>( x[i] );
		return ret;
	}

/*
=================================================
	Sign
----
	returns 1 or 0 or -1
=================================================
*/
	template <typename T>
	forceinline T SignOrZero (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return T( x > T(0) ) - T( x < T(0) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  SignOrZero (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SignOrZero( x[i] );
		return ret;
	}
	
/*
=================================================
	SetSign
=================================================
*/
	namespace _math_hidden_
	{
		template <bool bFloat, bool bSigned>
		struct _SetSign_impl
		{
			template <typename T>
			static T setsign (const T& x, bool s);
		};

		template <>
		struct _SetSign_impl< true, true >
		{
			template <typename T>
			static T setsign (const T& x, bool s)
			{
				typedef typename CompileTime::NearUInt::FromType<T>	uint_t;
				
				const uint	u_sign_bit	= CompileTime::TypeDescriptor::template GetTypeInfo<T>::SignBit();

				T	ret(x);
				ReferenceCast<uint_t>( ret ) &= ~( uint_t(1) << u_sign_bit );
				ReferenceCast<uint_t>( ret ) |=  ( uint_t(s) << u_sign_bit );
				return ret;
			}
		};

		template <>
		struct _SetSign_impl< false, true >
		{
			template <typename T>
			static T setsign (const T& x, bool s)
			{
				if ( (  s and x > (T)0 ) or
					 ( not s and x < (T)0 ) )
				{
					return x == MinValue<T>() ? -(x+1) : -x;
				}
				return x;
			}
		};

		template <>
		struct _SetSign_impl< true, false >
		{
			template <typename T>
			static T setsign (const T& x, bool s)
			{
				ASSUME( not s and "value already unsigned" );
				return x;
			}
		};

		template <>
		struct _SetSign_impl< false, false >
		{
			template <typename T>
			static T setsign (const T& x, bool s)
			{
				ASSUME( not s and "value already unsigned" );
				return x;
			}
		};

		template <typename T>
		struct _SetSign
		{
			static T setsign (const T& x, bool s)
			{
				return _SetSign_impl< CompileTime::IsFloat<T>, CompileTime::IsSigned<T> >::setsign( x, s );
			}
		};
	}

	template <typename T>
	inline T SetSign (const T &x, bool sign)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_SetSign<T>::setsign( x, sign );
	}

	template <typename T, usize I>
	inline Vec<T,I>  SetSign (const Vec<T,I> &x, bool sign)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SetSign( x[i], sign );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  SetSign (const Vec<T,I> &x, const Vec<bool,I> &sign)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SetSign( x[i], sign[i] );
		return ret;
	}
	
/*
=================================================
	CopySign
=================================================
*/
	namespace _math_hidden_
	{
		template <bool bFloat, bool bSigned>
		struct _CopySign_impl
		{
			template <typename T>
			static T copysign (const T& x, const T& s);
		};

		template <>
		struct _CopySign_impl<true,true>
		{
			template <typename T>
			static T copysign (const T& x, const T& s)
			{
				typedef typename CompileTime::NearUInt::FromType<T>	uint_t;
				
				const uint_t	u_sign_mask	= uint_t(1) << CompileTime::TypeDescriptor::template GetTypeInfo<T>::SignBit();

				T	ret(x);
				ReferenceCast<uint_t>( ret ) &= ~u_sign_mask;
				ReferenceCast<uint_t>( ret ) |=  ReferenceCast<uint_t>( s ) & u_sign_mask;
				return ret;
			}
		};

		template <>
		struct _CopySign_impl<false,true>
		{	
			template <typename T>
			static T copysign (const T& x, const T& s)
			{
				if ( ( s >= T(0) and x >= T(0) ) or
					 ( s <= T(0) and x <= T(0) ) )
				{
					return x;
				}
				return -x;
			}
		};

		template <>
		struct _CopySign_impl<true,false>
		{
			template <typename T>
			static T copysign (const T& x, const T& s)
			{
				return x;
			}
		};

		template <>
		struct _CopySign_impl<false,false>
		{
			template <typename T>
			static T copysign (const T& x, const T& s)
			{
				return x;
			}
		};

		template <typename T>
		struct _CopySign
		{
			static T copysign (const T& x, const T& s)
			{
				return _CopySign_impl< CompileTime::IsFloat<T>,
									   CompileTime::IsSigned<T> >::copysign( x, s );
			}
		};
	}
	
	template <typename T>
	inline T CopySign (const T& from, const T& to)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_CopySign<T>::copysign( to, from );
	}

	template <typename T, usize I>
	inline Vec<T,I>  CopySign (const Vec<T,I> &from, const Vec<T,I> &to)
	{
		Vec<T,I>	ret;
		FOR( i, ret )	ret[i] = CopySign( from[i], to[i] );
		return ret;
	}



//-------------------------------------------------------------------
//	Compare operations

	
/*
=================================================
	IsZero
=================================================
*/
	namespace _math_hidden_
	{
		template <bool I>
		struct _IsZero_impl
		{};

		template <>
		struct _IsZero_impl<true> {
			template <typename T>	forceinline static bool is_zero (const T& x)	{ return x == T(0); }
		};

		template <>
		struct _IsZero_impl<false> {
			template <typename T>	forceinline static bool is_zero (const T& x)	{ return Abs(x) <= Epsilon<T>(); }
		};

		template <typename T>
		struct _IsZero {
			forceinline static bool is_zero (const T& x) {
				return _IsZero_impl< CompileTime::IsInteger<T> >::is_zero( x );
			}
		};
	}

	template <typename T>
	forceinline bool IsZero (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_IsZero<T>::is_zero( x );
	}

	template <>
	forceinline bool IsZero (const bool& x)
	{
		return not x;
	}

	template <typename T, usize I>
	forceinline bool IsZero (const Vec<T,I>& x)
	{
		return All( x.IsZero() );
	}
	
/*
=================================================
	IsNotZero
=================================================
*/
	template <typename T>
	forceinline bool IsNotZero (const T& x)
	{
		return not IsZero( x );
	}

	template <>
	forceinline bool IsNotZero (const bool& x)
	{
		return x;
	}
	
/*
=================================================
	Equals
=================================================
*/
	namespace _math_hidden_
	{
		// integer
		template <bool F>
		struct _Equals_impl
		{
			template <typename T>
			forceinline static bool eq (const T& a, const T& b)
			{ 
				return a == b;
			}

			template <typename T>
			forceinline static bool eqa (const T& a, const T& b, uint ac)
			{
				typedef typename CompileTime::NearUInt::FromType<T>	uint_t;
				return uint_t( Abs( a - b ) ) <= ( uint_t(1) << ac );
			}
		};

		template <typename T>	struct _FloatAccuracy			{ static const uint	value = CompileTime::SizeOf<T>::bits / 3; };
		template <>				struct _FloatAccuracy<double>	{ static const uint	value = CompileTime::SizeOf<double>::bits * 2 / 3; };

		
		// float
		template <>
		struct _Equals_impl<true>
		{
			template <typename T>
			forceinline static bool eq (const T& a, const T& b)
			{
				return eqa( a, b, _FloatAccuracy<T>::value );
			}

			template <typename T>
			forceinline static bool eqa (const T& a, const T& b, uint ac)
			{
				typedef typename CompileTime::NearInt::FromType<T>	int_t;
				typedef typename CompileTime::NearUInt::FromType<T>	uint_t;

				int_t	i_a = ReferenceCast<int_t>(a);
				int_t	i_b	= ReferenceCast<int_t>(b);

				if ( i_a < int_t(0) )	i_a = MinValue<int_t>() - i_a;
				if ( i_b < int_t(0) )	i_b = MinValue<int_t>() - i_b;

				uint_t	diff = (uint_t)Abs( i_a - i_b );

				return ( diff <= ( uint_t(1) << ac ) );
			}
		};
		
		template <typename T>
		struct _Equals
		{
			forceinline static bool eq (const T& a, const T& b)
			{
				return _Equals_impl< CompileTime::IsFloat<T> >::eq( a, b );
			}

			forceinline static bool eqa (const T& a, const T& b, uint ac)
			{
				return _Equals_impl< CompileTime::IsFloat<T> >::eqa( a, b, ac );
			}
		};
	}

	template <typename T>
	forceinline bool Equals (const T& a, const T& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_Equals<T>::eq( a, b );
	}

	template <typename T, usize I>
	inline Vec<bool,I>  Equals (const Vec<T,I> &a, const Vec<T,I> &b)
	{
		Vec<bool,I>		ret;
		FOR( i, ret )	ret[i] = Equals( a[i], b[i] );
		return ret;
	}

/*
=================================================
	EqualsWithError
=================================================
*/
	template <typename T>
	forceinline bool EqualsWithError (const T& a, const T& b, /*Bits*/uint accuracy)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_Equals<T>::eqa( a, b, accuracy );
	}

	template <typename T, usize I>
	inline Vec<bool,I>  EqualsWithError (const Vec<T,I> &a, const Vec<T,I> &b, /*Bits*/uint accuracy)
	{
		Vec<bool,I>		ret;
		FOR( i, ret )	ret[i] = EqualsWithError( a[i], b[i], accuracy );
		return ret;
	}
	
/*
=================================================
	MiddleValue
=================================================
*/
	namespace _math_hidden_
	{
		// float
		template <typename T, bool IsFloat, bool IsSigned>
		struct _MiddleValue {
			forceinline static T Get (const T& a, const T& b)	{ return (a + b) * T(0.5); }	// TODO: is it good for big values?
		};

		// unsigned int
		template <typename T>
		struct _MiddleValue< T, false, false > {
			forceinline static T Get (const T& a, const T& b)	{ return a < b ? (a + (b-a) / T(2)) : (b + (a-b) / T(2)); }
		};
		
		// signed int
		template <typename T>
		struct _MiddleValue< T, false, true > {
			forceinline static T Get (const T& a, const T& b)	{ return a + (b-a) / T(2); }
		};

	}	// _math_hidden_

	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B>  MiddleValue (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;

		return _math_hidden_::_MiddleValue< T, CompileTime::IsFloat<T>, CompileTime::IsSigned<T> >::Get( T(a), T(b) );
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  MiddleValue (const Vec<A,I>& a, const Vec<B,I>& b)
	{
		typedef typename CompileTime::MainType<A,B>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = MiddleValue( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	Clamp
----
	clamp to inner range
=================================================
*/
	template <typename A, typename B, typename C>
	forceinline typename CompileTime::MainType<A,B,C>  Clamp (const A& value, const B& minValue, const C& maxValue)
	{
		ASSERT( minValue <= maxValue );
		return Min( maxValue, Max( value, minValue ) );
	}

	template <typename A, typename B, typename C, usize I>
	inline Vec< typename CompileTime::MainType<A,B,C>, I >  Clamp (const Vec<A,I>& value, const Vec<B,I>& minValue, const Vec<C,I>& maxValue)
	{
		typedef typename CompileTime::MainType<A,B,C>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Clamp( value[i], minValue[i], maxValue[i] );
		return ret;
	}

	template <typename A, typename B, typename C, usize I>
	inline Vec< typename CompileTime::MainType<A,B,C>, I >  Clamp (const Vec<A,I>& value, const B& minValue, const C& maxValue)
	{
		typedef typename CompileTime::MainType<A,B,C>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Clamp( value[i], minValue, maxValue );
		return ret;
	}
	
/*
=================================================
	Clamp
----
	clamp to outer range
=================================================
*/
	template <typename A, typename B, typename C>
	forceinline typename CompileTime::MainType<A,B,C> ClampOut (const A& value, const B& minValue, const C& maxValue)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and
					   CompileTime::IsScalarOrEnum<B> and
					   CompileTime::IsScalarOrEnum<C> );

		ASSERT( minValue <= maxValue );

		typedef typename CompileTime::MainType<A,B,C>	T;

		const T	middle = MiddleValue( minValue, maxValue );
		
		return	( value < middle ? 
				( value < minValue ?  T(value) : T(minValue) ) :
				( value > maxValue ?  T(value) : T(maxValue) ) );
	}
	
	template <typename A, typename B, typename C, usize I>
	inline Vec< typename CompileTime::MainType<A,B,C>, I >  ClampOut (const Vec<A,I>& value, const Vec<B,I>& minValue, const Vec<C,I>& maxValue)
	{
		typedef typename CompileTime::MainType<A,B,C>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = ClampOut( value[i], minValue[i], maxValue[i] );
		return ret;
	}
	
	template <typename A, typename B, typename C, usize I>
	inline Vec< typename CompileTime::MainType<A,B,C>, I >  ClampOut (const Vec<A,I>& value, const B& minValue, const C& maxValue)
	{
		typedef typename CompileTime::MainType<A,B,C>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = ClampOut( value[i], minValue, maxValue );
		return ret;
	}
	
/*
=================================================
	Wrap
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T, bool isInteger>
		struct _Wrap_Impl
		{
			forceinline static T Get (const T& value, const T& minValue, const T& maxValue)
			{
				// check for NaN
				if ( minValue >= maxValue )
					return minValue;

				T	result = T( minValue + Mod( value - minValue, maxValue - minValue ) );
		
				if ( result < minValue )
					result += (maxValue - minValue);

				return result;
			}
		};

		template <typename T>
		struct _Wrap_Impl<T, true>
		{
			forceinline static T Get (const T& value, const T& minValue, const T& maxValue)
			{
				return _Wrap_Impl<T, false>::Get( value, minValue, maxValue + T(1) );
			}
		};
	}
	
	template <typename A, typename B, typename C>
	forceinline typename CompileTime::MainType<A,B,C>  Wrap (const A& value, const B& minValue, const C& maxValue)
	{
		// Warning: float value never equal maxValue!
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and
					   CompileTime::IsScalarOrEnum<B> and
					   CompileTime::IsScalarOrEnum<C> );

		typedef typename CompileTime::MainType<A,B,C>	T;

		ASSERT( minValue <= maxValue );

		return _math_hidden_::_Wrap_Impl< T, CompileTime::IsInteger<T> >::Get( T(value), T(minValue), T(maxValue) );
	}

	template <typename A, typename B, typename C, usize I>
	inline Vec< typename CompileTime::MainType<A,B,C>, I >  Wrap (const Vec<A,I>& value, const Vec<B,I>& minValue, const Vec<C,I>& maxValue)
	{
		typedef typename CompileTime::MainType<A,B,C>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Wrap( value[i], minValue[i], maxValue[i] );
		return ret;
	}

	template <typename A, typename B, typename C, usize I>
	inline Vec< typename CompileTime::MainType<A,B,C>, I >  Wrap (const Vec<A,I>& value, const B& minValue, const C& maxValue)
	{
		typedef typename CompileTime::MainType<A,B,C>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Wrap( value[i], minValue, maxValue );
		return ret;
	}
	
/*
=================================================
	helpers
=================================================
*/
	namespace _math_hidden_
	{
		template <usize C>
		struct _AnyAllMost_impl
		{
			template <typename T, usize I>	forceinline static bool Any (const GXMath::Vec<T,I> &v)		{ return _AnyAllMost_impl<C-1>::Any( v ) | GXMath::IsNotZero( v[C] ); }
			template <typename T, usize I>	forceinline static bool All (const GXMath::Vec<T,I> &v)		{ return _AnyAllMost_impl<C-1>::All( v ) & GXMath::IsNotZero( v[C] ); }
		};
		
		template <>
		struct _AnyAllMost_impl<0>
		{
			template <typename T, usize I>	forceinline static bool Any (const GXMath::Vec<T,I> &v)		{ return GXMath::IsNotZero( v[0] ); }
			template <typename T, usize I>	forceinline static bool All (const GXMath::Vec<T,I> &v)		{ return GXMath::IsNotZero( v[0] ); }
		};

		template <typename T, usize I>
		struct _AnyAllMost
		{
			forceinline static bool  All(const GXMath::Vec<T,I> &v)		{ return _AnyAllMost_impl<I-1>::All( v ); }
			forceinline static bool  Any(const GXMath::Vec<T,I> &v)		{ return _AnyAllMost_impl<I-1>::Any( v ); }
		};
	}
	
/*
=================================================
	All
=================================================
*/
	template <typename T>
	forceinline bool All (const T& x)
	{
		return IsNotZero( x );
	}

	template <typename T, usize I>
	forceinline bool All (const Vec<T,I> &x)
	{
		return _math_hidden_::_AnyAllMost<T,I>::All( x );
	}

/*
=================================================
	Any
=================================================
*/
	template <typename T>
	forceinline bool Any (const T& x)
	{
		return IsNotZero( x );
	}

	template <typename T, usize I>
	forceinline bool Any (const Vec<T,I> &x)
	{
		return _math_hidden_::_AnyAllMost<T,I>::Any( x );
	}
	
/*
=================================================
	Max
=================================================
*/
	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B>  Max (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;
		return a > b ? T(a) : T(b);
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  Max (const Vec<A,I> &a, const Vec<B,I> &b)
	{
		typedef typename CompileTime::MainType<A,B>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Max( a[i], b[i] );
		return ret;
	}
	
	template <typename A, typename B, typename C>
	forceinline typename CompileTime::MainType<A,B,C>  Max (const A& a, const B& b, const C& c)
	{
		return Max( Max( a, b ), c );
	}
	
	template <typename A, typename B, typename C, typename D>
	forceinline typename CompileTime::MainType<A,B,C,D>  Max (const A& a, const B& b, const C& c, const D& d)
	{
		return Max( Max( a, b ), Max( c, d ) );
	}
	
/*
=================================================
	Min
=================================================
*/
	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B>  Min (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;
		return a > b ? T(b) : T(a);
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  Min (const Vec<A,I> &a, const Vec<B,I> &b)
	{
		typedef typename CompileTime::MainType<A,B>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Min( a[i], b[i] );
		return ret;
	}
	
	template <typename A, typename B, typename C>
	forceinline typename CompileTime::MainType<A,B,C>  Min (const A& a, const B& b, const C& c)
	{
		return Min( Min( a, b ), c );
	}
	
	template <typename A, typename B, typename C, typename D>
	forceinline typename CompileTime::MainType<A,B,C,D>  Min (const A& a, const B& b, const C& c, const D& d)
	{
		return Min( Min( a, b ), Min( c, d ) );
	}
	
/*
=================================================
	MinAbs
=================================================
*/
	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B>  MinAbs (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;
		return Abs(a) > Abs(b) ? T(b) : T(a);
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  MinAbs (const Vec<A,I> &a, const Vec<B,I> &b)
	{
		typedef typename CompileTime::MainType<A,B>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = MinAbs( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	MaxAbs
=================================================
*/
	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B> MaxAbs (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;
		return Abs(a) > Abs(b) ? T(a) : T(b);
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  MaxAbs (const Vec<A,I> &a, const Vec<B,I> &b)
	{
		typedef typename CompileTime::MainType<A,B>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = MaxAbs( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	MinMax
=================================================
*/
	template <typename A, typename B>
	inline void	MinMax (OUT typename CompileTime::MainType<A,B>& tMin,
						OUT typename CompileTime::MainType<A,B>& tMax,
						const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;

		const bool	res = ( a > b );
		tMin = res ? T(b) : T(a);
		tMax = res ? T(a) : T(b);
	}

	template <typename A, typename B, usize I>
	inline void	MinMax (OUT Vec< typename CompileTime::MainType<A,B>, I >& tMin,
						OUT Vec< typename CompileTime::MainType<A,B>, I >& tMax,
						const Vec<A,I>& a, const Vec<B,I>& b)
	{
		FOR( i, a ) {
			MinMax( tMin[i], tMax[i], a[i], b[i] );
		}
	}
	
/*
=================================================
	MinMag
=================================================
*/
	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B>  MinMag (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;

		if ( Abs(a) > Abs(b) )	return T(b);
		if ( Abs(a) < Abs(b) )	return T(a);
		return Min( a, b );
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  MinMag (const Vec<A,I>& a, const Vec<B,I>& b)
	{
		typedef typename CompileTime::MainType<A,B>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = MinMag( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	MaxMag
=================================================
*/
	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B>  MaxMag (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>	T;

		if ( Abs(a) > Abs(b) )	return T(a);
		if ( Abs(a) < Abs(b) )	return T(b);
		return Max( a, b );
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  MaxMag (const Vec<A,I>& a, const Vec<B,I>& b)
	{
		typedef typename CompileTime::MainType<A,B>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = MaxMag( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	Mid
=================================================
*/
	template <typename A, typename B, typename C>
	forceinline typename CompileTime::MainType<A,B,C>  Mid (const A& a, const B& b, const C& c)
	{
		return Min( Max( a, b ), Max( b, c ) );
	}

	template <typename A, typename B, typename C, usize I>
	inline Vec< typename CompileTime::MainType<A,B,C>, I >  Mid (const Vec<A,I>& a, const Vec<B,I>& b, const Vec<C,I>& c)
	{
		typedef typename CompileTime::MainType<A,B,C>	T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Mid( a[i], b[i], c[i] );
		return ret;
	}
	
/*
=================================================
	Pow
=================================================
*/
	template <typename T, typename B>
	forceinline T Pow (const T&x, const B& y)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> and CompileTime::IsScalarOrEnum<B> );
		ASSERT( x >= T(0) or y == Floor(y) );	// if x < 0 and y not integer then result is NaN

		typedef typename CompileTime::MainType<T,B>				main_t;
		typedef typename _math_hidden_::ToNearFloat< main_t >	_float_t;
		return (T) ::pow( _float_t( x ), _float_t( y ) );
	}

	template <typename T, typename B, usize I>
	inline Vec<T,I>  Pow (const Vec<T,I>& x, const Vec<B,I>& y)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Pow( x[i], y[i] );
		return ret;
	}
	
/*
=================================================
	Ln
=================================================
*/
	template <typename T>
	forceinline T Ln (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::log( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Ln (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Ln( x[i] );
		return ret;
	}


	// deprecated
	namespace _math_hidden_
	{
		template <uint Base, typename T>
		inline T Log (const T& x)
		{
			return Log( x, T( Base ) );
		}

		template <uint Base, typename T, usize I>
		inline Vec<T,I>  Log (const Vec<T,I>& x)
		{
			Vec<T,I>		ret;
			FOR( i, ret )	ret[i] = Log<Base>( x[i] );
			return ret;
		}

	}	// _math_hidden_

	
/*
=================================================
	Log
=================================================
*/
	template <typename T>
	forceinline T  Log (const T& x, const T& base)
	{
		return Ln( x ) / Ln( base );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Log (const Vec<T,I>& x, const T& base)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Log( x[i], base );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  Log (const Vec<T,I>& x, const Vec<T,I>& base)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Log( x[i], base[i] );
		return ret;
	}
	
/*
=================================================
	Log2
=================================================
*/
	template <typename T>
	forceinline T Log2 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		static const T	base = Ln( T(2) );
		return Ln( x ) / base;
	}

	template <typename T, usize I>
	inline Vec<T,I>  Log2 (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Log2( x[i] );
		return ret;
	}

/*
=================================================
	Log10
=================================================
*/
	template <typename T>
	forceinline T Log10 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::log10( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Log10 (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Log10( x[i] );
		return ret;
	}
	
/*
=================================================
	Exp
=================================================
*/
	template <typename T>
	forceinline T Exp (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::exp( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Exp (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Exp( x[i] );
		return ret;
	}

/*
=================================================
	Exp2
=================================================
*/
	template <typename T>
	forceinline T Exp2 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::pow( _float_t( 2 ), _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Exp2 (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Exp2( x[i] );
		return ret;
	}
	
/*
=================================================
	Exp10
=================================================
*/
	template <typename T>
	forceinline T  Exp10 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::pow( _float_t( 10 ), _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Exp10 (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Exp10( x[i] );
		return ret;
	}
	
/*
=================================================
	CeilPowerOfTwo
=================================================
*/
	template <typename T>
	forceinline T CeilPowerOfTwo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( T(1) << ( IntLog2(x) + not IsPowerOfTwo(x) ) ) * Sign(x);
	}

	template <typename T, usize I>
	inline Vec<T,I>  CeilPowerOfTwo (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = CeilPowerOfTwo( x[i] );
		return ret;
	}

/*
=================================================
	FloorPowerOfTwo
=================================================
*/
	template <typename T>
	forceinline T FloorPowerOfTwo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( T(1) << IntLog2(x) ) * Sign(x);
	}

	template <typename T, usize I>
	inline Vec<T,I>  FloorPowerOfTwo (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = FloorPowerOfTwo( x[i] );
		return ret;
	}

/*
=================================================
	NearPowerOfTwo
=================================================
*/
	template <typename T>
	forceinline T NearPowerOfTwo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		
		const T		absx	= Abs( x );
		const uint	pot		= IntLog2( x );
		const T		a		= T(1) << (pot + 1);
		const T		b		= T(1) << pot;
		const T		c		= ((a - b) >> 1) + b;
		return ( absx >= c ? a : b ) * Sign( x );
	}

	template <typename T, usize I>
	inline Vec<T,I>  NearPowerOfTwo (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = NearPowerOfTwo( x[i] );
		return ret;
	}
	
/*
=================================================
	IsPowerOfTwo
=================================================
*/
	template <typename T>
	forceinline bool IsPowerOfTwo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		
		const T	a = Abs( x );
		return ( a != 0 and ( (a & (a - 1)) == 0 ) );
	}

	template <typename T, usize I>
	inline Vec<bool,I>  IsPowerOfTwo (const Vec<T,I> &x)
	{
		Vec<bool,I>		ret;
		FOR( i, ret )	ret[i] = IsPowerOfTwo( x[i] );
		return ret;
	}
	
/*
=================================================
	Square
=================================================
*/
	template <typename T>
	forceinline T Square (const T& x)
	{
		return x * x;
	}

	template <typename T, usize I>
	inline Vec<T,I>  Square (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Square( x[i] );
		return ret;
	}
	
/*
=================================================
	Sqrt
=================================================
*/
	template <typename T>
	forceinline T Sqrt (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		ASSERT( x >= T(0) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::sqrt( _float_t( x ) );
	}

#ifdef GX_FAST_MATH
	template <>
	forceinline T Sqrt (const float& x)
	{
		return FSqrt( x );
	}
#endif

	template <typename T, usize I>
	inline Vec<T,I>  Sqrt (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Sqrt( x[i] );
		return ret;
	}
	
/*
=================================================
	InvSqrt
=================================================
*/
	template <typename T>
	forceinline T InvSqrt (const T& x)
	{
		return T(1) / Sqrt( x );
	}

#ifdef GX_FAST_MATH
	template <>
	forceinline float InvSqrt (const float &x)
	{
		return FInvSqrt( x );
	}
#endif

	template <typename T, usize I>
	inline Vec<T,I>  InvSqrt (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = InvSqrt( x[i] );
		return ret;
	}
	
/*
=================================================
	SquareSign
=================================================
*/
	template <typename T>
	forceinline T SquareSign (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return ( x < T(0) ? -Square(x) : Square(x) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  SquareSign (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SquareSign( x[i] );
		return ret;
	}

	
	
//-------------------------------------------------------------------
//	Other

	
/*
=================================================
	helpers
=================================================
*/
	namespace _math_hidden_
	{
		template <typename A, typename B, bool IsInt>
		struct ModNearType
		{
			typedef typename CompileTime::MainType<A,B>		main_t;

			forceinline static main_t  Mod (const A& l, const B& r)
			{
				typedef _math_hidden_::ToNearFloat< main_t >	_float_t;
				return (main_t) ::fmod( _float_t(l), _float_t(r) );
			}

			template <typename C>
			forceinline static main_t  SafeMod (const A& l, const B& r, const C& def)
			{
				typedef _math_hidden_::ToNearFloat< main_t >	_float_t;
				return IsNotZero( r ) ? (main_t) ::fmod( _float_t(l), _float_t(r) ) : maint_t(def);
			}
		};

		template <typename A, typename B>
		struct ModNearType<A,B,true>
		{
			typedef typename CompileTime::MainType<A,B>		main_t;

			forceinline static main_t  Mod (const A& l, const B& r)
			{
				return main_t( main_t(l) % main_t(r) );
			}

			template <typename C>
			forceinline static main_t  SafeMod (const A& l, const B& r, const C& def)
			{
				return IsNotZero( r ) ? main_t( main_t(l) % main_t(r) ) : main_t(def);
			}
		};

	}	// _math_hidden_

/*
=================================================
	Mod
=================================================
*/
	template <typename A, typename B>
	forceinline typename CompileTime::MainType<A,B>  Mod (const A& left, const B& right)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>		T;

		return _math_hidden_::ModNearType< A, B, CompileTime::IsInteger<T> >::Mod( left, right );
	}

	template <typename A, typename B, usize I>
	inline Vec< typename CompileTime::MainType<A,B>, I >  Mod (const Vec<A,I> &left, const Vec<B,I> &right)
	{
		typedef typename CompileTime::MainType<A,B>		T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Mod( left[i], right[i] );
		return ret;
	}

/*
=================================================
	SafeMod
=================================================
*/
	template <typename A, typename B, typename C>
	forceinline typename CompileTime::MainType<A,B>  SafeMod (const A& left, const B& right, const C& defValue)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::MainType<A,B>		T;

		return _math_hidden_::ModNearType< A, B, CompileTime::IsInteger<T> >::SafeMod( left, right, defValue );
	}

	template <typename A, typename B, usize I, typename C>
	inline Vec< typename CompileTime::MainType<A,B>, I >  SafeMod (const Vec<A,I> &left, const Vec<B,I> &right, const C& defValue)
	{
		typedef typename CompileTime::MainType<A,B>		T;
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SafeMod( left[i], right[i], defValue );
		return ret;
	}
	
/*
=================================================
	Floor
=================================================
*/
	template <typename T>
	forceinline T  Floor (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::floor( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Floor (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Floor( x[i] );
		return ret;
	}

/*
=================================================
	Ceil
=================================================
*/
	template <typename T>
	forceinline T  Ceil (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::ceil( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Ceil (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Ceil( x[i] );
		return ret;
	}
	
/*
=================================================
	Fract
----
	GLSL-style fract wich return value in range 0..1
=================================================
*/
	template <typename T>
	forceinline T  Fract (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

	#if 1
		return x - Floor(x);	// GLSL style 0..1

	#elif 0
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		_float_t	tmp;
		return (T) ::modf( _float_t( x ), &tmp );	// -1..1

	#elif 0

		return x - Trunc(x);	// -1..1
	#endif
	}

	template <typename T, usize I>
	inline Vec<T,I>  Fract (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Fract( x[i] );
		return ret;
	}

/*
=================================================
	Trunc
=================================================
*/
	template <typename T>
	forceinline T  Trunc (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

	#if 1
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::trunc( _float_t( x ) );

	#else
		return CopySign( x, Floor( Abs(x) ) );	// if trunc not supported
	#endif
	}

	template <typename T, usize I>
	inline Vec<T,I>  Trunc (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Trunc( x[i] );
		return ret;
	}
	
/*
=================================================
	ModF
=================================================
*/
	template <typename T>
	struct ModF_Result
	{
		T	intgr;
		T	fract;

		ModF_Result (T i, T f) : intgr(i), fract(f) {}
	};

	template <typename T, usize I>
	struct ModF_Result< Vec<T,I> >
	{
		Vec<T,I>	intgr;
		Vec<T,I>	fract;
	};

	template <typename T>
	forceinline ModF_Result<T>  ModF (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

		_float_t	i;
		_float_t	f = ::modf( _float_t( x ), &i );

		return ModF_Result<T>( i, f );
	}

	template <typename T, usize I>
	inline ModF_Result< Vec<T,I> >  ModF (const Vec<T,I> &x)
	{
		ModF_Result< Vec<T,I> >		ret;

		FOR( i, ret )
		{
			auto tmp = ModF( x[i] );

			ret.intgr[i] = tmp.intgr;
			ret.fract[i] = tmp.fract;
		}
		return ret;
	}
	
/*
=================================================
	Round
=================================================
*/
	namespace _math_hidden_
	{
		template <bool ResultIsInteger, bool ValueIsInteger>
		struct _TRound	// int to int, int to float
		{
			template <typename R, typename T>
			static forceinline R Round (const T& val)
			{
				return R( val );
			}
		};
		
		template <>
		struct _TRound< true, false >	// float to int
		{
			template <typename R, typename T>
			static forceinline R Round (const T& val)
			{
				typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

				return (R) ::round( _float_t( val ) );
				//return R( val + ( val > T(0) ? T(0.5) : T(-0.5) ) );	// if round not supported
			}
		};
		
		template <>
		struct _TRound< false, false >	// float to float
		{
			template <typename R, typename T>
			static forceinline R Round (const T& val)
			{
				typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

				return (R) ::round( _float_t( val ) );
				//return (R) Trunc( val + ( val > T(0) ? T(0.5) : T(-0.5) ) );	// if round not supported
			}
		};

		template <typename R, typename T>
		forceinline R _Round (const T& x)
		{
			return _TRound<	CompileTime::IsInteger<R>, CompileTime::IsInteger<T> >::template Round< R, T >( x );
		}

	}	// _math_hidden_

	template <typename T>
	forceinline T Round (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		//CompileTime::MustBeFloat<T>();

		return _math_hidden_::_Round< T, T >( x );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Round (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Round( x[i] );
		return ret;
	}

/*
=================================================
	RoundTo
=================================================
*/
	template <typename R, typename T>
	forceinline R RoundTo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		//CompileTime::MustBeFloat<T>();

		return _math_hidden_::_Round< R, T >( x );
	}

	template <typename R, typename T, usize I>
	inline Vec<R,I>  RoundTo (const Vec<T,I> &x)
	{
		Vec<R,I>		ret;
		FOR( i, ret )	ret[i] = RoundTo<R>( x[i] );
		return ret;
	}
	
/*
=================================================
	RoundToInt
=================================================
*/
	template <typename T>
	forceinline typename CompileTime::NearInt::FromType<T>  RoundToInt (const T& val)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		return _math_hidden_::_Round< typename CompileTime::NearInt::FromType<T>, T >( val );
	}

	template <typename T, usize I>
	inline Vec< typename CompileTime::NearInt::FromType<T>, I >  RoundToInt (const Vec<T,I> &x)
	{
		Vec< typename CompileTime::NearInt::FromType<T>, I >	ret;
		FOR( i, ret )	ret[i] = RoundToInt( x[i] );
		return ret;
	}

/*
=================================================
	RoundToUInt
=================================================
*/
	template <typename T>
	forceinline typename CompileTime::NearUInt::FromType<T>  RoundToUInt (const T& val)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		return _math_hidden_::_Round< typename CompileTime::NearUInt::FromType<T>, T >( val );
	}

	template <typename T, usize I>
	inline Vec< typename CompileTime::NearUInt::FromType<T>, I >  RoundToUInt (const Vec<T,I> &x)
	{
		Vec< typename CompileTime::NearUInt::FromType<T>, I >	ret;
		FOR( i, ret )	ret[i] = RoundToUInt( x[i] );
		return ret;
	}

/*
=================================================
	RoundTo
----
	round to base
=================================================
*/
	template <typename T>
	forceinline T  RoundTo (const T& x, const T& base)
	{
		return Round( x / base ) * base;
	}

	template <typename T, usize I>
	inline Vec<T,I>  RoundTo (const Vec<T,I> &x, const T& base)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = RoundTo( x[i] );
		return ret;
	}
	
/*
=================================================
	SafeDiv
=================================================
*/
	template <typename T>
	forceinline T SafeDiv (const T& left, const T& right, const T& defVal)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return IsNotZero(right) ? (left / right) : defVal;
	}

	template <typename T, usize I>
	inline Vec<T,I>  SafeDiv (const Vec<T,I> &left, const Vec<T,I> &right, const T& defVal)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SafeDiv( left[i], right[i], defVal );
		return ret;
	}
	
/*
=================================================
	IntFactorial
=================================================
*/
	template <typename T>
	forceinline T  IntFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );

		typedef CompileTime::NearUInt::FromType<T>	uint_t;

		const uint_t	count	= RoundToUInt( x );
		T				result  = T(1);

		for (uint_t i = 2; i <= count; ++i) {
			result *= T(i);
		}
		return result;
	}

	template <typename T, usize I>
	inline Vec<T,I>  IntFactorial (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = IntFactorial( x[i] );
		return ret;
	}
	
/*
=================================================
	IntSuperFactorial
=================================================
*/
	template <typename T>
	forceinline T  IntSuperFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );
		
		typedef CompileTime::NearUInt::FromType<T>	uint_t;

		const uint_t	count	= RoundToUInt( x );
		T				result  = T(1);

		for (uint_t i = 1; i < count; ++i) {
			result *= Pow( T(count-i+1), T(i) );
		}
		return result;
	}

	template <typename T, usize I>
	inline Vec<T,I>  IntSuperFactorial (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = IntSuperFactorial( x[i] );
		return ret;
	}
	
/*
=================================================
	IntHyperFactorial
=================================================
*/
	template <typename T>
	forceinline T  IntHyperFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );
		
		typedef CompileTime::NearUInt::FromType<T>	uint_t;

		const uint_t	count	= RoundToUInt( x );
		T				result  = T(1);

		for (uint_t i = 2; i <= count; ++i) {
			result *= Pow( T(i), T(i) );
		}
		return result;
	}

	template <typename T, usize I>
	inline Vec<T,I>  IntHyperFactorial (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = IntHyperFactorial( x[i] );
		return ret;
	}

/*
=================================================
	IntDoubleFactorial
=================================================
*/
	template <typename T>
	forceinline T  IntDoubleFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );
		
		typedef CompileTime::NearUInt::FromType<T>	uint_t;

		const uint_t	val		= RoundToUInt( x );
		const uint_t	count	= val / 2;
		T				result  = T(1);

		for (uint_t i = 0; i < count; ++i) {
			result *= ( T(val) - T(2*i) );
		}
		return result;
	}

	template <typename T, usize I>
	inline Vec<T,I>  IntDoubleFactorial (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = IntDoubleFactorial( x[i] );
		return ret;
	}
	
/*
=================================================
	IntGammaFunction
=================================================
*/
	template <typename T>
	forceinline T  IntGammaFunction (const T& x)
	{
		return IntFactorial( x - T(1) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  IntGammaFunction (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = IntGammaFunction( x[i] );
		return ret;
	}

/*
=================================================
	IntLnGammaFunction
=================================================
*/
	template <typename T>
	forceinline T  IntLnGammaFunction (const T& x)
	{
		return Ln( Abs( IntGammaFunction( x ) ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  IntLnGammaFunction (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = IntLnGammaFunction( x[i] );
		return ret;
	}
	
/*
=================================================
	Factorial
=================================================
*/
	template <typename T>
	forceinline T  Factorial (const T& x)
	{
		return (T) FastFactorial( double(x) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Factorial (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Factorial( x[i] );
		return ret;
	}

/*
=================================================
	Gamma
=================================================
*/
	template <typename T>
	forceinline T  Gamma (const T& x)
	{
		return (T) FastGammaFunction( double(x) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Gamma (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Gamma( x[i] );
		return ret;
	}

/*
=================================================
	LnGamma
=================================================
*/
	template <typename T>
	forceinline T LnGamma (const T& x)
	{
		return (T) FastLnGammaFunction( double(x) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  LnGamma (const Vec<T,I>& x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = LnGammaFunction( x[i] );
		return ret;
	}
	
/*
=================================================
	IsOdd
=================================================
*/
	template <typename T>
	forceinline bool  IsOdd (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x & T(1) ) == T(1);
	}

	template <typename T, usize I>
	inline Vec<bool,I>  IsOdd (const Vec<T,I> &x)
	{
		Vec<bool,I>		ret;
		FOR( i, ret )	ret[i] = IsOdd( x[i] );
		return ret;
	}

/*
=================================================
	IsEven
=================================================
*/
	template <typename T>
	forceinline bool  IsEven (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x & T(1) ) == T(0);
	}

	template <typename T, usize I>
	inline Vec<bool,I>  IsEven (const Vec<T,I> &x)
	{
		Vec<bool,I>		ret;
		FOR( i, ret )	ret[i] = IsEven( x[i] );
		return ret;
	}
	
/*
=================================================
	ToOdd
=================================================
*/
	template <typename T>
	forceinline T  ToOdd (const T &x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x | T(1) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  ToOdd (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = ToOdd( x[i] );
		return ret;
	}

/*
=================================================
	ToEven
=================================================
*/
	template <typename T>
	forceinline T  ToEven (const T &x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x & ~T(1) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  ToEven (const Vec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = ToEven( x[i] );
		return ret;
	}
	
/*
=================================================
	Step
=================================================
*/
	template <typename T>
	forceinline T  Step (const T& x, const T& edge)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		return x < edge ? T(0) : T(1);
	}

	template <typename T, usize I>
	inline Vec<T,I>  Step (const Vec<T,I> &x, const Vec<T,I> &edge)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Step( x[i], edge[i] );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  Step (const Vec<T,I> &x, const T &edge)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Step( x[i], edge );
		return ret;
	}

/*
=================================================
	LinearStep
=================================================
*/
	template <typename T>
	forceinline T  LinearStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		return Clamp( (x - edge0) / (edge1 - edge0), T(0), T(1) );
	}
	
	template <typename T, usize I>
	inline Vec<T,I>  LinearStep (const Vec<T,I> &x, const Vec<T,I> &edge0, const Vec<T,I> &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = LinearStep( x[i], edge0[i], edge1[i] );
		return ret;
	}
	
	template <typename T, usize I>
	inline Vec<T,I>  LinearStep (const Vec<T,I> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = LinearStep( x[i], edge0, edge1 );
		return ret;
	}

/*
=================================================
	SmoothStep
=================================================
*/
	template <typename T>
	forceinline T  SmoothStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		T t = Clamp( (x - edge0) / (edge1 - edge0), T(0), T(1) );
		return t * t * (T(3) - T(2) * t);
	}

	template <typename T, usize I>
	inline Vec<T,I>  SmoothStep (const Vec<T,I> &x, const Vec<T,I> &edge0, const Vec<T,I> &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SmoothStep( x[i], edge0[i], edge1[i] );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  SmoothStep (const Vec<T,I> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SmoothStep( x[i], edge0, edge1 );
		return ret;
	}

/*
=================================================
	BumpStep
=================================================
*/
	template <typename T>
	forceinline T  BumpStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		return T(1) - Abs( LinearStep( x, edge0, edge1 ) - T(0.5) ) * T(2);
	}

	template <typename T, usize I>
	inline Vec<T,I>  BumpStep (const Vec<T,I> &x, const Vec<T,I> &edge0, const Vec<T,I> &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = BumpStep( x[i], edge0[i], edge1[i] );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  BumpStep (const Vec<T,I> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = BumpStep( x[i], edge0, edge1 );
		return ret;
	}

/*
=================================================
	SmoothBumpStep
=================================================
*/
	template <typename T>
	forceinline T  SmoothBumpStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		T	t = BumpStep( x, edge0, edge1 );
		return t * t * (T(3) - T(2) * t);
	}

	template <typename T, usize I>
	inline Vec<T,I>  SmoothBumpStep (const Vec<T,I> &x, const Vec<T,I> &edge0, const Vec<T,I> &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SmoothBumpStep( x[i], edge0[i], edge1[i] );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  SmoothBumpStep (const Vec<T,I> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SmoothBumpStep( x[i], edge0, edge1 );
		return ret;
	}

/*
=================================================
	AlignToLarge
----
	align to largest value
=================================================
*/
	template <typename T>
	forceinline constexpr T  AlignToLarge (const T& value, const usize align)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return T( ((value + (T(align)-1)) / T(align)) * T(align) );
	}

	template <typename T, usize I>
	Vec<T,I>  AlignToLarge (const Vec<T,I>& value, const usize align)
	{
		Vec<T,I> res;
		FOR( i, res )	res[i] = AlignToLarge( value[i], align );
		return res;
	}

	template <typename T>
	forceinline constexpr T  AlignToLarge (const T& value, const BytesU align)
	{
		return AlignToLarge( value, (usize)align );
	}


}	// GXMath
}	// GX_STL
