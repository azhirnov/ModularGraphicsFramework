// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Physics_Gravity (OUT String &src)
	{
		src << R"#(/*
	Physics functions for gravity simulations.
	
	license: free
*/

#include <Math/Math.glsl>

real3 GravityForce (const real3 position, const real3 center, const real gravity);
real3 GravityForce (const real3 position, const real3 center, const real mass1, const real mass2);


//-----------------------------------------------------------------------------


/*
=================================================
	GravityForce
----
	calculate force that inversely proportional to
	the square of the distance
=================================================
*/
real3 GravityForce (const real3 position, const real3 center, const real gravity)
{
	const real3 v = center - position;
	return Normalize( v ) * gravity / Dot( v, v );
}

/*
=================================================
	GravityForce
----
	calculate real gravity force based on physics values
=================================================
*/
real3 GravityForce (const real3 position, const real3 center, const real mass1, const real mass2)
{
	return GravityForce( position, center, mass1 * mass2 * real(6.67e-11) );
}
)#";

	}
}	// glsl_vfs
