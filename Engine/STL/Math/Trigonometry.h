// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/CompileTime/TypeListHelpers.h"
#include "MathConstants.h"
#include "MathTypes.h"
#include "Engine/STL/Dimensions/RadiansAndDegrees.h"

namespace GX_STL
{
namespace GXMath
{

/*
=================================================
	helpers
=================================================
*/
	namespace _math_hidden_
	{
		template <bool b64>		struct _ToNearFloat			{ typedef float  type; };
		template <>				struct _ToNearFloat<true>	{ typedef double type; };

		template <typename T>
		struct _ToNearFloatImpl
		{
			STATIC_ASSERT( CompileTime::IsScalar<T> );

			typedef typename _ToNearFloat< sizeof(T) >= sizeof(double) >::type	type;
		};

		template <typename T>
		using ToNearFloat = typename _ToNearFloatImpl<T>::type;
	}
	
/*
=================================================
	Sin
=================================================
*/
	template <typename T>
	inline T  Sin (const Radians<T>& x)
	{
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::sin( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Sin (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Sin( x[i] );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  Sin (const DegreesVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Sin( x[i] );
		return ret;
	}

/*
=================================================
	Cos
=================================================
*/
	template <typename T>
	inline T  Cos (const Radians<T>& x)
	{
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::cos( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Cos (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Cos( x[i] );
		return ret;
	}

	template <typename T, usize I>
	inline Vec<T,I>  Cos (const DegreesVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Cos( x[i] );
		return ret;
	}

/*
=================================================
	SinCos
=================================================
*/
	template <typename T>
	inline Vec<T,2>  SinCos (const Radians<T>& x)
	{
		Vec<T,2>	ret;
		ret.x = Sin( x );
		ret.y = Cos( x );
		return ret;
	}
	
/*
=================================================
	ASin
=================================================
*/
	template <typename T>
	inline Radians<T>  ASin (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		ASSERT( x >= T(-1) and x <= T(1) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (Radians<T>) ::asin( _float_t( x ) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ASin (const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ASin( x[i] );
		return ret;
	}
	
/*
=================================================
	ACos
=================================================
*/
	template <typename T>
	inline Radians<T>  ACos (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		ASSERT( x >= T(-1) and x <= T(1) );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (Radians<T>) ::acos( _float_t( x ) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ACos (const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ACos( x[i] );
		return ret;
	}

/*
=================================================
	ASinCos
=================================================
*/
	template <typename T>
	inline RadiansVec<T,2>  ASinCos (const T& x)
	{
		RadiansVec<T,2>	ret;
		ret.x = ASin( x );
		ret.y = ACos( x );
		return ret;
	}
	
/*
=================================================
	SinH
=================================================
*/
	template <typename T>
	inline T  SinH (const Radians<T>& x)
	{
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::sinh( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  SinH (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = SinH( x[i] );
		return ret;
	}

/*
=================================================
	CosH
=================================================
*/
	template <typename T>
	inline T  CosH (const Radians<T>& x)
	{
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::cosh( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  CosH (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = CosH( x[i] );
		return ret;
	}

/*
=================================================
	SinCosH
=================================================
*/
	template <typename T>
	inline Vec<T,2>  SinCosH (const Radians<T>& x)
	{
		Vec<T,2>	ret;
		ret.x = SinH( x );
		ret.y = CosH( x );
		return ret;
	}
	
/*
=================================================
	ASinH
=================================================
*/
	template <typename T>
	inline Radians<T>  ASinH (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return Radians<T>( SignOrZero( x ) * Ln( x + Sqrt( (x*x) + T(1) ) ) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ASinH (const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ASinH( x[i] );
		return ret;
	}

/*
=================================================
	ACosH
=================================================
*/
	template <typename T>
	inline Radians<T>  ACosH (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		ASSERT( x >= T(1) );
		return (Radians<T>) Ln( x + Sqrt( (x*x) - T(1) ) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ACosH (const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ACosH( x[i] );
		return ret;
	}

/*
=================================================
	ASinCosH
=================================================
*/
	template <typename T>
	inline RadiansVec<T,2>  ASinCosH (const T& x)
	{
		RadiansVec<T,2>	ret;
		ret.x = ASinH( x );
		ret.y = ACosH( x );
		return ret;
	}
	
/*
=================================================
	Tan
=================================================
*/
	template <typename T>
	inline T  Tan (const Radians<T>& x)
	{
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::tan( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  Tan (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = Tan( x[i] );
		return ret;
	}
	
/*
=================================================
	CoTan
=================================================
*/
	template <typename T>
	inline T  CoTan (const Radians<T>& x)
	{
		return SafeDiv( T(1), Tan( x ), T(0) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  CoTan (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = CoTan( x[i] );
		return ret;
	}
	
/*
=================================================
	TanH
=================================================
*/
	template <typename T>
	inline T  TanH (const Radians<T>& x)
	{
		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (T) ::tanh( _float_t( x ) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  TanH (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = TanH( x[i] );
		return ret;
	}
	
/*
=================================================
	CoTanH
=================================================
*/
	template <typename T>
	inline T  CoTanH (const Radians<T>& x)
	{
		return SafeDiv( T(1), TanH( x ), T(0) );
	}

	template <typename T, usize I>
	inline Vec<T,I>  CoTanH (const RadiansVec<T,I> &x)
	{
		Vec<T,I>		ret;
		FOR( i, ret )	ret[i] = CoTanH( x[i] );
		return ret;
	}
	
/*
=================================================
	ATan
=================================================
*/
	template <typename T>
	inline Radians<T>  ATan (const T& y_over_x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (Radians<T>) ::atan( _float_t( y_over_x ) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ATan (const Vec<T,I> &y_over_x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ATan( y_over_x[i] );
		return ret;
	}
	
/*
=================================================
	ATan
=================================================
*/
	template <typename T>
	inline Radians<T>  ATan (const T& y, const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		typedef typename _math_hidden_::ToNearFloat<T>  _float_t;
		return (Radians<T>) ::atan2( _float_t( y ), _float_t( x ) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ATan (const Vec<T,I> &y, const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ATan( y[i], x[i] );
		return ret;
	}
	
/*
=================================================
	ACoTan
=================================================
*/
	template <typename T>
	inline Radians<T>  ACoTan (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return (Radians<T>) SafeDiv( T(1), ATan( x ), T(0) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ACoTan (const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ACoTan( x[i] );
		return ret;
	}
	
/*
=================================================
	ATanH
=================================================
*/
	template <typename T>
	inline Radians<T>  ATanH (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		ASSERT( x > T(-1) and x < T(1) );

		if ( Abs(x) == T(1) )	return Infinity<T>();	else
		if ( Abs(x) > T(1) )	return NaN<T>();		else
								return Ln( (T(1) + x) / (T(1) - x) ) / T(2);
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ATanH (const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ATanH( x[i] );
		return ret;
	}
	
/*
=================================================
	ACoTanH
=================================================
*/
	template <typename T>
	inline Radians<T>  ACoTanH (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum<T> );

		return (Radians<T>) SafeDiv( T(1), ATanH( x ), T(0) );
	}

	template <typename T, usize I>
	inline RadiansVec<T,I>  ACoTanH (const Vec<T,I> &x)
	{
		RadiansVec<T,I>	ret;
		FOR( i, ret )	ret[i] = ACoTanH( x[i] );
		return ret;
	}


}	// GXMath
}	// GX_STL
