// This is generated file
// Origin file: 'Engine.Graphics\Pipelines/Default3.ppln'
#include "all_pipelines.h"

namespace Pipelines
{

void Create_default3 (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	// RenderState
	descr.renderState = RenderState();
	descr.dynamicStates = EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;
	descr.supportedShaders = EShader::Vertex | EShader::Fragment;
	descr.supportedPrimitives = EPrimitive::TriangleList | EPrimitive::TriangleStrip;

	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddTexture( "un_ColorTexture", EImage::Tex2D, EPixelFormatClass::AnyColorChannels | EPixelFormatClass::LinearColorSpace | EPixelFormatClass::AnyFloat | EPixelFormatClass::AnyNorm, 0u, 0u, EShader::Fragment )
			.Finish();

	descr.attribs = VertexAttribs()
			.Add( "at_Position", EVertexAttribute::Float2, 0, "" )
			.Add( "at_Color", EVertexAttribute::Float4, 2, "" )
			.Add( "at_Texcoord", EVertexAttribute::Float2, 1, "" );

	descr.Vertex().StringGLSL( 
R"#(#version 450 core


#define SH_VERTEX           (1<<0)
#define SH_TESS_CONTROL     (1<<1)
#define SH_TESS_EVALUATION  (1<<2)
#define SH_GEOMETRY         (1<<3)
#define SH_FRAGMENT         (1<<4)
#define SH_COMPUTE          (1<<5)

#ifdef GL_ARB_gpu_shader_int64
#extension GL_ARB_gpu_shader_int64 : require
//#define ARB_gpu_shader_int64_enabled  1
#endif

// for vulkan compatibility
#ifdef GL_ARB_separate_shader_objects
#extension GL_ARB_separate_shader_objects : enable
#define ARB_separate_shader_objects_enabled  1
#endif

#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4

#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

#ifdef ARB_gpu_shader_int64_enabled
# define ilong		int64_t
# define ilong2		i64vec2
# define ilong3		i64vec3
# define ilong4		i64vec4
# define ulong		uint64_t
# define ulong2		u64vec2
# define ulong3		u64vec3
# define ulong4		u64vec4
#endif // ARB_gpu_shader_int64_enabled

#define float2		vec2
#define float3		vec3
#define float4		vec4
			
#define float2x2	mat2x2
#define float2x3	mat2x3
#define float2x4	mat2x4
#define float3x2	mat3x2
#define float3x3	mat3x3
#define float3x4	mat3x4
#define float4x2	mat4x2
#define float4x3	mat4x3
#define float4x4	mat4x4

#define double2		dvec2
#define double3		dvec3
#define double4		dvec4
			
#define double2x2	dmat2x2
#define double2x3	dmat2x3
#define double2x4	dmat2x4
#define double3x2	dmat3x2
#define double3x3	dmat3x3
#define double3x4	dmat3x4
#define double4x2	dmat4x2
#define double4x3	dmat4x3
#define double4x4	dmat4x4

#ifdef VULKAN
#define PUSH_CONSTANT( _name_ )	layout (std140, push_constant) uniform _name_
#else
#define PUSH_CONSTANT( _name_ )	layout (std140) uniform pushConst_##_name_
#endif
		
#define SHADER	SH_VERTEX
#ifdef VULKAN
# define gl_VertexID    gl_VertexIndex
# define gl_InstanceID  gl_InstanceIndex
#else
# define gl_VertexIndex    gl_VertexID
# define gl_InstanceIndex  gl_InstanceID
#endif
out gl_PerVertex {
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
	float gl_CullDistance[];
};

layout(location=0) in   vec2 at_Position;
layout(location=1) in   vec2 at_Texcoord;
layout(location=2) in   vec4 at_Color;
layout(location=0) smooth out   vec2 v_Texcoord;
layout(location=1) smooth out   vec4 v_Color;




void main ()
{
	gl_Position = vec4( at_Position, float( 0.0f ), float( 1.0f ) );
	v_Texcoord = at_Texcoord;
	v_Color = at_Color;
}



)#"_str );
	descr.Vertex().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080006, 0x00000027, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x000D000F, 0x00000000, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000E, 0x00000013, 0x0000001D, 
0x0000001E, 0x00000020, 0x00000022, 0x00000025, 0x00000026, 0x00030007, 0x00000001, 0x00000000, 0x002D0003, 0x00000002, 0x000001C2, 0x00000001, 
0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 
0x6F725065, 0x73736563, 0x63206465, 0x6E65696C, 0x706F2074, 0x6C676E65, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 
0x72617420, 0x2D746567, 0x20766E65, 0x6E65706F, 0x2F0A6C67, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x72746E65, 0x6F702D79, 
0x20746E69, 0x6E69616D, 0x696C230A, 0x3120656E, 0x0000000A, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 
0x00090004, 0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 
0x00000000, 0x00060005, 0x0000000C, 0x505F6C67, 0x65567265, 0x78657472, 0x00000000, 0x00060006, 0x0000000C, 0x00000000, 0x505F6C67, 0x7469736F, 
0x006E6F69, 0x00070006, 0x0000000C, 0x00000001, 0x505F6C67, 0x746E696F, 0x657A6953, 0x00000000, 0x00070006, 0x0000000C, 0x00000002, 0x435F6C67, 
0x4470696C, 0x61747369, 0x0065636E, 0x00070006, 0x0000000C, 0x00000003, 0x435F6C67, 0x446C6C75, 0x61747369, 0x0065636E, 0x00030005, 0x0000000E, 
0x00000000, 0x00050005, 0x00000013, 0x505F7461, 0x7469736F, 0x006E6F69, 0x00050005, 0x0000001D, 0x65545F76, 0x6F6F6378, 0x00006472, 0x00050005, 
0x0000001E, 0x545F7461, 0x6F637865, 0x0064726F, 0x00040005, 0x00000020, 0x6F435F76, 0x00726F6C, 0x00050005, 0x00000022, 0x435F7461, 0x726F6C6F, 
0x00000000, 0x00050005, 0x00000025, 0x565F6C67, 0x65747265, 0x00444978, 0x00060005, 0x00000026, 0x495F6C67, 0x6174736E, 0x4965636E, 0x00000044, 
0x00050048, 0x0000000C, 0x00000000, 0x0000000B, 0x00000000, 0x00050048, 0x0000000C, 0x00000001, 0x0000000B, 0x00000001, 0x00050048, 0x0000000C, 
0x00000002, 0x0000000B, 0x00000003, 0x00050048, 0x0000000C, 0x00000003, 0x0000000B, 0x00000004, 0x00030047, 0x0000000C, 0x00000002, 0x00040047, 
0x00000013, 0x0000001E, 0x00000000, 0x00040047, 0x0000001D, 0x0000001E, 0x00000000, 0x00040047, 0x0000001E, 0x0000001E, 0x00000001, 0x00040047, 
0x00000020, 0x0000001E, 0x00000001, 0x00040047, 0x00000022, 0x0000001E, 0x00000002, 0x00040047, 0x00000025, 0x0000000B, 0x00000005, 0x00040047, 
0x00000026, 0x0000000B, 0x00000006, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 
0x00000008, 0x00000007, 0x00000004, 0x00040015, 0x00000009, 0x00000020, 0x00000000, 0x0004002B, 0x00000009, 0x0000000A, 0x00000001, 0x0004001C, 
0x0000000B, 0x00000007, 0x0000000A, 0x0006001E, 0x0000000C, 0x00000008, 0x00000007, 0x0000000B, 0x0000000B, 0x00040020, 0x0000000D, 0x00000003, 
0x0000000C, 0x0004003B, 0x0000000D, 0x0000000E, 0x00000003, 0x00040015, 0x0000000F, 0x00000020, 0x00000001, 0x0004002B, 0x0000000F, 0x00000010, 
0x00000000, 0x00040017, 0x00000011, 0x00000007, 0x00000002, 0x00040020, 0x00000012, 0x00000001, 0x00000011, 0x0004003B, 0x00000012, 0x00000013, 
0x00000001, 0x0004002B, 0x00000007, 0x00000015, 0x00000000, 0x0004002B, 0x00000007, 0x00000016, 0x3F800000, 0x00040020, 0x0000001A, 0x00000003, 
0x00000008, 0x00040020, 0x0000001C, 0x00000003, 0x00000011, 0x0004003B, 0x0000001C, 0x0000001D, 0x00000003, 0x0004003B, 0x00000012, 0x0000001E, 
0x00000001, 0x0004003B, 0x0000001A, 0x00000020, 0x00000003, 0x00040020, 0x00000021, 0x00000001, 0x00000008, 0x0004003B, 0x00000021, 0x00000022, 
0x00000001, 0x00040020, 0x00000024, 0x00000001, 0x0000000F, 0x0004003B, 0x00000024, 0x00000025, 0x00000001, 0x0004003B, 0x00000024, 0x00000026, 
0x00000001, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x00040008, 0x00000001, 0x00000069, 0x00000000, 
0x0004003D, 0x00000011, 0x00000014, 0x00000013, 0x00050051, 0x00000007, 0x00000017, 0x00000014, 0x00000000, 0x00050051, 0x00000007, 0x00000018, 
0x00000014, 0x00000001, 0x00070050, 0x00000008, 0x00000019, 0x00000017, 0x00000018, 0x00000015, 0x00000016, 0x00050041, 0x0000001A, 0x0000001B, 
0x0000000E, 0x00000010, 0x0003003E, 0x0000001B, 0x00000019, 0x00040008, 0x00000001, 0x0000006A, 0x00000000, 0x0004003D, 0x00000011, 0x0000001F, 
0x0000001E, 0x0003003E, 0x0000001D, 0x0000001F, 0x00040008, 0x00000001, 0x0000006B, 0x00000000, 0x0004003D, 0x00000008, 0x00000023, 0x00000022, 
0x0003003E, 0x00000020, 0x00000023, 0x000100FD, 0x00010038 });

	descr.fragOutput = FragmentOutputState()
			.Add( "out_Color", EFragOutput::Float4, 0 );

	descr.Fragment().StringGLSL( 
R"#(#version 450 core


#define SH_VERTEX           (1<<0)
#define SH_TESS_CONTROL     (1<<1)
#define SH_TESS_EVALUATION  (1<<2)
#define SH_GEOMETRY         (1<<3)
#define SH_FRAGMENT         (1<<4)
#define SH_COMPUTE          (1<<5)

#ifdef GL_ARB_gpu_shader_int64
#extension GL_ARB_gpu_shader_int64 : require
//#define ARB_gpu_shader_int64_enabled  1
#endif

// for vulkan compatibility
#ifdef GL_ARB_separate_shader_objects
#extension GL_ARB_separate_shader_objects : enable
#define ARB_separate_shader_objects_enabled  1
#endif

#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4

#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

#ifdef ARB_gpu_shader_int64_enabled
# define ilong		int64_t
# define ilong2		i64vec2
# define ilong3		i64vec3
# define ilong4		i64vec4
# define ulong		uint64_t
# define ulong2		u64vec2
# define ulong3		u64vec3
# define ulong4		u64vec4
#endif // ARB_gpu_shader_int64_enabled

#define float2		vec2
#define float3		vec3
#define float4		vec4
			
#define float2x2	mat2x2
#define float2x3	mat2x3
#define float2x4	mat2x4
#define float3x2	mat3x2
#define float3x3	mat3x3
#define float3x4	mat3x4
#define float4x2	mat4x2
#define float4x3	mat4x3
#define float4x4	mat4x4

#define double2		dvec2
#define double3		dvec3
#define double4		dvec4
			
#define double2x2	dmat2x2
#define double2x3	dmat2x3
#define double2x4	dmat2x4
#define double3x2	dmat3x2
#define double3x3	dmat3x3
#define double3x4	dmat3x4
#define double4x2	dmat4x2
#define double4x3	dmat4x3
#define double4x4	dmat4x4

#ifdef VULKAN
#define PUSH_CONSTANT( _name_ )	layout (std140, push_constant) uniform _name_
#else
#define PUSH_CONSTANT( _name_ )	layout (std140) uniform pushConst_##_name_
#endif
		
#define SHADER	SH_FRAGMENT

layout(location=0) smooth in   vec2 v_Texcoord;
layout(location=1) smooth in   vec4 v_Color;
layout(location=0) out   vec4 out_Color;

layout(binding=0) uniform sampler2D un_ColorTexture;



void main ()
{
	out_Color = (texture( un_ColorTexture, v_Texcoord ) * v_Color);
}



)#"_str );
	descr.Fragment().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080006, 0x00000019, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0008000F, 0x00000004, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000A, 0x00000012, 0x00000016, 
0x00030010, 0x00000005, 0x00000008, 0x00030007, 0x00000001, 0x00000000, 0x002D0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 
0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 
0x63206465, 0x6E65696C, 0x706F2074, 0x6C676E65, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 
0x20766E65, 0x6E65706F, 0x2F0A6C67, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x72746E65, 0x6F702D79, 0x20746E69, 0x6E69616D, 
0x696C230A, 0x3120656E, 0x0000000A, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 0x415F4C47, 
0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00050005, 
0x0000000A, 0x5F74756F, 0x6F6C6F43, 0x00000072, 0x00060005, 0x0000000E, 0x435F6E75, 0x726F6C6F, 0x74786554, 0x00657275, 0x00050005, 0x00000012, 
0x65545F76, 0x6F6F6378, 0x00006472, 0x00040005, 0x00000016, 0x6F435F76, 0x00726F6C, 0x00040047, 0x0000000A, 0x0000001E, 0x00000000, 0x00040047, 
0x0000000E, 0x00000022, 0x00000000, 0x00040047, 0x0000000E, 0x00000021, 0x00000000, 0x00040047, 0x00000012, 0x0000001E, 0x00000000, 0x00040047, 
0x00000016, 0x0000001E, 0x00000001, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 
0x00000008, 0x00000007, 0x00000004, 0x00040020, 0x00000009, 0x00000003, 0x00000008, 0x0004003B, 0x00000009, 0x0000000A, 0x00000003, 0x00090019, 
0x0000000B, 0x00000007, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x0003001B, 0x0000000C, 0x0000000B, 0x00040020, 
0x0000000D, 0x00000000, 0x0000000C, 0x0004003B, 0x0000000D, 0x0000000E, 0x00000000, 0x00040017, 0x00000010, 0x00000007, 0x00000002, 0x00040020, 
0x00000011, 0x00000001, 0x00000010, 0x0004003B, 0x00000011, 0x00000012, 0x00000001, 0x00040020, 0x00000015, 0x00000001, 0x00000008, 0x0004003B, 
0x00000015, 0x00000016, 0x00000001, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x00040008, 0x00000001, 
0x0000005B, 0x00000000, 0x0004003D, 0x0000000C, 0x0000000F, 0x0000000E, 0x0004003D, 0x00000010, 0x00000013, 0x00000012, 0x00050057, 0x00000008, 
0x00000014, 0x0000000F, 0x00000013, 0x0004003D, 0x00000008, 0x00000017, 0x00000016, 0x00050085, 0x00000008, 0x00000018, 0x00000014, 0x00000017, 
0x0003003E, 0x0000000A, 0x00000018, 0x000100FD, 0x00010038 });

};
};
