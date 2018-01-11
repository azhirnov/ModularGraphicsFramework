/*
	Default signed distance fields.

	from http://iquilezles.org/www/articles/distfunctions/distfunctions.htm

	license: ?

	depends of:
		Math/Quaternion
*/


// Shapes
real SDF_Sphere (const real3 position, const real radius);
real SDF_Ellipsoid (const real3 position, const real3 radius);
real SDF_Box (const real3 position, const real3 halfSize);
real SDF_Torus (const real3 position, const real2 firstAndSecondRadius);
real SDF_Cylinder (const real3 position, const real2 radiusHeight);
//real SDF_Cone (const real3 position, const real2 radiusHeight);
real SDF_Plane (const real3 position, const real4 n);
real SDF_HexagonalPrism (const real3 position, const real2 h);
real SDF_TriangularPrism (const real3 position, const real2 h);
real SDF_Capsule (const real3 position, const real3 a, const real3 b, const real r);


// Unions
real SDF_OpUnite (const real d1, const real d2);
real SDF_OpSub (const real d1, const real d2);
real SDF_OpIntersect (const real d1, const real d2);


// Transformation
real3 SDF_Move (const real3 position, const real3 move);
real3 SDF_Rotate (const real3 position, const quat q);
real3 SDF_Transform (const real3 position, const quat q, const real3 move);


//-----------------------------------------------------------------------------


// Shapes

real SDF_Sphere (const real3 position, const real radius)
{
	return Length( position ) - radius;
}


real SDF_Ellipsoid (const real3 position, const real3 radius)
{
    return ( Length( position / radius ) - 1.0 ) * Min( Min( radius.x, radius.y ), radius.z );
}


real SDF_Box (const real3 position, const real3 halfSize)
{
	const real3	d = Abs( position ) - halfSize;
	return Min( Max( d.x, Max( d.y, d.z ) ), 0.0 ) + Length( Max( d, 0.0 ) );
}


real SDF_Torus (const real3 position, const real2 firstAndSecondRadius)
{
	const real2	q = real2( Length( position.xz ) - firstAndSecondRadius.x, position.y );

	return Length( q ) - firstAndSecondRadius.y;
}


real SDF_Cylinder (const real3 position, const real2 radiusHeight)
{
	const real2	d = Abs( real2( Length( position.xz ), position.y ) ) - radiusHeight;

	return Min( Max( d.x, d.y ), 0.0 ) + Length( Max( d, 0.0 ) );
}

/*
real SDF_Cone (real3 position, real2 radiusHeight)
{
	real3	up   = real3( 0.0, radiusHeight.y, 0.0 );
	real3	down = -up;

	down.xz = Normalize( position.xz ) * radiusHeight.x;

	Line3	line;
	Line3_Create( &line, up, down );

	return Line3_MinDistance( &line, position );

	//real2	d = Abs( (real2)( length( position.xz ), position.y ) );// - radiusHeight;

	//d.x -= radiusHeight.x / 

	//return min( max( d.x, d.y ), 0.0 ) + length( max( d, 0.0 ) );

	//return dot( normalize( cone ), (real2)( length( position.xy ), position.z ) );
}
*/

real SDF_Plane (const real3 position, const real4 n)
{
	// n must be normalized
	return Dot( position, n.xyz ) + n.w;
}


real SDF_HexagonalPrism (const real3 position, const real2 h)
{
	const real3	q = Abs( position );

	return Max( q.z - h.y, Max( q.x * 0.866025 + q.y * 0.5, q.y ) - h.x );
}


real SDF_TriangularPrism (const real3 position, const real2 h)
{
    const real3	q = Abs( position );

    return Max( q.z - h.y, Max( q.x * 0.866025 + position.y * 0.5, -position.y ) - h.x * 0.5 );
}


real SDF_Capsule (const real3 position, const real3 a, const real3 b, const real r)
{
	const real3	pa = position - a;
	const real3	ba = position - a;
	const real	h  = Clamp( Dot( pa, ba ) / Dot( ba, ba ), 0.0, 1.0 );

	return Length( pa - ba * h ) - r;
}


// Unions

real SDF_OpUnite (const real d1, const real d2)
{
	return Min( d1, d2 );
}


real SDF_OpSub (const real d1, const real d2)
{
	return Max( d1, -d2 );
}


real SDF_OpIntersect (const real d1, const real d2)
{
	return Max( d1, d2 );
}


// Transformation

real3 SDF_Move (const real3 position, const real3 move)
{
	return position - move;
}


real3 SDF_Rotate (const real3 position, const quat q)
{
	return QMul( QInverse( q ), position );
}


real3 SDF_Transform (const real3 position, const quat q, const real3 move)
{
	return SDF_Rotate( SDF_Move( position, move ), q );
}
