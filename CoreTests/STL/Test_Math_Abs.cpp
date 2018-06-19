// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_Abs ()
{
	TEST( Abs( -100 ) == 100 );
	TEST( Abs( 5 ) == 5 );
	TEST( Abs( MinValue<int>() ) >= 0 );
	TEST( Abs( MaxValue<int>() ) == MaxValue<int>() );

	TEST( Abs( -3.0f ) == 3.0f );
	TEST( Abs( MinValue<float>() ) >= 0.0f );
}
