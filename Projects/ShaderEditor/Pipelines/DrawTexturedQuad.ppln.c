
#ifdef ANGELSCRIPT
void main ()
{
	GraphicsPipeline ppln = GraphicsPipeline();
	
	ppln.shaders.vertex.LoadSelf();
	ppln.shaders.fragment.LoadSelf();
	
	ppln.shaderFormat = EShaderSrcFormat_GLSL;

	ppln.renderState.inputAssembly.topology	= EPrimitive_TriangleStrip;

	ppln.dynamicStates |= EPipelineDynamicState_Viewport;
	ppln.dynamicStates |= EPipelineDynamicState_Scissor;
}
#endif	// ANGELSCRIPT
//-----------------------------------------------------------------------------


#ifdef SHADER
#if SHADER & SH_VERTEX

const float2	g_Positions[] = {
	{ -1.0,  1.0 },
	{ -1.0, -1.0 },
	{  1.0,  1.0 },
	{  1.0, -1.0 }
};
const float2	g_Texcoords[] = {
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 1.0f }
};

out float2 v_Texcoord;

void main() {
	gl_Position	= float4( g_Positions[gl_VertexIndex], 0.0, 1.0 );
	v_Texcoord = g_Texcoords[gl_VertexIndex];
}
#endif
//-----------------------------------------------------------------------------


#if SHADER & SH_FRAGMENT

uniform sampler2D un_ColorTexture;

out float4	out_Color;
in  float2	v_Texcoord;

void main()
{
    //out_Color = texelFetch( un_ColorTexture, int2(coord), 0 );
	out_Color = texture( un_ColorTexture, v_Texcoord );
}
#endif
#endif	// SHADER
