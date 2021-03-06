// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Physics_Magnetism (OUT String &src)
	{
		src << R"#(/*
	Physics functions for magnetism simulations.
	
	license: free
*/

#include <Math/Math.glsl>

real3 LinearMagneticFieldForce (const real3 velocity, const real3 magneticInduction);

real3 SphericalMagneticFieldForce (const real3 velocity, const real3 postion,
								   const real3 northPos, const real3 southPos,
								   const real induction);


//-----------------------------------------------------------------------------


/*
=================================================
	LinearMagneticFieldForce
=================================================
*/
real3 LinearMagneticFieldForce (const real3 velocity, const real3 magneticInduction)
{
	// lorentz force
	return Cross( velocity, magneticInduction );
}

/*
=================================================
	SphericalMagneticFieldForce
=================================================
*/
real3 SphericalMagneticFieldForce (const real3 velocity, const real3 postion,
								   const real3 northPos, const real3 southPos,
								   const real induction)
{
	const real3	nv = postion - northPos;
	const real3	n  = Normalize( nv ) * induction / Dot( nv, nv );
	const real3	sv = southPos - postion;
	const real3	s  = Normalize( sv ) * induction / Dot( sv, sv );
	return LinearMagneticFieldForce( velocity, n + s );
}
)#";

	}
}	// glsl_vfs
