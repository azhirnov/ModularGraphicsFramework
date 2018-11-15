// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Types.h"

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
	#elif 1
		T c	= RVREF( a );
		a	= RVREF( b );
		b	= RVREF( c );
	#else
		T c{ RVREF(a) };
		PlacementDelete( a );	UnsafeMem::PlacementNew<T>( AddressOf(a), RVREF(b) );
		PlacementDelete( b );	UnsafeMem::PlacementNew<T>( AddressOf(b), RVREF(c) );
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
