// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise_Turbulence (OUT String &src)
	{
		src << R"#(

real3 Turbulence (const real3 point, const real power, const real frequency, const real lacunarity,
				  const real persistence, const int roughness, const int seed);


//-----------------------------------------------------------------------------


//	This file based on code from "libNoise" project by Jason Bevins http://libnoise.sourceforge.net/
//	Released under the terms of the GNU Lesser General Public License.

real3 Turbulence (const real3 point, const real power, const real frequency, const real lacunarity,
				  const real persistence, const int roughness, const int seed)
{
	const real3 p0 = point + real3( 12414.0, 65124.0, 31337.0 ) / 65536.0;
	const real3 p1 = point + real3( 26519.0, 18128.0, 60493.0 ) / 65536.0;
	const real3 p2 = point + real3( 53820.0, 11213.0, 44845.0 ) / 65536.0;

	const real3 distort = real3(PerlinNoise3D( p0, frequency, lacunarity, persistence, roughness, seed ),
								PerlinNoise3D( p1, frequency, lacunarity, persistence, roughness, seed ),
								PerlinNoise3D( p2, frequency, lacunarity, persistence, roughness, seed ))
								* power + point;
	return distort;
}

)#";

	}
}	// glsl_vfs
