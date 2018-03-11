
#ifdef __cplusplus
#include "Engine/PipelineCompiler/Pipelines/ComputePipeline.h"

using namespace PipelineCompiler;

DECL_PIPELINE( CopyFloatImage2D, ComputePipeline,
{
	shader.Load( __FILE__ );

	shaderFormat = EShaderSrcFormat::GXSL;
})
#endif


#ifdef SHADER
#if SHADER & SH_COMPUTE

layout(rgba8)	readonly  uniform image2D  un_SrcImage;
layout(rgba32f)	writeonly uniform image2D  un_DstImage;

void main ()
{
	int2	coord = int3(gl_GlobalInvocationID).xy;
	float4	color = imageLoad( un_SrcImage, coord );

	imageStore( un_DstImage, coord, color );
}

#endif
#endif	// SHADER
