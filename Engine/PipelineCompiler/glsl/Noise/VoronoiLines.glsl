/*
	Voronoi lines
*/

// range [0;1]
real VoronoiLines (const real2 point, real jitter, real thickness);


//-----------------------------------------------------------------------------


// from https://www.shadertoy.com/view/4lV3zc

// hash without sine
// https://www.shadertoy.com/view/4djSRW
real2 hash22 (const real2 p)
{
	real3 p3 = fract(real3(p.xyx) * real3(.1031, .11369, .13787));
    p3 += dot(p3, p3.yzx+19.19);
    return fract(real2((p3.x+p3.y)*p3.z, (p3.x+p3.z)*p3.y));
}

// get hash offset (jitter and p are assigned in main)
#define getH(d) jitter * hash22(floor(p-d)) - d;

// basic line function (ends are clipped not rounded)
real line(in real2 p, const real2 p0, const real2 p1) {
    const real2 n = normalize(p1-p0);
    p -= p0;
    const real l = .5 * length(p1-p0);
    return abs(dot(p, real2(n.y, -n.x))) + step(l, abs(dot(p, n)-l));
}

real VoronoiLines (const real2 p, real jitter, real thickness)
{
    // adjust cell jitter
    // usually this would be a fixed number or 1.0 (not considered)
	jitter = clamp( jitter, 0.0, 1.0 );

    // random offsets of neighbors (clockwise starting north)
    const real3 v = real3(-1.0, 0.0, 1.0);
    real2 o[10];
    o[0] = getH(v.yz);
    o[1] = getH(v.zz);
    o[2] = getH(v.zy);
    o[3] = getH(v.zx);
    o[4] = getH(v.yx);
    o[5] = getH(v.xx);
    o[6] = getH(v.xy);
    o[7] = getH(v.xz);
    o[8] = o[0];	 // trying to prevent excess code
	o[9] = o[1];	// <--'
    
    // center offset
    const real2 c = getH( 0.0 );
    
    // center cell dot (we won't know which lines connect, so all line caps contribute)
    real f = length( fract(p) - c );
    
    // check neighbors and combine into f:
    for (int n = 0; n < 8; n++)
	{
        // dots from neighbors
        f = min( f, length( fract(p) - o[n] ) );
        
        // lines between center and neighbors
        if ( distance( c, o[n] ) < 1.0 )
			f = min( f, line( fract(p), c, o[n] ) );
        
        // lines between adjacent neighbors
        if ( distance( o[n], o[n+1] ) < 1.0 )
			f = min( f, line( fract(p), o[n], o[n+1] ) );
        
        // lines between every other neighbor
        if ( distance( o[n], o[n+2] ) < 1.0 )
			f = min( f, line( fract(p), o[n], o[n+2] ) );
    }
    
    // adjust line thickness
	thickness = clamp( thickness, 0.0, 1.0 );
    f = min( f / (1.0 - thickness), 1.0 );

	return f;
}
