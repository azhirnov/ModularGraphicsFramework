
#ifdef __cplusplus
#include "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h"

using namespace PipelineCompiler;

DECL_PIPELINE( DrawTexturedQuad, GraphicsPipeline,
{
	shaderFormat = EShaderSrcFormat::GLSL;

	shaders.vertex.Load( __FILE__ );
	shaders.fragment.Load( __FILE__ );

	renderState.inputAssembly.topology	= EPrimitive::TriangleStrip;

	dynamicStates = EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;
})
#endif


#ifdef SHADER
#if SHADER & SH_VERTEX

const float2	g_Positions[] = {
	{ -1.0,  1.0 },
	{ -1.0, -1.0 },
	{  1.0,  1.0 },
	{  1.0, -1.0 }
};

void main() {
	gl_Position	= float4( g_Positions[gl_VertexIndex], 0.0, 1.0 );
}
#endif


#if SHADER & SH_FRAGMENT

uniform sampler2D un_ColorTexture;

out float4	out_Color;

void main() {
    out_Color = texelFetch( un_ColorTexture, int2(gl_FragCoord.xy), 0 );
}
#endif
#endif	// SHADER
