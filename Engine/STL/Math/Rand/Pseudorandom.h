// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "RandEngine.h"

namespace GX_STL
{
namespace GXMath
{
	
	//
	// Pseudorandom
	//

	struct Pseudorandom : public Noninstancable
	{
		typedef float	float_t;
		typedef int		int_t;


	// Float -1.0..1.0
	public:
		template <typename T>
		static float_t Float (const T &val)
		{
			return _Float( val * T(16.0) );
		}
		
		template <typename T, usize I>
		static float_t Float (const Vec<T,I> &v)
		{
			const float4	control( 40.0f, 64.0f, 256.0f, 96.0f );

			Vec<T,I>		tmp;
			FOR( i, tmp )	tmp[i] = v[i] * (T) control[i];
			return _Float( tmp.Sum() );
		}

		template <typename T>
		static float_t Float (ArrayCRef<T> arr)
		{
			return _FloatI( Int( arr ) );
		}
		
		template <typename T, usize I>
		static Vec<float_t,I> FVec (const Vec<T,I> &v)
		{
			const float4	control( 40.0f, 64.0f, 256.0f, 96.0f );

			Vec<float_t,I>	res;
			FOR( i, res )	res[i] = Float( v[i] * control[i] );
			return res;
		}


	private:
		template <typename T>
		static float_t _Float (const T &x)
		{
			int_t n = int_t(x);
			n = (n << 13) ^ n;
			return _FloatI( _Int( n ) );
		}

		static float_t _FloatI (const int_t &x)
		{
			return float_t(1.0) - float_t(x) / float_t(1073741824.0);
		}


	// Integer
	public:
		// TODO: change for int_t type
		static int_t Int (const int &x)			{ return _Int( x ); }
		static int_t Int (const ilong &x)		{ return _Int( (x & 0xFFFFFFFF) ^ (x >> 32) ); }
		/*
		template <typename T, usize I>
		static int_t Int (const Vec<T,I> &x)
		{
			//Vec<int_t,I>	tmp
		}
		*/
		template <typename T>
		static int_t Int (ArrayCRef<T> arr)
		{
			int_t	x = 0;
			FOR( i, arr )	x ^= Int( arr[i] );
			return x;
		}

		template <typename T, usize I>
		static Vec<int_t,I> IVec (const Vec<T,I> &v)
		{
			Vec<int_t,I>	res;
			FOR( i, res )	res[i] = Int( v[i] );
			return res;
		}


	private:
		static int_t _Int (const int_t &n)
		{
			// TODO: change magic numbers for int_t types
			return SetSign( n * (n * n * 15731 + 789221) + 1376312589, false );

			// from libnoise
			//  n = (n >> 13) ^ n;
			//  int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
			//  return 1.0 - ((double)nn / 1073741824.0);

		}


	};	// Pseudorandom


}	// GXMath
}	// GX_STL
