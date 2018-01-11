/*
	Blur postprocess
*/

float4 SimpleBlur (sampler2D tex, float2 uv, int radius, float uvScale);


//-----------------------------------------------------------------------------


/*
=================================================
	SimpleBlur
=================================================
*/
float4 SimpleBlur (sampler2D tex, float2 uv, int radius, float uvScale)
{
	float	kernel	 = 0.0;
	float4	color	 = float4(0.0);
	float2	uv_scale = uvScale / float2(textureSize( tex, 0 ).xy);

	for (int y = -radius; y <= radius; ++y)
	{
		for (int x = -radius; x <= radius; ++x)
		{
			float2	c = float2(x, y);
			float	k = 1.0 / Pow( 2.0, Abs(c.x) + Abs(c.y) );

			color  += texture( tex, uv + c * uv_scale ) * k;
			kernel += k;
		}
	}
	return color / kernel;
}
