// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Math3D_AABBox (OUT String &src)
	{
		src << R"#(/*
	Axis aligned bounding box.
	
	license: free

	depends of:
		Common/Cmp
		Math/Math
*/


struct AABBox
{
	real3	min;
	real3	max;
};

bool	AABBox_IsInside (const AABBox box, const real3 pos);

real3	AABBox_Center (const AABBox box);
real3	AABBox_Size (const AABBox box);
real3	AABBox_GetPointInBox (const AABBox box, const real3 interpolation);

// for particles
real3	AABBox_Wrap (const AABBox box, const real3 pos);
real3	AABBox_Clamp (const AABBox box, const real3 pos);
bool	AABBox_Rebound (const AABBox box, inout real3 pos, inout real3 vel);


//-----------------------------------------------------------------------------


/*
=================================================
	AABBox_IsInside
=================================================
*/
bool AABBox_IsInside (const AABBox box, const real3 pos)
{
	return AllGreaterEqual( pos, box.min ) and AllLessEqual( pos, box.max );
}

/*
=================================================
	AABBox_Center
=================================================
*/
real3 AABBox_Center (const AABBox box)
{
	return (box.min + box.max) * 0.5;
}

/*
=================================================
	AABBox_Size
=================================================
*/
real3 AABBox_Size (const AABBox box)
{
	return (box.max - box.min);
}

/*
=================================================
	AABBox_GetPointInBox
=================================================
*/
real3 AABBox_GetPointInBox (const AABBox box, const real3 interpolation)
{
	return AABBox_Center(box) + AABBox_Size(box) * interpolation * 0.5;
}

/*
=================================================
	AABBox_Wrap
=================================================
*/
real3 AABBox_Wrap (const AABBox box, const real3 pos)
{
	return Wrap( pos, box.min, box.max );
}

/*
=================================================
	AABBox_Clamp
=================================================
*/
real3 AABBox_Clamp (const AABBox box, const real3 pos)
{
	return Clamp( pos, box.min, box.max );
}

/*
=================================================
	AABBox_Rebound
=================================================
*/
bool AABBox_Rebound (const AABBox box, inout real3 pos, inout real3 vel)
{
	if ( AABBox_IsInside( box, pos ) )
		return false;

	pos = AABBox_Clamp( box, pos );
	vel = -vel;

	return true;
}
)#";

	}
}	// glsl_vfs
