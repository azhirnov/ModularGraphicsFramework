
#if 0 //def __cplusplus
#include "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h"

using namespace PipelineCompiler;

DECL_PIPELINE( Default, GraphicsPipeline,
{
	shaders.vertex.Load( __FILE__ );
	shaders.fragment.Load( __FILE__ );

	renderState.inputAssembly.topology	= EPrimitive::TriangleList;

	dynamicStates = EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;
})
#endif


#ifdef SHADER
#if SHADER & SH_VERTEX

out float3	v_Color;
out float2	v_Texcoord;

const float2	g_Positions[3] = float2[](
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
);

const float3	g_Colors[3] = float3[](
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position	= float4( g_Positions[gl_VertexIndex], 0.0, 1.0 );
    v_Color		= g_Colors[gl_VertexIndex];
	v_Texcoord	= g_Positions[gl_VertexIndex] + 0.5;
}
#endif


#if SHADER & SH_FRAGMENT

uniform sampler2D un_ColorTexture;

in  float2	v_Texcoord;
in  float3	v_Color;

out float4	out_Color;

float4 Temp (const float3 inColor)
{
    const float4 col = texture(un_ColorTexture, v_Texcoord) * float4(inColor, 1.0);
	return col;
}

void main() {
	out_Color = Temp( v_Color );
}
#endif
#endif	// SHADER
