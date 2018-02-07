
#ifdef __cplusplus
#include "Engine/PipelineCompiler/Pipelines/ComputePipeline.h"

using namespace PipelineCompiler;

DECL_PIPELINE( DefaultCompute1, ComputePipeline,
{
	shader.Load( __FILE__ );

	//shaderFormat = EShaderSrcFormat::GXSL;
})
#endif


#ifdef SHADER
#if SHADER & SH_COMPUTE

layout(binding=0, rgba8) writeonly uniform image2D  un_OutImage;

layout(binding=1, std140) readonly buffer SSBO
{
	vec4 data;

} ssb;

void main ()
{
	vec2 uv = vec2(gl_GlobalInvocationID.xy) / vec2((gl_WorkGroupSize * gl_NumWorkGroups).xy);
	//int idx = int(gl_LocalInvocationIndex) + 1;
	vec4 fragColor = vec4(sin(uv.x), cos(uv.y), 1.0, ssb.data.r);

	imageStore( un_OutImage, ivec2(gl_GlobalInvocationID.xy), fragColor );
}

#endif
#endif	// SHADER
