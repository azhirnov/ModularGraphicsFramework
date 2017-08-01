// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/AllFunc.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	CountOf
=================================================
*/
	template <typename T, usize I>
	constexpr forceinline usize  CountOf (const T (&)[I])
	{
		return I;
	}
	
	template <typename T, T First, T ...Values>
	constexpr forceinline usize  CountOf ()
	{
		return sizeof...(Values) + 1;
	}
	
	template <typename ...Types>
	constexpr forceinline usize  CountOf ()
	{
		return sizeof...(Types);
	}


}	// GXTypes
}	// GX_STL
