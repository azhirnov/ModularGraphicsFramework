
#ifdef __cplusplus
#include "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h"

using namespace PipelineCompiler;

DECL_PIPELINE( Multitextured, GraphicsPipeline,
{
	shaders.vertex.Load( __FILE__ );
	shaders.fragment.Load( __FILE__ );

	renderState.inputAssembly.topology	= EPrimitive::TriangleList;

	dynamicStates = EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;
})
#endif


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


#if SHADER & SH_FRAGMENT

layout (std140) uniform UB_0
{
	float4	color0;
} ub0;

layout (std140) uniform UB_1
{
    float4  color0;
    float4  color1;
} ub1;

uniform sampler2D un_ColorTexture;
uniform sampler2D un_SecondTexture;

in  float2	v_Texcoord;

out float4	out_Color;

void main() {
    out_Color = texture(un_ColorTexture, v_Texcoord) * ub0.color0 +
                texture(un_SecondTexture, v_Texcoord) * ub1.color1;
}
#endif
#endif	// SHADER
