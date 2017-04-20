// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_FloorCeilTruncRoundFract ()
{
	float a0 = Floor( 3.45f );		ASSERT( Equals( a0, 3.0f ) );
	float a1 = Floor( -3.45f );		ASSERT( Equals( a1, -4.0f ) );
	
	float b0 = Ceil( 3.45f );		ASSERT( Equals( b0, 4.0f ) );
	float b1 = Ceil( -3.45f );		ASSERT( Equals( b1, -3.0f ) );

	float c0 = Round( 3.45f );		ASSERT( Equals( c0, 3.0f ) );
	float c1 = Round( 3.55f );		ASSERT( Equals( c1, 4.0f ) );
	float c2 = Round( -3.45f );		ASSERT( Equals( c2, -3.0f ) );
	float c3 = Round( -3.55f );		ASSERT( Equals( c3, -4.0f ) );
	
	float d0 = Fract( 3.45f );		ASSERT( Equals( d0, 0.45f ) );
	float d1 = Fract( -3.55f );		ASSERT( Equals( d1, 0.45f ) );

	float e0 = Trunc( 3.45f );		ASSERT( Equals( e0, 3.0f ) );
	float e1 = Trunc( -3.45f );		ASSERT( Equals( e1, -3.0f ) );
}
