// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_Factorial ()
{
	double	res;

	res = IntFactorial( 4.0 );					ASSERT( Equals( res, 24.0 ) );
	res = FastFactorial( 4.0 );					ASSERT( Equals( res, 24.0 ) );
	res = FastApproximatedFactorial( 4.0 );		ASSERT( RoundToInt( res ) == 24 );
	res = IntSuperFactorial( 4.0 );				ASSERT( Equals( res, 288.0 ) );
	res = IntHyperFactorial( 4.0 );				ASSERT( Equals( res, 27648.0 ) );
	res = IntDoubleFactorial( 9.0 );			ASSERT( Equals( res, 945.0 ) );
	res = IntGammaFunction( 5.0 );				ASSERT( Equals( res, 24.0 ) );
	//res = LnGammaFunction( 5.0 );				ASSERT( Equals(); );
	res = FastGammaFunction( 5.0 );				ASSERT( Equals( res, 24.0 ) );
	res = FastLnGammaFunction( 0.5 );			ASSERT( Equals( res, 0.572365 ) );
	res = FastGammaFunction( 0.5 );				ASSERT( Equals( res, 1.772454 ) );
}
