// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/Trigonometry.h"
#include "Core/STL/Dimensions/ByteAndBit.h"
#include "Core/STL/Math/FastMath.h"
#include "Core/STL/Math/Trigonometry.h"
#include "Core/STL/Algorithms/ArrayUtils.h"
#include "Core/STL/CompileTime/TypeInfoFunc.h"

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
			template <typename T>	forceinline static constexpr T abs (const T& val)
			{
			#if 0
				return std::abs( val );

			#elif 1
				typedef typename CompileTime::NearUInt::FromType<T>	UInt_t;
				const UInt_t mask = ((UInt_t(1) << (CompileTime::SizeOf<T>::bits-1))-1);

				// if val == MinValue result is undefined, remove sign bit anyway
				return std::abs( val ) & mask;

			#elif 0
				// from http://graphics.stanford.edu/~seander/bithacks.html#IntegerAbs
				typedef typename CompileTime::NearUInt::FromType<T>	UInt_t;

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
			template <typename T>	forceinline static constexpr T abs (const T& val)		{ return val; }
		};

		template <>
		struct _Abs_impl<false,false>
		{
			template <typename T>	forceinline static constexpr T abs (const T& val)		{ return val; }
		};

		template <>
		struct _Abs_impl<true,false>
		{
			template <typename T>
			forceinline static T abs (const T& val)
			{
				typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

				return T(std::abs( _float_t(val) ));
			}
		};
		
		template <typename T>
		struct _Abs
		{
			forceinline static constexpr T abs (const T& val)
			{
				return _Abs_impl< CompileTime::IsSigned<T>, CompileTime::IsInteger<T> >::template abs( val );
			}
		};
	}
	
	template <typename T>
	ND_ forceinline constexpr T  Abs (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_Abs<T>::abs( x );
	}

	template <typename T, usize I, ulong U>
	ND_ inline constexpr Vec<T,I,U>  Abs (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
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
	ND_ forceinline T  Sign (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return ( x < T(0) ? T(-1) : T(1) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Sign (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Sign<T>( x[i] );
		return ret;
	}

/*
=================================================
	SignOrZero
----
	returns 1 or 0 or -1
=================================================
*/
	template <typename T>
	ND_ forceinline T  SignOrZero (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return T( x > T(0) ) - T( x < T(0) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SignOrZero (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
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
				typedef typename CompileTime::NearUInt::FromType<T>	UInt_t;
				
				const uint	u_sign_bit	= CompileTime::TypeDescriptor::template GetTypeInfo<T>::SignBit();

				T	ret(x);
				ReferenceCast<UInt_t>( ret ) &= ~( UInt_t(1) << u_sign_bit );
				ReferenceCast<UInt_t>( ret ) |=  ( UInt_t(s) << u_sign_bit );
				return ret;
			}
		};

		template <>
		struct _SetSign_impl< false, true >
		{
			template <typename T>
			static T setsign (const T& x, bool s)
			{
				if ( (  s and x > T(0) ) or
					 ( not s and x < T(0) ) )
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
	ND_ inline T  SetSign (const T &x, bool sign)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_SetSign<T>::setsign( x, sign );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SetSign (const Vec<T,I,U> &x, bool sign)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SetSign( x[i], sign );
		return ret;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SetSign (const Vec<T,I,U> &x, const Vec<bool,I,U> &sign)
	{
		Vec<T,I,U>		ret;
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
				typedef typename CompileTime::NearUInt::FromType<T>	UInt_t;
				
				const UInt_t	u_sign_mask	= UInt_t(1) << CompileTime::TypeDescriptor::template GetTypeInfo<T>::SignBit();

				T	ret(x);
				ReferenceCast<UInt_t>( ret ) &= ~u_sign_mask;
				ReferenceCast<UInt_t>( ret ) |=  ReferenceCast<UInt_t>( s ) & u_sign_mask;
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
	ND_ inline T  CopySign (const T& from, const T& to)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_CopySign<T>::copysign( to, from );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  CopySign (const Vec<T,I,U> &from, const Vec<T,I,U> &to)
	{
		Vec<T,I,U>	ret;
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
			template <typename T>	forceinline static constexpr bool is_zero (const T& x)	{ return x == T(0); }
		};

		template <>
		struct _IsZero_impl<false> {
			template <typename T>	forceinline static constexpr bool is_zero (const T& x)	{ return Abs(x) <= Epsilon<T>(); }
		};

		template <typename T>
		struct _IsZero {
			forceinline static constexpr bool is_zero (const T& x) {
				return _IsZero_impl< CompileTime::IsInteger<T> >::is_zero( x );
			}
		};
	}

	template <typename T>
	ND_ forceinline constexpr bool  IsZero (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_IsZero<T>::is_zero( x );
	}

	template <>
	ND_ forceinline constexpr bool  IsZero (const bool& x)
	{
		return not x;
	}

	template <typename T, usize I, ulong U>
	ND_ forceinline constexpr bool  IsZero (const Vec<T,I,U>& x)
	{
		return All( x.IsZero() );
	}
	
/*
=================================================
	IsNotZero
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr bool  IsNotZero (const T& x)
	{
		return not IsZero( x );
	}

	template <>
	ND_ forceinline constexpr bool  IsNotZero (const bool& x)
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
			forceinline static constexpr bool eq (const T& a, const T& b)
			{ 
				return a == b;
			}

			template <typename T>
			forceinline static constexpr bool eqa (const T& a, const T& b, uint ac)
			{
				typedef typename CompileTime::NearUInt::FromType<T>	UInt_t;
				return UInt_t( Abs( a - b ) ) <= ( UInt_t(1) << ac );
			}
		};

		template <typename T>	struct _FloatAccuracy			{ static constexpr uint	value = CompileTime::SizeOf<T>::bits / 3; };
		template <>				struct _FloatAccuracy<double>	{ static constexpr uint	value = CompileTime::SizeOf<double>::bits * 2 / 3; };

		
		// float
		template <>
		struct _Equals_impl<true>
		{
			template <typename T>
			forceinline static constexpr bool eq (const T& a, const T& b)
			{
				return eqa( a, b, _FloatAccuracy<T>::value );
			}

			template <typename T>
			forceinline static constexpr bool eqa (const T& a, const T& b, uint ac)
			{
				typedef typename CompileTime::NearInt::FromType<T>	Int_t;
				typedef typename CompileTime::NearUInt::FromType<T>	UInt_t;

				Int_t	i_a = ReferenceCast<Int_t>(a);
				Int_t	i_b	= ReferenceCast<Int_t>(b);

				if ( i_a < Int_t(0) )	i_a = MinValue<Int_t>() - i_a;
				if ( i_b < Int_t(0) )	i_b = MinValue<Int_t>() - i_b;

				UInt_t	diff = UInt_t(Abs( i_a - i_b ));

				return ( diff <= ( UInt_t(1) << ac ) );
			}
		};
		
		template <typename T>
		struct _Equals
		{
			forceinline static constexpr bool eq (const T& a, const T& b)
			{
				return _Equals_impl< CompileTime::IsFloat<T> >::eq( a, b );
			}

			forceinline static constexpr bool eqa (const T& a, const T& b, uint ac)
			{
				return _Equals_impl< CompileTime::IsFloat<T> >::eqa( a, b, ac );
			}
		};
	}

	template <typename T>
	ND_ forceinline constexpr bool  Equals (const T& a, const T& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_Equals<T>::eq( a, b );
	}

	template <typename T, usize I, ulong U>
	ND_ inline constexpr Vec<bool,I,U>  Equals (const Vec<T,I,U> &a, const Vec<T,I,U> &b)
	{
		Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = Equals( a[i], b[i] );
		return ret;
	}

/*
=================================================
	EqualsWithError
=================================================
*/
	template <typename T>
	ND_ forceinline bool  EqualsWithError (const T& a, const T& b, /*Bits*/uint accuracy)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return _math_hidden_::_Equals<T>::eqa( a, b, accuracy );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<bool,I,U>  EqualsWithError (const Vec<T,I,U> &a, const Vec<T,I,U> &b, /*Bits*/uint accuracy)
	{
		Vec<bool,I,U>	ret;
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
	ND_ forceinline auto  MiddleValue (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>	T;

		return _math_hidden_::_MiddleValue< T, CompileTime::IsFloat<T>, CompileTime::IsSigned<T> >::Get( T(a), T(b) );
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  MiddleValue (const Vec<A,I,U>& a, const Vec<B,I,U>& b)
	{
		typedef typename CompileTime::GenType<A,B>	T;
		Vec<T,I,U>		ret;
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
	ND_ forceinline auto  Clamp (const A& value, const B& minValue, const C& maxValue)
	{
		ASSERT( minValue <= maxValue );
		return Min( maxValue, Max( value, minValue ) );
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  Clamp (const Vec<A,I,U>& value, const Vec<B,I,U>& minValue, const Vec<C,I,U>& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Clamp( value[i], minValue[i], maxValue[i] );
		return ret;
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  Clamp (const Vec<A,I,U>& value, const B& minValue, const C& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Clamp( value[i], minValue, maxValue );
		return ret;
	}
	
/*
=================================================
	ClampOut
----
	clamp to outer range
=================================================
*/
	template <typename A, typename B, typename C>
	ND_ forceinline auto  ClampOut (const A& value, const B& minValue, const C& maxValue)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and
					   CompileTime::IsScalarOrEnum<B> and
					   CompileTime::IsScalarOrEnum<C> );

		ASSERT( minValue <= maxValue );

		typedef typename CompileTime::GenType<A,B,C>	T;

		const T	middle = MiddleValue( minValue, maxValue );
		
		return	( value < middle ? 
				( value < minValue ?  T(value) : T(minValue) ) :
				( value > maxValue ?  T(value) : T(maxValue) ) );
	}
	
	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  ClampOut (const Vec<A,I,U>& value, const Vec<B,I,U>& minValue, const Vec<C,I,U>& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = ClampOut( value[i], minValue[i], maxValue[i] );
		return ret;
	}
	
	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  ClampOut (const Vec<A,I,U>& value, const B& minValue, const C& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = ClampOut( value[i], minValue, maxValue );
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
			template <typename T, usize I, ulong U>	forceinline static constexpr bool Any (const GXMath::Vec<T,I,U> &v)	{ return _AnyAllMost_impl<C-1>::Any( v ) | GXMath::IsNotZero( v[C] ); }
			template <typename T, usize I, ulong U>	forceinline static constexpr bool All (const GXMath::Vec<T,I,U> &v)	{ return _AnyAllMost_impl<C-1>::All( v ) & GXMath::IsNotZero( v[C] ); }
		};
		
		template <>
		struct _AnyAllMost_impl<0>
		{
			template <typename T, usize I, ulong U>	forceinline static constexpr bool Any (const GXMath::Vec<T,I,U> &v)	{ return GXMath::IsNotZero( v[0] ); }
			template <typename T, usize I, ulong U>	forceinline static constexpr bool All (const GXMath::Vec<T,I,U> &v)	{ return GXMath::IsNotZero( v[0] ); }
		};

		template <typename T, usize I, ulong U>
		struct _AnyAllMost
		{
			forceinline static constexpr bool  All(const GXMath::Vec<T,I,U> &v)		{ return _AnyAllMost_impl<I-1>::All( v ); }
			forceinline static constexpr bool  Any(const GXMath::Vec<T,I,U> &v)		{ return _AnyAllMost_impl<I-1>::Any( v ); }
		};
	}
	
/*
=================================================
	All
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr bool  All (const T& x)
	{
		return IsNotZero( x );
	}

	template <typename T, usize I, ulong U>
	ND_ forceinline constexpr bool  All (const Vec<T,I,U> &x)
	{
		return _math_hidden_::_AnyAllMost<T,I,U>::All( x );
	}

/*
=================================================
	Any
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr bool  Any (const T& x)
	{
		return IsNotZero( x );
	}

	template <typename T, usize I, ulong U>
	ND_ forceinline constexpr bool  Any (const Vec<T,I,U> &x)
	{
		return _math_hidden_::_AnyAllMost<T,I,U>::Any( x );
	}
	
/*
=================================================
	Max
=================================================
*/
	template <typename A, typename B>
	ND_ forceinline auto  Max (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>	T;
		return a > b ? T(a) : T(b);
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  Max (const Vec<A,I,U> &a, const Vec<B,I,U> &b)
	{
		typedef typename CompileTime::GenType<A,B>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Max( a[i], b[i] );
		return ret;
	}
	
	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  Max (const Vec<A,I,U> &a, const B &b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<B> );
		return Max( a, Vec<B,I,U>(b) );
	}
	
	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  Max (const A &a, const Vec<B,I,U> &b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> );
		return Max( Vec<A,I,U>(a), b );
	}

	template <typename A, typename B, typename C>
	ND_ forceinline auto  Max (const A& a, const B& b, const C& c)
	{
		return Max( Max( a, b ), c );
	}
	
	template <typename A, typename B, typename C, typename D>
	ND_ forceinline auto  Max (const A& a, const B& b, const C& c, const D& d)
	{
		return Max( Max( a, b ), Max( c, d ) );
	}
	
/*
=================================================
	Min
=================================================
*/
	template <typename A, typename B>
	ND_ forceinline auto  Min (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>	T;
		return a > b ? T(b) : T(a);
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  Min (const Vec<A,I,U> &a, const Vec<B,I,U> &b)
	{
		typedef typename CompileTime::GenType<A,B>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Min( a[i], b[i] );
		return ret;
	}
	
	template <typename A, typename B, typename C>
	ND_ forceinline auto  Min (const A& a, const B& b, const C& c)
	{
		return Min( Min( a, b ), c );
	}
	
	template <typename A, typename B, typename C, typename D>
	ND_ forceinline auto  Min (const A& a, const B& b, const C& c, const D& d)
	{
		return Min( Min( a, b ), Min( c, d ) );
	}
	
/*
=================================================
	MinAbs
=================================================
*/
	template <typename A, typename B>
	ND_ forceinline auto  MinAbs (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>	T;
		return Abs(a) > Abs(b) ? T(b) : T(a);
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  MinAbs (const Vec<A,I,U> &a, const Vec<B,I,U> &b)
	{
		typedef typename CompileTime::GenType<A,B>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = MinAbs( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	MaxAbs
=================================================
*/
	template <typename A, typename B>
	ND_ forceinline auto  MaxAbs (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>	T;
		return Abs(a) > Abs(b) ? T(a) : T(b);
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  MaxAbs (const Vec<A,I,U> &a, const Vec<B,I,U> &b)
	{
		typedef typename CompileTime::GenType<A,B>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = MaxAbs( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	MinMax_Result
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		struct _MinMaxResult
		{
			T	min;
			T	max;
		};

	}	// _math_hidden_
	
/*
=================================================
	MinMax
=================================================
*/
	template <typename A, typename B>
	inline auto  MinMax (const A& a, const B& b)
	{
		return _math_hidden_::_MinMaxResult< typename CompileTime::GenType<A,B> >{ Min(a,b), Max(a,b) };
	}

	template <typename A, typename B, typename C>
	inline auto  MinMax (const A& a, const B& b, const C &c)
	{
		return _math_hidden_::_MinMaxResult< typename CompileTime::GenType<A,B,C> >{ Min(a,b,c), Max(a,b,c) };
	}

	template <typename A, typename B, typename C, typename D>
	inline auto  MinMax (const A& a, const B& b, const C &c, const D &d)
	{
		return _math_hidden_::_MinMaxResult< typename CompileTime::GenType<A,B,C,D> >{ Min(a,b,c,d), Max(a,b,c,d) };
	}


	template <typename A, typename B, usize I, ulong U>
	inline auto  MinMax (const Vec<A,I,U>& a, const Vec<B,I,U>& b)
	{
		return _math_hidden_::_MinMaxResult< Vec< typename CompileTime::GenType<A,B> ,I,U> >{ Min(a,b), Max(a,b) };
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	inline auto  MinMax (const Vec<A,I,U>& a, const Vec<B,I,U>& b, const Vec<C,I,U>& c)
	{
		return _math_hidden_::_MinMaxResult< Vec< typename CompileTime::GenType<A,B,C> ,I,U> >{ Min(a,b,c), Max(a,b,c) };
	}

	template <typename A, typename B, typename C, typename D, usize I, ulong U>
	inline auto  MinMax (const Vec<A,I,U>& a, const Vec<B,I,U>& b, const Vec<C,I,U>& c, const Vec<D,I,U>& d)
	{
		return _math_hidden_::_MinMaxResult< Vec< typename CompileTime::GenType<A,B,C,D> ,I,U> >{ Min(a,b,c,d), Max(a,b,c,d) };
	}
	
/*
=================================================
	MinMag
=================================================
*/
	template <typename A, typename B>
	ND_ forceinline auto  MinMag (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>	T;

		if ( Abs(a) > Abs(b) )	return T(b);
		if ( Abs(a) < Abs(b) )	return T(a);
		return Min( a, b );
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  MinMag (const Vec<A,I,U>& a, const Vec<B,I,U>& b)
	{
		typedef typename CompileTime::GenType<A,B>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = MinMag( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	MaxMag
=================================================
*/
	template <typename A, typename B>
	ND_ forceinline auto  MaxMag (const A& a, const B& b)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>	T;

		if ( Abs(a) > Abs(b) )	return T(a);
		if ( Abs(a) < Abs(b) )	return T(b);
		return Max( a, b );
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  MaxMag (const Vec<A,I,U>& a, const Vec<B,I,U>& b)
	{
		typedef typename CompileTime::GenType<A,B>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = MaxMag( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	Mid
=================================================
*/
	template <typename A, typename B, typename C>
	ND_ forceinline auto  Mid (const A& a, const B& b, const C& c)
	{
		return Min( Max( a, b ), Max( b, c ) );
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  Mid (const Vec<A,I,U>& a, const Vec<B,I,U>& b, const Vec<C,I,U>& c)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Mid( a[i], b[i], c[i] );
		return ret;
	}
	
/*
=================================================
	Pow
=================================================
*/
	template <typename T, typename B>
	ND_ forceinline T  Pow (const T&x, const B& y)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> and CompileTime::IsScalarOrEnum<B> );
		ASSERT( x >= T(0) or y == Floor(y) );	// if x < 0 and y not integer then result is NaN

		typedef typename CompileTime::GenType<T,B>				main_t;
		typedef typename _math_hidden_::ToNearFloat< main_t >	_float_t;
		return T(std::pow( _float_t( x ), _float_t( y ) ));
	}

	template <typename T, typename B, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Pow (const Vec<T,I,U>& x, const Vec<B,I,U>& y)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Pow( x[i], y[i] );
		return ret;
	}
	
/*
=================================================
	Ln
=================================================
*/
	template <typename T>
	ND_ forceinline T  Ln (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::log( _float_t( x ) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Ln (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Ln( x[i] );
		return ret;
	}


	namespace _math_hidden_
	{
		template <uint Base, typename T>
		GX_DEPRECATED("")
		inline T Log (const T& x)
		{
			return Log( x, T( Base ) );
		}

		template <uint Base, typename T, usize I, ulong U>
		GX_DEPRECATED("")
		inline Vec<T,I,U>  Log (const Vec<T,I,U>& x)
		{
			Vec<T,I,U>		ret;
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
	ND_ forceinline T  Log (const T& x, const T& base)
	{
		return Ln( x ) / Ln( base );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Log (const Vec<T,I,U>& x, const T& base)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Log( x[i], base );
		return ret;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Log (const Vec<T,I,U>& x, const Vec<T,I,U>& base)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Log( x[i], base[i] );
		return ret;
	}
	
/*
=================================================
	Log2
=================================================
*/
	template <typename T>
	ND_ forceinline T Log2 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		static const T	base = Ln( T(2) );
		return Ln( x ) / base;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Log2 (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Log2( x[i] );
		return ret;
	}

/*
=================================================
	Log10
=================================================
*/
	template <typename T>
	ND_ forceinline T  Log10 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::log10( _float_t( x ) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Log10 (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Log10( x[i] );
		return ret;
	}
	
/*
=================================================
	Exp
=================================================
*/
	template <typename T>
	ND_ forceinline T  Exp (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::exp( _float_t( x ) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Exp (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Exp( x[i] );
		return ret;
	}

/*
=================================================
	Exp2
=================================================
*/
	template <typename T>
	ND_ forceinline T  Exp2 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::pow( _float_t( 2 ), _float_t( x ) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Exp2 (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Exp2( x[i] );
		return ret;
	}
	
/*
=================================================
	Exp10
=================================================
*/
	template <typename T>
	ND_ forceinline T  Exp10 (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::pow( _float_t( 10 ), _float_t( x ) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Exp10 (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Exp10( x[i] );
		return ret;
	}
	
/*
=================================================
	CeilPowerOfTwo
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T  CeilPowerOfTwo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( T(1) << ( IntLog2(x) + not IsPowerOfTwo(x) ) ) * Sign(x);
	}

	template <typename T, usize I, ulong U>
	ND_ inline constexpr Vec<T,I,U>  CeilPowerOfTwo (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = CeilPowerOfTwo( x[i] );
		return ret;
	}

/*
=================================================
	FloorPowerOfTwo
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T  FloorPowerOfTwo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( T(1) << IntLog2(x) ) * Sign(x);
	}

	template <typename T, usize I, ulong U>
	ND_ inline constexpr Vec<T,I,U>  FloorPowerOfTwo (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = FloorPowerOfTwo( x[i] );
		return ret;
	}

/*
=================================================
	NearPowerOfTwo
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T  NearPowerOfTwo (const T& x)
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

	template <typename T, usize I, ulong U>
	ND_ inline constexpr Vec<T,I,U>  NearPowerOfTwo (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = NearPowerOfTwo( x[i] );
		return ret;
	}
	
/*
=================================================
	IsPowerOfTwo
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr bool  IsPowerOfTwo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		
		const T	a = Abs( x );
		return ( a != 0 and ( (a & (a - 1)) == 0 ) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline constexpr Vec<bool,I,U>  IsPowerOfTwo (const Vec<T,I,U> &x)
	{
		Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = IsPowerOfTwo( x[i] );
		return ret;
	}
	
/*
=================================================
	Square
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T  Square (const T& x)
	{
		return x * x;
	}

	template <typename T, usize I, ulong U>
	ND_ inline constexpr Vec<T,I,U>  Square (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Square( x[i] );
		return ret;
	}
	
/*
=================================================
	Sqrt
=================================================
*/
	template <typename T>
	ND_ forceinline T  Sqrt (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		ASSERT( x >= T(0) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::sqrt( _float_t( x ) ));
	}

#ifdef GX_FAST_MATH
	template <>
	ND_ forceinline T  Sqrt (const float& x)
	{
		return FSqrt( x );
	}
#endif

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Sqrt (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Sqrt( x[i] );
		return ret;
	}
	
/*
=================================================
	InvSqrt
=================================================
*/
	template <typename T>
	ND_ forceinline T  InvSqrt (const T& x)
	{
		return T(1) / Sqrt( x );
	}

#ifdef GX_FAST_MATH
	template <>
	ND_ forceinline float  InvSqrt (const float &x)
	{
		return FInvSqrt( x );
	}
#endif

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  InvSqrt (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = InvSqrt( x[i] );
		return ret;
	}
	
/*
=================================================
	SquareSign
=================================================
*/
	template <typename T>
	ND_ forceinline T  SquareSign (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return ( x < T(0) ? -Square(x) : Square(x) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SquareSign (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
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
			typedef typename CompileTime::GenType<A,B>		main_t;

			forceinline static main_t  Mod (const A& l, const B& r)
			{
				typedef _math_hidden_::ToNearFloat< main_t >	_float_t;
				return main_t(std::fmod( _float_t(l), _float_t(r) ));
			}

			template <typename C>
			forceinline static main_t  SafeMod (const A& l, const B& r, const C& def)
			{
				typedef _math_hidden_::ToNearFloat< main_t >	_float_t;
				return IsNotZero( r ) ? main_t(std::fmod( _float_t(l), _float_t(r) )) : maint_t(def);
			}
		};

		template <typename A, typename B>
		struct ModNearType<A,B,true>
		{
			typedef typename CompileTime::GenType<A,B>		main_t;

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
	ND_ forceinline auto  Mod (const A& left, const B& right)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>		T;

		return _math_hidden_::ModNearType< A, B, CompileTime::IsInteger<T> >::Mod( left, right );
	}

	template <typename A, typename B, usize I, ulong U>
	ND_ inline auto  Mod (const Vec<A,I,U> &left, const Vec<B,I,U> &right)
	{
		typedef typename CompileTime::GenType<A,B>		T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Mod( left[i], right[i] );
		return ret;
	}

/*
=================================================
	SafeMod
=================================================
*/
	template <typename A, typename B, typename C>
	ND_ forceinline auto  SafeMod (const A& left, const B& right, const C& defValue)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and CompileTime::IsScalarOrEnum<B> );

		typedef typename CompileTime::GenType<A,B>		T;

		return _math_hidden_::ModNearType< A, B, CompileTime::IsInteger<T> >::SafeMod( left, right, defValue );
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  SafeMod (const Vec<A,I,U> &left, const Vec<B,I,U> &right, const C& defValue)
	{
		typedef typename CompileTime::GenType<A,B>		T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SafeMod( left[i], right[i], defValue );
		return ret;
	}
	
/*
=================================================
	Floor
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T  Floor (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::floor( _float_t( x ) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Floor (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Floor( x[i] );
		return ret;
	}

/*
=================================================
	Ceil
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T  Ceil (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::ceil( _float_t( x ) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Ceil (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
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
	ND_ forceinline constexpr T  Fract (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

	#if 1
		return x - Floor(x);	// GLSL style 0..1

	#elif 0
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		_float_t	tmp;
		return T(std::modf( _float_t( x ), OUT &tmp ));	// -1..1

	#elif 0

		return x - Trunc(x);	// -1..1
	#endif
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Fract (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Fract( x[i] );
		return ret;
	}

/*
=================================================
	Trunc
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T  Trunc (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

	#if 1
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return T(std::trunc( _float_t( x ) ));

	#else
		return CopySign( x, Floor( Abs(x) ) );	// if trunc not supported
	#endif
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Trunc (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
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

	template <typename T, usize I, ulong U>
	struct ModF_Result< Vec<T,I,U> >
	{
		Vec<T,I,U>	intgr;
		Vec<T,I,U>	fract;
	};

	template <typename T>
	ND_ forceinline ModF_Result<T>  ModF (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

		_float_t	i;
		_float_t	f = std::modf( _float_t( x ), OUT &i );

		return ModF_Result<T>( i, f );
	}

	template <typename T, usize I, ulong U>
	ND_ inline ModF_Result< Vec<T,I,U> >  ModF (const Vec<T,I,U> &x)
	{
		ModF_Result< Vec<T,I,U> >		ret;

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
	Wrap
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		forceinline static T _Wrap (const T& value, const T& minValue, const T& maxValue)
		{
			// check for NaN
			if ( minValue >= maxValue )
				return minValue;

			T	result = T( minValue + GXMath::Mod( value - minValue, maxValue - minValue ) );
		
			if ( result < minValue )
				result += (maxValue - minValue);

			return result;
		}
	}	// _math_hidden_
	
	template <typename A, typename B, typename C>
	ND_ forceinline auto  Wrap (const A& value, const B& minValue, const C& maxValue)
	{
		// Warning: float value never equal maxValue!
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and
					   CompileTime::IsScalarOrEnum<B> and
					   CompileTime::IsScalarOrEnum<C> );

		typedef typename CompileTime::GenType<A,B,C>	T;

		ASSERT( minValue <= maxValue );

		return _math_hidden_::_Wrap( T(value), T(minValue), T(maxValue) + T(CompileTime::IsInteger<T>) );
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  Wrap (const Vec<A,I,U>& value, const Vec<B,I,U>& minValue, const Vec<C,I,U>& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Wrap( value[i], minValue[i], maxValue[i] );
		return ret;
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  Wrap (const Vec<A,I,U>& value, const B& minValue, const C& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Wrap( value[i], minValue, maxValue );
		return ret;
	}
	
/*
=================================================
	MirroredWrap
=================================================
*/
	namespace _math_hidden_
	{
		template <typename T>
		forceinline static T _MirroredWrapF (const T& value, const T& minValue, const T& maxValue)
		{
			// check for NaN
			if ( minValue >= maxValue )
				return minValue;
			
			const T	 size	= (maxValue - minValue) * T(2);
			const T	 val	= Fract( (value - minValue) / size );

			return Min( val, T(1) - val ) * size + minValue;
		}

		template <typename T>
		forceinline static T _MirroredWrapI (const T& value, const T& minValue, const T& maxValue)
		{
			STATIC_ASSERT( CompileTime::IsSigned<T> );

			// check for division by zero
			if ( minValue >= maxValue )
				return minValue;
			
			const T	 size	= (maxValue - minValue) * T(2);
			const T	 val	= Abs( value - minValue ) % size;

			return Min( val, size - val ) + minValue;
		}
	}	// _math_hidden_


	template <typename A, typename B, typename C>
	ND_ inline auto  MirroredWrap (const A& value, const B& minValue, const C& maxValue)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<A> and
					   CompileTime::IsScalarOrEnum<B> and
					   CompileTime::IsScalarOrEnum<C> );
		
		typedef typename CompileTime::GenType<A,B,C>	T;

		ASSERT( minValue <= maxValue );

		if_constexpr( CompileTime::IsInteger<T> )
			return _math_hidden_::_MirroredWrapI( T(value), T(minValue), T(maxValue) );
		else
			return _math_hidden_::_MirroredWrapF( T(value), T(minValue), T(maxValue) );
	}
	
	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  MirroredWrap (const Vec<A,I,U>& value, const Vec<B,I,U>& minValue, const Vec<C,I,U>& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = MirroredWrap( value[i], minValue[i], maxValue[i] );
		return ret;
	}

	template <typename A, typename B, typename C, usize I, ulong U>
	ND_ inline auto  MirroredWrap (const Vec<A,I,U>& value, const B& minValue, const C& maxValue)
	{
		typedef typename CompileTime::GenType<A,B,C>	T;
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = MirroredWrap( value[i], minValue, maxValue );
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
			static forceinline constexpr R Round (const T& val)
			{
				return R( val );
			}
		};
		
		template <>
		struct _TRound< true, false >	// float to int
		{
			template <typename R, typename T>
			static forceinline constexpr R Round (const T& val)
			{
				typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

				return R(std::round( _float_t( val ) ));
				//return R( val + ( val > T(0) ? T(0.5) : T(-0.5) ) );	// if round not supported
			}
		};
		
		template <>
		struct _TRound< false, false >	// float to float
		{
			template <typename R, typename T>
			static forceinline constexpr R Round (const T& val)
			{
				typedef typename _math_hidden_::ToNearFloat<T>  _float_t;

				return R(std::round( _float_t( val ) ));
				//return (R) Trunc( val + ( val > T(0) ? T(0.5) : T(-0.5) ) );	// if round not supported
			}
		};

		template <typename R, typename T>
		forceinline constexpr R _Round (const T& x)
		{
			return _TRound<	CompileTime::IsInteger<R>, CompileTime::IsInteger<T> >::template Round< R, T >( x );
		}

	}	// _math_hidden_

	template <typename T>
	ND_ forceinline constexpr T  Round (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		//CompileTime::MustBeFloat<T>();

		return _math_hidden_::_Round< T, T >( x );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Round (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Round( x[i] );
		return ret;
	}

/*
=================================================
	RoundTo
=================================================
*/
	template <typename R, typename T>
	ND_ forceinline constexpr R  RoundTo (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		//CompileTime::MustBeFloat<T>();

		return _math_hidden_::_Round< R, T >( x );
	}

	template <typename R, typename T, usize I, ulong U>
	ND_ inline Vec<R,I,U>  RoundTo (const Vec<T,I,U> &x)
	{
		Vec<R,I,U>		ret;
		FOR( i, ret )	ret[i] = RoundTo<R>( x[i] );
		return ret;
	}
	
/*
=================================================
	RoundToInt
=================================================
*/
	template <typename T>
	ND_ forceinline typename CompileTime::NearInt::FromType<T>  RoundToInt (const T& val)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		return _math_hidden_::_Round< typename CompileTime::NearInt::FromType<T>, T >( val );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec< typename CompileTime::NearInt::FromType<T>, I >  RoundToInt (const Vec<T,I,U> &x)
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
	ND_ forceinline typename CompileTime::NearUInt::FromType<T>  RoundToUInt (const T& val)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		return _math_hidden_::_Round< typename CompileTime::NearUInt::FromType<T>, T >( val );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec< typename CompileTime::NearUInt::FromType<T>, I >  RoundToUInt (const Vec<T,I,U> &x)
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
	ND_ forceinline constexpr T  RoundTo (const T& x, const T& base)
	{
		return Round( x / base ) * base;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  RoundTo (const Vec<T,I,U> &x, const T& base)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = RoundTo( x[i] );
		return ret;
	}
	
/*
=================================================
	SafeDiv
=================================================
*/
	template <typename T1, typename T2, typename T3>
	ND_ forceinline auto  SafeDiv (const T1& left, const T2& right, const T3& defVal)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T1> and CompileTime::IsScalarOrEnum<T2> and CompileTime::IsScalarOrEnum<T3> );

		using T = CompileTime::GenType<T1, T2, T3>;

		return IsNotZero(right) ? (T(left) / T(right)) : T(defVal);
	}
	
	template <typename T1, typename T2>
	ND_ forceinline auto  SafeDiv (const T1& left, const T2& right)
	{
		return SafeDiv( left, right, T1(0) );
	}

	template <typename T1, typename T2, typename T3, usize I, ulong U>
	ND_ inline auto  SafeDiv (const Vec<T1,I,U> &left, const Vec<T2,I,U> &right, const T3& defVal)
	{
		using T = CompileTime::GenType<T1, T2, T3>;

		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SafeDiv( left[i], right[i], defVal );
		return ret;
	}
	
	template <typename T1, typename T2, usize I, ulong U>
	ND_ inline auto  SafeDiv (const Vec<T1,I,U> &left, const Vec<T2,I,U> &right)
	{
		return SafeDiv( left, right, T1(0) );
	}

	template <typename T1, typename T2, typename T3, usize I, ulong U>
	ND_ inline auto  SafeDiv (const Vec<T1,I,U> &left, const T2 &right, const T3& defVal)
	{
		return SafeDiv( left, Vec<T2,I,U>(right), defVal );
	}
	
	template <typename T1, typename T2, usize I, ulong U>
	ND_ inline auto  SafeDiv (const Vec<T1,I,U> &left, const T2 &right)
	{
		return SafeDiv( left, right, T1(0) );
	}

	template <typename T1, typename T2, typename T3, usize I, ulong U>
	ND_ inline auto  SafeDiv (const T1 &left, const Vec<T2,I,U> &right, const T3& defVal)
	{
		return SafeDiv( Vec<T1,I,U>(left), right, defVal );
	}
	
	template <typename T1, typename T2, usize I, ulong U>
	ND_ inline auto  SafeDiv (const T1 &left, const Vec<T2,I,U> &right)
	{
		return SafeDiv( left, right, T1(0) );
	}

/*
=================================================
	IntFactorial
=================================================
*/
	template <typename T>
	ND_ forceinline T  IntFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );

		typedef CompileTime::NearUInt::FromType<T>	UInt_t;

		const UInt_t	count	= RoundToUInt( x );
		T				result  = T(1);

		for (UInt_t i = 2; i <= count; ++i) {
			result *= T(i);
		}
		return result;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  IntFactorial (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = IntFactorial( x[i] );
		return ret;
	}
	
/*
=================================================
	IntSuperFactorial
=================================================
*/
	template <typename T>
	ND_ forceinline T  IntSuperFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );
		
		typedef CompileTime::NearUInt::FromType<T>	UInt_t;

		const UInt_t	count	= RoundToUInt( x );
		T				result  = T(1);

		for (UInt_t i = 1; i < count; ++i) {
			result *= Pow( T(count-i+1), T(i) );
		}
		return result;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  IntSuperFactorial (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = IntSuperFactorial( x[i] );
		return ret;
	}
	
/*
=================================================
	IntHyperFactorial
=================================================
*/
	template <typename T>
	ND_ forceinline T  IntHyperFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );
		
		typedef CompileTime::NearUInt::FromType<T>	UInt_t;

		const UInt_t	count	= RoundToUInt( x );
		T				result  = T(1);

		for (UInt_t i = 2; i <= count; ++i) {
			result *= Pow( T(i), T(i) );
		}
		return result;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  IntHyperFactorial (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = IntHyperFactorial( x[i] );
		return ret;
	}

/*
=================================================
	IntDoubleFactorial
=================================================
*/
	template <typename T>
	ND_ forceinline T  IntDoubleFactorial (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		ASSERT( x >= T(0) );
		
		typedef CompileTime::NearUInt::FromType<T>	UInt_t;

		const UInt_t	val		= RoundToUInt( x );
		const UInt_t	count	= val / 2;
		T				result  = T(1);

		for (UInt_t i = 0; i < count; ++i) {
			result *= ( T(val) - T(2*i) );
		}
		return result;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  IntDoubleFactorial (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = IntDoubleFactorial( x[i] );
		return ret;
	}
	
/*
=================================================
	IntGammaFunction
=================================================
*/
	template <typename T>
	ND_ forceinline T  IntGammaFunction (const T& x)
	{
		return IntFactorial( x - T(1) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  IntGammaFunction (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = IntGammaFunction( x[i] );
		return ret;
	}

/*
=================================================
	IntLnGammaFunction
=================================================
*/
	template <typename T>
	ND_ forceinline T  IntLnGammaFunction (const T& x)
	{
		return Ln( Abs( IntGammaFunction( x ) ) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  IntLnGammaFunction (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = IntLnGammaFunction( x[i] );
		return ret;
	}
	
/*
=================================================
	Factorial
=================================================
*/
	template <typename T>
	ND_ forceinline T  Factorial (const T& x)
	{
		return T(FastFactorial( double(x) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Factorial (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Factorial( x[i] );
		return ret;
	}

/*
=================================================
	Gamma
=================================================
*/
	template <typename T>
	ND_ forceinline T  Gamma (const T& x)
	{
		return T(FastGammaFunction( double(x) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Gamma (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Gamma( x[i] );
		return ret;
	}

/*
=================================================
	LnGamma
=================================================
*/
	template <typename T>
	ND_ forceinline T  LnGamma (const T& x)
	{
		return T(FastLnGammaFunction( double(x) ));
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  LnGamma (const Vec<T,I,U>& x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = LnGammaFunction( x[i] );
		return ret;
	}
	
/*
=================================================
	IsOdd
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr bool  IsOdd (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x & T(1) ) == T(1);
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<bool,I,U>  IsOdd (const Vec<T,I,U> &x)
	{
		Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = IsOdd( x[i] );
		return ret;
	}

/*
=================================================
	IsEven
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr bool  IsEven (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x & T(1) ) == T(0);
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<bool,I,U>  IsEven (const Vec<T,I,U> &x)
	{
		Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = IsEven( x[i] );
		return ret;
	}
	
/*
=================================================
	ToOdd
=================================================
*/
	template <typename T>
	ND_ forceinline T  ToOdd (const T &x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x | T(1) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ToOdd (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = ToOdd( x[i] );
		return ret;
	}

/*
=================================================
	ToEven
=================================================
*/
	template <typename T>
	ND_ forceinline T  ToEven (const T &x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );

		return ( x & ~T(1) );
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ToEven (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = ToEven( x[i] );
		return ret;
	}
	
/*
=================================================
	Step
=================================================
*/
	template <typename T>
	ND_ forceinline T  Step (const T& x, const T& edge)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );

		return x < edge ? T(0) : T(1);
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Step (const Vec<T,I,U> &x, const Vec<T,I,U> &edge)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Step( x[i], edge[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  Step (const Vec<T,I,U> &x, const T &edge)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = Step( x[i], edge );
		return ret;
	}

/*
=================================================
	LinearStep
=================================================
*/
	template <typename T>
	ND_ forceinline T  LinearStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		return Clamp( (x - edge0) / (edge1 - edge0), T(0), T(1) );
	}
	
	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  LinearStep (const Vec<T,I,U> &x, const Vec<T,I,U> &edge0, const Vec<T,I,U> &edge1)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = LinearStep( x[i], edge0[i], edge1[i] );
		return ret;
	}
	
	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  LinearStep (const Vec<T,I,U> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = LinearStep( x[i], edge0, edge1 );
		return ret;
	}

/*
=================================================
	SmoothStep
=================================================
*/
	template <typename T>
	ND_ forceinline T  SmoothStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		T t = Clamp( (x - edge0) / (edge1 - edge0), T(0), T(1) );
		return t * t * (T(3) - T(2) * t);
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SmoothStep (const Vec<T,I,U> &x, const Vec<T,I,U> &edge0, const Vec<T,I,U> &edge1)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SmoothStep( x[i], edge0[i], edge1[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SmoothStep (const Vec<T,I,U> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SmoothStep( x[i], edge0, edge1 );
		return ret;
	}

/*
=================================================
	BumpStep
=================================================
*/
	template <typename T>
	ND_ forceinline T  BumpStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		return T(1) - Abs( LinearStep( x, edge0, edge1 ) - T(0.5) ) * T(2);
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  BumpStep (const Vec<T,I,U> &x, const Vec<T,I,U> &edge0, const Vec<T,I,U> &edge1)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = BumpStep( x[i], edge0[i], edge1[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  BumpStep (const Vec<T,I,U> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = BumpStep( x[i], edge0, edge1 );
		return ret;
	}

/*
=================================================
	SmoothBumpStep
=================================================
*/
	template <typename T>
	ND_ forceinline T  SmoothBumpStep (const T& x, const T& edge0, const T& edge1)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		ASSERT( edge0 < edge1 );

		T	t = BumpStep( x, edge0, edge1 );
		return t * t * (T(3) - T(2) * t);
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SmoothBumpStep (const Vec<T,I,U> &x, const Vec<T,I,U> &edge0, const Vec<T,I,U> &edge1)
	{
		Vec<T,I,U>		ret;
		FOR( i, ret )	ret[i] = SmoothBumpStep( x[i], edge0[i], edge1[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  SmoothBumpStep (const Vec<T,I,U> &x, const T &edge0, const T &edge1)
	{
		Vec<T,I,U>		ret;
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
	template <typename T, typename S>
	ND_ forceinline constexpr T  AlignToLarge (const T& value, const S& align)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		STATIC_ASSERT( CompileTime::IsInteger<S> and CompileTime::IsUnsigned<S> );

		const T	a = T(Max( align, S(1) ));

		return T( ((value + (a-1)) / a) * a );
	}

	template <typename T, usize I, ulong U, typename S>
	ND_ inline Vec<T,I,U>  AlignToLarge (const Vec<T,I,U>& value, const S& align)
	{
		Vec<T,I,U> res;
		FOR( i, res )	res[i] = AlignToLarge( value[i], align );
		return res;
	}

	template <typename T, typename B>
	ND_ forceinline constexpr T  AlignToLarge (const T& value, const Bytes<B> align)
	{
		return AlignToLarge( value, B(align) );
	}


}	// GXMath
}	// GX_STL
