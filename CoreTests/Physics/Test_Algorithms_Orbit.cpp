// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/Physics/Common.h"
#include "Core/Physics/Dimensions/DefaultTypes.h"

using namespace GXPhysics;
using namespace GXPhysics::Astrophysics;


static void TestOrbit1 ()
{
	Orbit<real>	o;

	o.semiMajorAxis	= AstronomicalUnits( 1.0_r );
	o.orbitalPeriod	= Days( 365.256363004_r );
	o.longOfAscNode	= (Rad) Deg( 348.73936_r );
	o.inclination	= (Rad) Deg( 7.155_r );
	o.perihelionArg	= (Rad) Deg( 114.20783_r );
	o.eccentricity	= 0.01671123_r;

	Orbit<real>::Position3_t	pos0, pos1, pos2, pos3, pos4;
	Orbit<real>::Length_t		len0, len1, len2, len3, len4;

	o.Update( o.orbitalPeriod * 0.0_r );	pos0 = o.position;	len0 = pos0.Length();
	o.Update( o.orbitalPeriod * 0.25_r );	pos1 = o.position;	len1 = pos1.Length();	
	o.Update( o.orbitalPeriod * 0.5_r );	pos2 = o.position;	len2 = pos2.Length();
	o.Update( o.orbitalPeriod * 0.75_r );	pos3 = o.position;	len3 = pos3.Length();
	o.Update( o.orbitalPeriod * 1.0_r );	pos4 = o.position;	len4 = pos4.Length();

	TEST( All(GXPhysics::Equals( pos0, pos4 )) );
	TEST( Equals( len0, len4 ) );
	TEST( Equals( len1, len3 ) );
	TEST( Equals( MiddleValue( len0, len2 ), o.semiMajorAxis ) );
}


extern void Test_Algorithms_Orbit ()
{
	TestOrbit1();
}
