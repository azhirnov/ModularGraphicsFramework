// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

using namespace PipelineCompiler;


extern bool Test_DeserializeGLSL_2 ()
{
	const String shader_src = R"#(
layout(location=0) out vec4 out_Color;

layout(location=2) uniform float un_Alpha;

struct FType
{
	vec4	f;
	dvec4	d;
};

layout(std140) uniform UB
{
	vec4	value1;
	FType	value2;
} ub;

void main ()
{
	out_Color = ub.value1 + ub.value2.f + gl_FragCoord.xyxy * un_Alpha;
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
