// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Dimensions/PhysicsValueMath.h"
#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Algorithms/StringParser.h"

namespace GXPhysics
{
namespace Astrophysics
{

	//
	// Axial Rotation
	//

	template <typename T>
	struct AxialRotation final
	{
	// types
	public:
		using Self		= AxialRotation<T>;
		using Days_t	= typename DefaultPhysicsValues< T >::Days;
		using Rad_t		= Radians<T>;
		using Value_t	= T;


	// variables
	public:
		// static
		Rad_t		axisTilt;		// axial tilt
		Days_t		period;			// sidereal rotation period

		// dynamic
		Rad_t		angle;			// sidereal rotation angle


	// methods
	public:
		AxialRotation (GX_DEFCTOR)
		{}

		AxialRotation (const Rad_t axisTilt, const Days_t period) :
			axisTilt(axisTilt), period(period)
		{}

		void Update (const Days_t time)
		{
			angle = Rad_t::Pi() * T(2) * time / period;
		}


		DEBUG_ONLY(
		String  ToString () const
		{
			String	s;
			s	<< "axial tilt: " << Degrees<T>( axisTilt ).ToString()
				<< "\nsidereal period: " << period.ToDebugString();

			StringParser::IncreaceIndent( s );
			("{\n" >> s) << "\n}";

			return s;
		})
	};



	//
	// Orbit
	//
	
	template <typename T>
	struct Orbit final
	{
	// types
	public:
		using Self			= Orbit<T>;
		using Value_t		= T;
		using AU_t			= typename DefaultPhysicsValues< T >::AstronomicalUnits;
		using AU3_t			= PhysicsValueVec< AU_t, 3 >;
		using Days_t		= typename DefaultPhysicsValues< T >::Days;
		using Rad_t			= Radians<T>;
		using Vel_t			= typename DefaultPhysicsValues< T >::MetersPerSeconds;
		using Position3_t	= AU3_t;
		using Length_t		= AU_t;


	// variables
	public:
		// static
		AU_t		semiMajorAxis;		// semi-major axis
		Days_t		orbitalPeriod;		// orbital period
		Rad_t		longOfAscNode;		// longitude of ascending node (start position depend of this)
		Rad_t		inclination;		// inclination (z coord) to sun equator
		Rad_t		perihelionArg;		// argument of perihelion (start position depend of this)
		Value_t		eccentricity;		// eccentricity
		Days_t		startTime;			// start orbital rotation time

		// dynamic
		AU3_t		position;			// current position in orbit


	// methods
	public:
		Orbit (GX_DEFCTOR) : eccentricity(0)
		{}

		void CalcPosition (OUT AU3_t &pos, Days_t time) const;

		void Update (Days_t time);
		//void Update2 (Days_t time, Days_t timeDelta, OUT MetersPerSeconds3 &velocity);

		void GetPoints (OUT Array<AU3_t> &points, usize maxPoints) const;

		AU_t	SemiMinorAxis ()	const;
		AU_t	ApoapsisRadius ()	const;	// max radius
		AU_t	PeriapsisRadius ()	const;	// min radius

		AU_t	Length ()			const;
		Vel_t	MeanVelocity ()		const;


		DEBUG_ONLY( String  ToString () const; )
	};



/*
=================================================
	CalcPosition
=================================================
*/
	template <typename T>
	inline void Orbit<T>::CalcPosition (OUT AU3_t &pos, const Days_t time) const
	{
		if ( orbitalPeriod.IsZero() )
		{
			pos = AU3_t();
			return;
		}

		const Rad_t		eo_M = T(2) * Rad_t::Pi() * ( (time + startTime) / orbitalPeriod );
		const Rad_t		eo_E = eo_M + ( eccentricity - eccentricity*eccentricity*eccentricity * T(0.5) ) * Sin( eo_M );
		const Rad_t		eo_v = T(2) * ATan( Sqrt( ( T(1) + eccentricity ) / ( T(1) - eccentricity ) ) * Tan( eo_E * T(0.5) ) );
		const AU_t		eo_r = ( semiMajorAxis * ( T(1) - eccentricity*eccentricity ) ) / ( T(1) + eccentricity * Cos( eo_v ) );
		const Rad_t		eo_u = eo_v + perihelionArg;
		
		const Vec<T,2>	sco = SinCos( longOfAscNode );
		const Vec<T,2>	sci = SinCos( inclination );
		const Vec<T,2>	scu = SinCos( eo_u );

		pos.x = ( eo_r * ( scu[1]*sco[1] - scu[0]*sco[0]*sci[1] ) );
		pos.y = ( eo_r * ( scu[1]*sco[0] + scu[0]*sco[1]*sci[1] ) );
		pos.z = ( eo_r * scu[0] * sci[0] );
	}

/*
=================================================
	Update
=================================================
*/
	template <typename T>
	inline void Orbit<T>::Update (const Days_t time)
	{
		CalcPosition( OUT position, time );
	}
	
/*
=================================================
	Update2
=================================================
*
	template <typename T>
	inline void Orbit<T>::Update2 (Days_t time, Days_t timeDelta, OUT MetersPerSeconds3 &velocity)
	{
		CHECK( not timeDelta.IsZero() );

		const length3_t	last_pos = position;

		Update( time );
		velocity = ( position - last_pos ) / timeDelta;
	}

/*
=================================================
	GetPoints
=================================================
*/
	template <typename T>
	inline void Orbit<T>::GetPoints (OUT Array<AU3_t> &points, usize maxPoints) const
	{
		points.Clear();

		const Days_t	step = orbitalPeriod / T(maxPoints);
		AU3_t			pos;

		for (usize i = 0; i < maxPoints; ++i)
		{
			CalcPosition( OUT pos, i*step );
			points.PushBack( pos );
		}
	}
	
/*
=================================================
	SemiMinorAxis
=================================================
*/
	template <typename T>
	inline typename Orbit<T>::AU_t  Orbit<T>::SemiMinorAxis () const
	{
		return semiMajorAxis * Sqrt( T(1.0) - Square( eccentricity ) );
	}
	
/*
=================================================
	ApoapsisRadius
=================================================
*/
	template <typename T>
	inline typename Orbit<T>::AU_t  Orbit<T>::ApoapsisRadius () const
	{
		return ( T(1.0) + Abs(eccentricity) ) * semiMajorAxis;
	}
	
/*
=================================================
	PeriapsisRadius
=================================================
*/
	template <typename T>
	inline typename Orbit<T>::AU_t  Orbit<T>::PeriapsisRadius () const
	{
		return ( T(1.0) - Abs(eccentricity) ) * semiMajorAxis;
	}

/*
=================================================
	Length
=================================================
*/
	template <typename T>
	inline typename Orbit<T>::AU_t  Orbit<T>::Length () const
	{
		const T  a = semiMajorAxis.ref();
		const T  b = SemiMinorAxis().ref();
		return AU_t( T(4) * Pi<T> * ( a * b + Square( a - b ) ) / ( a + b ) );
	}
	
/*
=================================================
	MeanVelocity
=================================================
*/
	template <typename T>
	inline typename Orbit<T>::Vel_t  Orbit<T>::MeanVelocity () const
	{
		return Length() / orbitalPeriod;
	}

/*
=================================================
	ToString
=================================================
*/
	DEBUG_ONLY(
	template <typename T>
	inline String  Orbit<T>::ToString () const
	{
		String	s;
		s	<< "semi-major axis: " << semiMajorAxis.ToDebugString()
			<< "\norbital period: " << orbitalPeriod.ToDebugString()
			<< "\nlongitude of ascending node: " << siDegrees( longOfAscNode ).ToString()
			<< "\ninclination: " << siDegrees( inclination ).ToString()
			<< "\nargument of perihelion: " << siDegrees( perihelionArg ).ToString()
			<< "\neccentricity: " << eccentricity
			<< "\nstart orbital rotation time: " << startTime.ToDebugString();
		StringParser::IncreaceIndent( s );
		("{\n" >> s) << "\n}";
		return s;
	})
	

}	// SpaceSimulator
}	// SpaceImpire
