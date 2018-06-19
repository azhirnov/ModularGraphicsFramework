// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise_FBM (OUT String &src)
	{
		src << R"#(/*
	FBM
*/

#include <Common/Defines.glsl>

//real FractionalBrownianMotion (in real2 point, int octaves);


//---------------------------------------------------------


// TODO:
// https://www.shadertoy.com/view/ldlXRS
// https://www.shadertoy.com/view/MdlXRS

/*
#define NUM_OCTAVES 5

float fbm(float x) {
    float v = 0.0;
    float a = 0.5;
    float shift = float(100);
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(x);
        x = x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}


float fbm(vec2 x) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(x);
        x = rot * x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}


float fbm(vec3 x) {
    float v = 0.0;
    float a = 0.5;
    vec3 shift = vec3(100);
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(x);
        x = x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}
*/
)#";

	}
}	// glsl_vfs
