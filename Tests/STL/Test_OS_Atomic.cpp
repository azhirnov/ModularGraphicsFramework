// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


void Test_Atomics_1 ()
{
	Atomic<int>	a{ 0 };

	int	a0 = a.Add( 10 );				TEST( a0 == 10 );
	int	a1 = a.Sub( 2 );				TEST( a1 == 8 );
	int	a2 = a.Inc();					TEST( a2 == 9 );
	int	a3 = a.Dec();					TEST( a3 == 8 );
	int	a4 = a.Or( 0x1234 );			TEST( a4 == 0x123C );
	int	a5 = a.And( 0x20F );			TEST( a5 == 0x20C );
	int	a6 = a.Xor( 0x210 );			TEST( a6 == 0x01C );
	int	a7 = a.Set( 22 );				TEST( a7 == 22 );
	int	a8 = a.CompareEx( 55, 22 );		TEST( a8 == 22 );
	int	a9 = a.CompareEx( 77, 22 );		TEST( a9 == 55 );
}


extern void Test_OS_Atomic ()
{
	Test_Atomics_1();
}
