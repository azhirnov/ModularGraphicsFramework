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
	constexpr forceinline usize CountOf (const T (&)[I])
	{
		return I;
	}


}	// GXTypes
}	// GX_STL
