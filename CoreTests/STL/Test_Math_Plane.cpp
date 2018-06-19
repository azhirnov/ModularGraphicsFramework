// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;

using plane	= Plane<float>;


static void Test_Plane0 ()
{
	plane	p0{ float3(0.0f), float3(0.0f, 1.0f, 0.0f), float3(1.0f, 0.0f, 1.0f) };
	plane	p1{ float3(0.0f), float3(0.0f, 1.0f, 0.0f), float3(-1.0f, 0.0f, 1.0f) };

	float3	a1;
	bool	i1 = p0.GetIntersection( p1, OUT a1 );
	GX_UNUSED( i1 );

	TEST( All( Equals( a1, float3(0.0f, -1.0f, 0.0f) ) ) );

	float3	a0;
	bool	i0 = p0.GetIntersection( float3(1.0f, 0.0f, 0.0f).Normalized(), float3(-2.0f, 0.0f, 1.0f), OUT a0 );
	GX_UNUSED( i0 );

	float3 n0, n1, n2;
	p0.GetPerpendiculars( n0, n1, n2 );
}


extern void Test_Math_Plane ()
{
	Test_Plane0();
}
