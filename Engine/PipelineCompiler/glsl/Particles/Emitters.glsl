/*
	Particle emitter shapes.

	license: free

	depends of:
		Math/Math
*/


real3 ParticleEmitter_Plane (const real pointIndex, const real pointsCount);
real3 ParticleEmitter_Plane (const real pointIndex, const real pointsCount, const real ratio);

real3 ParticleEmitter_Circle (const real pointIndex, const real pointsCount);
real3 ParticleEmitter_FillCircle (const real pointIndex, const real pointsCount);

real3 ParticleEmitter_Sphere (const real pointIndex, const real pointsCount);

// this is not a cone shape, only direction
real3 ParticleEmitter_ConeVector (const real pointIndex, const real pointsCount, const real zLength = 1.0);


//-----------------------------------------------------------------------------


/*
=================================================
	ParticleEmitter_Plane
=================================================
*/
real3 ParticleEmitter_Plane (const real pointIndex, const real pointsCount)
{
	const real side = Sqrt( pointsCount );
	return real3( ToSNorm( real2(Mod( pointIndex, side ), Floor( pointIndex / side )) / side ), 0.0 );
}

/*
=================================================
	ParticleEmitter_Plane
=================================================
*/
real3 ParticleEmitter_Plane (const real pointIndex, const real pointsCount, const real ratio)
{
	const real	side_x	 = Sqrt( pointsCount * ratio );
	const real	side_y	 = pointsCount / side_x;
	const real	max_side = Max( side_x, side_y );

	return real3( (real2(Mod( pointIndex, side_x ), Floor( pointIndex / side_x )) * 2.0 - real2(side_x, side_y)) / max_side, 0.0 );
}

/*
=================================================
	ParticleEmitter_Circle
=================================================
*/
real3 ParticleEmitter_Circle (const real pointIndex, const real pointsCount)
{
	return real3( SinCos( Pi * 2.0 * pointIndex / (pointsCount - 1.0) ), 0.0 );
}

/*
=================================================
	ParticleColor_FromNormalizedVelocity
=================================================
*/
real3 ParticleEmitter_FillCircle (const real pointIndex, const real pointsCount)
{
	const real2	p = ParticleEmitter_Plane( pointIndex, pointsCount ).xy;
	return real3( SinCos( Pi * 2.0 * p.x ) * p.y, 0.0 );
}

/*
=================================================
	ParticleEmitter_Sphere
=================================================
*/
real3 ParticleEmitter_Sphere (const real pointIndex, const real pointsCount)
{
	const real2	angle	= ParticleEmitter_Plane( pointIndex, pointsCount, 0.5 ).yx * Pi;
	const real2	theta	= SinCos( angle.x );
	const real2	phi		= SinCos( angle.y );

	return real3( theta.x * phi.y, theta.x * phi.x, theta.y );
}

/*
=================================================
	ParticleEmitter_ConeVector
=================================================
*/
real3 ParticleEmitter_ConeVector (const real pointIndex, const real pointsCount, const real zLength)
{
	const real2	p = ParticleEmitter_Plane( pointIndex, pointsCount ).xy;
	const real2	c = SinCos( Pi * 2.0 * p.x ) * p.y;
	return Normalize( real3( c, zLength ) );
}



/*
real2 Emitter_SpherePlane (real pointIndex, real pointsCount)
{
	/* /
	real2	c = Emitter_Plane( pointIndex, pointsCount );
	/* /
	real	ratio	 = 0.5;
	real	side_x	 = Sqrt( pointsCount * ratio );
	real	side_y	 = pointsCount / side_x;
	real	max_side = Max( side_x, side_y );

	real2	c = ToSNorm( real2(Mod( pointIndex, side_x ), Floor( pointIndex / side_x )) / real2(side_x, side_y) );
	//* /

	/* /
	c.x = Wrap( Sin( c.x * Pi * 0.5 ) + 1.0, -1.0, 1.00001 );
	/* /
#	define FUNC(x)	(1.0 / (0.5 + Abs(x)))

	real	min = FUNC( 1.0 );
	real	max = FUNC( 0.0 );

	c.y = ( LinearStep( FUNC( c.y ), min, max ) ) * Sign( c.y );
#	undef FUNC
	//** /

	return c * real2(ratio, 1.0);
	//return c;
	//return Clamp( c, -1.0, 1.0 );
}*/