// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "MathTypes.h"
#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/CompileTime/StringToID.h"
#include "Engine/STL/Math/2D/Rectangle.h"

namespace GX_STL
{
namespace GXMath
{

	// Normalized vectors
	using unormf1	= Vec< float, 1, "unorm"_StringToID >;
	using unormf2	= Vec< float, 2, "unorm"_StringToID >;
	using unormf3	= Vec< float, 3, "unorm"_StringToID >;
	using unormf4	= Vec< float, 4, "unorm"_StringToID >;
	
	using snormf1	= Vec< float, 1, "snorm"_StringToID >;
	using snormf2	= Vec< float, 2, "snorm"_StringToID >;
	using snormf3	= Vec< float, 3, "snorm"_StringToID >;
	using snormf4	= Vec< float, 4, "snorm"_StringToID >;

	using unormd1	= Vec< double, 1, "unorm"_StringToID >;
	using unormd2	= Vec< double, 2, "unorm"_StringToID >;
	using unormd3	= Vec< double, 3, "unorm"_StringToID >;
	using unormd4	= Vec< double, 4, "unorm"_StringToID >;

	using snormd1	= Vec< double, 1, "snorm"_StringToID >;
	using snormd2	= Vec< double, 2, "snorm"_StringToID >;
	using snormd3	= Vec< double, 3, "snorm"_StringToID >;
	using snormd4	= Vec< double, 4, "snorm"_StringToID >;

	using snormrectf	= Rectangle< float, "snorm"_StringToID >;
	using unormrectf	= Rectangle< float, "unorm"_StringToID >;

	using snormrectd	= Rectangle< double, "snorm"_StringToID >;
	using unormrectd	= Rectangle< double, "unorm"_StringToID >;



	//
	// Type Cast
	//

	struct TypeCast final : public Noninstancable
	{
		static constexpr ulong _UID_SNORM	= snormf1::UID;
		static constexpr ulong _UID_UNORM	= unormf1::UID;


		// Float SNorm to UNorm //
		template <typename T>
		CHECKRES inline static T  ToUNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );

			return value * T(0.5) + T(0.5);
		}

		template <typename T, usize I, ulong U>
		CHECKRES inline static Vec<T,I,U>  ToUNorm (const Vec<T,I,U> &value)
		{
			Vec<T,I,U>		res;
			FOR( i, res )	res[i] = ToUNorm( value[i] );
			return res;
		}

		template <typename T, usize I>
		CHECKRES inline static Vec<T,I,_UID_UNORM>  ToUNorm (const Vec<T,I,_UID_SNORM> &value)
		{
			Vec<T,I,_UID_UNORM>		res;
			FOR( i, res )	res[i] = ToUNorm( value[i] );
			return res;
		}


		// Float UNorm to SNorm //
		template <typename T>
		CHECKRES inline static T  ToSNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );

			return value * T(2.0) - T(1.0);
		}

		template <typename T, usize I, ulong U>
		CHECKRES inline static Vec<T,I,U>  ToSNorm (const Vec<T,I,U> &value)
		{
			Vec<T,I,U>		res;
			FOR( i, res )	res[i] = ToSNorm( value[i] );
			return res;
		}

		template <typename T, usize I>
		CHECKRES inline static Vec<T,I,_UID_SNORM>  ToSNorm (const Vec<T,I,_UID_UNORM> &value)
		{
			Vec<T,I,_UID_SNORM>		res;
			FOR( i, res )	res[i] = ToSNorm( value[i] );
			return res;
		}


		// UNormToInt //
		template <typename R, typename T>
		CHECKRES inline static R  UNormToInt (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );
			STATIC_ASSERT( CompileTime::IsUnsigned<R> );

			return R( value * MaxValue<R>() + T(0.5) );
		}
		
		template <typename R, typename T, usize I, ulong U>
		CHECKRES inline static Vec<R,I,U>  UNormToInt (const Vec<T,I,U> &value)
		{
			Vec<R,I,U>		res;
			FOR( i, res )	res[i] = UNormToInt<R>( value[i] );
			return res;
		}

		template <typename R, typename T, usize I>
		CHECKRES inline static Vec<R,I>  UNormToInt (const Vec<T,I,_UID_UNORM> &value)
		{
			Vec<R,I>		res;
			FOR( i, res )	res[i] = UNormToInt<R>( value[i] );
			return res;
		}
		

		// SNormToInt //
		template <typename R, typename T>
		CHECKRES inline static R  SNormToInt (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsFloat<T> );
			STATIC_ASSERT( CompileTime::IsSigned<R> );

			return R(	value < R(0) ?
						-value * MinValue<R>() - T(0.5) :
						value * MaxValue<R>() + T(0.5) );
		}
		
		template <typename R, typename T, usize I, ulong U>
		CHECKRES inline static Vec<R,I,U>  SNormToInt (const Vec<T,I,U> &value)
		{
			Vec<R,I,U>		res;
			FOR( i, res )	res[i] = SNormToInt<R>( value[i] );
			return res;
		}
		
		template <typename R, typename T, usize I>
		CHECKRES inline static Vec<R,I>  SNormToInt (const Vec<T,I,_UID_SNORM> &value)
		{
			Vec<R,I>		res;
			FOR( i, res )	res[i] = SNormToInt<R>( value[i] );
			return res;
		}


		// IntToUNorm //
		template <typename R, typename T>
		CHECKRES inline static R  IntToUNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsInteger<T> and CompileTime::IsUnsigned<T> );
			STATIC_ASSERT( CompileTime::IsFloat<R> );

			return R(value) / R(MaxValue<T>());
		}
		
		template <typename R, typename T, usize I, ulong U>
		CHECKRES inline static Vec<R,I,_UID_UNORM>  IntToUNorm (const Vec<T,I,U> &value)
		{
			Vec<R,I,_UID_UNORM>	res;
			FOR( i, res )	res[i] = IntToUNorm<R>( value[i] );
			return res;
		}


		// IntToSNorm //
		template <typename R, typename T>
		CHECKRES inline static R  IntToSNorm (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsInteger<T> and CompileTime::IsSigned<T> );
			STATIC_ASSERT( CompileTime::IsFloat<R> );

			return	 value < T(0) ?
					-R(value) / R(MinValue<T>()) :
					 R(value) / R(MaxValue<T>());
		}
		
		template <typename R, typename T, usize I, ulong U>
		CHECKRES inline static Vec<R,I,_UID_SNORM>  IntToSNorm (const Vec<T,I,U> &value)
		{
			Vec<R,I,_UID_SNORM>	res;
			FOR( i, res )	res[i] = IntToSNorm<R>( value[i] );
			return res;
		}

	};	// TypeCast


}	// GXMath
}	// GX_STL
