// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_Abs ()
{
	ASSERT( Abs( -100 ) == 100 );
	ASSERT( Abs( 5 ) == 5 );
	ASSERT( Abs( MinValue<int>() ) >= 0 );
	ASSERT( Abs( MaxValue<int>() ) == MaxValue<int>() );

	ASSERT( Abs( -3.0f ) == 3.0f );
	ASSERT( Abs( MinValue<float>() ) >= 0.0f );
}
