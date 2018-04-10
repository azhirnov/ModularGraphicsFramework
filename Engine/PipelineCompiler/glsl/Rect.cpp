// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Math2D_Rect (OUT String &src)
	{
		src << R"#(/*
	Rectangle.

	TODO: generate RectF, RectD, RectI, RectU...
	
	license: free

	depends of:
		Common/Cmp
		Math/Math
*/


struct Rect
{
	real2	min;	// left, bottom
	real2	max;	// right, top
};


Rect	Rect_FromPoints (const real2 p0, const real2 p1);
bool	Rect_Contains (const Rect rect, const real2 point);
bool	Rect_Intersects (const Rect left, const Rect right);


//-----------------------------------------------------------------------------


/*
=================================================
	Rect_FromPoints
=================================================
*/
Rect	Rect_FromPoints (const real2 p0, const real2 p1)
{
	Rect	rect;
	rect.min = real2( Min( p0.x, p1.x ), Min( p0.y, p1.y ) );
	rect.max = real2( Max( p0.x, p1.x ), Max( p0.y, p1.y ) );
	return rect;
}

/*
=================================================
	Rect_Contains
=================================================
*/
bool	Rect_Contains (const Rect rect, const real2 point)
{
	return	AllGreaterEqual( point, rect.min ) and
			AllLessEqual( point, rect.max );
}

/*
=================================================
	Rect_Intersects
=================================================
*/
bool	Rect_Intersects (const Rect left, const Rect right)
{
	return	( AllLess( left.min, right.max ) and AllGreater( left.max, right.min ) ) or
			( AllLess( right.max, left.min ) and AllGreater( right.min, left.max ) );
}
)#";

	}
}	// glsl_vfs
