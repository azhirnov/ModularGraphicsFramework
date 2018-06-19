// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Math_Quaternion (OUT String &src)
	{
		src << R"#(/*
	Quaternion functions.
	
	license: free
*/

#include <Common/Defines.glsl>

struct quat
{
	real4	data;
};


quat	QIdentity ();
quat	QCreate (const real4 vec);

quat	QNormalize (const quat q);
quat	QInverse (const quat q);

quat	QMul (const quat left, const quat right);
real3	QMul (const quat left, const real3 right);

real	QDot (const quat left, const quat right);
quat	QSlerp (const quat qx, const quat qy, const real factor);

real3	QDirection (const quat q);

quat	QRotationX (const real angleRad);
quat	QRotationY (const real angleRad);
quat	QRotationZ (const real angleRad);
quat	QRotation (const real3 anglesRad);

/*
real3x3 QToMatrix3 (in quat q);
real4x4 QToMatrix4 (in quat q, in real3 pos, in real4 w);
quat QComputeW (in real3 quat3);
real3 QToEuler (in quat q);
quat QFormAngleAxis (real angleRad, in real3 axis);
void QToAngleAxis (in quat q, out real angleRad, out real3 axis);
quat QRotationFromTo (in quat q, in real3 from, in real3 to);
*/


//-----------------------------------------------------------------------------

#include <Common/Cmp.glsl>
#include <Math/Math.glsl>

/*
=================================================
	QIdentity
=================================================
*/
quat QIdentity ()
{
	quat	ret;
	ret.data = real4( 0.0, 0.0, 0.0, 1.0 );
	return ret;
}

/*
=================================================
	QCreate
=================================================
*/
quat QCreate (const real4 vec)
{
	quat	ret;
	ret.data = vec;
	return ret;
}

/*
=================================================
	QNormalize
=================================================
*/
quat QNormalize (const quat q)
{
	quat	ret = q;
	real	n	= Dot( q.data, q.data );
	
	if ( n == 1.0 )
		return ret;
	
	ret.data /= Sqrt( n );
	return ret;
}

/*
=================================================
	QInverse
=================================================
*/
quat QInverse (const quat q)
{
	quat	ret;
	ret.data.xyz = -q.data.xyz;
	ret.data.w   = q.data.w;
	return ret;
}

/*
=================================================
	QMul
=================================================
*/
quat QMul (const quat left, const quat right)
{
	quat	ret;

	ret.data.xyz	= left.data.w * right.data.xyz + 
					  left.data.xyz * right.data.w +
					  Cross( left.data.xyz, right.data.xyz );

	real4	dt		= left.data.xyzw * right.data.xyzw;
	ret.data.w		= Dot( dt, real4( -1.0, -1.0, -1.0, 1.0 ) );

	return ret;
}

/*
=================================================
	QMul
=================================================
*/
real3 QMul (const quat left, const real3 right)
{
	real3	q	= left.data.xyz;
	real3	uv	= Cross( q, right );
	real3	uuv	= Cross( q, uv );

	return right + ((uv * left.data.w) + uuv) * 2.0;
}

/*
=================================================
	QDot
=================================================
*/
real QDot (const quat left, const quat right)
{
	return Dot( left.data, right.data );
}

/*
=================================================
	QSlerp
=================================================
*/
quat QSlerp (const quat qx, const quat qy, const real factor)
{
	quat	ret;
	real4	qz			= qy.data;
	real	cos_theta	= Dot( qx.data, qy.data );

	if ( cos_theta < 0.0 )
	{
		qz			= -qy.data;
		cos_theta	= -cos_theta;
	}

	if ( cos_theta > 1.0 - Epsilon() )
	{
		ret.data = Lerp( qx.data, qy.data, factor );
	}
	else
	{
		real	angle = ACos( cos_theta );

		ret.data =	( Sin( (1.0 - factor) * angle ) * qx.data +
					  Sin( factor * angle ) * qz ) / Sin( angle );
	}
	return ret;
}

/*
=================================================
	QDirection
=================================================
*/
real3 QDirection (const quat q)
{
	return real3( 2.0 * q.data.x * q.data.z + 2.0 * q.data.y * q.data.w,
				  2.0 * q.data.z * q.data.y - 2.0 * q.data.x * q.data.w,
				  1.0 - 2.0 - q.data.x * q.data.x - 2.0 * q.data.y * q.data.y );
}

/*
=================================================
	QRotationX
=================================================
*/
quat QRotationX (const real angleRad)
{
	quat	q;
	real	a = angleRad * 0.5;

	q.data = real4( Sin(a), 0.0, 0.0, Cos(a) );
	return q;
}

/*
=================================================
	QRotationY
=================================================
*/
quat QRotationY (const real angleRad)
{
	quat	q;
	real	a = angleRad * 0.5;

	q.data = real4( 0.0, Sin(a), 0.0, Cos(a) );
	return q;
}

/*
=================================================
	QRotationZ
=================================================
*/
quat QRotationZ (const real angleRad)
{
	quat	q;
	real	a = angleRad * 0.5;

	q.data = real4( 0.0, 0.0, Sin(a), Cos(a) );
	return q;
}

/*
=================================================
	QRotation
=================================================
*/
quat QRotation (const real3 anglesRad)
{
	return QMul( QMul( QRotationX( anglesRad.x ), QRotationY( anglesRad.y ) ), QRotationZ( anglesRad.z ) );
}
)#";

	}
}	// glsl_vfs
