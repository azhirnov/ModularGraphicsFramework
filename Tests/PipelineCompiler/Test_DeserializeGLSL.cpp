// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

using namespace PipelineCompiler;


extern bool Test_DeserializeGLSL ()
{
	const String shader_src = R"#(
//#extension GL_GOOGLE_include_directive : require
//#include "aaa.glsl"

//layout (local_size_x=1, local_size_y=1, local_size_z=1) in;

in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_binormal;
in vec2 v_texcoord;

layout(location=0) out vec4 f_color0;
layout(location=1) out vec4 f_color1;

layout(rgba32f) writeonly uniform image2D  unImage;

layout(binding=1) uniform sampler2D unTexture;

layout(std140) uniform UB
{
	vec2	value1;		// offset = 0
	float	value2;		// offset = 8
	vec3	value3;		// offset = 16
	vec2	value4;		// offset = 32
} ub;					// size = 48


struct FType {
	int		d;
	bvec2	e;
};

struct OType {
	uvec3	j;
	vec2	k;
	float	l[2];
	vec2	m;
	mat3	n[2];
};

layout(std140) uniform UBExample
{
	float		a;		// offset = 0, align = 4
	vec2		b;		// offset = 8, align = 8
	vec3		c;		// offset = 16, align = 16
	//struct {
	//	int		d;		// offset = 32, align = 4
	//	bvec2	e;		// offset = 40, align = 8
	//} f;				//				align = 16
	FType		f;

	float		g;		// offset = 48, align = 4
	float		h[2];	// offset = 64, align = 16
	mat2x3		i;		// offset = 96, align = 16
	//struct {
	//	uvec3	j;		// offset = 128, align = 16		offset = 304
	//	vec2	k;		// offset = 144, align = 8		offset = 320
	//	float	l[2];	// offset = 160, align = 16		offset = 320
	//	vec2	m;		// offset = 192, align = 8		offset = 368
	//	mat3	n[2];	// offset = 208, align = 16		offset = 384
	//} o[2];			//				 align = 16
	OType		o[2];

} ubExample;			// size = 480

layout(std430) buffer SSBExample
{
	float		a;		// offset = 0, align = 4
	vec2		b;		// offset = 8, align = 8
	vec3		c;		// offset = 16, align = 16
	//struct {			//				align = 8
	//	int		d;		// offset = 32, align = 4
	//	bvec2	e;		// offset = 40, align = 8
	//} f;				//				align = 8
	FType		f;

	float		g;		// offset = 48, align = 4
	float		h[2];	// offset = 52, align = 4
	mat2x3		i;		// offset = 64, align = 16
	//struct {			//				align = 16
	//	uvec3	j;		// offset = 96, align = 16
	//	vec2	k;		// offset = 112, align = 8
	//	float	l[2];	// offset = 120, align = 4
	//	vec2	m;		// offset = 128, align = 8
	//	mat3	n[2];	// offset = 144, align = 16
	//} o[2];			//				 align = 16		stride = 144
	OType		o[2];

} ssbExample;			// size = 384

struct Vertex
{
	vec3	pos;
	ivec2	index1;
	vec2	texc;
	int		index2;
	vec4	color;
	vec2	arr[4];
	vec4	buf[4];
};

layout(std140) restrict buffer SSBO
{
	Vertex		vertices[];		// stride = 48

} ssbo;

vec2 NormCoord (in ivec2 p, const ivec2 s)
{
	return vec2(p) / vec2(s-1);
}

void ToGlobalIndex (ivec2 p, ivec2 s, out int i)
{
	i = p.x + p.y * s.x;
}

void main ()
{
	int		j = int(gl_FragCoord.x) & 2;
	ivec2	p = ivec2(gl_FragCoord.xy); //ivec2(gl_GlobalInvocationID.xy);
	ivec2	s = ivec2(1024, 1024);		//ivec2(gl_NumWorkGroups.xy) * ivec2(gl_WorkGroupSize.xy);
	vec2	n = NormCoord( p + int(ssbo.vertices[0].arr[j].y), s );
	int		i = 0;	ToGlobalIndex( p, s, i );
	vec4	c = texture( unTexture, n );

	ssbo.vertices[i].texc += n;

	//ToGlobalIndex( p, s + ivec2(3,4), ssbo.vertices[i].index2 );

	imageStore( unImage, p, vec4(n.xyx + c.xxz + ub.value4.xxx * ssbo.vertices[i].pos, ubExample.a + ssbExample.a) );

	f_color0 = vec4( n.xxyy );
	f_color1 = vec4( c );
}
	)#";

	struct Util {
		static bool Replace (StringCRef typeName, INOUT ShaderCompiler::FieldTypeInfo &field)
		{
			if ( typeName == "Vertex" and field.name == "texc" )
			{
				field.type	= EShaderVariable::Float4;
				field.name	= "color";
				field.index	= 4;
				return true;
			}
			if ( typeName == "Vertex" and field.name == "index2" )
			{
				field.type	= EShaderVariable::Int2;
				field.name	= "index1";
				field.index	= 1;
				return true;
			}
			if ( typeName == "Vertex" and field.name == "arr" )
			{
				field.type	= EShaderVariable::Float4;
				field.name	= "buf";
				field.index	= 6;
				return true;
			}

			return false;
		}
	};

	DeserializedShader		des;
	String					log;
	Array<StringCRef>		source;		source << shader_src;

	CHECK_ERR( ShaderCompiler::Instance()->
		Deserialize( EShaderSrcFormat::GLSL, EShader::Fragment, source, "main", OUT log, OUT des ) );

	des.CalculateOffsets();
	des.CalculateLocations();

	LOG( des.ToString().cstr(), ELog::Debug );


	BinaryArray				binary;
	ShaderCompiler::Config	cfg;

	cfg.typeReplacer	= DelegateBuilder( &Util::Replace );
	cfg.target			= EShaderDstFormat::GLSL_Source;
	cfg.optimize		= false;
	
	CHECK_ERR( ShaderCompiler::Instance()->Translate( EShader::Fragment, source, "main", cfg, OUT log, OUT binary ) );

	const String	tmp_src	= (const char *) binary.ptr();

	source.Clear();
	source << tmp_src;

	cfg.typeReplacer	= Uninitialized;
	cfg.target			= EShaderDstFormat::GLSL_Binary;
	CHECK_ERR( ShaderCompiler::Instance()->Translate( EShader::Fragment, source, "main", cfg, OUT log, OUT binary ) );

	return true;
}
