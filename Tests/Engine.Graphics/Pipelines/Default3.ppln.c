
#ifdef ANGELSCRIPT
void main ()
{
	GraphicsPipeline	ppln;

	ppln.shaders.vertex.LoadSelf();
	ppln.shaders.fragment.LoadSelf();
	
	ppln.supportedTopology |= EPrimitive_TriangleList;
	ppln.supportedTopology |= EPrimitive_TriangleStrip;

	ppln.dynamicStates |= EPipelineDynamicState_Viewport;
	ppln.dynamicStates |= EPipelineDynamicState_Scissor;
}
#endif
//-----------------------------------------------------------------------------


#ifdef SHADER
#if SHADER & SH_VERTEX

in  float2	at_Position;
in  float2	at_Texcoord;
in	float4	at_Color;

out float2	v_Texcoord;
out	float4	v_Color;

void main() {
    gl_Position	= float4( at_Position, 0.0, 1.0 );
	v_Texcoord	= at_Texcoord;
	v_Color		= at_Color;
}
#endif
//-----------------------------------------------------------------------------


#if SHADER & SH_FRAGMENT

uniform sampler2D un_ColorTexture;

in  float2	v_Texcoord;
in	float4	v_Color;

out float4	out_Color;

void main() {
    out_Color = texture(un_ColorTexture, v_Texcoord) * v_Color;
}
#endif
#endif	// SHADER
