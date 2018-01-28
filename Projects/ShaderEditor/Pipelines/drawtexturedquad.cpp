// This is generated file
// Origin file: 'C:\Projects\graphxgenengine\Projects\ShaderEditorTools\DrawTexturedQuad.cpp'
// Created at: 2018/01/17 - 20:21:46

#include "all_pipelines.h"

namespace Pipelines
{

void Create_drawtexturedquad (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	// RenderState
	descr.renderState = RenderState();
	// InputAssemblyState
	descr.renderState.inputAssembly.topology         = EPrimitive::TriangleStrip;
	descr.renderState.inputAssembly.primitiveRestart = false;

	descr.dynamicStates = EPipelineDynamicState::bits() | EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;
	descr.supportedShaders = EShader::bits() | EShader::Vertex | EShader::Fragment;
	descr.supportedPrimitives = EPrimitive::bits() | EPrimitive::TriangleStrip;

	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddTexture( "un_ColorTexture", EImage::Tex2D, EPixelFormatClass::RGBA | EPixelFormatClass::LinearColorSpace | EPixelFormatClass::AnyFloat | EPixelFormatClass::AnyNorm, 0, 0, EShader::bits() | EShader::Fragment )
			.Finish();

	descr.Vertex().StringGLSL( 
R"#(#version 450 core


#define SH_VERTEX           (1<<0)
#define SH_TESS_CONTROL     (1<<1)
#define SH_TESS_EVALUATION  (1<<2)
#define SH_GEOMETRY         (1<<3)
#define SH_FRAGMENT         (1<<4)
#define SH_COMPUTE          (1<<5)

#ifdef GL_ARB_gpu_shader_int64
#extension GL_ARB_gpu_shader_int64 : enable
#define ARB_gpu_shader_int64_enabled  1
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
	vec4	gl_Position;
};



const vec2 g_Positions[4] = { vec2(-1.0, 1.0), vec2(-1.0), vec2(1.0), vec2(1.0, -1.0) };

//---------------------------------


//---------------------------------
void main ()
{
	gl_Position=vec4( g_Positions[gl_VertexID], float(0.0), float(1.0) );
}



)#"_str );
	descr.Vertex().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x00000029, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000000, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000E, 0x0000001C, 0x00030007, 
0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 
0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x63206465, 0x6E65696C, 0x75762074, 0x6E616B6C, 
0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 0x20766E65, 0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 
0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 0x696F702D, 0x6D20746E, 0x0A6E6961, 0x6E696C23, 0x0A312065, 0x00000000, 
0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 
0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00060005, 0x0000000C, 0x505F6C67, 0x65567265, 
0x78657472, 0x00000000, 0x00060006, 0x0000000C, 0x00000000, 0x505F6C67, 0x7469736F, 0x006E6F69, 0x00070006, 0x0000000C, 0x00000001, 0x505F6C67, 
0x746E696F, 0x657A6953, 0x00000000, 0x00070006, 0x0000000C, 0x00000002, 0x435F6C67, 0x4470696C, 0x61747369, 0x0065636E, 0x00070006, 0x0000000C, 
0x00000003, 0x435F6C67, 0x446C6C75, 0x61747369, 0x0065636E, 0x00030005, 0x0000000E, 0x00000000, 0x00060005, 0x0000001C, 0x565F6C67, 0x65747265, 
0x646E4978, 0x00007865, 0x00050005, 0x0000001F, 0x65646E69, 0x6C626178, 0x00000065, 0x00050048, 0x0000000C, 0x00000000, 0x0000000B, 0x00000000, 
0x00050048, 0x0000000C, 0x00000001, 0x0000000B, 0x00000001, 0x00050048, 0x0000000C, 0x00000002, 0x0000000B, 0x00000003, 0x00050048, 0x0000000C, 
0x00000003, 0x0000000B, 0x00000004, 0x00030047, 0x0000000C, 0x00000002, 0x00040047, 0x0000001C, 0x0000000B, 0x0000002A, 0x00020013, 0x00000003, 
0x00030021, 0x00000004, 0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 0x00000008, 0x00000007, 0x00000004, 0x00040015, 0x00000009, 
0x00000020, 0x00000000, 0x0004002B, 0x00000009, 0x0000000A, 0x00000001, 0x0004001C, 0x0000000B, 0x00000007, 0x0000000A, 0x0006001E, 0x0000000C, 
0x00000008, 0x00000007, 0x0000000B, 0x0000000B, 0x00040020, 0x0000000D, 0x00000003, 0x0000000C, 0x0004003B, 0x0000000D, 0x0000000E, 0x00000003, 
0x00040015, 0x0000000F, 0x00000020, 0x00000001, 0x0004002B, 0x0000000F, 0x00000010, 0x00000000, 0x00040017, 0x00000011, 0x00000007, 0x00000002, 
0x0004002B, 0x00000009, 0x00000012, 0x00000004, 0x0004001C, 0x00000013, 0x00000011, 0x00000012, 0x0004002B, 0x00000007, 0x00000014, 0xBF800000, 
0x0004002B, 0x00000007, 0x00000015, 0x3F800000, 0x0005002C, 0x00000011, 0x00000016, 0x00000014, 0x00000015, 0x0005002C, 0x00000011, 0x00000017, 
0x00000014, 0x00000014, 0x0005002C, 0x00000011, 0x00000018, 0x00000015, 0x00000015, 0x0005002C, 0x00000011, 0x00000019, 0x00000015, 0x00000014, 
0x0007002C, 0x00000013, 0x0000001A, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x00040020, 0x0000001B, 0x00000001, 0x0000000F, 0x0004003B, 
0x0000001B, 0x0000001C, 0x00000001, 0x00040020, 0x0000001E, 0x00000007, 0x00000013, 0x00040020, 0x00000020, 0x00000007, 0x00000011, 0x0004002B, 
0x00000007, 0x00000023, 0x00000000, 0x00040020, 0x00000027, 0x00000003, 0x00000008, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 
0x000200F8, 0x00000006, 0x0004003B, 0x0000001E, 0x0000001F, 0x00000007, 0x00040008, 0x00000001, 0x00000064, 0x00000000, 0x0004003D, 0x0000000F, 
0x0000001D, 0x0000001C, 0x0003003E, 0x0000001F, 0x0000001A, 0x00050041, 0x00000020, 0x00000021, 0x0000001F, 0x0000001D, 0x0004003D, 0x00000011, 
0x00000022, 0x00000021, 0x00050051, 0x00000007, 0x00000024, 0x00000022, 0x00000000, 0x00050051, 0x00000007, 0x00000025, 0x00000022, 0x00000001, 
0x00070050, 0x00000008, 0x00000026, 0x00000024, 0x00000025, 0x00000023, 0x00000015, 0x00050041, 0x00000027, 0x00000028, 0x0000000E, 0x00000010, 
0x0003003E, 0x00000028, 0x00000026, 0x000100FD, 0x00010038 });

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
#extension GL_ARB_gpu_shader_int64 : enable
#define ARB_gpu_shader_int64_enabled  1
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

layout(location=0) out   vec4 out_Color;

layout(binding=0) uniform sampler2D un_ColorTexture;




//---------------------------------
void main ()
{
	out_Color=texelFetch( un_ColorTexture, ivec2( gl_FragCoord.xy ), int(0) );
}



)#"_str );
	descr.Fragment().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x0000001B, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000004, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000A, 0x00000011, 0x00030010, 
0x00000005, 0x00000007, 0x00030007, 0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 0x50656C75, 
0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x63206465, 
0x6E65696C, 0x75762074, 0x6E616B6C, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 0x20766E65, 
0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 0x696F702D, 0x6D20746E, 0x0A6E6961, 
0x6E696C23, 0x0A312065, 0x00000000, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 0x415F4C47, 
0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00050005, 
0x0000000A, 0x5F74756F, 0x6F6C6F43, 0x00000072, 0x00060005, 0x0000000E, 0x435F6E75, 0x726F6C6F, 0x74786554, 0x00657275, 0x00060005, 0x00000011, 
0x465F6C67, 0x43676172, 0x64726F6F, 0x00000000, 0x00040047, 0x0000000A, 0x0000001E, 0x00000000, 0x00040047, 0x0000000E, 0x00000022, 0x00000000, 
0x00040047, 0x0000000E, 0x00000021, 0x00000000, 0x00040047, 0x00000011, 0x0000000B, 0x0000000F, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 
0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 0x00000008, 0x00000007, 0x00000004, 0x00040020, 0x00000009, 0x00000003, 0x00000008, 
0x0004003B, 0x00000009, 0x0000000A, 0x00000003, 0x00090019, 0x0000000B, 0x00000007, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 
0x00000000, 0x0003001B, 0x0000000C, 0x0000000B, 0x00040020, 0x0000000D, 0x00000000, 0x0000000C, 0x0004003B, 0x0000000D, 0x0000000E, 0x00000000, 
0x00040020, 0x00000010, 0x00000001, 0x00000008, 0x0004003B, 0x00000010, 0x00000011, 0x00000001, 0x00040017, 0x00000012, 0x00000007, 0x00000002, 
0x00040015, 0x00000015, 0x00000020, 0x00000001, 0x00040017, 0x00000016, 0x00000015, 0x00000002, 0x0004002B, 0x00000015, 0x00000018, 0x00000000, 
0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x00040008, 0x00000001, 0x0000005B, 0x00000000, 0x0004003D, 
0x0000000C, 0x0000000F, 0x0000000E, 0x0004003D, 0x00000008, 0x00000013, 0x00000011, 0x0007004F, 0x00000012, 0x00000014, 0x00000013, 0x00000013, 
0x00000000, 0x00000001, 0x0004006E, 0x00000016, 0x00000017, 0x00000014, 0x00040064, 0x0000000B, 0x00000019, 0x0000000F, 0x0007005F, 0x00000008, 
0x0000001A, 0x00000019, 0x00000017, 0x00000002, 0x00000018, 0x0003003E, 0x0000000A, 0x0000001A, 0x000100FD, 0x00010038 });

};
};
