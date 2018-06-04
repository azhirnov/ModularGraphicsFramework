// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/AllFunc.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	EnumEqMask
=================================================
*/
	template <typename T1, typename T2, typename T3>
	ND_ forceinline constexpr bool EnumEqMask (const T1& left, const T2& right, const T3& mask)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum< T1 > );
		STATIC_ASSERT( CompileTime::IsScalarOrEnum< T2 > );
		STATIC_ASSERT( CompileTime::IsScalarOrEnum< T3 > );

		using T	= typename CompileTime::NearUInt::FromSize< CompileTime::MaxFrom< usize, sizeof(T1), sizeof(T2), sizeof(T3) >() >;

		return ( T(left) & T(mask) ) == ( T(right) & T(mask) );
	}
	
/*
=================================================
	EnumEq
=================================================
*/
	template <typename T1, typename T2>
	ND_ forceinline constexpr bool EnumEq (const T1& left, const T2& right)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum< T1 > );
		STATIC_ASSERT( CompileTime::IsScalarOrEnum< T2 > );

		using T	= typename CompileTime::NearUInt::FromSize< CompileTime::Max< usize, sizeof(T1), sizeof(T2) > >;

		return ( T(left) & T(right) ) == T(right);
	}
	
/*
=================================================
	EnumAnyEq
=================================================
*/
	template <typename T1, typename T2>
	ND_ forceinline constexpr bool  EnumAnyEq (const T1& left, const T2& right)
	{
		STATIC_ASSERT( CompileTime::IsScalarOrEnum< T1 > );
		STATIC_ASSERT( CompileTime::IsScalarOrEnum< T2 > );

		using T	= typename CompileTime::NearUInt::FromSize< CompileTime::MaxFrom< usize, sizeof(T1), sizeof(T2) >() >;

		return ( T(left) & T(right) ) != T(0);
	}


}	// GXTypes
}	// GX_STL
