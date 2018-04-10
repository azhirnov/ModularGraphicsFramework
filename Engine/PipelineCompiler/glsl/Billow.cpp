// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise_Billow (OUT String &src)
	{
		src << R"#(/*
	Billow

	depends of: Math/Math, Noise/_NoiseUtils
*/

real Billow (in real3 point, const real frequency, const real lacunarity, const real persistence, const int octaveCount, const int seed);


//-----------------------------------------------------------------------------


//	This file based on code from "libNoise" project by Jason Bevins http://libnoise.sourceforge.net/
//	Released under the terms of the GNU Lesser General Public License.

real Billow (in real3 point, const real frequency, const real lacunarity, const real persistence, const int octaveCount, const int seed)
{
	real	value	= 0.0;
	real	signal	= 0.0;
	real	pers	= 1.0;
	
	point *= frequency;

	for (int octave = 0; octave < octaveCount; ++octave)
	{
		const real3	n		= MakeInt32Range( point );
		const int	oseed	= (seed + octave) & 0xffffffff;

		signal = GradientCoherentNoise3D( n, oseed );
		signal = 2.0 * Abs(signal) - 1.0;
		value += signal * pers;
		
		point *= lacunarity;
		pers  *= persistence;
	}

	value += 0.5;
	return value;
}
)#";

	}
}	// glsl_vfs
