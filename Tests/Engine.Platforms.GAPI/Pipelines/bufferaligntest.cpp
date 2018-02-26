// This is generated file
// Origin file: 'C:\Projects\graphxgenengine\Tests\Engine.Platforms.GAPI.Tools\Compute\BufferAlignTest.cpp'
// Created at: 2018/02/25 - 23:59:15

#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang
{
	struct BufferAlign_Struct
	{
		Int4 i4;
		Float2 f2;
		Bool b1;
		Float _padding0;
		UInt4 u3;
		Int i1;
		Float _padding1;
		Float _padding2;
		Float _padding3;
		Bool4 b3;
		Float f1;
		Float _padding4;
		Float _padding5;
		Float _padding6;
	};
	
	struct BufferAlign_SSBO
	{
		BufferAlign_Struct src;
		BufferAlign_Struct dst1;
		BufferAlign_Struct dst2;
	};
	
	
	//---------------------------------
	
	static void sw_bufferaligntest_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::StorageBuffer< BufferAlign_SSBO, Impl::EStorageAccess::Coherent > ssb;	_helper_.GetStorageBuffer( 0, ssb );
		const Int4 C_hb75lg1 = Int4( 1, -2, 3, -4 );
		const Float2 C_kbipui1 = Float2( 3.0999999, 5.5 );
		const Bool C_true = Bool( true );
		const Int C_9 = Int( 9 );
		const Int C_8 = Int( 8 );
		const Int C_7 = Int( 7 );
		const Int C_1193046 = Int( 1193046 );
		const Bool C_false = Bool( false );
		const Float C_1d4335 = Float( 1.43350005 );
	
		// shader
	{
		ssb->dst1=ssb->src;
		ssb->dst2.i4=C_hb75lg1;
		ssb->dst2.f2=C_kbipui1;
		ssb->dst2.b1=C_true;
		Int4 H_qenhp1;
		H_qenhp1=Int4( ssb->dst2.u3 );
		H_qenhp1.xyz=Int3( 9, 8, 7 );
		ssb->dst2.u3=UInt4( H_qenhp1 );
		ssb->dst2.i1=C_1193046;
		Bool4 H_lsg2a3;
		H_lsg2a3=ssb->dst2.b3;
		H_lsg2a3.xyz=Bool3( false, true, false );
		ssb->dst2.b3=H_lsg2a3;
		ssb->dst2.f1=C_1d4335;
	}
	
	
	}
}	// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_bufferaligntest (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	descr.supportedShaders = EShader::bits() | EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddStorageBuffer( "ssb", 288_b, 0_b, true, false, 0, 0, EShader::bits() | EShader::Compute )
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

struct BufferAlign_Struct
{
	int4  i4;  // offset: 0, align: 16
	float2  f2;  // offset: 16, align: 8
	bool  b1;  // offset: 24, align: 4
	float  _padding0;  // offset: 28, align: 4
	uint4  u3;  // offset: 32, align: 16
	int  i1;  // offset: 48, align: 4
	float  _padding1;  // offset: 52, align: 4
	float  _padding2;  // offset: 56, align: 4
	float  _padding3;  // offset: 60, align: 4
	bool4  b3;  // offset: 64, align: 16
	float  f1;  // offset: 80, align: 4
	float  _padding4;  // offset: 84, align: 4
	float  _padding5;  // offset: 88, align: 4
	float  _padding6;  // offset: 92, align: 4
};



layout(binding=0) layout(std430)  buffer BufferAlign_SSBO {
	layout(offset=0, align=16) readonly BufferAlign_Struct  src;
	layout(offset=96, align=16) writeonly BufferAlign_Struct  dst1;
	layout(offset=192, align=16) writeonly BufferAlign_Struct  dst2;

} ssb;


layout (local_size_x=1, local_size_y=1, local_size_z=1) in;


//---------------------------------

void main ()
{
	ssb.dst1=ssb.src;
	ssb.dst2.i4=ivec4( 1, -2, 3, -4 );
	ssb.dst2.f2=vec2( 3.0999999, 5.5 );
	ssb.dst2.b1=bool( true );
	ssb.dst2.u3.xyz=uvec3( 9, 8, 7 );
	ssb.dst2.i1=int( 1193046 );
	ssb.dst2.b3.xyz=bvec3( false, true, false );
	ssb.dst2.f1=float( 1.43350005 );
}



)#"_str );
	descr.Compute().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x00000047, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0005000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x00060010, 0x00000005, 0x00000011, 
0x00000001, 0x00000001, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 
0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 
0x63206465, 0x6E65696C, 0x75762074, 0x6E616B6C, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 
0x20766E65, 0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 0x696F702D, 0x6D20746E, 
0x0A6E6961, 0x6E696C23, 0x0A312065, 0x00000000, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 
0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 
0x00070005, 0x0000000D, 0x66667542, 0x6C417265, 0x5F6E6769, 0x75727453, 0x00007463, 0x00040006, 0x0000000D, 0x00000000, 0x00003469, 0x00040006, 
0x0000000D, 0x00000001, 0x00003266, 0x00040006, 0x0000000D, 0x00000002, 0x00003162, 0x00060006, 0x0000000D, 0x00000003, 0x6461705F, 0x676E6964, 
0x00000030, 0x00040006, 0x0000000D, 0x00000004, 0x00003375, 0x00040006, 0x0000000D, 0x00000005, 0x00003169, 0x00060006, 0x0000000D, 0x00000006, 
0x6461705F, 0x676E6964, 0x00000031, 0x00060006, 0x0000000D, 0x00000007, 0x6461705F, 0x676E6964, 0x00000032, 0x00060006, 0x0000000D, 0x00000008, 
0x6461705F, 0x676E6964, 0x00000033, 0x00040006, 0x0000000D, 0x00000009, 0x00003362, 0x00040006, 0x0000000D, 0x0000000A, 0x00003166, 0x00060006, 
0x0000000D, 0x0000000B, 0x6461705F, 0x676E6964, 0x00000034, 0x00060006, 0x0000000D, 0x0000000C, 0x6461705F, 0x676E6964, 0x00000035, 0x00060006, 
0x0000000D, 0x0000000D, 0x6461705F, 0x676E6964, 0x00000036, 0x00070005, 0x0000000E, 0x66667542, 0x6C417265, 0x5F6E6769, 0x4F425353, 0x00000000, 
0x00040006, 0x0000000E, 0x00000000, 0x00637273, 0x00050006, 0x0000000E, 0x00000001, 0x31747364, 0x00000000, 0x00050006, 0x0000000E, 0x00000002, 
0x32747364, 0x00000000, 0x00030005, 0x00000010, 0x00627373, 0x00050048, 0x0000000D, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000000D, 
0x00000001, 0x00000023, 0x00000010, 0x00050048, 0x0000000D, 0x00000002, 0x00000023, 0x00000018, 0x00050048, 0x0000000D, 0x00000003, 0x00000023, 
0x0000001C, 0x00050048, 0x0000000D, 0x00000004, 0x00000023, 0x00000020, 0x00050048, 0x0000000D, 0x00000005, 0x00000023, 0x00000030, 0x00050048, 
0x0000000D, 0x00000006, 0x00000023, 0x00000034, 0x00050048, 0x0000000D, 0x00000007, 0x00000023, 0x00000038, 0x00050048, 0x0000000D, 0x00000008, 
0x00000023, 0x0000003C, 0x00050048, 0x0000000D, 0x00000009, 0x00000023, 0x00000040, 0x00050048, 0x0000000D, 0x0000000A, 0x00000023, 0x00000050, 
0x00050048, 0x0000000D, 0x0000000B, 0x00000023, 0x00000054, 0x00050048, 0x0000000D, 0x0000000C, 0x00000023, 0x00000058, 0x00050048, 0x0000000D, 
0x0000000D, 0x00000023, 0x0000005C, 0x00050048, 0x0000000E, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000000E, 0x00000001, 0x00000023, 
0x00000060, 0x00050048, 0x0000000E, 0x00000002, 0x00000023, 0x000000C0, 0x00030047, 0x0000000E, 0x00000003, 0x00040047, 0x00000010, 0x00000022, 
0x00000000, 0x00040047, 0x00000010, 0x00000021, 0x00000000, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00040015, 0x00000007, 
0x00000020, 0x00000001, 0x00040017, 0x00000008, 0x00000007, 0x00000004, 0x00030016, 0x00000009, 0x00000020, 0x00040017, 0x0000000A, 0x00000009, 
0x00000002, 0x00040015, 0x0000000B, 0x00000020, 0x00000000, 0x00040017, 0x0000000C, 0x0000000B, 0x00000004, 0x0010001E, 0x0000000D, 0x00000008, 
0x0000000A, 0x0000000B, 0x00000009, 0x0000000C, 0x00000007, 0x00000009, 0x00000009, 0x00000009, 0x0000000C, 0x00000009, 0x00000009, 0x00000009, 
0x00000009, 0x0005001E, 0x0000000E, 0x0000000D, 0x0000000D, 0x0000000D, 0x00040020, 0x0000000F, 0x00000002, 0x0000000E, 0x0004003B, 0x0000000F, 
0x00000010, 0x00000002, 0x0004002B, 0x00000007, 0x00000011, 0x00000001, 0x0004002B, 0x00000007, 0x00000012, 0x00000000, 0x00040020, 0x00000013, 
0x00000002, 0x0000000D, 0x0004002B, 0x00000007, 0x00000017, 0x00000002, 0x0004002B, 0x00000007, 0x00000018, 0xFFFFFFFE, 0x0004002B, 0x00000007, 
0x00000019, 0x00000003, 0x0004002B, 0x00000007, 0x0000001A, 0xFFFFFFFC, 0x0007002C, 0x00000008, 0x0000001B, 0x00000011, 0x00000018, 0x00000019, 
0x0000001A, 0x00040020, 0x0000001C, 0x00000002, 0x00000008, 0x0004002B, 0x00000009, 0x0000001E, 0x40466666, 0x0004002B, 0x00000009, 0x0000001F, 
0x40B00000, 0x0005002C, 0x0000000A, 0x00000020, 0x0000001E, 0x0000001F, 0x00040020, 0x00000021, 0x00000002, 0x0000000A, 0x00020014, 0x00000023, 
0x00030029, 0x00000023, 0x00000024, 0x0004002B, 0x0000000B, 0x00000025, 0x00000001, 0x0004002B, 0x0000000B, 0x00000026, 0x00000000, 0x00040020, 
0x00000028, 0x00000002, 0x0000000B, 0x0004002B, 0x00000007, 0x0000002A, 0x00000004, 0x00040017, 0x0000002B, 0x0000000B, 0x00000003, 0x0004002B, 
0x0000000B, 0x0000002C, 0x00000009, 0x0004002B, 0x0000000B, 0x0000002D, 0x00000008, 0x0004002B, 0x0000000B, 0x0000002E, 0x00000007, 0x0006002C, 
0x0000002B, 0x0000002F, 0x0000002C, 0x0000002D, 0x0000002E, 0x00040020, 0x00000030, 0x00000002, 0x0000000C, 0x0004002B, 0x00000007, 0x00000034, 
0x00000005, 0x0004002B, 0x00000007, 0x00000035, 0x00123456, 0x00040020, 0x00000036, 0x00000002, 0x00000007, 0x0004002B, 0x00000007, 0x00000038, 
0x00000009, 0x00040017, 0x00000039, 0x00000023, 0x00000004, 0x00040017, 0x0000003A, 0x00000023, 0x00000003, 0x0003002A, 0x00000023, 0x0000003B, 
0x0006002C, 0x0000003A, 0x0000003C, 0x0000003B, 0x00000024, 0x0000003B, 0x0006002C, 0x0000002B, 0x0000003D, 0x00000025, 0x00000025, 0x00000025, 
0x0006002C, 0x0000002B, 0x0000003E, 0x00000026, 0x00000026, 0x00000026, 0x0004002B, 0x00000007, 0x00000043, 0x0000000A, 0x0004002B, 0x00000009, 
0x00000044, 0x3FB77CEE, 0x00040020, 0x00000045, 0x00000002, 0x00000009, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 
0x00000006, 0x00040008, 0x00000001, 0x00000074, 0x00000000, 0x00050041, 0x00000013, 0x00000014, 0x00000010, 0x00000012, 0x0004003D, 0x0000000D, 
0x00000015, 0x00000014, 0x00050041, 0x00000013, 0x00000016, 0x00000010, 0x00000011, 0x0003003E, 0x00000016, 0x00000015, 0x00040008, 0x00000001, 
0x00000075, 0x00000000, 0x00060041, 0x0000001C, 0x0000001D, 0x00000010, 0x00000017, 0x00000012, 0x0003003E, 0x0000001D, 0x0000001B, 0x00040008, 
0x00000001, 0x00000076, 0x00000000, 0x00060041, 0x00000021, 0x00000022, 0x00000010, 0x00000017, 0x00000011, 0x0003003E, 0x00000022, 0x00000020, 
0x00040008, 0x00000001, 0x00000077, 0x00000000, 0x000600A9, 0x0000000B, 0x00000027, 0x00000024, 0x00000025, 0x00000026, 0x00060041, 0x00000028, 
0x00000029, 0x00000010, 0x00000017, 0x00000017, 0x0003003E, 0x00000029, 0x00000027, 0x00040008, 0x00000001, 0x00000078, 0x00000000, 0x00060041, 
0x00000030, 0x00000031, 0x00000010, 0x00000017, 0x0000002A, 0x0004003D, 0x0000000C, 0x00000032, 0x00000031, 0x0009004F, 0x0000000C, 0x00000033, 
0x00000032, 0x0000002F, 0x00000004, 0x00000005, 0x00000006, 0x00000003, 0x0003003E, 0x00000031, 0x00000033, 0x00040008, 0x00000001, 0x00000079, 
0x00000000, 0x00060041, 0x00000036, 0x00000037, 0x00000010, 0x00000017, 0x00000034, 0x0003003E, 0x00000037, 0x00000035, 0x00040008, 0x00000001, 
0x0000007A, 0x00000000, 0x000600A9, 0x0000002B, 0x0000003F, 0x0000003C, 0x0000003D, 0x0000003E, 0x00060041, 0x00000030, 0x00000040, 0x00000010, 
0x00000017, 0x00000038, 0x0004003D, 0x0000000C, 0x00000041, 0x00000040, 0x0009004F, 0x0000000C, 0x00000042, 0x00000041, 0x0000003F, 0x00000004, 
0x00000005, 0x00000006, 0x00000003, 0x0003003E, 0x00000040, 0x00000042, 0x00040008, 0x00000001, 0x0000007B, 0x00000000, 0x00060041, 0x00000045, 
0x00000046, 0x00000010, 0x00000017, 0x00000043, 0x0003003E, 0x00000046, 0x00000044, 0x000100FD, 0x00010038 });
	descr.Compute().StringCL( 
R"#(
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
			
struct BufferAlign_Struct
{
	int4 i4;
	float2 f2;
	int b1;
	float _padding0;
	uint4 u3;
	int i1;
	float _padding1;
	float _padding2;
	float _padding3;
	int4 b3;
	float f1;
	float _padding4;
	float _padding5;
	float _padding6;
};

struct BufferAlign_SSBO
{
	struct BufferAlign_Struct src;
	struct BufferAlign_Struct dst1;
	struct BufferAlign_Struct dst2;
};


//---------------------------------

__constant int4 C_hb75lg1 = (int4)( 1, -2, 3, -4 );
__constant float2 C_kbipui1 = (float2)( 3.0999999, 5.5 );
__constant int C_true = (int)( true );
__constant int C_9 = (int)( 9 );
__constant int C_8 = (int)( 8 );
__constant int C_7 = (int)( 7 );
__constant int C_1193046 = (int)( 1193046 );
__constant int C_false = (int)( false );
__constant float C_1d4335 = (float)( 1.43350005 );

//---------------------------------

kernel void main (
	/*0*/__global write_only struct BufferAlign_SSBO* ssb)
{
	ssb->dst1=ssb->src;
	ssb->dst2.i4=C_hb75lg1;
	ssb->dst2.f2=C_kbipui1;
	ssb->dst2.b1=C_true;
	int4 H_qenhp1;
	H_qenhp1=convert_int4( ssb->dst2.u3 );
	H_qenhp1.xyz=(int3)( 9, 8, 7 );
	ssb->dst2.u3=convert_uint4( H_qenhp1 );
	ssb->dst2.i1=C_1193046;
	int4 H_lsg2a3;
	H_lsg2a3=ssb->dst2.b3;
	H_lsg2a3.xyz=(int3)( false, true, false );
	ssb->dst2.b3=H_lsg2a3;
	ssb->dst2.f1=C_1d4335;
}


)#"_str );
	descr.Compute().FuncSW( &SWShaderLang::sw_bufferaligntest_comp );

};
};
