// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Types_Time ()
{
	uint	a0 = 1;
	TimeU	a1 = TimeU::FromSeconds( a0 );
	uint	a2 = a1.Seconds();
	ASSERT( a0 == a2 );

	float	b0 = 1.2345f;
	TimeF	b1 = TimeF::FromSeconds( b0 );
	float	b2 = b1.Seconds();
	ASSERT( Equals( b0, b2 ) );
}
