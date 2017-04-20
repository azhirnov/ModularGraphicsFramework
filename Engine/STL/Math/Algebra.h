// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Mathematics.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Trigonometry
	//

	struct Trigonometry : public Noninstancable
	{
	};



	//
	// Pythagoras Theorem
	//

	struct Pythagoras : public Noninstancable
	{
		//                            |\ d
		// d = sqrt(x*x + y*y)  for x |_\
		//                             y
		template <typename T>
		static T Hypotenuse (const T& x, const T& y)
		{
			return Sqrt( Square( x ) + Square( y ) );
		}
		
		//                            |\ d
		// b = sqrt(d*d - a*a)  for b |_\
		//                             a
		template <typename T>
		static T Cathetus (const T& a, const T& d)
		{
			ASSERT( Abs(a) < Abs(d) );
			return Sqrt( Square( d ) - Square( a ) );
		}
	};



	//
	// Linear equation
	//

	struct LinearEquation : public Noninstancable
	{
		// ax + b = 0
		template <typename T>
		static T  GetX (const T& a, const T& b = T(0))
		{
			return ( -b / a );
		}

		// ax + by + c = 0
		template <typename T>
		static T  GetY (const T& x, const T& a, const T& b, const T& c)
		{
			return ( a * x + c ) / -b;
		}

		// y = kx + m
		template <typename T>
		static T  GetY (const T& x, const T& k, const T& m)
		{
			return ( k*x + m );
		}
	};



	//
	// Quadratic equation
	//

	struct QuadraticEquation : public Noninstancable
	{
		// ax^2 + bx + c = 0
		// TODO: use Pair
		template <typename T>
		static uint GetX12 (OUT T& x1, OUT T& x2, const T& a, const T& b, const T& c = T(0))
		{
			ASSERT( IsNotZero( a ) );
			
			const T	disc = b*b - T(4)*a*c;
			
			if ( disc < T(0) )
			{
				WARNING( "D < 0" );
				x1 = x2 = MaxValue<T>();
				return 0;
			}

			const T	bb_4ac_2a	= Sqrt( disc ) / ( T(2) * a );
			const T	b_2a		= -b / ( T(2) * a );

			x1 = ( b_2a + bb_4ac_2a );
			x2 = ( b_2a - bb_4ac_2a );

			return IsZero( disc ) ? 1 : 2;
		}
	};



	//
	// Cubic Equation
	//

	struct CubicEquation : public Noninstancable
	{
		/*template <typename T>
		T croot (const T& x)
		{
			if ( All( x < T(0) )
				return -Pow( -x, T(1.0/3.0) );

			return Pow( x, T(1.0/3.0) );
		}

		// ax^3 + bx^2 + cx + d = 0
		template <typename T>
		static void GetX123 (OUT T& x1, OUT T& x2, OUT T& x3, const T& a, const T& b, const T& c, const T& d = T(0))
		{
			ASSERT( IsNotZero( a ) );

			//const T dist =	+ T(18) * a * b * c * d 
			//				- T(4) * Pow<3>( b ) * d
			//				+ Pow<2>( b * c )
			//				- T(4) * a * Pow<3>( c )
			//				- T(27) * Pow<2>( a * d );

			
			const T	p = ( T(3) * a * c - b * b ) / ( T(3) * a * a );
			const T	q = ( T(2) * b * b * b - T(9) * a * b * c + T(27) * a * a * d ) / ( T(27) * a * a * a );
			const T	s = ( q * q / T(4) ) + ( p * p * p / T(27) );
			
			T f;

			if ( IsZero( q ) )		f = Pi<T> * T(2);							else
			if ( All( q < T(0) ) )	f = ATan( -T(2) * Sqrt(-s) / q );			else
			if ( All( q > T(0) ) )	F = ATan( -T(2) * Sqrt(-s) / q ) + Pi<T>;

			StaticArray< T, 3*2 > x;
			
			if ( All( s < T(0) ) )
			{
				x[0].first = 2.0 * sqrt( -p/3.0 ) * cos( F/3.0) - B / (3.0*A);
				x[1].first = 2.0 * sqrt( -p/3.0 ) * cos( (F/3.0) +2.0*M_PI/3.0)-B/(3.0*A);
				x[2].first = 2.0 * sqrt( -p/3.0 ) * cos( (F/3.0) +4.0*M_PI/3.0)-B/(3.0*A);
			}
			else
			if ( IsZero( s ) )
			{
				x[0].first = 2.0*croot(-q/2.0)-B/(3.0*A);
				x[1].first = -croot(-q/2.0)-B/(3.0*A);
				x[2].first = -croot(-q/2.0)-B/(3.0*A);
			}
			else
			if ( All( s > T(0) ) )
			{
				double temp1 = croot((-q/2.0)+sqrt(S)) + croot((-q/2.0)-sqrt(S));
				double temp2 = croot((-q/2.0)+sqrt(S)) - croot((-q/2.0)-sqrt(S));
				x[0].first = temp1 - B/(3.0*A);
				x[1].first = -temp1/2.0 - B/(3.0*A); x[1].second = sqrt(3)*temp2/2.0;
				x[2].first = -temp1/2.0 - B/(3.0*A); x[2].second = -sqrt(3)*temp2/2.0;
			}

		}*/
		/*
		// ax^3 + bx^2 + cx + d = 0
		template <typename T>
		static void GetX123 (OUT T& x1, OUT T& x2, OUT T& x3, const T& d, const T& h, const T& g, const T& f = T(0))
		{
			const T	a = h / d;
			const T b = g / d;
			const T	c = f / d;

			const T	q = ( Pow<2>( a ) - T(3) * b ) / T(9);
			const T	r = ( T(2) * Pow<3>( a ) - T(9) * a * b + T(27) * c ) * T(54);

			const T	r2 = Pow<2>( r );
			const T	q3 = Pow<3>( q );

			if ( r2 < q3 )
			{
				const T	t = ACos( r / Sqrt( q3 ) ) / T(3);
				x1 = -T(2) * Sqrt( q ) * Cos( t ) - a / T(3);
				x2 = -T(2) * Sqrt( q ) * Cos( t + Pi<T> * T(2.0/3.0) ) - a / T(3);
				x3 = -T(2) * Sqrt( q ) * Cos( t - Pi<T> * T(2.0/3.0) ) - a / T(3);
			}
			else
			{
				const T aa = -SignOrZero( r ) * Pow( Abs( r ) + Sqrt( r2 - q3 ), T(1.0/3.0) );
				const T bb = IsZero( SignOrZero( aa ) ) ? T(0) : q / aa;
				x1 = (aa + bb) - a / T(3);

				//const T m = (aa + bb) / T(2) - a / T(3);
				//const T k = Pow( T(3), T(0.5) ) * (aa - bb) / T(2);

				if ( Equals( aa, bb ) )
				{
					x2 = x3 = -aa - a / T(3);
				}
			}
		}*/
	};


}	// GXMath
}	// GX_STL
