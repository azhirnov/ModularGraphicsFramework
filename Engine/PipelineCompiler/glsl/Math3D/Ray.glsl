/*
	3D Ray
	
	license: free

	depends of:
		Math/Quaternion
		Math3D/Line3
*/

struct Ray
{
	real3	origin;		// camera (eye, light, ...) position
	real3	pos;		// current position
	real3	dir;		// normalized direction
};


Ray		Ray_Create (const real3 origin, const real3 direction);

// from camera
//Ray		Ray_FromScreen (const real3 origin, const real nearPlane, const int2 screenSize, const int2 screenCoord);

Ray		Ray_FromScreen (const real3 origin, const quat rotation, const real fovX, const real nearPlane,
						const int2 screenSize, const int2 screenCoord);

Ray		Ray_FromSphereScreen (const real3 origin, const real fovX, const real nearPlane, const int2 screenSize, const int2 screenCoord);

// TODO: rename
Ray		Ray_From (const real3 leftBottom, const real3 rightBottom, const real3 leftTop, const real3 rightTop,
				  const real3 origin, const real nearPlane, const float2 unormCoord);

real3	Ray_CalcX (const Ray ray, const real2 pointYZ);
real3	Ray_CalcY (const Ray ray, const real2 pointXZ);
real3	Ray_CalcZ (const Ray ray, const real2 pointXY);

bool	Ray_Contains (const Ray ray, const real3 point);

void	Ray_Rotate (inout Ray ray, const quat rotation);
void	Ray_Move (inout Ray ray, const real length);

real	Ray_Length (const Ray ray);
void	Ray_SetLength (inout Ray ray, const real length);

Line3	Ray_ToLine (const Ray ray, const real length);


//-----------------------------------------------------------------------------


/*
=================================================
	Ray_Create
=================================================
*/
Ray		Ray_Create (const real3 origin, const real3 direction)
{
	Ray	result;
	result.origin	= origin;
	result.pos		= origin;
	result.dir		= direction;
	return result;
}

/*
=================================================
	Ray_FromScreen
----
	create ray for raytracing, raymarching, ...
=================================================
*
Ray		Ray_FromScreen (const real3 origin, const real nearPlane, const int2 screenSize, const int2 screenCoord)
{
	// project screen point to plane
	const int2	scr_center	= screenSize / 2;
	const real2	point		= real2( screenCoord - scr_center ) / real2( scr_center );
	const real3	vec			= real3( point.x, nearPlane, point.y );
	
	Ray	ray;
	ray.origin	= origin;
	ray.dir		= Normalize( vec );
	ray.pos		= ray.origin + ray.dir * nearPlane;

	return ray;
}

/*
=================================================
	Ray_FromScreen
----
	create ray for raytracing, raymarching, ...
=================================================
*/
Ray		Ray_FromScreen (const real3 origin, const quat rotation, const real fovX, const real nearPlane,
						const int2 screenSize, const int2 screenCoord)
{
	real2	scr_size	= real2(screenSize);
	real2	coord		= real2(screenCoord);

	real	ratio		= scr_size.y / scr_size.x;
	real 	fovY 		= fovX * ratio;
	real2 	scale		= nearPlane / Cos( real2(fovX, fovY) * 0.5 );
	real2 	uv 			= (coord - scr_size * 0.5) / (scr_size.x * 0.5) * scale;

	Ray	ray;
	ray.origin	= origin;
	ray.dir		= Normalize( QMul( rotation, Normalize( real3(uv.x, 1.0, uv.y) ) ) );
	ray.pos		= origin + ray.dir * nearPlane;

	return ray;
}

/*
=================================================
	Ray_FromSphereScreen
----
	used when target image projected to sphere
	with horizontal angle 'fovX'
=================================================
*/
Ray		Ray_FromSphereScreen (const real3 origin, const real fovX, const real nearPlane, const int2 screenSize, const int2 screenCoord)
{
	// project screen point to sphere
	const int2	scr_center	= screenSize / 2;
	const real2	point		= real2( screenCoord - scr_center ) / real2( scr_center );
	const real	scr_ratio	= real( screenSize.y ) / real( screenSize.x );
	const real2	sphere_size	= real2( 1.0, scr_ratio ) * fovX * nearPlane;
	const real2	angle		= point * sphere_size * 0.5 / nearPlane;
	const real3	vec			= SphericalToCartesian( angle, nearPlane );
	
	Ray	ray;
	ray.origin	= origin;
	ray.dir		= Normalize( vec );
	ray.pos		= ray.origin + ray.dir * nearPlane;

	return ray;
}

/*
=================================================
	Ray_From
----
	create ray from frustum rays and origin
=================================================
*/
Ray		Ray_From (const real3 leftBottom, const real3 rightBottom, const real3 leftTop, const real3 rightTop,
				  const real3 origin, const real nearPlane, const float2 unormCoord)
{
	const real2	coord	= unormCoord;
	const real3	vec		= Lerp( Lerp( leftBottom, rightBottom, coord.x ),
								Lerp( leftTop, rightTop, coord.x ),
								coord.y );

	Ray	ray;
	ray.origin	= origin;
	ray.dir		= Normalize( vec );
	ray.pos		= ray.origin + ray.dir * nearPlane;

	return ray;
}

/*
=================================================
	Ray_CalcX
=================================================
*/
real3	Ray_CalcX (const Ray ray, const real2 pointYZ)
{
	const real	x = ray.pos.x + ray.dir.x * (pointYZ[1] - ray.pos.z) / ray.dir.z;

	return real3( x, pointYZ[0], pointYZ[1] );
}

/*
=================================================
	Ray_CalcY
=================================================
*/
real3	Ray_CalcY (const Ray ray, const real2 pointXZ)
{
	const real	y = ray.pos.y + ray.dir.y * (pointXZ[1] - ray.pos.z) / ray.dir.z;

	return real3( pointXZ[0], y, pointXZ[1] );
}

/*
=================================================
	Ray_CalcZ
=================================================
*/
real3	Ray_CalcZ (const Ray ray, const real2 pointXY)
{
	const real	z = ray.pos.z + ray.dir.z * (pointXY[0] - ray.pos.x) / ray.dir.x;

	return real3( pointXY[0], pointXY[1], z );
}

/*
=================================================
	Ray_Contains
=================================================
*/
bool	Ray_Contains (const Ray ray, const real3 point)
{
	// z(x), z(y)
	const real2	z = ray.pos.zz + ray.dir.zz * (point.xy - ray.pos.xy) / ray.dir.xy;

	// z(x) == z(y) and z(x) == point.z
	return Equals( z.x, z.y ) and Equals( z.x, point.z );
}

/*
=================================================
	Ray_Rotate
=================================================
*/
void	Ray_Rotate (inout Ray ray, const quat rotation)
{
	// ray.origin - const
	ray.dir = Normalize( QMul( rotation, ray.dir ) );
	ray.pos	= Distance( ray.origin, ray.pos ) * ray.dir;
}

/*
=================================================
	Ray_Move
=================================================
*/
void	Ray_Move (inout Ray ray, const real length)
{
	ray.pos += ray.dir * length;
}

/*
=================================================
	Ray_Length
=================================================
*/
real	Ray_Length (const Ray ray)
{
	return Distance( ray.origin, ray.pos );
}

/*
=================================================
	Ray_SetLength
=================================================
*/
void	Ray_SetLength (inout Ray ray, const real length)
{
	ray.pos = ray.origin + ray.dir * length;
}

/*
=================================================
	Ray_ToLine
=================================================
*/
Line3	Ray_ToLine (const Ray ray, const real length)
{
	Line3	result;
	result.begin = ray.pos;
	result.end   = ray.pos + ray.dir * length;
	return result;
}
