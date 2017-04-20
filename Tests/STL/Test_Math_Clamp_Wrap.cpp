// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_Clamp_Wrap ()
{
	float a0 = Clamp( 1.0f, 2.0f, 5.0f );		ASSERT( Equals( a0, 2.0f ) );
	float a1 = Clamp( 6.0f, 2.0f, 5.0f );		ASSERT( Equals( a1, 5.0f ) );
	float a2 = Clamp( 4.0f, 2.0f, 5.0f );		ASSERT( Equals( a2, 4.0f ) );
	float a3 = Clamp( 1.0f, 0.0f, 0.0f );		ASSERT( Equals( a3, 0.0f ) );

	float b0 = Wrap( 1.0f, 2.0f, 5.0f );		ASSERT( Equals( b0, 4.0f ) );
	float b1 = Wrap( 6.0f, 2.0f, 5.0f );		ASSERT( Equals( b1, 3.0f ) );
	float b2 = Wrap( 4.0f, 2.0f, 5.0f );		ASSERT( Equals( b2, 4.0f ) );
	float b4 = Wrap( 1.5f, 2.0f, 5.0f );		ASSERT( Equals( b4, 4.5f ) );
	float b5 = Wrap( 5.5f, 2.0f, 5.0f );		ASSERT( Equals( b5, 2.5f ) );
	float b6 = Wrap( 15.0f, 0.0f, 5.0f );		ASSERT( Equals( b6, 0.0f ) );
	float b7 = Wrap( 2.0f, -5.0f, 0.0f );		ASSERT( Equals( b7, -3.0f ) );

	float c0 = ClampOut( 1.0f, 2.0f, 5.0f );	ASSERT( Equals( c0, 1.0f ) );
	float c1 = ClampOut( 6.0f, 2.0f, 5.0f );	ASSERT( Equals( c1, 6.0f ) );
	float c2 = ClampOut( 4.0f, 2.0f, 5.0f );	ASSERT( Equals( c2, 5.0f ) );
	float c3 = ClampOut( 0.0f, 0.0f, 0.0f );	ASSERT( Equals( c3, 0.0f ) );

	int d0 = Wrap( -1, 0, 5 );					ASSERT( d0 == 5 );
	int d1 = Wrap(  5, 0, 5 );					ASSERT( d1 == 5 );
	int	d2 = Wrap( 1, -5, 0 );					ASSERT( d2 == -5 );
	int	d3 = Wrap( 2, -5, 0 );					ASSERT( d3 == -4 );
}
