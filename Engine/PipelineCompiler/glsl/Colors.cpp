// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Particles_Colors (OUT String &src)
	{
		src << R"#(/*
	Particles color function.
	
	license: free

	depends of:
		Math/Math
		Math/Color
*/


uint ParticleColor_FromNormalizedVelocity (const float3 velocity);
uint ParticleColor_FromVelocity (const float3 velocity);
uint ParticleColor_FromVelocityLength (const float3 velocity);


//-----------------------------------------------------------------------------


/*
=================================================
	ParticleColor_FromNormalizedVelocity
=================================================
*/
uint ParticleColor_FromNormalizedVelocity (const float3 velocity)
{
	return packUnorm4x8( float4( ToUNorm( Normalize( velocity ) ), 1.0 ) );
}

/*
=================================================
	ParticleColor_FromVelocity
=================================================
*/
uint ParticleColor_FromVelocity (const float3 velocity)
{
	return packUnorm4x8( float4( ToUNorm( Clamp( velocity, -1.0, 1.0 ) ), 1.0 ) );
}

/*
=================================================
	ParticleColor_FromVelocityLength
=================================================
*/
uint ParticleColor_FromVelocityLength (const float3 velocity)
{
	const float vel = 1.0 - Clamp( Length( velocity ), 0.0, 1.0 );
	return packUnorm4x8( float4( HSVtoRGB( float3( vel, 1.0, 1.0 ) ), 1.0 ) );
}
)#";

	}
}	// glsl_vfs
