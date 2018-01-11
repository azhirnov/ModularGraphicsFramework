/*
	Color functions.
*/

float3  RGBtoHSV (const float3 rgb);
float3  HSVtoRGB (const float3 hsv);
float4  HSVtoRGBA (const float3 hsv, const float alpha);
float4  HSVtoRGBA (const float h, const float s, const float v, const float alpha);


//-----------------------------------------------------------------------------


/*
=================================================
	RGBtoHSV
=================================================
*/
float3  RGBtoHSV (const float3 rgb)
{
	// from http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv

	float	k = 0.0;
	float	r = rgb.x;
	float	g = rgb.y;
	float	b = rgb.z;
	float	t = 0.0;

	if ( g < b )
	{
		t = g;	g = b;	b = t;
		k = -1.0;
	}

	if ( r < g )
	{
		t = r;	r = g;	g = t;
		k = -2.0 / 6.0 - k;
	}

	float	chroma = r - min( g, b );

	return float3( abs( k + (g - b) / 6.0 * chroma + 1e-20 ),
				   chroma / ( r + 1e-20 ),
				   r );
}

/*
=================================================
	HSVtoRGB
=================================================
*/
float3 HSVtoRGB (const float3 hsv)
{
	// from http://chilliant.blogspot.ru/2014/04/rgbhsv-in-hlsl-5.html
	
	float3 col = float3( abs( hsv.x * 6.0 - 3.0 ) - 1.0,
						 2.0 - abs( hsv.x * 6.0 - 2.0 ),
						 2.0 - abs( hsv.x * 6.0 - 4.0 ) );

	return (( clamp( col, float3(0.0), float3(1.0) ) - 1.0 ) * hsv.y + 1.0 ) * hsv.z;
}

/*
=================================================
	HSVtoRGBA
=================================================
*/
float4  HSVtoRGBA (const float3 hsv, const float alpha)
{
	return float4( HSVtoRGB( hsv ), alpha );
}

float4  HSVtoRGBA (const float h, const float s, const float v, const float alpha)
{
	return float4( HSVtoRGB(float3( h, s, v )), alpha );
}
