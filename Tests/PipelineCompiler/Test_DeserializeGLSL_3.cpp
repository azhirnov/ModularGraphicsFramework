// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

using namespace PipelineCompiler;


extern bool Test_DeserializeGLSL_3 ()
{
	const String shader_src = R"#(
#ifdef VULKAN
#define PUSH_CONSTANT( _name_ )	layout (std140, push_constant) uniform _name_
#else
#define PUSH_CONSTANT( _name_ )	layout (std140) uniform pushConst_##_name_
#endif

#define float4		vec4
#define double4		dvec4
#define int2		ivec2

layout(location=0) out float4 out_Color;

layout(location=2) uniform float un_Alpha;

layout(binding=0) uniform sampler2D un_Texture0;
layout(binding=1) uniform sampler2D	un_Texture1;
layout(binding=2) uniform sampler3D un_Texture2;

layout(rgba32f) uniform readonly image2D un_Image0;
layout(rgba32f) uniform readonly image2D un_Image1;

struct FType
{
	float4	f;
	double4	d;
};

PUSH_CONSTANT( UB )
{
	float4	value1;
	FType	value2;
} ub;

void main ()
{
	float4 f1 = texture(un_Texture0, gl_FragCoord.xy) + texture(un_Texture1, gl_FragCoord.yx) + texture(un_Texture2, gl_FragCoord.xyz);
	float4 f2 = imageLoad(un_Image0, int2(gl_FragCoord.xy)) + imageLoad(un_Image1, int2(gl_FragCoord.yx));
	float4 f = f1 * f2;

	out_Color = ub.value1 + ub.value2.f + gl_FragCoord.xyxy * f * un_Alpha;
}
	)#";

	DeserializedShader		des;
	String					log;
	Array<StringCRef>		source;		source << shader_src;

	CHECK_ERR( ShaderCompiler::Instance()->
		Deserialize( EShaderSrcFormat::GLSL, EShader::Fragment, source, "main", OUT log, OUT des ) );

	des.CalculateOffsets();
	LOG( des.ToString().cstr(), ELog::Debug );

	des.ReplaceTypesInStructs();
	des.CalculateOffsets();

	//des.CalculateLocations();

	LOG( des.ToString().cstr(), ELog::Debug );
	return true;
}
