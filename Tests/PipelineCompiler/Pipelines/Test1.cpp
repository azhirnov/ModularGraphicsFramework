
#if 0 //def __cplusplus
#include "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h"

using namespace PipelineCompiler;

static const struct _ final : GraphicsPipeline
{
	_ () : GraphicsPipeline( __FILE__ )
	{
		//attribs	.Add<float3>( "position" )
		//		.Add<float2>( "texcoord" );

		shaders.vertex.Load( __FILE__ );
		shaders.fragment.Load( __FILE__ );

		auto&	cbuf = renderState.color.buffers[0];

		cbuf.blend			= true;
		cbuf.colorMask		= bool4( true, false, true, true );
		cbuf.blendFuncSrc	= EBlendFunc::OneMinusSrcAlpha;
		cbuf.blendFuncDst	= EBlendFunc::One;
		cbuf.blendMode		= EBlendEq::Add;

		// depth write only
		renderState.depth.enabled	= true;
		renderState.depth.func		= ECompareFunc::Always;
		renderState.depth.write		= true;

		renderState.inputAssembly.topology	= EPrimitive::TriangleList;

		dynamicStates = EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;

		//bindings.UniformBuffer( "", "" );
	}

} test1Pipeline;

#endif

#ifdef SHADER
struct TVertexData
{
	vec4		shadowPos;
	vec3		viewPos;
	vec3		normal;
	vec2		texcoord;
};

struct Mat4
{
	float4x4	m;
};

struct Color
{
	float4		v;
};

struct Camera
{
	Mat4		mvp;
	Mat4		mv;
};

struct Light
{
	Mat4		shadowMat;
	Color		color;
};

struct Material
{
	Color		diffuseColor;
};

layout(std140, binding=0)
uniform UB
{
	Camera		camera;
	Material	mtr;
	Light		light;
} ub;

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
	//} f;				// align = 16
	FType		f;

	float		g;		// offset = 48, align = 4
	float		h[2];	// offset = 64, align = 16
	mat2x3		i;		// offset = 96, align = 16
	//struct {
	//	uvec3	j;		// offset = 128, align = 16
	//	vec2	k;		// offset = 144, align = 8
	//	float	l[2];	// offset = 160, align = 16
	//	vec2	m;		// offset = 192, align = 8
	//	mat3	n[2];	// offset = 208, align = 16
	//} o[2];			// align 16
	OType		o[2];

} ubExample;

layout(std430) buffer SSBExample
{
	float		a;		// offset = 0, align = 4
	vec2		b;		// offset = 8, align = 8
	vec3		c;		// offset = 16, align = 16
	//struct {			// align = 8
	//	int		d;		// offset = 32, align = 4
	//	bvec2	e;		// offset = 40, align = 8
	//} f;				// align = 8
	FType		f;

	float		g;		// offset = 48, align = 4
	float		h[2];	// offset = 52, align = 4
	mat2x3		i;		// offset = 64, align = 16
	//struct {			// align 16
	//	uvec3	j;		// offset = 96, align = 16
	//	vec2	k;		// offset = 112, align = 8
	//	float	l[2];	// offset = 120, align = 4
	//	vec2	m;		// offset = 128, align = 8
	//	mat3	n[2];	// offset = 144, align = 16
	//} o[2];			// align 16
	OType		o[2];

} ssbExample;

layout(std430) buffer SSBDynamic
{
	uint	count;
	float4	colors[];

} ssbDynamic;


#if SHADER & SH_VERTEX

in vec3		at_Position;
in vec3		at_Normal;
in vec2		at_Texcoord;

out TVertexData  Output;

void main ()
{
	gl_Position			= ub.camera.mvp.m * vec4(at_Position, 1.0);
	Output.viewPos		= (ub.camera.mv.m * vec4(at_Position, 1.0)).xyz;
	Output.normal		= (ub.camera.mv.m * vec4(at_Normal, 0.0)).xyz;
	Output.shadowPos	= ub.light.shadowMat.m * vec4(at_Position, 1.0);
	Output.texcoord		= at_Texcoord * ubExample.a * ssbExample.a;
}
#endif


#if SHADER & SH_FRAGMENT

uniform sampler2D			un_ColorTexture;
uniform sampler2DShadow		un_ShadowMap;

in TVertexData  Input;

out vec4	out_Color;

float4 ReadColor (float4 colors[], uint index)
{
	return float4( colors[index].xyz, 1.0 );
}

void main ()
{
	float	shading = textureProj( un_ShadowMap, Input.shadowPos ).r *
						dot( normalize(Input.normal), -normalize(Input.viewPos) );

	uint idx = uint(Input.texcoord.x * float(ssbDynamic.count) + 0.5);
	float4 color0 = ssbDynamic.colors[idx];		//ReadColor( ssbDynamic.colors, idx );

	out_Color = texture( un_ColorTexture, Input.texcoord ) * ub.mtr.diffuseColor.v * ub.light.color.v * shading * ubExample.a * ssbExample.a +
				color0;
}
#endif
#endif	// SHADER
