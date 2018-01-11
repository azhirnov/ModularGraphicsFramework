/*
	Hash functions.

	depends of:
		Math/Math
*/

// Weyl hash
float  WeylHash (const int2 c);		// [0;1]
float  WeylHash (const float2 c);	// [0;1]
double WeylHash (const double2 c);	// [0;1]

// Mod hash
float  ModHash (float2 uv);			// [0;1]
double ModHash (double2 uv);		// [0;1]

// Hugo Elias hash
float  HEHash (uint n);				// [0;1]


//---------------------------------------------------------


float WeylHash (const int2 c)
{
	// from https://www.shadertoy.com/view/MsV3z3

	const int x = 0x3504f333 * c.x * c.x + c.y;
	const int y = 0xf1bbcdcb * c.y * c.y + c.x;
    
	return float(x*y) * (2.0 / 8589934592.0) + 0.5;
}


float WeylHash (const float2 c)
{
	// from https://www.shadertoy.com/view/Xdy3Rc

	// a pair of Weyl values with low star discrepancy
	const float2 W = float2( 0.5545497, 0.308517 );

	const float2 v = c * Fract( c * W );
	return Fract( v.x * v.y );
}


double WeylHash (const double2 c)
{
	// from https://www.shadertoy.com/view/Xdy3Rc
	
	// a pair of Weyl values with low star discrepancy
	const double2 W = double2( 0.5545497, 0.308517 );

	const double2 v = c * Fract( c * W );
	return Fract( v.x * v.y );
}


float ModHash (float2 uv)
{
	// from https://www.shadertoy.com/view/Xts3R7

    uv = Abs( Mod( 10.0 * Fract( (uv + 1.1312) * 31.0 ), uv + 2.0 ) );
    uv = Abs( Mod( uv.x * Fract( (uv + 1.721711) * 17.0 ), uv ) );
    return Fract( 10.0 * (7.0 * uv.y + 31.0 * uv.x) );
}


double ModHash (double2 uv)
{
	// from https://www.shadertoy.com/view/Xts3R7

    uv = Abs( Mod( 10.0 * Fract( (uv + 1.1312) * 31.0 ), uv + 2.0 ) );
    uv = Abs( Mod( uv.x * Fract( (uv + 1.721711) * 17.0 ), uv ) );
    return Fract( 10.0 * (7.0 * uv.y + 31.0 * uv.x) );
}


float  HEHash (uint n)
{
	// from https://www.shadertoy.com/view/llGSzw
	
	// integer hash copied from Hugo Elias
	n = (n << 13U) ^ n;
	n = n * (n * n * 15731U + 789221U) + 1376312589U;
    
	// floating point conversion from http://iquilezles.org/www/articles/sfrand/sfrand.htm
	return uintBitsToFloat( (n>>9U) | 0x3f800000U ) - 1.0;
}
