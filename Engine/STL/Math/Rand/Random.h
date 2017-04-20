// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "RandEngine.h"
#include "Engine/STL/Containers/String.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Random Generator
	//

	template <typename RndEngine>
	struct TRandomGenerator
	{
		struct WithChance;
		struct NormalDistribution;

	private:
		static RndEngine	_engine;


	// Utils
	public:
		static bool Initialize ()
		{
			return _engine(), true;
		}


	// Sign (1 or -1)
		static int Sign (const int = 0)
		{
			return SetSign( 1, Bool() );
		}
		
		template <usize I>
		static Vec<int,I> Sign (const Vec<int,I> = Vec<int,I>())
		{
			return SetSign( Vec<int,I>(1), Bool<I>() );
		}

		template <typename T>
		static T SignVec ()
		{
			return Sign( T() );
		}


	// Sign Or Zero (-1, 0, 1)
		static int SignOrZero (const int = 0)
		{
			const uint	val = RndEngine()();
			return SetSign( (val >> 3) & 1, (val & 1) );
		}
		
		template <usize I>
		static Vec<int,I> SignOrZero (const Vec<int,I> = Vec<int,I>())
		{
			Vec<int,I>		res;
			FOR( i, res )	res[i] = SignOrZero( int() );
			return res;
		}

		template <typename T>
		static T SignOrZeroVec ()
		{
			return SignOrZero( T() );
		}


	// Boolean
		static bool Bool (const bool = false)
		{
			return RndEngine()() & 1;
		}
		
		template <usize I>
		static Vec<bool,I> Bool (const Vec<bool,I> & = Vec<bool,I>())
		{
			Vec<bool,I>	res;
			FOR( i, res )	res[i] = Bool();
			return res;
		}

		template <typename T>
		static T BVec ()
		{
			return Bool( T() );
		}


	// Integer
		template <typename T>
		static T Int (const T & = T())
		{
			return (T) RndEngine()();
		}
		
		template <typename T, usize I>
		static Vec<T,I> Int (const Vec<T,I> & = Vec<T,I>())
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = Int<T>();
			return res;
		}

		template <typename T>
		static T IVec ()
		{
			return Int( T() );
		}


	// Float
		template <typename T>
		static T Float (const T & = T())
		{
			return (T) RndEngine()() * T(0.1);
		}
		
		template <typename T, usize I>
		static Vec<T,I> Float (const Vec<T,I> & = Vec<T,I>())
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = Float<T>();
			return res;
		}

		template <typename T>
		static T FVec ()
		{
			return Float( T() );
		}
		

	// Float Unsigned Normalized	0.0 ... 1.0
		template <typename T>
		static T UNorm (const T & = T())
		{
			STATIC_ASSERT( CompileTime::IsFloat<T>, "return value must be float!" );
			
			RndEngine	eng;
			T			res = T( eng() ) / T( eng.Max() );

			ASSERT( res >= 0.0f and res <= 1.0f );
			return res;
		}
		
		template <typename T, usize I>
		static Vec<T,I> UNorm (const Vec<T,I> & = Vec<T,I>())
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = UNorm<T>();
			return res;
		}

		template <typename T>
		static T UNormVec ()
		{
			return UNorm( T() );
		}
		

	// Float Signed Normalized	-1.0 ... 1.0
		template <typename T>
		static T SNorm (const T & = T())
		{
			STATIC_ASSERT( CompileTime::IsFloat<T>, "return value must be float!" );

			return UNorm<T>() * T(2) - T(1);
		}
		
		template <typename T, usize I>
		static Vec<T,I> SNorm (const Vec<T,I> & = Vec<T,I>())
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = SNorm<T>();
			return res;
		}

		template <typename T>
		static T SNormVec ()
		{
			return SNorm( T() );
		}


	// Float In Range
		template <typename T>
		static T FloatRange (const T& minValue, const T& maxValue)
		{
			ASSERT( minValue < maxValue );
			return UNorm<T>() * (maxValue - minValue) + minValue;
		}
		
		template <typename T, usize I>
		static Vec<T,I> FloatRange (const Vec<T,I> &minValue, const Vec<T,I> &maxValue)
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = FloatRange<T>( minValue[i], maxValue[i] );
			return res;
		}


	// Integer In Range
		template <typename T>
		static T IntRange (const T& minValue, const T& maxValue)
		{
			typedef typename CompileTime::NearFloat::FromType<T>  float_t;
			return (T) RoundToInt( FloatRange( float_t(minValue), float_t(maxValue) ) );
		}
		
		template <typename T, usize I>
		static Vec<T,I> IntRange (const Vec<T,I> &minValue, const Vec<T,I> &maxValue)
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = IntRange<T>( minValue[i], maxValue[i] );
			return res;
		}


	// Range
		template <typename T>
		static T Range (const T& minValue, const T& maxValue)
		{
			return IntRange( minValue, maxValue );
		}
		
		template <typename T, usize I>
		static Vec<T,I> Range (const Vec<T,I> &minValue, const Vec<T,I> &maxValue)
		{
			return IntRange( minValue, maxValue );
		}

		
	// String
		template <typename T>
		static void RandomString (T minValue, T maxValue, usize uLength, INOUT TString<T> &str)
		{
			typedef typename CompileTime::NearUInt::FromType<T>	type;

			if ( minValue == 0 )						minValue = 1;
			if ( type(maxValue) <= type(minValue) )		maxValue = minValue+1;

			str.Reserve( str.Length() + uLength + 1 );

			for (usize i = 0; i < uLength; ++i) {
				str << T( IntRange( type(minValue), type(maxValue) ) );
			}
		}

	
		template <typename T>
		static void RandomString2 (T minValue, T maxValue, usize minLength, usize maxLength, INOUT TString<T> &str)
		{
			typedef typename CompileTime::NearUInt::FromType<T>	type;

			if ( minValue == 0 )						minValue = 1;
			if ( type(maxValue) <= type(minValue) )		maxValue = minValue+1;
			const usize									u_length = IntRange( minLength, maxLength );

			str.Reserve( str.Length() + u_length + 1 );

			for (usize i = 0; i < u_length; ++i) {
				str << T( IntRange( type(minValue), type(maxValue) ) );
			}
		}

	};	// TRandomGenerator
	

	template <typename RndEngine>
	RndEngine  TRandomGenerator< RndEngine >::_engine;


	typedef TRandomGenerator< DefaultRandomEngine >		Random;
	typedef TRandomGenerator< PlatformRandomEngine >	ImprovedRandom;


}	// GXMath
}	// GX_STL
