// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


#ifdef GX_PHYSICS_DIMENSIONS_ENABLED
static void TestPhysicsValue ()
{
	typedef DefaultPhysicsValues< float >::Seconds					Sec;
	typedef DefaultPhysicsValues< float >::Meters					M;
	typedef DefaultPhysicsValues< float >::Millimeters				MM;
	typedef DefaultPhysicsValues< float >::Kilograms				Kg;
	typedef DefaultPhysicsValues< float >::MetersPerSeconds			Vel;
	typedef DefaultPhysicsValues< float >::MetersPerSquareSeconds	Accel;
	typedef DefaultPhysicsValues< float >::Newtons					N;
	typedef DefaultPhysicsValues< float >::GConstant				G;
	typedef DefaultPhysicsValues< float >::SolarMass				SM;
	typedef DefaultPhysicsValues< float >::AstronomicalUnits		AU;

	typedef DefaultPhysicsValues< float >::Daltons			Da;
	typedef DefaultPhysicsValues< float >::Grams			Gr;
	typedef DefaultPhysicsValues< float >::Moles			Mol;
	typedef DefaultPhysicsValues< float >::GramsPerMoles	MolMass;

	typedef PhysicsValueVec< M, 1 >		M1;
	typedef PhysicsValueVec< M, 3 >		M3;
	typedef PhysicsValueVec< MM, 3 >	MM3;
	typedef PhysicsValueVec< Vel, 3 >	Vel3;
	
	const M		distance = M(10.0f);
	const Sec	time = Sec(2.0f);
	const Kg	mass = Kg(8.0f);
	const MM	dist = M(1.0f);
	const Sec	y = DefaultPhysicsValues< float >::Years(1.0f);

	Vel		velocity		= distance / time;
	Sec		time2			= distance / velocity;
	//Sec		time3			= velocity / distance;	// error
	Accel	acceleration	= velocity / time;
	//Accel	accel2			= distance / time.Pow< -2 >();	// error
	N		force			= acceleration * mass;

	M		length			= velocity * time + acceleration * time.Square() * 0.5f;

	float	f = length / length;

	length.To< MM >();

	Vel3	v3 = MM3( M3( M(1.0f), M(2.0f), M(3.0f) ) ) / Sec( 0.5f );

	Vel		v1 = v3.Length();

	auto m = M1( M(1.0f) ).Volume();

	GX_UNUSED( f, m );
	
	/*WARNING( m.ToDebugString().cstr() );

	WARNING( time2.ToString().cstr() );
	WARNING( velocity.ToString().cstr() );
	WARNING( acceleration.ToString().cstr() );
	WARNING( force.ToString().cstr() );
	WARNING( dist.ToString().cstr() );
	
	auto n = G(1) * (SM(1) * SM(2)) / AU(1).Square();

	WARNING( n.ToDebugString().cstr() );*/
}

#else

static void TestPhysicsValue ()
{
}
#endif	// GX_PHYSICS_DIMENSIONS_ENABLED


extern void Test_Dimensions_PhysicsValue ()
{
	STATIC_ASSERT( sizeof(Bytes<uint>) == sizeof(uint) );
	STATIC_ASSERT( sizeof(Bits<uint>) == sizeof(uint) );

	TestPhysicsValue();
}
