// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	based on code from OpenGL Mathematics (GLM) from http://glm.g-truc.net/
*/

#pragma once

#include "Engine/STL/Math/MathFunc.h"

namespace GX_STL
{
namespace GXMath
{
	
/*
=================================================
	FSin
=================================================
*/
	template <typename T> 
	inline T FSin (const T& x)
	{
		return ( x - ((x * x * x) / T(6)) + ((x * x * x * x * x) /
					T(120)) - ((x * x * x * x * x * x * x) / T(5040)) );
	}
	
/*
=================================================
	FCos
=================================================
*/
	template <typename T> 
	inline T FCos (const T& x)
	{
		return ( T(1) - (x * x * T(0.5)) + (x * x * x * x * T(0.041666666666)) -
					(x * x * x * x * x * x * T(0.00138888888888)) );
	}
	
/*
=================================================
	FTan
=================================================
*/
	template <typename T> 
	inline T FTan (const T& x)
	{
		return ( x + (x * x * x * T(0.3333333333)) + (x * x * x * x * x * T(0.1333333333333)) +
				(x * x * x * x * x * x * x * T(0.0539682539)) );
	}
	
/*
=================================================
	FAsin
=================================================
*/
	template <typename T> 
	inline T FAsin (const T& x)
	{
		return ( x + (x * x * x * T(0.166666667)) + (x * x * x * x * x * T(0.075)) +
				(x * x * x * x * x * x * x * T(0.0446428571)) +
				(x * x * x * x * x * x * x * x * x * T(0.0303819444)) );
	}
	
/*
=================================================
	FAcos
=================================================
*/
	template <typename T> 
	inline T FAcos (const T& x)
	{
		return ( T(1.5707963267948966192313216916398) - FAsin(x) );
	}
	
/*
=================================================
	FAtan
=================================================
*/
	template <typename T> 
	inline T FAtan (const T& y, const T& x)
	{
		return Abs( FAtan(y / x) ) * ( SignOrZero(y) * SignOrZero(x) );
	}
	
/*
=================================================
	FAtan
=================================================
*/
	template <typename T> 
	inline T FAtan (const T& x)
	{
		return ( x - (x * x * x * T(0.333333333333)) + (x * x * x * x * x * T(0.2)) -
				(x * x * x * x * x * x * x * T(0.1428571429)) +
				(x * x * x * x * x * x * x * x * x * T(0.111111111111)) -
				(x * x * x * x * x * x * x * x * x * x * x * T(0.0909090909)) );
	}
	
/*
=================================================
	FInvSqrt
=================================================
*/
	inline float FInvSqrt (const float& x)
	{
		float	tmp		= x;
		float	xhalf	= 0.5f * tmp;
		uint	i		= ReferenceCast<uint>( x );
		i	= 0x5f3759df - (i >> 1);
		tmp	= ReferenceCast<float>( i );
		tmp	= tmp * ( 1.5f - xhalf * tmp * tmp );
		return tmp;
	}
	
/*
=================================================
	FSqrt
=================================================
*/
	inline float FSqrt (const float& x)
	{
		return 1.f / FInvSqrt(x);
	}
	
/*
=================================================
	FastSqrt
=================================================
*/
#	if 0
	// from http://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b

	inline float FastSqrt (float x)
	{
		uint	i = ReferenceCast<uint>( x );
		i  += 127 << 23;
		i >>= 1;
		return ReferenceCast<float>( x );
	}
#	endif

/*
=================================================
	FastPowI
----
	O(log n)
=================================================
*/
	template <typename T>
	inline T FastPowI (T a, T b)
	{
		CompileTime::MustBeInteger<T>();

		T	res = 1;

		while ( b )
		{
			if ( b & 1 )
				res *= a;
			a *= a;
			b >>= 1;
		}
		return res;
	}
	
/*
=================================================
	FastApproximatedFactorial
=================================================
*/
	template <typename T>
	inline T  FastApproximatedFactorial (T n)
	{
		return Sqrt( Pi<T> * T(2) * n ) * Pow( n / EulerNumber<T> , n );
	}
	
/*
=================================================
	FastFactorial
=================================================
*/
	inline double FastGammaFunction (double x);
	inline double FastLnGammaFunction (double x);

	inline double FastFactorial (double x)
	{
		return FastGammaFunction( x ) * x;
	}

/*
=================================================
	FastGammaFunction
----
	from http://www.johndcook.com/blog/cpp_gamma/
=================================================
*/
	inline double FastGammaFunction (double x)
	{
		ASSERT( x > 0.0 );

		if ( x <= 0.0 )
			return -Infinity<double>();

		const double gamma = 0.577215664901532860606512090;

		if ( x < 0.001 )
			return 1.0 / ( x * ( 1.0 + gamma * x ) );

		if ( x < 12.0 )
		{
			double	y = x;
			int		n = 0;
			bool	arg_was_less_than_one = (y < 1.0);

			if ( arg_was_less_than_one )
			{
				y += 1.0;
			}
			else
			{
				n  = int( Floor(y) ) - 1;
				y -= n;
			}

			static const double p[] =
			{
				-1.71618513886549492533811E+0,
				 2.47656508055759199108314E+1,
				-3.79804256470945635097577E+2,
				 6.29331155312818442661052E+2,
				 8.66966202790413211295064E+2,
				-3.14512729688483675254357E+4,
				-3.61444134186911729807069E+4,
				 6.64561438202405440627855E+4
			};

			static const double q[] =
			{
				-3.08402300119738975254353E+1,
				 3.15350626979604161529144E+2,
				-1.01515636749021914166146E+3,
				-3.10777167157231109440444E+3,
				 2.25381184209801510330112E+4,
				 4.75584627752788110767815E+3,
				-1.34659959864969306392456E+5,
				-1.15132259675553483497211E+5
			};

			double	num = 0.0;
			double	den = 1.0;
			int		i;
			double	z = y - 1;

			for (i = 0; i < 8; i++)
			{
				num = ( num + p[i] ) * z;
				den = den * z + q[i];
			}
			double	result = num / den + 1.0;

			if (arg_was_less_than_one)
			{
				result /= ( y - 1.0 );
			}
			else
			{
				for (i = 0; i < n; i++)
					result *= y++;
			}

			return result;
		}

		if (x > 171.624)
		{
			return Infinity<double>();
		}

		return Exp( FastLnGammaFunction(x) );
	}

/*
=================================================
	FastLnGammaFunction
----
	from http://www.johndcook.com/blog/cpp_gamma/
=================================================
*/
	inline double FastLnGammaFunction (double x)
	{
		ASSERT( x > 0.0 );

		if ( x <= 0.0 )
			return -Infinity<double>();

		if ( x < 12.0 )
			return Ln( Abs( FastGammaFunction(x) ) );

		static const double c[8] =
		{
			 1.0/12.0,
			-1.0/360.0,
			1.0/1260.0,
			-1.0/1680.0,
			1.0/1188.0,
			-691.0/360360.0,
			1.0/156.0,
			-3617.0/122400.0
		};

		double	z	= 1.0 / Square( x );
		double	sum	= c[7];

		for (int i = 6; i >= 0; i--)
		{
			sum *= z;
			sum += c[i];
		}
		double	series = sum / x;

		static const double halfLogTwoPi = 0.91893853320467274178032973640562;

		double logGamma = (x - 0.5) * Ln(x) - x + halfLogTwoPi + series;    
		return logGamma;
	}


}	// GXMath
}	// GX_STL
