// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Math2D_Line2 (OUT String &src)
	{
		src << R"#(/*
	2D Line.
	
	license: free

	depends of:
		Common/Defines
		Math/Math
		Math/Utils
		Math2D/Rect
*/

struct Line2
{
	real2	begin;
	real2	end;
};


Rect	Line_ToRect (const Line2 line);

bool	Line_Contains (const Line2 line, const real2 point);

bool	Line_RayIntersection (const Line2 left, const Line2 right, out real2 result);
bool	Line_Intersection (const Line2 left, const Line2 right, out real2 result);

bool	Line_RayRectIntersection (const Line2 line, const Rect rect, out Line2 result);
bool	Line_RectIntersection (const Line2 line, const Rect rect, out Line2 result);

real3	Line_GetEquation (const Line2 line);	// returns ABC
real2	Line_GetEquationY (const Line2 line);	// returns KB

real2	Line_Direction (const Line2 line);
real	Line_Length (const Line2 line);

Line2	Line_RayPerpendicular (const Line2 line, const real2 point);
bool	Line_Perpendicular (const Line2 line, const real2 point, out Line2 result);

real	Line_MinDistance (const Line2 line, const real2 point);


//-----------------------------------------------------------------------------


/*
=================================================
	Line_ToRect
=================================================
*/
Rect	Line_ToRect (const Line2 line)
{
	return Rect_FromPoints( line.begin, line.end );
}

/*
=================================================
	Line_Contains
=================================================
*/
bool	Line_Contains (const Line2 line, const real2 point)
{
	const Rect	rect = Line_ToRect( line );

	if ( Rect_Contains( rect, point ) )
		return false;

	const real3	abc = Line_GetEquation( line );

	return IsZero( Dot( point, abc.xy ) + abc.z );
}

/*
=================================================
	Line_RayIntersection
=================================================
*/
bool	Line_RayIntersection (const Line2 a, const Line2 b, out real2 result)
{
	const real2	av = a.begin - a.end;
	const real2	bv = b.begin - b.end;
	const real	c  = Cross2( av, bv );

	if ( IsZero( c ) )
		return false;

	const real	ca = Cross2( av, av );
	const real	cb = Cross2( bv, bv );

	result = ( ca * bv - cb * av ) / c;
	return true;
}

/*
=================================================
	Line_Intersection
=================================================
*/
bool	Line_Intersection (const Line2 a, const Line2 b, out real2 result)
{
	if ( not Line_RayIntersection( a, b, OUT result ) )
		return false;

	return	Rect_Contains( Line_ToRect( a ), result ) and
			Rect_Contains( Line_ToRect( b ), result );
}

/*
=================================================
	Line_RayRectIntersection
----
	returns line that contains intersection points
=================================================
*/
bool	Line_RayRectIntersection (const Line2 line, const Rect rect, out Line2 result)
{
	const Rect	line_rect = Line_ToRect( line );

	if ( not Rect_Intersects( line_rect, rect ) )
		return false;

	real2	cross_points[4] = { real2(0.0), real2(0.0), real2(0.0), real2(0.0) };
	int		i = 0;

	Line2	line_b;
	real2	tmp;

	// left
	line_b.begin = rect.min;
	line_b.end   = real2( rect.min.x, rect.max.y );

	if ( Line_RayIntersection( line, line_b, OUT tmp ) and Rect_Contains( rect, tmp ) )
	{
		cross_points[i++] = tmp;
	}

	// right
	line_b.begin = real2( rect.max.x, rect.min.y );
	line_b.end   = rect.max;

	if ( Line_RayIntersection( line, line_b, OUT tmp ) and Rect_Contains( rect, tmp ) )
	{
		cross_points[i++] = tmp;
	}

	// bottom
	line_b.begin = rect.min;
	line_b.end   = real2( rect.max.x, rect.min.y );
	
	if ( Line_RayIntersection( line, line_b, OUT tmp ) and Rect_Contains( rect, tmp ) )
	{
		cross_points[i++] = tmp;
	}

	// top
	line_b.begin = real2( rect.min.x, rect.max.y );
	line_b.end   = rect.max;
	
	if ( Line_RayIntersection( line, line_b, OUT tmp ) and Rect_Contains( rect, tmp ) )
	{
		cross_points[i++] = tmp;
	}

	result.begin = cross_points[0];
	result.end   = cross_points[0];

	for (; i > 1; --i)
	{
		if ( NotAllEqual( cross_points[i-1], result.end ) )
			break;
	}

	return i > 0;
}

/*
=================================================
	Line_RectIntersection
----
	returns line that contains intersection points
=================================================
*/
bool	Line_RectIntersection (const Line2 line, const Rect rect, out Line2 result)
{
	if ( not Line_RayRectIntersection( line, rect, OUT result ) )
		return false;

	int			count		= 2;
	const Rect	line_rect	= Line_ToRect( line );

	if ( not Rect_Contains( line_rect, result.begin ) )
	{
		result.begin = result.end;
		--count;
	}
	
	if ( not Rect_Contains( line_rect, result.end ) )
	{
		result.end = result.begin;
		--count;
	}

	return count > 2;
}

/*
=================================================
	Line_GetEquation
=================================================
*/
real3	Line_GetEquation (const Line2 line)
{
	// Ax + By + C = 0
	return real3( line.begin.y - line.end.y,
				  line.end.x - line.begin.x,
				  Cross2( line.begin, line.end ) );
}

/*
=================================================
	Line_GetEquationY
=================================================
*/
real2	Line_GetEquationY (const Line2 line)
{
	const real3	abc = Line_GetEquation( line );

	return SafeDiv( -abc.xz, abc.y, 0.0 );
}

/*
=================================================
	Line_Direction
=================================================
*/
real2	Line_Direction (const Line2 line)
{
	return Normalize( line.end - line.begin );
}

/*
=================================================
	Line_Length
=================================================
*/
real	Line_Length (const Line2 line)
{
	return Distance( line.begin, line.end );
}

/*
=================================================
	Line_RayPerpendicular
=================================================
*/
Line2	Line_RayPerpendicular (const Line2 line, const real2 point)
{
	const real2	vec		= line.end - line.begin;
	const real2	vec_sq	= vec * vec;
	real2		base;
	
	base.x = ( line.begin.x * vec_sq.y + point.x * vec_sq.x + vec.x * vec.y * (point.y - line.begin.y) ) / (vec_sq.x + vec_sq.y);
	base.y = vec.x * (point.x - base.x) / vec.y + point.y;

	Line2	result;
	result.begin = point;
	result.end   = base;

	return result;
}

/*
=================================================
	Line_Perpendicular
=================================================
*/
bool	Line_Perpendicular (const Line2 line, const real2 point, out Line2 result)
{
	result = Line_RayPerpendicular( line, point );

	return Line_Contains( line, result.end );
}

/*
=================================================
	Line_Perpendicular
=================================================
*/
real	Line_MinDistance (const Line2 line, const real2 point)
{
	const real3	abc	= Line_GetEquation( line );
	const real	d	= ( abc.x * point.x + abc.y * point.y + abc.z ) / Sqrt( Dot( abc.xy, abc.xy ) );
	
	return Min( d, Min( Distance( point, line.begin ), distance( point, line.end ) ) );
}
)#";

	}
}	// glsl_vfs
