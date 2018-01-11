// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


static void Test_Frustum0 ()
{
	using EPlane = Frustum<float>::EPlane;

	PerspectiveCamera<float>	camera;
	Frustum<float>				f;

	camera.Create( Transformation<float>(), Rad(60.0_deg), 1.5f, float2(0.1f, 100.0f) );

	auto m = camera.ViewProjMatrix();

	f.Setup( m );
	
	auto const&	p0 = f.GetPlane( EPlane::Left );
	auto const&	p1 = f.GetPlane( EPlane::Right );
	auto const&	p2 = f.GetPlane( EPlane::Top );
	auto const&	p3 = f.GetPlane( EPlane::Bottom );

	Line2<float> l0{ float2(), p0.Normal().xz() };
	Line2<float> l1{ float2(), p1.Normal().xz() };
	
	Line2<float> l2{ float2(), p2.Normal().yz() };
	Line2<float> l3{ float2(), p3.Normal().yz() };

	Deg		b0 = l0.AngleBetweenLines( l1 ).ToDegrees();
	Deg		b1 = Deg::Pi() - Abs(b0);
	
	Deg		b2 = l2.AngleBetweenLines( l3 ).ToDegrees();
	Deg		b3 = Deg::Pi() - Abs(b2);

	Deg		c0 = ACos(Dot( p0.Normal(), p1.Normal() )).ToDegrees();
	Deg		c1 = ACos(Dot( p2.Normal(), p3.Normal() )).ToDegrees();

	float3	a0, a1, a2, a3;
	bool	i0 = p3.GetIntersection( p0, OUT a0 );
	bool	i1 = p0.GetIntersection( p2, OUT a1 );
	bool	i2 = p1.GetIntersection( p3, OUT a2 );
	bool	i3 = p2.GetIntersection( p1, OUT a3 );

}


extern void Test_Math_Frustum ()
{
	Test_Frustum0();
}
