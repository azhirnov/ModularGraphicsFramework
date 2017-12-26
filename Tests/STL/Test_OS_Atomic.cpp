// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


void Test_Atomics_1 ()
{
	using Op = OS::AtomicOp;

	volatile int	a = 0;

	int	a0 = Op::Add( a, 10 );			TEST( a0 == 10 );
	int	a1 = Op::Sub( a, 2 );			TEST( a1 == 8 );
	int	a2 = Op::Inc( a );				TEST( a2 == 9 );
	int	a3 = Op::Dec( a );				TEST( a3 == 8 );
	int	a4 = Op::Or( a, 0x1234 );		TEST( a4 == 0x123C );
	int	a5 = Op::And( a, 0x20F );		TEST( a5 == 0x20C );
	int	a6 = Op::Xor( a, 0x210 );		TEST( a6 == 0x01C );
	int	a7 = Op::Set( a, 22 );			TEST( a7 == 22 );
	int	a8 = Op::CmpExch( a, 55, 22 );	TEST( a8 == 22 );
	int	a9 = Op::CmpExch( a, 77, 22 );	TEST( a9 == 55 );
}


extern void Test_OS_Atomic ()
{
	Test_Atomics_1();
}
