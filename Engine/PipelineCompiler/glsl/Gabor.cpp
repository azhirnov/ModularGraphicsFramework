// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise_Gabor (OUT String &src)
	{
		src << R"#(/*
	Gabor noise
*/

#include <Common/Defines.glsl>

real GaborNoise (in real2 point, int numBlobs, real blobSize);


//-----------------------------------------------------------------------------

// TODO
// https://www.shadertoy.com/view/4dGGz1
// https://www.shadertoy.com/view/XsBGDc
// https://www.shadertoy.com/view/llsGWl
// https://www.shadertoy.com/view/XlsGDs

//-----------------------------------------------------------------------------



// inspired from https://www.shadertoy.com/view/MdjGWy

//#define NB 100      // number or gabor blobs
//#define SIZE 0.22   // size of gabor blobs
                    // freq tuned by mouse.x

float gauss (float x, float blobSize)
{
    return exp( -(x*x) / (blobSize*blobSize) );
}

float rnd(vec2 uv, int z) 
{
	if      (z==0) return texture2D(iChannel1,uv).r;
	else if (z==1) return texture2D(iChannel1,uv).g;
	else if (z==2) return texture2D(iChannel1,uv).b;
	else           return texture2D(iChannel1,uv).a;
}

float rndi(int i, int j)
{
	vec2 uv = vec2(.5+float(i),.5+float(j))/ iChannelResolution[1].x;
	return texture2D(iChannel1,uv).r;
}

float gabor (in vec2 pos, in vec2 dir, float blobSize)
{
    float g = gauss( pos.x, blobSize ) * gauss( pos.y, blobSize );
    float s = 0.5 * sin( dot( pos, dir ) * 2.0 * 3.14159265358979 );
	return g*s;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.y;
	float freq = mix(10., iResolution.x/10., iMouse.x/iResolution.x);
	float d = 1.5*iMouse.y/iResolution.y - .5;
	vec3 col= vec3(0.);
	
	for (int i=0; i<NB; i++) {
		vec2 pos = vec2(1.5*rndi(i,0),rndi(i,1));
		vec2 dir = (1.+d)*vec2(rndi(i,2),rndi(i,3))-d;
		col += gabor(uv-pos, freq*dir)*texture2D(iChannel0,pos).rgb;
	}
    fragColor = vec4(col,1.0);
}
                  

real GaborNoise (in real2 point, int numBlobs, real blobSize)
{
	return 0.0;
}
)#";

	}
}	// glsl_vfs
