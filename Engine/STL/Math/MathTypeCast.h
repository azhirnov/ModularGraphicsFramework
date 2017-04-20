// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathTypes.h"
#include "Engine/STL/CompileTime/NewTypeInfo.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Type Cast
	//

	struct TypeCast : public Noninstancable
	{
		// Float SNorm to UNorm //
		template <typename T>
		inline static T  ToUNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );

			return value * T(0.5) + T(0.5);
		}

		template <typename T, usize I>
		inline static T  ToUNorm (const Vec<T,I> &value)
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = ToUNorm( value[i] );
			return res;
		}


		// Float UNorm to SNorm //
		template <typename T>
		inline static T  ToSNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );

			return value * T(2.0) - T(1.0);
		}

		template <typename T, usize I>
		inline static T  ToSNorm (const Vec<T,I> &value)
		{
			Vec<T,I>		res;
			FOR( i, res )	res[i] = ToSNorm( value[i] );
			return res;
		}


		// UNormToInt //
		template <typename R, typename T>
		inline static R  UNormToInt (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );
			STATIC_ASSERT( CompileTime::IsUnsigned<R> );

			return R( value * MaxValue<R>() + T(0.5) );
		}
		

		template <typename R, typename T, usize I>
		inline static Vec<R,I>  UNormToInt (const Vec<T,I> &value)
		{
			Vec<R,I>	res;

			FOR( i, res ) {
				res[i] = UNormToInt<R>( value[i] );
			}
			return res;
		}
		

		// SNormToInt //
		template <typename R, typename T>
		inline static R  SNormToInt (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );
			STATIC_ASSERT( CompileTime::IsSigned<R> );

			return R(	value < R(0) ?
						-value * MinValue<R>() - T(0.5) :
						value * MaxValue<R>() + T(0.5) );
		}
		

		template <typename R, typename T, usize I>
		inline static Vec<R,I>  SNormToInt (const Vec<T,I> &value)
		{
			GXMath::Vec<R,I>	res;

			FOR( i, res ) {
				res[i] = SNormToInt<R>( value[i] );
			}
			return res;
		}


		// IntToUNorm //
		template <typename R, typename T>
		inline static R  IntToUNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsInteger<T> and CompileTime::IsUnsigned<T> );
			STATIC_ASSERT( CompileTime::IsFloat<R> );

			return R(value) / R(MaxValue<T>());
		}
		

		template <typename R, typename T, usize I>
		inline static Vec<R,I>  IntToUNorm (const Vec<T,I> &value)
		{
			Vec<R,I>	res;

			FOR( i, res ) {
				res[i] = IntToUNorm<R>( value[i] );
			}
			return res;
		}


		// IntToSNorm //
		template <typename R, typename T>
		inline static R  IntToSNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsInteger<T> and CompileTime::IsSigned<T> );
			STATIC_ASSERT( CompileTime::IsFloat<R> );

			return	 value < T(0) ?
					-R(value) / R(MinValue<T>()) :
					 R(value) / R(MaxValue<T>());
		}
		

		template <typename R, typename T, usize I>
		inline static Vec<R,I>  IntToSNorm (const Vec<T,I> &value)
		{
			Vec<R,I>	res;

			FOR( i, res ) {
				res[i] = IntToSNorm<R>( value[i] );
			}
			return res;
		}
	};


}	// GXMath
}	// GX_STL
