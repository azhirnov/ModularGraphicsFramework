// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Dimensions/PhysicsValueMath.h"
#include "Engine/Physics/Dimensions/AstrophysicsConstants.h"
#include "Engine/Physics/CompileTime/PhysTypeInfo.h"
#include "Engine/Physics/CompileTime/GenType.h"

namespace GXPhysics
{
namespace Astrophysics
{

/*
=================================================
	OrbitalSpeed
----
	https://en.wikipedia.org/wiki/Orbital_speed
=================================================
*/
	template <typename Distance, typename Mass>
	ND_ inline PX_AUTO(MetersPerSeconds)  OrbitalSpeed (const Distance distance, const Mass mass)
	{
		STATIC_ASSERT( IsDistanceUnits<Distance> and IsMassUnits<Mass> );

		using T = PXGenType< Distance, Mass >;

		return ( (typename DefaultPhysicsValues<T>::GConstant(T(1)) * mass ) / distance ).Sqrt();
	}

/*
=================================================
	EscapeVelocity
----
	https://en.wikipedia.org/wiki/Escape_velocity
=================================================
*/
	template <typename Distance, typename Mass>
	ND_ inline PX_AUTO(MetersPerSeconds)  EscapeVelocity (const Distance distance, const Mass mass)
	{
		STATIC_ASSERT( IsDistanceUnits<Distance> and IsMassUnits<Mass> );
		
		using T = PXGenType< Distance, Mass >;

		return ( (typename DefaultPhysicsValues<T>::GConstant(T(2)) * mass ) / distance ).Sqrt();
	}
	
/*
=================================================
	GravityAcceleration
=================================================
*/
	template <typename Distance, typename Mass>
	ND_ inline PX_AUTO(MetersPerSquareSeconds)  GravityAcceleration (const Distance distance, const Mass mass)
	{
		STATIC_ASSERT( IsDistanceUnits<Distance> and IsMassUnits<Mass> );
		
		using T = PXGenType< Distance, Mass >;

		return typename DefaultPhysicsValues<T>::GConstant(T(1)) * mass / distance.Square();
	}
	
/*
=================================================
	DistanceFromGravityAcceleration
=================================================
*/
	template <typename Accel, typename Mass>
	ND_ inline PX_AUTO(AstronomicalUnits)  DistanceFromGravityAcceleration (const Accel acceleration, const Mass mass)
	{
		STATIC_ASSERT( IsAccelerationUnits<Accel> and IsMassUnits<Mass> );
		
		using T = PXGenType< Accel, Mass >;

		return ( typename DefaultPhysicsValues<T>::GConstant(T(1)) * mass / acceleration ).Sqrt();
	}
	
/*
=================================================
	Gravity
=================================================
*/
	template <typename Distance, typename Mass1, typename Mass2>
	ND_ inline PX_AUTO(Newtons)  Gravity (const Distance distance, const Mass1 otherMass, const Mass2 selfMass)
	{
		STATIC_ASSERT( IsDistanceUnits<Distance> and IsMassUnits<Mass1> and IsMassUnits<Mass2> );

		return GravityAcceleration( distance, otherMass ) * selfMass;
	}
	
/*
=================================================
	HillSphere
----
	https://en.wikipedia.org/wiki/Hill_sphere
=================================================
*/
	template <typename Mass1, typename Mass2, typename Distance, typename Value>
	ND_ inline PX_AUTO(Meters)  HillSphere (const Mass1 heavierBodyMass, const Mass2 smallerBodyMass, const Distance smallerBodySemiMajorAxis, const Value eccentricity)
	{
		STATIC_ASSERT( IsMassUnits<Mass1> and IsMassUnits<Mass2> and IsDistanceUnits<Distance> );
		
		using T = PXGenType< Mass1, Mass2, Distance, Value >;

		ASSERT( eccentricity < Value(1) );
		return smallerBodySemiMajorAxis * (T(1) - eccentricity) * ( smallerBodyMass / (heavierBodyMass * T(3)) ).template Pow<1,3>();
	}
	
/*
=================================================
	SphereOfInfluence
----
	https://en.wikipedia.org/wiki/Sphere_of_influence_(astrodynamics)
=================================================
*/
	template <typename Mass1, typename Mass2, typename Distance>
	ND_ inline PX_AUTO(Meters)  SphereOfInfluence (const Mass1 heavierBodyMass, const Mass2 smallerBodyMass, const Distance semiMajorAxis)
	{
		STATIC_ASSERT( IsMassUnits<Mass1> and IsMassUnits<Mass2> and IsDistanceUnits<Distance> );

		return semiMajorAxis * (smallerBodyMass / heavierBodyMass).template Pow<2,5>();
	}

/*
=================================================
	RocheLobe
----
	https://en.wikipedia.org/wiki/Roche_lobe
=================================================
*/
	template <typename Mass1, typename Mass2>
	ND_ inline PX_AUTO(AstronomicalUnits)  RocheLobe (const Mass1 selfMass, const Mass2 otherMass)
	{
		STATIC_ASSERT( IsMassUnits<Mass1> and IsMassUnits<Mass2> );
		
		using T = PXGenType< Mass1, Mass2 >;

		const T		q	= ( selfMass / otherMass ).Get();
		const T		q23	= Pow( q, T(2) / T(3) );
		const T		q13	= Pow( q, T(1) / T(3) ); 

		return DefaultPhysicsValues<T>::AstronomicalUnits( ( T(0.49) * q23 ) / ( T(0.6) * q23 + Ln( T(1) + q13 ) ) );
	}
	
/*
=================================================
	RocheLimit
----
	https://en.wikipedia.org/wiki/Roche_limit
=================================================
*/
	struct RocheLimit
	{
		template <typename Mass1, typename Mass2, typename Radius>
		ND_ static PX_AUTO(Meters)  ForRigid (const Mass1 primaryMassOrDensity, const Mass2 satelliteMassOrDensity, const Radius satelliteRadius)
		{
			STATIC_ASSERT( (IsMassUnits<Mass1> and IsMassUnits<Mass2>) or (IsDensityUnits<Mass1> and IsDensityUnits<Mass2>) );
			STATIC_ASSERT( IsDistanceUnits<Radius> );
			
			using T = PXGenType< Mass1, Mass2, Radius >;

			return ( primaryMassOrDensity * T(2) / satelliteMassOrDensity ).template Pow<1,3>() * satelliteRadius;
		}
		
		
		template <typename Mass1, typename Mass2, typename Radius>
		ND_ static PX_AUTO(Meters)  ForFluid (const Mass1 primaryMassOrDensity, const Mass2 satelliteMassOrDensity, const Radius satelliteRadius)
		{
			// TODO: check is mass supported ???
			STATIC_ASSERT( (IsMassUnits<Mass1> and IsMassUnits<Mass2>) or (IsDensityUnits<Mass1> and IsDensityUnits<Mass2>) );
			STATIC_ASSERT( IsDistanceUnits<Radius> );
			
			using T = PXGenType< Mass1, Mass2, Radius >;

			return ( primaryMassOrDensity * T(2) / satelliteMassOrDensity ).template Pow<1,3>() * satelliteRadius;
		}
	};

/*
=================================================
	Kepler's Laws
----
	https://en.wikipedia.org/wiki/Kepler%27s_laws_of_planetary_motion
=================================================
*/
	struct KeplersLaws
	{
		// with star //
		template <typename Time, typename Distance>
		ND_ static PX_AUTO(SolarMass)  Mass (Time orbitalPeriod, Distance semiMajorAxis)
		{
			STATIC_ASSERT( IsTimeUnits<Time> and IsDistanceUnits<Distance> );
			
			using T = PXGenType< Time, Distance >;

			return	( Square( GXMath::Pi<T> * T(2) ) * semiMajorAxis.template Pow<3>() ) /
					( orbitalPeriod.template Pow<2>() * typename DefaultPhysicsValues<T>::GConstant(T(1)) );
		}
		
		template <typename Mass, typename Distance>
		ND_ static PX_AUTO(Days)  Period (Mass rootObjectMass, Distance semiMajorAxis)
		{
			STATIC_ASSERT( IsMassUnits<Mass> and IsDistanceUnits<Distance> );

			using T = PXGenType< Mass, Distance >;

			return ( ( Square( GXMath::Pi<T> * T(2) ) * semiMajorAxis.template Pow<3>() ) /
					 ( typename DefaultPhysicsValues<T>::GConstant(T(1)) * rootObjectMass ) ).Sqrt();
		}
		
		template <typename Mass, typename Time>
		ND_ static PX_AUTO(AstronomicalUnits)  SemiMajorAxis (Mass rootObjectMass, Time orbitalPeriod)
		{
			STATIC_ASSERT( IsMassUnits<Mass> and IsTimeUnits<Time> );
			
			using T = PXGenType< Mass, Time >;

			return (orbitalPeriod.template Pow<2>() * typename DefaultPhysicsValues<T>::GConstant(T(1)) *
					rootObjectMass / Square( GXMath::Pi<T> * T(2) )).template Pow<1,3>();
		}
		
		template <typename Mass, typename Distance, typename Time>
		ND_ static bool  Equation (Mass rootObjectMass, Distance semiMajorAxis, Time orbitalPeriod)
		{
			STATIC_ASSERT( IsMassUnits<Mass> and IsDistanceUnits<Distance> and IsTimeUnits<Time> );
			
			using T = PXGenType< Mass, Distance, Time >;

			const T		left	= ( semiMajorAxis.template Pow<3>() / orbitalPeriod.template Pow<2>() ).Get();
			const T		right	= ( typename DefaultPhysicsValues<T>::GConstant(T(1)) * rootObjectMass / Square( GXMath::Pi<T> * T(2) ) ).Get();
			const T		percent	= Abs( (left - right) / right );

			return percent < T(0.01);
		}


		// with other object //
		template <typename Distance1, typename Distance2, typename Time>
		ND_ static PX_AUTO(Days)  Period (Distance1 semiMajorAxis1, Distance2 semiMajorAxis2, Time orbitalPeriod2)
		{
			STATIC_ASSERT( IsDistanceUnits<Distance1> and IsDistanceUnits<Distance2> and IsTimeUnits<Time> );

			return ( orbitalPeriod2.template Pow<2>() * semiMajorAxis1.template Pow<3>() / semiMajorAxis2.template Pow<3>() ).Sqrt();
		}
	};

}	// Astrophysics
}	// GXPhysics
