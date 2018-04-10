// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise_VoronoiNoise (OUT String &src)
	{
		src << R"#(/*
	Voronoi noise
*/

// range [0;1]
real VoronoiNoise (const real2 point, const real cellCenterOffsetScale, const real smoothFactor);

//real VoronoiNoise2 (in real2 point);


//-----------------------------------------------------------------------------


// TODO: https://www.shadertoy.com/view/4tlSzl
// TODO: https://www.shadertoy.com/view/4lV3zc

//-----------------------------------------------------------------------------



// Created by inigo quilez - iq/2014
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.


// This is a procedural pattern that has 2 parameters, that generalizes cell-noise, 
// perlin-noise and voronoi, all of which can be written in terms of the former as:
//
// cellnoise(x) = pattern(0,0,x)
// perlin(x) = pattern(0,1,x)
// voronoi(x) = pattern(1,0,x)
//
// From this generalization of the three famouse patterns, a new one (which I call 
// "Voronoise") emerges naturally. It's like perlin noise a bit, but within a jittered 
// grid like voronoi):
//
// voronoise(x) = pattern(1,1,x)
//
// Not sure what one would use this generalization for, because it's slightly slower 
// than perlin or voronoise (and certainly much slower than cell noise), and in the 
// end as a shading TD you just want one or another depending of the type of visual 
// features you are looking for, I can't see a blending being needed in real life.  
// But well, if only for the math fun it was worth trying. And they say a bit of 
// mathturbation can be healthy anyway!

// More info here: http://iquilezles.org/www/articles/voronoise/voronoise.htm

real3 hash3 (const real2 p)
{
	const real3 q = real3(	dot( p, real2( 127.1, 311.7 )), 
							dot( p, real2( 269.5, 183.3 )), 
							dot( p, real2( 419.2, 371.9 )) );
	return fract( sin(q) * 43758.5453 );
}

real iqnoise (const real2 x, const real u, const real v)
{
	real2 p = floor(x);
	real2 f = fract(x);
		
	real k = 1.0 + 63.0 * pow( 1.0 - v, 4.0 );
	real va = 0.0;
	real wt = 0.0;

	for (int j = -2; j <= 2; j++)
	for (int i = -2; i <= 2; i++)
	{
		real2 g = real2( real(i), real(j) );
		real3 o = hash3( p + g ) * real3( u, u, 1.0 );
		real2 r = g - f + o.xy;
		real d = dot(r,r);
		real ww = pow( 1.0 - smoothstep( 0.0, 1.414, sqrt(d) ), k );
		va += o.z * ww;
		wt += ww;
	}
	
	return va / wt;
}

real VoronoiNoise (const real2 point, const real cellCenterOffsetScale, const real smoothFactor)
{
	return iqnoise( point, cellCenterOffsetScale, smoothFactor );
}

//-----------------------------------------------------------------------------



//  https://www.shadertoy.com/view/lsjGWD
//  by Pietro De Nicola
//
/*
vec2 hash (vec2 p)
{
	p = vec2( dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3)));
	return fract(sin(p)*43758.5453);
}

float voronoi (in vec2 x)
{
	float t = time/SWITCH_TIME;
	float function = mod(t,4.0);
	bool  multiply_by_F1 = mod(t,8.0)  >= 4.0;
	bool  inverse = mod(t,16.0) >= 8.0;
	float distance_type = mod(t/16.0,4.0);

	vec2 n = floor( x );
	vec2 f = fract( x );

	float F1 = 8.0;
	float F2 = 8.0;

	for (int j = -1; j <= 1; j++)
	{
		for( int i=-1; i<=1; i++ )
		{
			vec2 g = vec2(i,j);
			vec2 o = hash( n + g );

			o = 0.5 + 0.41*sin( time + 6.2831*o );  
			vec2 r = g - f + o;

			float d =   distance_type < 1.0 ? dot(r,r)  :               // euclidean^2
						distance_type < 2.0 ? sqrt(dot(r,r)) :          // euclidean
						distance_type < 3.0 ? abs(r.x) + abs(r.y) :     // manhattan
						distance_type < 4.0 ? max(abs(r.x), abs(r.y)) : // chebyshev
						0.0;

			if( d<F1 ) { 
				F2 = F1; 
				F1 = d; 
			} else if( d<F2 ) {
				F2 = d;
			}
		}
	}

	float c =	function < 1.0 ? F1 : 
				function < 2.0 ? F2 : 
				function < 3.0 ? F2-F1 :
				function < 4.0 ? (F1+F2)/2.0 : 
				0.0;

	if( multiply_by_F1 )    c *= F1;
	if( inverse )           c = 1.0 - c;

	return c;
}

float fbm (in vec2 point, float lacunarity, float amplitude, int octaves)
{
	float signal = 0.0;
	float pers = 1.0;
	float m = 0.0;

	for (int i = 0; i < octaves; i++)
	{
		signal += pers * voronoi( point );
		m += pers;
		pers *= persistence;
		point *= lacunarity;
	}
	return signal / m;
}
*/
/*
real VoronoiNoise2 (in real2 point, int octaves)
{
}
*/
)#";

	}
}	// glsl_vfs
