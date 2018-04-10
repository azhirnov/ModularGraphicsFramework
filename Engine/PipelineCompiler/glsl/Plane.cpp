// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Math3D_Plane (OUT String &src)
	{
		src << R"#(/*
	3D Plane
	
	license: free

	depends of:
		Math3D/Line3
*/

struct Plane
{
	real3	normal;
	real3	point;
};


Plane	Plane_Create (real3 p0, real3 p1, real3 p2);
bool	Plane_RayIntersect (const Plane plane, const Line3 line, out real3 result);


//-----------------------------------------------------------------------------


/*
=================================================
	Plane_Create
=================================================
*/
Plane	Plane_Create (real3 p0, real3 p1, real3 p2)
{
	Plane	plane;
	plane.normal = Normalize( Cross( p1 - p0, p2 - p0 ) );
	plane.point  = p0;
	return plane;
}

/*
=================================================
	Plane_RayIntersect
=================================================
*/
bool	Plane_RayIntersect (const Plane plane, const Line3 line, out real3 result)
{
	const real3	u	= Line_Direction( line );
	const real3	n	= plane.normal;
	const real3	w	= line.begin - plane.point;

	const real	nu	= Dot( n, u );
	const real	nw	= Dot( n, w );

	// is parallel?
	if ( IsZero( nu ) )
		return false;

	result = (-nw / nu) * u + line.begin;
	return true;
}
)#";

	}
}	// glsl_vfs
