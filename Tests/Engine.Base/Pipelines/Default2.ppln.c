
#ifdef ANGELSCRIPT
void main ()
{
	GraphicsPipeline	ppln;

	ppln.shaders.vertex.LoadSelf();
	ppln.shaders.fragment.LoadSelf();

	ppln.renderState.inputAssembly.topology = EPrimitive_TriangleList;

	ppln.dynamicStates |= EPipelineDynamicState_Viewport;
	ppln.dynamicStates |= EPipelineDynamicState_Scissor;
}
#endif
//-----------------------------------------------------------------------------


#ifdef SHADER
#if SHADER & SH_VERTEX

in  float2	at_Position;
in  float2	at_Texcoord;

out float2	v_Texcoord;

void main() {
    gl_Position	= float4( at_Position, 0.0, 1.0 );
	v_Texcoord	= at_Texcoord;
}
#endif
//-----------------------------------------------------------------------------


#if SHADER & SH_FRAGMENT

layout (std140) uniform UB
{
	float4	color;

} ub;

uniform sampler2D un_ColorTexture;

in  float2	v_Texcoord;

out float4	out_Color;

void main() {
    out_Color = texture(un_ColorTexture, v_Texcoord) * ub.color;
}
#endif
#endif	// SHADER
