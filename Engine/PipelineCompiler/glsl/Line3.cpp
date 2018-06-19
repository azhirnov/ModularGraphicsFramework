// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Math3D_Line3 (OUT String &src)
	{
		src << R"#(/*
	3D Line.
	
	license: free
*/

#include <Common/Defines.glsl>

struct Line3
{
	real3	begin;
	real3	end;
};


real3	Line_Direction (const Line3 line);
real	Line_Length (const Line3 line);

real	Line_MinDistance (const Line3 line, const real3 point);


//-----------------------------------------------------------------------------

#include <Math/Math.glsl>
#include <Math/Utils.glsl>

/*
=================================================
	Line_Direction
=================================================
*/
real3	Line_Direction (const Line3 line)
{
	return Normalize( line.end - line.begin );
}

/*
=================================================
	Line_Length
=================================================
*/
real	Line_Length (const Line3 line)
{
	return Distance( line.begin, line.end );
}

/*
=================================================
	Line_MinDistance
=================================================
*/
real	Line_MinDistance (const Line3 line, const real3 point)
{
	const real3	v = line.end - line.begin;
	const real3	p = point - line.begin;

	const real	a = Cross2( v.zy, p.zy );
	const real	b = Cross2( v.xz, p.xz );
	const real	c = Cross2( v.yx, p.yx );

	const real	d = Sqrt( (a*a + b*b + c*c) / Dot( v, v ) );

	return Min( d, Min( Distance( point, line.begin ), Distance( point, line.end ) ) );
}
)#";

	}
}	// glsl_vfs
