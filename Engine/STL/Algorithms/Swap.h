// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	SwapValues
=================================================
*/
	template <typename T>
	forceinline void SwapValues (INOUT T &a, INOUT T &b)
	{
	#if 0
		T c(a);
		a = b;
		b = c;
	#else
		T c	= RVREF( a );
		a	= RVREF( b );
		b	= RVREF( c );
	#endif
	}

	template <typename T>
	forceinline void SwapValuesWithTemp (INOUT T &a, INOUT T &b, INOUT T &temp)
	{
	#if 0
		temp = a;
		a    = b;
		b    = temp;
	#else
		temp = RVREF( a );
		a	 = RVREF( b );
		b	 = RVREF( temp );
	#endif
	}
	
/*
=================================================
	SwapValues
=================================================
*/
	template <typename C, typename T>
	forceinline void SwapMembers (C *a, C *b, T C:: *member)
	{
		SwapValues( a->*member, b->*member );
	}

	template <typename C, typename T>
	forceinline void SwapMembers (C &a, C &b, T C:: *member)
	{
		SwapValues( a.*member, b.*member );
	}


}	// GXTypes
}	// GX_STL
