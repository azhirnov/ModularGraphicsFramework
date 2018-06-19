// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_FloorCeilTruncRoundFract ()
{
	float a0 = Floor( 3.45f );		TEST( Equals( a0, 3.0f ) );
	float a1 = Floor( -3.45f );		TEST( Equals( a1, -4.0f ) );
	
	float b0 = Ceil( 3.45f );		TEST( Equals( b0, 4.0f ) );
	float b1 = Ceil( -3.45f );		TEST( Equals( b1, -3.0f ) );

	float c0 = Round( 3.45f );		TEST( Equals( c0, 3.0f ) );
	float c1 = Round( 3.55f );		TEST( Equals( c1, 4.0f ) );
	float c2 = Round( -3.45f );		TEST( Equals( c2, -3.0f ) );
	float c3 = Round( -3.55f );		TEST( Equals( c3, -4.0f ) );
	
	float d0 = Fract( 3.45f );		TEST( Equals( d0, 0.45f ) );
	float d1 = Fract( -3.55f );		TEST( Equals( d1, 0.45f ) );

	float e0 = Trunc( 3.45f );		TEST( Equals( e0, 3.0f ) );
	float e1 = Trunc( -3.45f );		TEST( Equals( e1, -3.0f ) );
}
