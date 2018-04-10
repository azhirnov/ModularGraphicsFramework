// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise__NoiseUtils (OUT String &src)
	{
		src << R"#(/*
	Noise Utilites

	depends of: Common/Defines, Math/Math, Hash/Hash2
*/

real MakeInt32Range (const real n);
real2 MakeInt32Range (const real2 n);
real3 MakeInt32Range (const real3 n);

real GradientCoherentNoise2D (const real2 p, const int seed);
real GradientCoherentNoise3D (const real3 p, const int seed);


//-----------------------------------------------------------------------------


//	This file based on code from "libNoise" project by Jason Bevins http://libnoise.sourceforge.net/
//	Released under the terms of the GNU Lesser General Public License.

real MakeInt32Range (const real n)
{
	const real VALUE = 1073741824.0;
	return n >= VALUE ? 2.0 * Mod( n, VALUE ) - VALUE : ( n <= -VALUE ? 2.0 * Mod( n, VALUE ) + VALUE : n );
}

real2 MakeInt32Range (const real2 n)
{
	return real2( MakeInt32Range(n.x), MakeInt32Range(n.y) );
}

real3 MakeInt32Range (const real3 n)
{
	return real3( MakeInt32Range(n.x), MakeInt32Range(n.y), MakeInt32Range(n.z) );
}

#define SCurve5DEF( _type_ ) \
	const _type_ a3 = a * a * a; \
	const _type_ a4 = a3 * a; \
	const _type_ a5 = a4 * a; \
	return 6.0 * a5 - 15.0 * a4 + 10.0 * a3;

real  SCurve5 (const real a)  { SCurve5DEF( real ) }
real2 SCurve5 (const real2 a) { SCurve5DEF( real2 ) }
real3 SCurve5 (const real3 a) { SCurve5DEF( real3 ) }

#undef SCurve5DEF

real GradientNoise3DV3 (const real3 f, const int3 i, const int seed)
{
	/*const int X_NOISE_GEN		= 1619;
	const int Y_NOISE_GEN		= 31337;
	const int Z_NOISE_GEN		= 6971;
	const int SEED_NOISE_GEN	= 1013;
	const int SHIFT_NOISE_GEN	= 8;

	int vectorIndex = ( X_NOISE_GEN    * i.x +
						Y_NOISE_GEN    * i.y +
						Z_NOISE_GEN    * i.z +
						SEED_NOISE_GEN * seed) & 0xffffffff;
	vectorIndex ^= (vectorIndex >> SHIFT_NOISE_GEN);
	vectorIndex &= 0xff;
	real3 gradient	= unRandomVectors[ vectorIndex ].xyz;*/

	const real3 point		= f - real3(i);
	const real3 gradient	= ToSNorm(real3(
									Hash_Uniform( (i.xy + 10.0) * 0.587362, (i.z + 10.0) * 0.239186 ),
									Hash_Uniform( (i.yx + 20.0) * 0.881538, (i.z + 20.0) * 0.170744 ),
									Hash_Uniform( (i.xz + 30.0) * 0.367614, (i.y + 30.0) * 0.922091 )
								  ));
	return Dot( gradient, point ) * 2.12;
}

real GradientNoise3D (const real3 f, const int ix, const int iy, const int iz, const int seed)
{
	return GradientNoise3DV3( f, int3(ix, iy, iz), seed );
}

real GradientCoherentNoise2D (const real2 p, const int seed)
{
	const int2	p0	= int2(	p.x > 0.0 ? int(p.x) : int(p.x) - 1,
							p.y > 0.0 ? int(p.y) : int(p.y) - 1 );
	const int2	p1	= p0 + 1;
	const real2	s	= SCurve5( p - real2(p0) );
	real2		n, i0;
	
	n.x  = GradientNoise3D( real3(p, 0.0), p0.x, p0.y, 0, seed );
	n.y  = GradientNoise3D( real3(p, 0.0), p1.x, p0.y, 0, seed );
	i0.x = mix( n.x, n.y, s.x );

	n.x  = GradientNoise3D( real3(p, 0.0), p0.x, p1.y, 0, seed );
	n.y  = GradientNoise3D( real3(p, 0.0), p1.x, p1.y, 0, seed );
	i0.y = mix( n.x, n.y, s.x );

	return mix( i0.x, i0.y, s.y );
}

real GradientCoherentNoise3D (const real3 p, const int seed)
{
	const int3	p0	= int3( p.x > 0.0 ? int(p.x) : int(p.x) - 1,
							p.y > 0.0 ? int(p.y) : int(p.y) - 1,
							p.z > 0.0 ? int(p.z) : int(p.z) - 1 );
	const int3	p1	= p0 + 1;
	const real3	s	= SCurve5( p - real3(p0) );

	real2	n,
			i0,
			i1;
			
	n.x  = GradientNoise3D( p, p0.x, p0.y, p0.z, seed );
	n.y  = GradientNoise3D( p, p1.x, p0.y, p0.z, seed );
	i0.x = mix( n.x, n.y, s.x );

	n.x  = GradientNoise3D( p, p0.x, p1.y, p0.z, seed );
	n.y  = GradientNoise3D( p, p1.x, p1.y, p0.z, seed );
	i1.x = mix( n.x, n.y, s.x );

	i0.y = mix( i0.x, i1.x, s.y );


	n.x  = GradientNoise3D( p, p0.x, p0.y, p1.z, seed );
	n.y  = GradientNoise3D( p, p1.x, p0.y, p1.z, seed );
	i0.x = mix( n.x, n.y, s.x );

	n.x  = GradientNoise3D( p, p0.x, p1.y, p1.z, seed );
	n.y  = GradientNoise3D( p, p1.x, p1.y, p1.z, seed );
	i1.x = mix( n.x, n.y, s.x );

	i1.y = mix( i0.x, i1.x, s.y );

	return mix( i0.y, i1.y, s.z );
}
)#";

	}
}	// glsl_vfs
