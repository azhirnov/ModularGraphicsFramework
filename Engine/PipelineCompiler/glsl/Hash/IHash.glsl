/*
	Integer hash functions.

	depends of:
		Math/Math
*/


uint IHash (const uint n);

uint IWeylHash (const uint2 p);
uint IWeylHash2 (const uint2 p);


//---------------------------------------------------------


#ifdef ARB_gpu_shader_int64_enabled
uint IHash (const uint n)
{
	// from http://www.everfall.com/paste/id.php?8nr29r9lsnk5
	// from http://www.gamedev.ru/code/forum/?id=215866

    ulong	state = n;
    state	= state * state;
    state	= state * 6364136223846793005UL + 1442695040888963407UL;

    uint32	xorshifted	= uint( ( (state >> 18u) ^ state) >> 27u );
    uint32	rot			= uint( state >> 59u );

    return (xorshifted >> rot  | (xorshifted << ((-rot) & 31));
}
#endif


uint3 IWeylConst ()
{
	return uint3(
		0x3504f333u,	// W0 = 3*2309*128413 
		0xf1bbcdcbu,	// W1 = 7*349*1660097 
		741103597u		// M = 13*83*686843
	);
}


uint IWeylHash (const uint2 p)
{
	// from https://www.shadertoy.com/view/4dlcR4
	uint	x = p.x;
	uint	y = p.y;

	x *= IWeylConst().x;	// x' = Fx(x)
	y *= IWeylConst().y;	// y' = Fy(y)
	x ^= y;					// combine
	x *= IWeylConst.z;		// MLCG constant
	return x;
}

uint IWeylHash2 (const uint2 p)
{
	// from https://www.shadertoy.com/view/4dlcR4
	uint	x = p.x;
	uint	y = p.y;
	
	x *= IWeylConst().x;	// x' = Fx(x)
	y *= IWeylConst().y;	// y' = Fy(y)
	x += IWeylConst().y;	// some odd constant
	y += IWeylConst().x;	// some odd constant
	x ^= y;					// combine
	x *= IWeylConst().z;	// MLCG constant
	return x;
}
