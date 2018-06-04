// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_Clamp_Wrap ()
{
	/*const auto TriangleWave = [] (float val) {
		float	value = Fract( val / 20.0f );
		return Min( value, 1.0f - value ) * 2.0f;
	};

	for (int i = 0; i < 30; ++i)
	{
		float f = TriangleWave( float(i) );
		f += 0.00001f;
	}*/

	float a0 = Clamp( 1.0f, 2.0f, 5.0f );				TEST( Equals( a0, 2.0f ) );
	float a1 = Clamp( 6.0f, 2.0f, 5.0f );				TEST( Equals( a1, 5.0f ) );
	float a2 = Clamp( 4.0f, 2.0f, 5.0f );				TEST( Equals( a2, 4.0f ) );
	float a3 = Clamp( 1.0f, 0.0f, 0.0f );				TEST( Equals( a3, 0.0f ) );
	float a4 = Clamp( Infinity<float>(), 0.0f, 1.0f );	TEST( Equals( a4, 1.0f ) );
	float a5 = Clamp( NaN<float>(), 0.0f, 1.0f );		TEST( Equals( a5, 0.0f ) );

	float b0 = Wrap( 1.0f, 2.0f, 5.0f );				TEST( Equals( b0, 4.0f ) );
	float b1 = Wrap( 6.0f, 2.0f, 5.0f );				TEST( Equals( b1, 3.0f ) );
	float b2 = Wrap( 4.0f, 2.0f, 5.0f );				TEST( Equals( b2, 4.0f ) );
	float b4 = Wrap( 1.5f, 2.0f, 5.0f );				TEST( Equals( b4, 4.5f ) );
	float b5 = Wrap( 5.5f, 2.0f, 5.0f );				TEST( Equals( b5, 2.5f ) );
	float b6 = Wrap( 15.0f, 0.0f, 5.0f );				TEST( Equals( b6, 0.0f ) );
	float b7 = Wrap( 2.0f, -5.0f, 0.0f );				TEST( Equals( b7, -3.0f ) );
	float b8 = Wrap( Infinity<float>(), 0.0f, 1.0f );	TEST( Equals( b8, 0.0f ) );
	float b9 = Wrap( NaN<float>(), 0.0f, 1.0f );		TEST( IsNaN( b9 ) );
	float b10 = Wrap( 3.99f, 0.0f, 4.0f );				TEST( Equals( b10, 3.99f ) );

	float c0 = ClampOut( 1.0f, 2.0f, 5.0f );				TEST( Equals( c0, 1.0f ) );
	float c1 = ClampOut( 6.0f, 2.0f, 5.0f );				TEST( Equals( c1, 6.0f ) );
	float c2 = ClampOut( 4.0f, 2.0f, 5.0f );				TEST( Equals( c2, 5.0f ) );
	float c3 = ClampOut( 0.0f, 0.0f, 0.0f );				TEST( Equals( c3, 0.0f ) );
	float c4 = ClampOut( Infinity<float>(), 0.0f, 1.0f );	TEST( IsInfinity( c4 ) );
	float c5 = ClampOut( NaN<float>(), 0.0f, 1.0f );		TEST( Equals( c5, 1.0f ) );

	int d0 = Wrap( -1, 0, 5 );							TEST( d0 == 5 );
	int d1 = Wrap(  5, 0, 5 );							TEST( d1 == 5 );
	int	d2 = Wrap( 1, -5, 0 );							TEST( d2 == -5 );
	int	d3 = Wrap( 2, -5, 0 );							TEST( d3 == -4 );
	int	d4 = Wrap( MinValue<int>(), -5, 5 );			TEST( d4 >= -5 and d4 <= 5 );
	int	d5 = Wrap( MaxValue<int>(), -5, 5 );			TEST( d5 >= -5 and d5 <= 5 );
	
	float e0 = MirroredWrap( 2.0f, 1.0f, 5.0f );				TEST( Equals( e0, 2.0f ) );
	float e1 = MirroredWrap( 6.0f, 1.0f, 5.0f );				TEST( Equals( e1, 4.0f ) );
	float e2 = MirroredWrap( -1.0f, 1.0f, 5.0f );				TEST( Equals( e2, 3.0f ) );
	float e3 = MirroredWrap( 5.0f, 1.0f, 5.0f );				TEST( Equals( e3, 5.0f ) );
	float e4 = MirroredWrap( 0.0f, 1.0f, 5.0f );				TEST( Equals( e4, 2.0f ) );
	float e5 = MirroredWrap( Infinity<float>(), 1.0f, 5.0f );	TEST( Equals( e5, 1.0f ) );
	float e6 = MirroredWrap( NaN<float>(), 1.0f, 5.0f );		TEST( IsNaN( e6 ) );
	
	int f0 = MirroredWrap( 2, 1, 5 );							TEST( f0 == 2 );
	int f1 = MirroredWrap( 5, 1, 5 );							TEST( f1 == 5 );
	int f2 = MirroredWrap( 6, 1, 5 );							TEST( f2 == 4 );
	int f3 = MirroredWrap( 0, 1, 5 );							TEST( f3 == 2 );
	int	f4 = MirroredWrap( MinValue<int>(), -5, 5 );			TEST( f4 >= -5 and f4 <= 5 );
	int	f5 = MirroredWrap( MaxValue<int>(), -5, 5 );			TEST( f5 >= -5 and f5 <= 5 );
}
