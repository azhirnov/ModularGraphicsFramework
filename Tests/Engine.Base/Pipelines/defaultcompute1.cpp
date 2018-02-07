// This is generated file
// Origin file: 'C:\Projects\graphxgenengine\Tests\PipelineCompiler\Pipelines\DefaultCompute1.cpp'
// Created at: 2018/02/02 - 00:40:13

#include "all_pipelines.h"

namespace Pipelines
{

void Create_defaultcompute1 (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	descr.supportedShaders = EShader::bits() | EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddImage( "un_OutImage", EImage::Tex2D, EPixelFormat::RGBA8_UNorm, true, false, 0, 0, EShader::bits() | EShader::Compute )
			.AddStorageBuffer( "ssb", 16_b, 0_b, false, true, 0, 1, EShader::bits() | EShader::Compute )
			.Finish();

	descr.Compute().StringGLSL( 
R"#(#version 450 core


#define SH_VERTEX           (1<<0)
#define SH_TESS_CONTROL     (1<<1)
#define SH_TESS_EVALUATION  (1<<2)
#define SH_GEOMETRY         (1<<3)
#define SH_FRAGMENT         (1<<4)
#define SH_COMPUTE          (1<<5)

#ifdef GL_ARB_gpu_shader_int64
#extension GL_ARB_gpu_shader_int64 : require
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
		
#define SHADER	SH_COMPUTE


layout(binding=0) layout(rgba8) writeonly uniform image2D un_OutImage;

layout(binding=0) layout(std430) readonly buffer SSBO {
	layout(offset=0, align=16) vec4  data;    // offset: 0, align: 16

} ssb;


layout (local_size_x=1, local_size_y=1, local_size_z=1) in;


//---------------------------------
void main ()
{
	vec2 uv;
	uv=(vec2( gl_GlobalInvocationID.xy )/vec2( (uvec3(1)*gl_NumWorkGroups).xy ));
	vec4 fragColor;
	fragColor=vec4( sin( uv.x ), cos( uv.y ), float(1.0), ssb.data.x );
	imageStore( un_OutImage, ivec2( gl_GlobalInvocationID.xy ), fragColor );
}



)#"_str );
	descr.Compute().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x00000039, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000E, 0x00000015, 0x00060010, 
0x00000005, 0x00000011, 0x00000001, 0x00000001, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 
0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 
0x6F725065, 0x73736563, 0x63206465, 0x6E65696C, 0x75762074, 0x6E616B6C, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 
0x72617420, 0x2D746567, 0x20766E65, 0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 
0x696F702D, 0x6D20746E, 0x0A6E6961, 0x6E696C23, 0x0A312065, 0x00000000, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 
0x00343674, 0x00090004, 0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 
0x6E69616D, 0x00000000, 0x00030005, 0x0000000A, 0x00007675, 0x00080005, 0x0000000E, 0x475F6C67, 0x61626F6C, 0x766E496C, 0x7461636F, 0x496E6F69, 
0x00000044, 0x00070005, 0x00000015, 0x4E5F6C67, 0x6F576D75, 0x72476B72, 0x7370756F, 0x00000000, 0x00050005, 0x0000001D, 0x67617266, 0x6F6C6F43, 
0x00000072, 0x00040005, 0x00000027, 0x4F425353, 0x00000000, 0x00050006, 0x00000027, 0x00000000, 0x61746164, 0x00000000, 0x00030005, 0x00000029, 
0x00627373, 0x00050005, 0x00000032, 0x4F5F6E75, 0x6D497475, 0x00656761, 0x00040047, 0x0000000E, 0x0000000B, 0x0000001C, 0x00040047, 0x00000015, 
0x0000000B, 0x00000018, 0x00040048, 0x00000027, 0x00000000, 0x00000018, 0x00050048, 0x00000027, 0x00000000, 0x00000023, 0x00000000, 0x00030047, 
0x00000027, 0x00000003, 0x00040047, 0x00000029, 0x00000022, 0x00000000, 0x00040047, 0x00000029, 0x00000021, 0x00000001, 0x00040047, 0x00000032, 
0x00000022, 0x00000000, 0x00040047, 0x00000032, 0x00000021, 0x00000000, 0x00030047, 0x00000032, 0x00000019, 0x00020013, 0x00000003, 0x00030021, 
0x00000004, 0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 0x00000008, 0x00000007, 0x00000002, 0x00040020, 0x00000009, 0x00000007, 
0x00000008, 0x00040015, 0x0000000B, 0x00000020, 0x00000000, 0x00040017, 0x0000000C, 0x0000000B, 0x00000003, 0x00040020, 0x0000000D, 0x00000001, 
0x0000000C, 0x0004003B, 0x0000000D, 0x0000000E, 0x00000001, 0x00040017, 0x0000000F, 0x0000000B, 0x00000002, 0x0004002B, 0x0000000B, 0x00000013, 
0x00000001, 0x0006002C, 0x0000000C, 0x00000014, 0x00000013, 0x00000013, 0x00000013, 0x0004003B, 0x0000000D, 0x00000015, 0x00000001, 0x00040017, 
0x0000001B, 0x00000007, 0x00000004, 0x00040020, 0x0000001C, 0x00000007, 0x0000001B, 0x0004002B, 0x0000000B, 0x0000001E, 0x00000000, 0x00040020, 
0x0000001F, 0x00000007, 0x00000007, 0x0004002B, 0x00000007, 0x00000026, 0x3F800000, 0x0003001E, 0x00000027, 0x0000001B, 0x00040020, 0x00000028, 
0x00000002, 0x00000027, 0x0004003B, 0x00000028, 0x00000029, 0x00000002, 0x00040015, 0x0000002A, 0x00000020, 0x00000001, 0x0004002B, 0x0000002A, 
0x0000002B, 0x00000000, 0x00040020, 0x0000002C, 0x00000002, 0x00000007, 0x00090019, 0x00000030, 0x00000007, 0x00000001, 0x00000000, 0x00000000, 
0x00000000, 0x00000002, 0x00000004, 0x00040020, 0x00000031, 0x00000000, 0x00000030, 0x0004003B, 0x00000031, 0x00000032, 0x00000000, 0x00040017, 
0x00000036, 0x0000002A, 0x00000002, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x0004003B, 0x00000009, 
0x0000000A, 0x00000007, 0x0004003B, 0x0000001C, 0x0000001D, 0x00000007, 0x00040008, 0x00000001, 0x00000061, 0x00000000, 0x0004003D, 0x0000000C, 
0x00000010, 0x0000000E, 0x0007004F, 0x0000000F, 0x00000011, 0x00000010, 0x00000010, 0x00000000, 0x00000001, 0x00040070, 0x00000008, 0x00000012, 
0x00000011, 0x0004003D, 0x0000000C, 0x00000016, 0x00000015, 0x00050084, 0x0000000C, 0x00000017, 0x00000014, 0x00000016, 0x0007004F, 0x0000000F, 
0x00000018, 0x00000017, 0x00000017, 0x00000000, 0x00000001, 0x00040070, 0x00000008, 0x00000019, 0x00000018, 0x00050088, 0x00000008, 0x0000001A, 
0x00000012, 0x00000019, 0x0003003E, 0x0000000A, 0x0000001A, 0x00040008, 0x00000001, 0x00000063, 0x00000000, 0x00050041, 0x0000001F, 0x00000020, 
0x0000000A, 0x0000001E, 0x0004003D, 0x00000007, 0x00000021, 0x00000020, 0x0006000C, 0x00000007, 0x00000022, 0x00000002, 0x0000000D, 0x00000021, 
0x00050041, 0x0000001F, 0x00000023, 0x0000000A, 0x00000013, 0x0004003D, 0x00000007, 0x00000024, 0x00000023, 0x0006000C, 0x00000007, 0x00000025, 
0x00000002, 0x0000000E, 0x00000024, 0x00060041, 0x0000002C, 0x0000002D, 0x00000029, 0x0000002B, 0x0000001E, 0x0004003D, 0x00000007, 0x0000002E, 
0x0000002D, 0x00070050, 0x0000001B, 0x0000002F, 0x00000022, 0x00000025, 0x00000026, 0x0000002E, 0x0003003E, 0x0000001D, 0x0000002F, 0x00040008, 
0x00000001, 0x00000064, 0x00000000, 0x0004003D, 0x00000030, 0x00000033, 0x00000032, 0x0004003D, 0x0000000C, 0x00000034, 0x0000000E, 0x0007004F, 
0x0000000F, 0x00000035, 0x00000034, 0x00000034, 0x00000000, 0x00000001, 0x0004007C, 0x00000036, 0x00000037, 0x00000035, 0x0004003D, 0x0000001B, 
0x00000038, 0x0000001D, 0x00040063, 0x00000033, 0x00000037, 0x00000038, 0x000100FD, 0x00010038 });
	descr.Compute().StringCL( 
R"#(#define FORMAT( _fmt_ )

// Functions for GLSL compatibility

#define Gen_FloatTemplates( _gen_ ) \
	_gen_( float ) \
	_gen_( float2 ) \
	_gen_( float3 ) \
	_gen_( float4 )

#define Gen_DoubleTemplates( _gen_ ) \
	_gen_( double ) \
	_gen_( double2 ) \
	_gen_( double3 ) \
	_gen_( double4 )
	

// Fract
#define GenTemplate_Fract( _type_ ) \
	_type_ fractTempl_##_type_ (_type_ x) { \
		_type_	ipart; \
		return fract( x, &ipart ); \
	}
	Gen_FloatTemplates( GenTemplate_Fract )
	Gen_DoubleTemplates( GenTemplate_Fract )
#undef GenTemplate_Fract


#undef Gen_FloatTemplates
#undef Gen_DoubleTemplates
			
struct SSBO{
	float4 data;
};


//---------------------------------

__constant float C_1d0 = (float)(1.0);

//---------------------------------


//---------------------------------
kernel void main (
	/*0*/write_only FORMAT(rgba8_unorm) image2d_t un_OutImage,
	/*1*/__global read_only struct SSBO* ssb)
{
	int2 H_fjkh5n;
	H_fjkh5n=convert_int3( (uint3)(get_global_id(0),  get_global_id(1),  get_global_id(2)) ).xy;
	float2 H_xulukk;
	H_xulukk=convert_float2( H_fjkh5n );
	int2 H_k5ommu;
	H_k5ommu=convert_int3( (uint3)(get_num_groups(0), get_num_groups(1), get_num_groups(2)) ).xy;
	float2 H_nnu7tn1;
	H_nnu7tn1=convert_float2( H_k5ommu );
	float2 uv;
	uv=(H_xulukk/H_nnu7tn1);
	float fragColor;
	fragColor=sin( uv.x );
	float fragColor1;
	fragColor1=cos( uv.y );
	float4 H_imjy6u1;
	H_imjy6u1=(float4)( fragColor, fragColor1, C_1d0, ssb->data.x );
	write_imagef(un_OutImage, H_fjkh5n, H_imjy6u1);
}


)#"_str );

};
};
