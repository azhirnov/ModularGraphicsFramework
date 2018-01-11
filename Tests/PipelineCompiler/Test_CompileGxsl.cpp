// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

using namespace PipelineCompiler;


extern bool Test_CompileGxsl ()
{
	const char shader_src[] = R"#(
#define GXSL

layout (local_size_x=1, local_size_y=1, local_size_z=1) in;

coherent layout(std430) buffer SSB
{
	float	inValue;
	int		outResult;
	int		bufSize;
	vec4	staticBuf[16];
	vec4	buf[];
} ssb;

#ifdef GXSL
int SomeFunc (vec4 arr[])
{
	return int(arr[arr.length()/2].x + arr[arr.length()/3].y);
}

int SomeFunc (const vec4 arr[16])
{
	return int(arr[arr.length()/2].x);
}
#endif

struct LocalStruct
{
	vec4	v;
};

shared vec4 sharedMemory[64];

void main()
{
	int	result = 0;
	float f = ssb.inValue;	// check runtime value and compile-time, all code must work in both configurations

	result += ivec3(float(1.0 + f).xxx).y;
	result += all( greaterThan( vec4(1.0+f), vec4(0.9+f) ) ) ? 1 : 0;
	result += not(bvec3(f > 0.5)).y ? 0 : 1;
	result += (~uvec4(0x0F0 | (f>0.5?1:0))).z == 0xFFFFFF0F ? 1 : 0;
	result += ssb.buf.length();
	result += ssb.staticBuf.length();					
	result += sharedMemory.length();

#ifdef GXSL
	result += all( vec4(1.0+f) > vec4(0.9+f) ) ? 1 : 0;
	result += (vec4(2.0+f) < (3.0+f)).y ? 1 : 0;
	result += (!bvec3(f > 0.5)).y ? 0 : 1;

	result += SomeFunc( ssb.buf );
	result += SomeFunc( ssb.staticBuf );
	result += SomeFunc( sharedMemory ); 
#endif

	int hash = 0;
	for (int i = 0; i < ssb.buf.length(); ++i) {
		hash ^= i;
	}
	result += hash > 0xFFFF ? 1 : 0;

	LocalStruct st;		st.v = ssb.buf[0];
	result += st.v.x > 0.5 ? 0 : 1;

	ssb.outResult = result;
}
	)#";

	String					log;
	Array<StringCRef>		source;		source << shader_src;
	BinaryArray				binary;
	BinaryArray				binary2;
	ShaderCompiler::Config	cfg;

	cfg.source			= EShaderSrcFormat::GXSL;
	cfg.target			= EShaderDstFormat::GLSL_Source;
	cfg.optimize		= false;
	cfg.skipExternals	= false;
	
	if ( not ShaderCompiler::Instance()->Translate( EShader::Compute, source, "main", cfg, OUT log, OUT binary ) )
		RETURN_ERR( log );
	
	LOG( (const char*)binary.ptr(), ELog::Info );
	

	source.Clear();
	source
		<<	"#define float4 vec4\n"
		<<	"#define uint4 uvec4\n"
		<<	"#define float3 vec3\n"
		<<	"#define int3 ivec3\n"
		<<	"#define bool3 bvec3\n"
		<< (const char *)binary.ptr();

	cfg.source			= EShaderSrcFormat::GLSL;
	cfg.optimize		= true;
	cfg.skipExternals	= false;
	
	if ( not ShaderCompiler::Instance()->Translate( EShader::Compute, source, "main", cfg, OUT log, OUT binary2 ) )
		RETURN_ERR( log );

	LOG( (const char*)binary2.ptr(), ELog::Info );
	return true;
}
