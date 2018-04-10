// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise_Perlin (OUT String &src)
	{
		src << R"#(/*
	Perlin noise

	depends of: Noise/_NoiseUtils
*/

// range [-1; 1]
real PerlinNoise2D (in real2 point, const real frequency, const real lacunarity,
					const real persistence, const int octaveCount, const int seed);

real PerlinNoise3D (in real3 point, const real frequency, const real lacunarity,
					const real persistence, const int octaveCount, const int seed);


//-----------------------------------------------------------------------------

// TODO:
//	https://www.shadertoy.com/view/Xs23D3
//	https://www.shadertoy.com/view/XslGRr
//-----------------------------------------------------------------------------



//	This file based on code from "libNoise" project by Jason Bevins http://libnoise.sourceforge.net/
//	Released under the terms of the GNU Lesser General Public License.

real PerlinNoise2D (in real2 point, const real frequency, const real lacunarity,
					const real persistence, const int octaveCount, const int seed)
{
	real	value	= 0.0;
	real	pers	= 1.0;
	
	point *= frequency;
	
	for (int octave = 0; octave < octaveCount; octave++)
	{
		const real2	n		= MakeInt32Range( point );
		const int	oseed	= (seed + octave) & 0xffffffff;
		const real	signal	= GradientCoherentNoise2D( n, oseed );

		value += signal * pers;
		point *= lacunarity;
		pers  *= persistence;
	}
	return value;
}

real PerlinNoise3D (in real3 point, const real frequency, const real lacunarity,
					const real persistence, const int octaveCount, const int seed)
{
	real	value	= 0.0;
	real	pers	= 1.0;
	
	point *= frequency;
	
	for (int octave = 0; octave < octaveCount; octave++)
	{
		const real3	n		= MakeInt32Range( point );
		const int	oseed	= (seed + octave) & 0xffffffff;
		const real	signal	= GradientCoherentNoise3D( n, oseed );

		value += signal * pers;
		point *= lacunarity;
		pers  *= persistence;
	}
	return value;
}
)#";

	}
}	// glsl_vfs
