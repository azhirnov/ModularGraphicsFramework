// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Dimensions/PhysicsValue.h"
#include "Engine/Physics/Dimensions/PhysicsValueVec.h"

namespace GXPhysics
{

	typedef DefaultPhysicsValues< real >::NonDimensional				NonDimensional;

	// time
	typedef DefaultPhysicsValues< real >::Seconds						Seconds;				// s
	typedef DefaultPhysicsValues< real >::Minutes						Minutes;				// min
	typedef DefaultPhysicsValues< real >::Hours							Hours;					// h
	typedef DefaultPhysicsValues< real >::Days							Days;					// d
	typedef DefaultPhysicsValues< real >::Years							Years;					// y
	typedef DefaultPhysicsValues< real >::MillionYears					MillionYears;			// 1 000 000 * y

	// mass
	typedef DefaultPhysicsValues< real >::Daltons						Daltons;				// Da (a.m.u.)
	typedef DefaultPhysicsValues< real >::Grams							Grams;					// gr
	typedef DefaultPhysicsValues< real >::Kilograms						Kilograms;				// kg
	typedef DefaultPhysicsValues< real >::EarthMass						EarthMass;				// M(ear)
	typedef DefaultPhysicsValues< real >::SolarMass						SolarMass;				// M(sol)

	// mass flow rate
	typedef DefaultPhysicsValues< real >::KilogramsPerSeconds			KilogramsPerSeconds;	// kg / s

	// length
	typedef DefaultPhysicsValues< real >::Meters						Meters;					// m
	typedef DefaultPhysicsValues< real >::Kilometers					Kilometers;				// km
	typedef DefaultPhysicsValues< real >::Megameters					Megameters;				// Mm
	typedef DefaultPhysicsValues< real >::Parsecs						Parsecs;				// pc
	typedef DefaultPhysicsValues< real >::AstronomicalUnits				AstronomicalUnits;		// au
	typedef DefaultPhysicsValues< real >::LightSeconds					LightSeconds;			// c*s
	typedef DefaultPhysicsValues< real >::LightMinutes					LightMinutes;			// c*min
	typedef DefaultPhysicsValues< real >::LightHours					LightHours;				// c*h
	typedef DefaultPhysicsValues< real >::LightDays						LightDays;				// c*d
	typedef DefaultPhysicsValues< real >::LightYears					LightYears;				// c*y
	typedef DefaultPhysicsValues< real >::SolarRadius					SolarRadius;			// R(sol)

	// area
	typedef DefaultPhysicsValues< real >::SquareMeters					SquareMeters;			// m^2

	// volume
	typedef DefaultPhysicsValues< real >::CubicMeters					CubicMeters;			// m^3

	// vector length
	typedef PhysicsValueVec< Meters,3 >									Meters3;				// m[3]
	typedef PhysicsValueVec< AstronomicalUnits, 3 >						AstronomicalUnits3;		// au[3]
	typedef PhysicsValueVec< LightSeconds, 3 >							LightSeconds3;			// c*s[3]
	typedef PhysicsValueVec< LightYears, 2 >							LightYears2;			// c*y[2]
	typedef PhysicsValueVec< LightYears, 3 >							LightYears3;			// c*y[3]
	typedef PhysicsValueVec< LightYears, 4 >							LightYears4;			// c*y[4]
	typedef PhysicsValueVec< Parsecs, 3 >								Parsecs3;				// pc[3]

	// speed
	typedef DefaultPhysicsValues< real >::MetersPerSeconds				MetersPerSeconds;		// m/s
	typedef DefaultPhysicsValues< real >::LightSpeeds					LightSpeeds;			// c
	typedef DefaultPhysicsValues< real >::AstronomicalUnitsPerSeconds	AUperSeconds;			// AU/s

	// vector speed
	typedef PhysicsValueVec< MetersPerSeconds, 3 >						MetersPerSeconds3;		// m/s[3]
	typedef PhysicsValueVec< LightSpeeds, 3 >							LightSpeeds3;			// c[3]
	typedef PhysicsValueVec< AUperSeconds, 3 >							AUperSeconds3;			// AU/s[3]

	// acceleration
	typedef DefaultPhysicsValues< real >::MetersPerSquareSeconds		MetersPerSquareSeconds;	// m/s^2
	typedef DefaultPhysicsValues< real >::GAcceleration					GAcceleration;			// g

	// vector acceleration
	typedef PhysicsValueVec< MetersPerSquareSeconds, 3 >				MetersPerSquareSeconds3;	// m/s^2[3]
	typedef PhysicsValueVec< GAcceleration, 3 >							GAcceleration3;				// g[3]

	// impulse
	typedef DefaultPhysicsValues< real >::KilogramMetersPerSeconds		KilogramMetersPerSeconds;	// kg*m/s

	// force
	typedef DefaultPhysicsValues< real >::Newtons						Newtons;				// N

	// energy
	typedef DefaultPhysicsValues< real >::Joules						Joules;					// J
	typedef DefaultPhysicsValues< real >::Megajoules					Megajoules;				// MJ

	// luminosity
	typedef DefaultPhysicsValues< real >::Watts							Watts;
	typedef DefaultPhysicsValues< real >::SolarLuminosity				SolarLuminosity;		// L(sol)

	// temperature
	typedef DefaultPhysicsValues< real >::Kelvins						Kelvins;				// K

	// density
	typedef DefaultPhysicsValues< real >::KilogramsPerCubicMeters		KilogramsPerCubicMeters;	// kg / m^3
	
	// pressure
	typedef DefaultPhysicsValues< real >::Pascals						Pascals;				// Pa

	// 
	typedef DefaultPhysicsValues< real >::Moles							Moles;					// mol

	//  magnetic flux density
	typedef DefaultPhysicsValues< real >::Teslas						Teslas;					// T

	// molar mass
	typedef DefaultPhysicsValues< real >::GramsPerMoles					GramsPerMoles;			// gr/mol
	typedef DefaultPhysicsValues< real >::KilogramsPerMoles				KilogramsPerMoles;		// kg/mol

	// constants
	typedef DefaultPhysicsValues< real >::GConstant						GConstant;				// G
	typedef DefaultPhysicsValues< real >::AvogadroConstant				AvogadroConstant;		// Na
	typedef DefaultPhysicsValues< real >::VacuumPermeabilityConst		VacuumPermeabilityConst;// mu0
	typedef DefaultPhysicsValues< real >::GasConstant					GasConstant;			// R
	

	
	
namespace Astrophysics
{

/*
=================================================
	DistanceToString
=================================================
*/
	inline String DistanceToString (Meters distance)
	{
		String	str;

		if ( distance > Meters(Parsecs(1.0r)) )
			str << Parsecs(distance).ref() << " pc";
		else
		if ( distance > Meters(LightYears(1.0r)) )
			str << LightYears(distance).ref() << " ly";
		else
		if ( distance > Meters(LightDays(1.0r)) )
			str << LightDays(distance).ref() << " ld";
		else
		if ( distance > Meters(LightHours(1.0r)) )
			str << LightHours(distance).ref() << " lh";
		else
		if ( distance > Meters(LightMinutes(1.0r)) )
			str << LightMinutes(distance).ref() << " lm";
		else
		if ( distance > Meters(LightSeconds(1.0r)) )
			str << LightSeconds(distance).ref() << " ls";
		else
		if ( distance > Meters(AstronomicalUnits(0.01r)) )
			str << AstronomicalUnits(distance).ref() << " au";
		else
			str << distance.ref() << " m";

		return str;
	}
	
}	// Astrophysics
}	// GXPhysics
