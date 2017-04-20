// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Random.h"

namespace GX_STL
{
namespace GXMath
{
	
	//
	// Random Generator
	//

	template <typename RndEngine>
	struct TRandomGenerator<RndEngine>::NormalDistribution : public Noninstancable
	{
	public:

	// Float
		template <typename T>
		static T Float (const T expectedValue = T(0), const T standartDeviation = T(1))
		{
			return expectedValue + _Float<T>( T(-2), T(2) ) * standartDeviation;
		}


	// Integer
		template <typename T>
		static T Int (const T expectedValue, const T standartDeviation)
		{
			CompileTime::MustBeInteger<T>();
			typedef CompileTime::NearFloat::FromType<T>	float_t;
			return Round<T>( Float( float_t(expectedValue), float_t(standartDeviation) ) );
		}
		

	// Wrapped
		template <typename T>
		static T UNormWrap (const T expectedValue = T(0.5))
		{
			ASSERT( expectedValue >= T(0) and expectedValue <= T(1) );
			const T val = expectedValue + _FloatSNorm<T>() * T(0.5);
			return	val < T(0) ? T(1) + val :
					val > T(1) ? val - T(1) : val;
		}
		
		
		template <typename T>
		static T SNormWrap (const T expectedValue = T(0))
		{
			ASSERT( expectedValue >= T(-1) and expectedValue <= T(1) );
			const T val = expectedValue + _FloatSNorm<T>();
			return	val < T(-1) ? T(2) + val :
					val > T( 1) ? val - T(2) : val;
		}

		
		template <typename T>
		static T FloatRangeWrap (const T minValue, const T maxValue, const T expectedValue)
		{
			ASSERT( minValue < maxValue );
			const T dev = maxValue - minValue;
			return UNormWrap( (expectedValue - minValue) / dev ) * dev + minValue;
		}


	// Clamped
		template <typename T>
		static T UNorm (const T expectedValue = T(0.5))
		{
			ASSERT( expectedValue >= T(0) and expectedValue <= T(1) );
			return expectedValue + _Float( -expectedValue, T(1)-expectedValue );
		}
		

		template <typename T>
		static T SNorm (const T expectedValue = T(0))
		{
			ASSERT( expectedValue >= T(-1) and expectedValue <= T(1) );
			return expectedValue + _Float( T(-1)-expectedValue, T(1)-expectedValue );
		}


		template <typename T>
		static T FloatRange (const T minValue, const T maxValue, const T expectedValue)
		{
			ASSERT( minValue < maxValue );
			const T dev = maxValue - minValue;
			return UNorm( (expectedValue - minValue) / dev ) * dev + minValue;
		}


	private:
		template <typename T>
		static T _SNorm ()
		{
			return TRandomGenerator<RndEngine>::SNorm<T>();
		}
		

		template <typename T>
		static T _FloatSNorm ()
		{
			return _Float( T(-1), T(1) );
		}


		template <typename T>
		static T _Float (const T minValue, const T maxValue)
		{
			CompileTime::MustBeFloat<T>();

			T		randoms[2]	= { _SNorm<T>(), _SNorm<T>() };
			T		results[2]	= {};
			uint	i			= 0;

			for (;; ++i)
			{
				if ( _BoxMullerTransform( randoms[0], randoms[1], results[0], results[1] ) and
					 (results[i&1] >= minValue and results[i&1] <= maxValue) )
				{
					break;
				}

				randoms[i&1] = _SNorm<T>();
			}
			return results[i&1];
		}


		template <typename T>
		static bool _BoxMullerTransform (const T x, const T y, OUT T &z0, OUT T &z1)
		{
			const T s = x*x + y*y;

			if ( s <= _MinS<T>() or s > T(1) )
				return false;
			
			const T	k = Sqrt( T(-2) * Ln( s ) / s ) * T(1.0 / 3.0);	// normalized

			z0 = k * x;
			z1 = k * y;

			return true;
		}


		template <typename T>
		static constexpr T  _MinS ()
		{
			return T(1.0e-6);
		}
	};


}	// GXMath
}	// GX_STL
