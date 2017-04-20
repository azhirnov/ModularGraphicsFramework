// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathTypes3D.h"

namespace GX_STL
{
namespace GXMath
{
	
/*
=================================================
	CartesianToSpherical
----
	Cartesian -> Spherical

	x - radius
	y - polar angle Theta
	z - azimuthal angle Phi
=================================================
*/
	template <typename T>
	inline Vec<T,3>  CartesianToSpherical (const Vec<T,3> &cartesian)
	{
		CompileTime::MustBeFloat<T>();

		Vec<T,3>	spherical;

		spherical.x = cartesian.Length();
		ASSERT( IsNotZero( spherical.x ) );

		spherical.y = ACos( cartesian.z / spherical.x );
		spherical.z = ATan( cartesian.y, spherical.x );

		return spherical;
	}
	
/*
=================================================
	SphericalToCartesian
----
	Spherical -> Cartesian

	x - radius
	y - polar angle Theta
	z - azimuthal angle Phi
=================================================
*/
	template <typename T>
	inline Vec<T,3>  SphericalToCartesian (const Vec<T,3> &spherical)
	{
		CompileTime::MustBeFloat<T>();

		Vec<T,3>		cartesian;
		Vec<T,2> const	theta = SinCos( spherical.y );
		Vec<T,2> const	phi   = SinCos( spherical.z );

		cartesian.x = spherical.x * theta.x * phi.y;
		cartesian.y = spherical.x * theta.x * phi.x;
		cartesian.z = spherical.x * theta.y;

		return cartesian;
	}

/*
=================================================
	SphericalToCilindrical
----
	Spherical -> Cilindrical

	spherical:					cilindrical:
	x - radius					x - radius
	y - polar angle Theta		y - azimuthal angle Phi
	z - azimuthal angle Phi		z - elevation
=================================================
*/
	template <typename T>
	inline Vec<T,3>  SphericalToCilindrical (const Vec<T,3> &spherical)
	{
		CompileTime::MustBeFloat<T>();

		Vec<T,3>		cilindrical;
		Vec<T,2> const	theta = SinCos( spherical.y );

		cilindrical.x = spherical.x * theta.x;
		cilindrical.y = spherical.z;
		cilindrical.z = spherical.x * theta.y;

		return cilindrical;
	}
	
/*
=================================================
	CilindricalToSpherical
----
	Cilindrical -> Spherical

	spherical:					cilindrical:
	x - radius					x - radius
	y - polar angle Theta		y - azimuthal angle Phi
	z - azimuthal angle Phi		z - elevation
=================================================
*/
	template <typename T>
	inline Vec<T,3>  CilindricalToSpherical (const Vec<T,3> &cilindrical)
	{
		CompileTime::MustBeFloat<T>();

		Vec<T,3>		spherical;

		spherical.x = cilindrical.xz().Length();
		spherical.y = ATan( cilindrical.x, cilindrical.y );
		spherical.z = cilindrical.y;

		return spherical;
	}
	
/*
=================================================
	CartesianToCilindrical
----
	Cartesian -> Cilindrical

	x - radius
	y - azimuthal angle Phi
	z - elevation
=================================================
*/
	template <typename T>
	inline Vec<T,3>  CartesianToCilindrical (const Vec<T,3> &cartesian)
	{
		CompileTime::MustBeFloat<T>();

		Vec<T,3>	cilindrical;

		cilindrical.x = cartesian.xy().Length();
		cilindrical.y = ATan( cartesian.y / cartesian.x );
		cilindrical.z = cartesian.z;

		return cilindrical;
	}
	
/*
=================================================
	CartesianToCilindrical
----
	Cilindrical -> Cartesian

	x - radius
	y - azimuthal angle Phi
	z - elevation
=================================================
*/
	template <typename T>
	inline Vec<T,3>  CilindricalToCartesian (const Vec<T,3> &cilindrical)
	{
		CompileTime::MustBeFloat<T>();

		Vec<T,3>		cartesian;
		Vec<T,2> const	phi = SinCos( cilindrical.y );

		cartesian.x = cilindrical.x * phi.y;
		cartesian.y = cilindrical.x * phi.x;
		cartesian.z = cilindrical.z;

		return cartesian;
	}


}	// GXMath
}	// GX_STL
