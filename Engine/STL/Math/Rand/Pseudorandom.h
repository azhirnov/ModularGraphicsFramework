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
		typedef float	Float_t;
		typedef int		Int_t;


	// Float -1.0..1.0
	public:
		template <typename T>
		static Float_t Float (const T &val)
		{
			return _Float( val * T(16.0) );
		}
		
		template <typename T, usize I, ulong U>
		static Float_t Float (const Vec<T,I,U> &v)
		{
			const float4	control( 40.0f, 64.0f, 256.0f, 96.0f );

			Vec<T,I,U>		tmp;
			FOR( i, tmp )	tmp[i] = v[i] * (T) control[i];
			return _Float( tmp.Sum() );
		}

		template <typename T>
		static Float_t Float (ArrayCRef<T> arr)
		{
			return _FloatI( Int( arr ) );
		}
		
		template <typename T, usize I, ulong U>
		static Vec<Float_t,I,U> FVec (const Vec<T,I,U> &v)
		{
			const float4	control( 40.0f, 64.0f, 256.0f, 96.0f );

			Vec<Float_t,I,U>	res;
			FOR( i, res )	res[i] = Float( v[i] * control[i] );
			return res;
		}


	private:
		template <typename T>
		static Float_t _Float (const T &x)
		{
			Int_t n = Int_t(x);
			n = (n << 13) ^ n;
			return _FloatI( _Int( n ) );
		}

		static Float_t _FloatI (const Int_t &x)
		{
			return Float_t(1.0) - Float_t(x) / Float_t(1073741824.0);
		}


	// Integer
	public:
		// TODO: change for Int_t type
		static Int_t Int (const int &x)			{ return _Int( x ); }
		static Int_t Int (const ilong &x)		{ return _Int( (x & 0xFFFFFFFF) ^ (x >> 32) ); }
		/*
		template <typename T, usize I, ulong U>
		static Int_t Int (const Vec<T,I,U> &x)
		{
			//Vec<Int_t,I,U>	tmp
		}
		*/
		template <typename T>
		static Int_t Int (ArrayCRef<T> arr)
		{
			Int_t	x = 0;
			FOR( i, arr )	x ^= Int( arr[i] );
			return x;
		}

		template <typename T, usize I, ulong U>
		static Vec<Int_t,I,U> IVec (const Vec<T,I,U> &v)
		{
			Vec<Int_t,I,U>	res;
			FOR( i, res )	res[i] = Int( v[i] );
			return res;
		}


	private:
		static Int_t _Int (const Int_t &n)
		{
			// TODO: change magic numbers for Int_t types
			return SetSign( n * (n * n * 15731 + 789221) + 1376312589, false );

			// from libnoise
			//  n = (n >> 13) ^ n;
			//  int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
			//  return 1.0 - ((double)nn / 1073741824.0);

		}


	};	// Pseudorandom


}	// GXMath
}	// GX_STL
