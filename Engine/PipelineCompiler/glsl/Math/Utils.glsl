/*
	Other math functions.
	
	license: free

	depends of:
		Math/Math
*/


real2 SphericalCorrection (const real2 snormCoord);
real3 SphericalToCartesian (const real2 spherical, const real radius);

real2 ResCorrectionAspect (const real2 dim);

real Cross2 (const real2 a, const real2 b);

real ToLinearDepth (const real depth, const real2 clipPlanes);
real ToNonLinearDepth (const real depth, const real2 clipPlanes);


//-----------------------------------------------------------------------------


/*
=================================================
	SphericalCorrection
=================================================
*/
real2 SphericalCorrection (const real2 snormCoord)
{
	const real2	c = snormCoord;
	const real	h = 1.0 - Abs( c.y );
	const real	R = 1.0;
	const real	r = Sqrt( 2.0 * h * R - h*h );

	return ( c * real2( r / R, 1.0 ) );
}

/*
=================================================
	SphericalToCartesian
=================================================
*/
real3 SphericalToCartesian (const real2 spherical, const real radius)
{
	const real2	a = SinCos( spherical.x );
	const real2	b = SinCos( spherical.y );
	return real3(	radius * a.x * b.y,
					radius * a.x * b.x,
					radius * a.y );
}

/*
=================================================
	ResCorrectionAspect
=================================================
*/
real2 ResCorrectionAspect (const real2 dim)
{
	// proportional_coords = normalized_coords * aspect
	return	dim.x < dim.y ?
			real2( 1.0, dim.x / dim.y ) :
			real2( dim.y / dim.x, 1.0 );
}

/*
=================================================
	Cross2
=================================================
*/
real Cross2 (const real2 a, const real2 b)
{
	return a.x * b.y - a.y * b.x;
}

/*
=================================================
	ToLinearDepth
----
	to convert non-linear depth from gl_FragCoord.z
	to linear depth in range [0..1]
=================================================
*/
real ToLinearDepth (const real depth, const real2 clipPlanes)
{
	return	( 2.0 * clipPlanes.x * clipPlanes.y ) /
			( clipPlanes.y + clipPlanes.x - ToSNorm( depth ) * (clipPlanes.y - clipPlanes.x) );
}

/*
=================================================
	ToLinearDepth
----
	to convert linear depth to non-linear depth
	(for assigning to gl_FragDepth)
=================================================
*/
real ToNonLinearDepth (const real depth, const real2 clipPlanes)
{
	return ToUNorm( (clipPlanes.y + clipPlanes.x - 2.0 * clipPlanes.x * clipPlanes.y / depth) / (clipPlanes.y - clipPlanes.x) );
}
