// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Noise_Simplex (OUT String &src)
	{
		src << R"#(

// range [-1;1]
real SimplexNoise (const real2 x);
real SimplexNoise (const real3 x);
real SimplexNoise (const real4 x);
real SimplexNoiseFractal (const real3 x);


//-----------------------------------------------------------------------------


real sn_permute (const real x) { return floor(mod(((x*34.0)+1.0)*x, 289.0)); }
real3 sn_permute (const real3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }
real4 sn_permute (const real4 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

real taylorInvSqrt (const real r) { return 1.79284291400159 - 0.85373472095314 * r; }
real4 taylorInvSqrt (const real4 r) { return 1.79284291400159 - 0.85373472095314 * r; }

// Simplex 2D noise
//
real snoise (const real2 v)
{
	const real4 C = real4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);
	real2 i  = floor(v + dot(v, C.yy) );
	real2 x0 = v -   i + dot(i, C.xx);
	real2 i1;
	i1 = (x0.x > x0.y) ? real2(1.0, 0.0) : real2(0.0, 1.0);
	real4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	i = mod(i, 289.0);
	real3 p = sn_permute( sn_permute( i.y + real3(0.0, i1.y, 1.0 ))
	+ i.x + real3(0.0, i1.x, 1.0 ));
	real3 m = max(0.5 - real3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
	m = m*m ;
	m = m*m ;
	real3 x = 2.0 * fract(p * C.www) - 1.0;
	real3 h = abs(x) - 0.5;
	real3 ox = floor(x + 0.5);
	real3 a0 = x - ox;
	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
	real3 g;
	g.x  = a0.x  * x0.x  + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}

//  Simplex 3D Noise 
//  by Ian McEwan, Ashima Arts
//
real snoise (const real3 v)
{
	const real2  C = real2(1.0/6.0, 1.0/3.0) ;
	const real4  D = real4(0.0, 0.5, 1.0, 2.0);

	// First corner
	real3 i  = floor(v + dot(v, C.yyy) );
	real3 x0 = v - i + dot(i, C.xxx);

	// Other corners
	real3 g = step(x0.yzx, x0.xyz);
	real3 l = 1.0 - g;
	real3 i1 = min( g.xyz, l.zxy );
	real3 i2 = max( g.xyz, l.zxy );

	//  x0 = x0 - 0. + 0.0 * C 
	real3 x1 = x0 - i1 + 1.0 * C.xxx;
	real3 x2 = x0 - i2 + 2.0 * C.xxx;
	real3 x3 = x0 - 1. + 3.0 * C.xxx;

	// Permutations
	i = mod(i, 289.0 ); 
	real4 p = sn_permute( sn_permute( sn_permute( 
				i.z + real4(0.0, i1.z, i2.z, 1.0 ))
			+ i.y + real4(0.0, i1.y, i2.y, 1.0 )) 
			+ i.x + real4(0.0, i1.x, i2.x, 1.0 ));

	// Gradients
	// ( N*N points uniformly over a square, mapped onto an octahedron.)
	real n_ = 1.0/7.0; // N=7
	real3  ns = n_ * D.wyz - D.xzx;

	real4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

	real4 x_ = floor(j * ns.z);
	real4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

	real4 x = x_ *ns.x + ns.yyyy;
	real4 y = y_ *ns.x + ns.yyyy;
	real4 h = 1.0 - abs(x) - abs(y);

	real4 b0 = real4( x.xy, y.xy );
	real4 b1 = real4( x.zw, y.zw );

	real4 s0 = floor(b0)*2.0 + 1.0;
	real4 s1 = floor(b1)*2.0 + 1.0;
	real4 sh = -step(h, real4(0.0));

	real4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
	real4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

	real3 p0 = real3(a0.xy,h.x);
	real3 p1 = real3(a0.zw,h.y);
	real3 p2 = real3(a1.xy,h.z);
	real3 p3 = real3(a1.zw,h.w);

	//Normalise gradients
	real4 norm = taylorInvSqrt(real4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;

	// Mix final noise value
	real4 m = max(0.6 - real4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
	m = m * m;
	return 42.0 * dot( m*m, real4( dot(p0,x0), dot(p1,x1), 
								dot(p2,x2), dot(p3,x3) ) );
}

//  Simplex 4D Noise 
//  by Ian McEwan, Ashima Arts
//
real4 grad4 (const real j, const real4 ip)
{
	const real4 ones = real4(1.0, 1.0, 1.0, -1.0);
	real4 p,s;

	p.xyz = floor( fract (real3(j) * ip.xyz) * 7.0) * ip.z - 1.0;
	p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
	s = real4(lessThan(p, real4(0.0)));
	p.xyz = p.xyz + (s.xyz*2.0 - 1.0) * s.www; 

	return p;
}

real snoise (const real4 v)
{
	const real2  C = real2( 0.138196601125010504,  // (5 - sqrt(5))/20  G4
						0.309016994374947451); // (sqrt(5) - 1)/4   F4
	// First corner
	real4 i  = floor(v + dot(v, C.yyyy) );
	real4 x0 = v -   i + dot(i, C.xxxx);

	// Other corners

	// Rank sorting originally contributed by Bill Licea-Kane, AMD (formerly ATI)
	real4 i0;

	real3 isX = step( x0.yzw, x0.xxx );
	real3 isYZ = step( x0.zww, x0.yyz );
	//  i0.x = dot( isX, real3( 1.0 ) );
	i0.x = isX.x + isX.y + isX.z;
	i0.yzw = 1.0 - isX;

	//  i0.y += dot( isYZ.xy, real2( 1.0 ) );
	i0.y += isYZ.x + isYZ.y;
	i0.zw += 1.0 - isYZ.xy;

	i0.z += isYZ.z;
	i0.w += 1.0 - isYZ.z;

	// i0 now contains the unique values 0,1,2,3 in each channel
	real4 i3 = clamp( i0, 0.0, 1.0 );
	real4 i2 = clamp( i0-1.0, 0.0, 1.0 );
	real4 i1 = clamp( i0-2.0, 0.0, 1.0 );

	//  x0 = x0 - 0.0 + 0.0 * C 
	real4 x1 = x0 - i1 + 1.0 * C.xxxx;
	real4 x2 = x0 - i2 + 2.0 * C.xxxx;
	real4 x3 = x0 - i3 + 3.0 * C.xxxx;
	real4 x4 = x0 - 1.0 + 4.0 * C.xxxx;

	// Permutations
	i = mod(i, 289.0); 
	real j0 = sn_permute( sn_permute( sn_permute( sn_permute(i.w) + i.z) + i.y) + i.x);
	real4 j1 = sn_permute( sn_permute( sn_permute( sn_permute (
				i.w + real4(i1.w, i2.w, i3.w, 1.0 ))
			+ i.z + real4(i1.z, i2.z, i3.z, 1.0 ))
			+ i.y + real4(i1.y, i2.y, i3.y, 1.0 ))
			+ i.x + real4(i1.x, i2.x, i3.x, 1.0 ));
	// Gradients
	// ( 7*7*6 points uniformly over a cube, mapped onto a 4-octahedron.)
	// 7*7*6 = 294, which is close to the ring size 17*17 = 289.

	real4 ip = real4(1.0/294.0, 1.0/49.0, 1.0/7.0, 0.0) ;

	real4 p0 = grad4(j0,   ip);
	real4 p1 = grad4(j1.x, ip);
	real4 p2 = grad4(j1.y, ip);
	real4 p3 = grad4(j1.z, ip);
	real4 p4 = grad4(j1.w, ip);

	// Normalise gradients
	real4 norm = taylorInvSqrt(real4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;
	p4 *= taylorInvSqrt(dot(p4,p4));

	// Mix contributions from the five corners
	real3 m0 = max(0.6 - real3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.0);
	real2 m1 = max(0.6 - real2(dot(x3,x3), dot(x4,x4)            ), 0.0);
	m0 = m0 * m0;
	m1 = m1 * m1;
	return 49.0 * ( dot(m0*m0, real3( dot( p0, x0 ), dot( p1, x1 ), dot( p2, x2 )))
				+ dot(m1*m1, real2( dot( p3, x3 ), dot( p4, x4 ) ) ) );
}

//  <www.shadertoy.com/view/XsX3zB>
//  by Nikita Miropolskiy

/* discontinuous pseudorandom uniformly distributed in [-0.5, +0.5]^3 */
real3 random3 (const real3 c)
{
	real j = 4096.0*sin(dot(c,real3(17.0, 59.4, 15.0)));
	real3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}

const real F3 =  0.3333333;
const real G3 =  0.1666667;

real snoise2 (const real3 p)
{
	real3 s = floor(p + dot(p, real3(F3)));
	real3 x = p - s + dot(s, real3(G3));

	real3 e = step(real3(0.0), x - x.yzx);
	real3 i1 = e*(1.0 - e.zxy);
	real3 i2 = 1.0 - e.zxy*(1.0 - e);

	real3 x1 = x - i1 + G3;
	real3 x2 = x - i2 + 2.0*G3;
	real3 x3 = x - 1.0 + 3.0*G3;

	real4 w, d;

	w.x = dot(x, x);
	w.y = dot(x1, x1);
	w.z = dot(x2, x2);
	w.w = dot(x3, x3);

	w = max(0.6 - w, 0.0);

	d.x = dot(random3(s), x);
	d.y = dot(random3(s + i1), x1);
	d.z = dot(random3(s + i2), x2);
	d.w = dot(random3(s + 1.0), x3);

	w *= w;
	w *= w;
	d *= w;

	return dot(d, real4(52.0));
}

real snoiseFractal (const real3 m)
{
	return   0.5333333 * snoise2(m)
				+0.2666667 * snoise2(2.0*m)
				+0.1333333 * snoise2(4.0*m)
				+0.0666667 * snoise2(8.0*m);
}

real SimplexNoise (const real2 x)
{
	return snoise( x );
}

real SimplexNoise (const real3 x)
{
	return snoise( x );
}

real SimplexNoise (const real4 x)
{
	return snoise( x );
}

real SimplexNoiseFractal (const real3 x)
{
	return snoiseFractal( x );
}
)#";

	}
}	// glsl_vfs
