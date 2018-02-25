// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/Physics/Common.h"
#include "Engine/Physics/Dimensions/DefaultTypes.h"

using namespace GXPhysics;
using namespace GXPhysics::Astrophysics;


static void TestOrbit1 ()
{
	Orbit<real>	o;

	o.semiMajorAxis	= AstronomicalUnits( 1.0r );
	o.orbitalPeriod	= Days( 365.256363004r );
	o.longOfAscNode	= (Rad) Deg( 348.73936r );
	o.inclination	= (Rad) Deg( 7.155r );
	o.perihelionArg	= (Rad) Deg( 114.20783r );
	o.eccentricity	= 0.01671123r;

	Orbit<real>::Position3_t	pos0, pos1, pos2, pos3, pos4;
	Orbit<real>::Length_t		len0, len1, len2, len3, len4;

	o.Update( o.orbitalPeriod * 0.0r );		pos0 = o.position;	len0 = pos0.Length();
	o.Update( o.orbitalPeriod * 0.25r );	pos1 = o.position;	len1 = pos1.Length();	
	o.Update( o.orbitalPeriod * 0.5r );		pos2 = o.position;	len2 = pos2.Length();
	o.Update( o.orbitalPeriod * 0.75r );	pos3 = o.position;	len3 = pos3.Length();
	o.Update( o.orbitalPeriod * 1.0r );		pos4 = o.position;	len4 = pos4.Length();

	TEST( All(GXPhysics::Equals( pos0, pos4 )) );
	TEST( Equals( len0, len4 ) );
	TEST( Equals( len1, len3 ) );
	TEST( Equals( MiddleValue( len0, len2 ), o.semiMajorAxis ) );
}


extern void Test_Algorithms_Orbit ()
{
	TestOrbit1();
}
