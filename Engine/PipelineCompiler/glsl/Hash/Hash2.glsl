/*
	Hash functions.
*/

// range [0;1]
float Hash_Uniform (const float2 point, const float time);
float Hash_Triangular (const float2 point, const float time);
float Hash_Gaussianish (const float2 point, const float time);
float Hash_MoarGaussianish (const float2 point, const float time);


//-----------------------------------------------------------------------------


// from https://www.shadertoy.com/view/4ssXRX

//note: uniformly distributed, normalized rand, [0;1[
float nrand (const float2 n)
{
	return fract( sin( dot( n.xy, float2(12.9898, 78.233) ) ) * 43758.5453 );
}

//note: remaps v to [0;1] in interval [a;b]
float remap (const float a, const float b, const float v)
{
	return clamp( (v-a) / (b-a), 0.0, 1.0 );
}

float n1rand (const float2 n, const float time)
{
	const float t = fract( time );
	const float nrnd0 = nrand( n + 0.07*t );
	return nrnd0;
}

float n2rand (const float2 n, const float time)
{
	const float t = fract( time );
	const float nrnd0 = nrand( n + 0.07*t );
	const float nrnd1 = nrand( n + 0.11*t );
	return (nrnd0+nrnd1) / 2.0;
}

float n3rand (const float2 n, const float time)
{
	const float t = fract( time );
	const float nrnd0 = nrand( n + 0.07*t );
	const float nrnd1 = nrand( n + 0.11*t );
	const float nrnd2 = nrand( n + 0.13*t );
	return (nrnd0+nrnd1+nrnd2) / 3.0;
}

float n4rand (const float2 n, const float time)
{
	const float t = fract( time );
	const float nrnd0 = nrand( n + 0.07*t );
	const float nrnd1 = nrand( n + 0.11*t );	
	const float nrnd2 = nrand( n + 0.13*t );
	const float nrnd3 = nrand( n + 0.17*t );
	return (nrnd0+nrnd1+nrnd2+nrnd3) / 4.0;
}

float n8rand (const float2 n, const float time)
{
	const float t = fract( time );
	const float nrnd0 = nrand( n + 0.07*t );
	const float nrnd1 = nrand( n + 0.11*t );	
	const float nrnd2 = nrand( n + 0.13*t );
	const float nrnd3 = nrand( n + 0.17*t );
    
	const float nrnd4 = nrand( n + 0.19*t );
	const float nrnd5 = nrand( n + 0.23*t );
	const float nrnd6 = nrand( n + 0.29*t );
	const float nrnd7 = nrand( n + 0.31*t );
    
	return (nrnd0 + nrnd1 + nrnd2 + nrnd3 + nrnd4 + nrnd5 + nrnd6 + nrnd7) / 8.0;
}

float n4rand_inv (const float2 n, const float time)
{
	const float t = fract( time );
	const float nrnd0 = nrand( n + 0.07*t );
	const float nrnd1 = nrand( n + 0.11*t );	
	const float nrnd2 = nrand( n + 0.13*t );
	const float nrnd3 = nrand( n + 0.17*t );
	const float nrnd4 = nrand( n + 0.19*t );
	const float v1 = (nrnd0 + nrnd1 + nrnd2+nrnd3) / 4.0;
	const float v2 = 0.5 * remap( 0.0, 0.5, v1 ) + 0.5;
	const float v3 = 0.5 * remap( 0.5, 1.0, v1 );
	return (nrnd4<0.5) ? v2 : v3;
}

float Hash_Uniform (const float2 point, const float time)
{
	return n1rand( point, time );
}

float Hash_Triangular (const float2 point, const float time)
{
	return n2rand( point, time );
}

float Hash_Gaussianish (const float2 point, const float time)
{
	return n4rand( point, time );
}

float Hash_MoarGaussianish (const float2 point, const float time)
{
	return n8rand( point, time );
}
