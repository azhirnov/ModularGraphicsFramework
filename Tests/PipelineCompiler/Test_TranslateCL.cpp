// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

using namespace PipelineCompiler;


extern bool Test_TranslateCL ()
{
	const String shader_src = R"#(
layout (local_size_x=1, local_size_y=1, local_size_z=1) in;

layout(rgba32f) readonly  uniform image2D  un_InImage;
layout(rgba32f) writeonly uniform image2D  un_OutImage;

void LoadImage (const ivec2 p, out vec4 col)
{
	col = pow( imageLoad( un_InImage, p ), vec4(2.2) );
}

void StoreImage (const ivec2 p, in vec4 col)
{
	imageStore( un_OutImage, p, pow( col, vec4(1.0/2.2) ) );
}

void main ()
{
	ivec2	p	= ivec2(gl_GlobalInvocationID.xyz);
	vec4	col;

	LoadImage( p, col );
	StoreImage( p, col * 10.0 );
}
)#";

	
	BinaryArray				binary;
	ShaderCompiler::Config	cfg;
	String					log;
	Array<StringCRef>		source;		source << shader_src;

	//cfg.target	= EShaderDstFormat::GLSL_Source;
	cfg.target	= EShaderDstFormat::CL_Source;
	
	CHECK_ERR( ShaderCompiler::Instance()->Translate( EShader::Compute, source, "main", cfg, OUT log, OUT binary ) );
	return true;
}