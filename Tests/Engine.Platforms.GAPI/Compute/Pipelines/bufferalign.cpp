// This is generated file
// Origin file: 'Compute/Pipelines/BufferAlign.ppln'
// Created at: 2018/04/25 - 16:07:56

#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang
{
	#define INOUT
	#define IN
	#define OUT
	
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
	
	static void sw_bufferalign_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::StorageBuffer< BufferAlign_SSBO, Impl::EStorageAccess::Coherent > ssb;	_helper_.GetStorageBuffer( 0, ssb );
	
		// shader
	{
		ssb->dst1 = ssb->src;
		ssb->dst2.i4 = Int4( 1, -2, 3, -4 );
		ssb->dst2.f2 = Float2( 3.0999999f, 5.5f );
		ssb->dst2.b1 = Bool( true );
		ssb->dst2.u3.xyz = UInt3( 9U, 8U, 7U );
		ssb->dst2.i1 = Int( 1193046 );
		ssb->dst2.b3.xyz = Bool3( false, true, false );
		ssb->dst2.f1 = Float( 1.43350005f );
	}
	
	
	}
}	// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_bufferalign (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	descr.supportedShaders = EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddStorageBuffer( "ssb", 288_b, 0_b, EShaderMemoryModel::Default, 0, 0, EShader::Compute )
			.Finish();

	descr.Compute().StringGLSL( 
R"#(#version 450 core
layout (local_size_x=1, local_size_y=1, local_size_z=1) in;

struct BufferAlign_Struct
{
	ivec4 i4;
	vec2 f2;
	bool b1;
	float _padding0;
	uvec4 u3;
	int i1;
	float _padding1;
	float _padding2;
	float _padding3;
	bvec4 b3;
	float f1;
	float _padding4;
	float _padding5;
	float _padding6;
};


//---------------------------------

layout(binding=0) layout(std430) buffer BufferAlign_SSBO{
	readonly BufferAlign_Struct src;
	writeonly BufferAlign_Struct dst1;
	writeonly BufferAlign_Struct dst2;
} ssb;

//---------------------------------

void main()
{
	ssb.dst1 = ssb.src;
	ssb.dst2.i4 = ivec4( 1, -2, 3, -4 );
	ssb.dst2.f2 = vec2( 3.0999999f, 5.5f );
	ssb.dst2.b1 = bool( true );
	ssb.dst2.u3.xyz = uvec3( 9U, 8U, 7U );
	ssb.dst2.i1 = int( 1193046 );
	ssb.dst2.b3.xyz = bvec3( false, true, false );
	ssb.dst2.f1 = float( 1.43350005f );
}


)#"_str );
	descr.Compute().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x00000047, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0005000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x00060010, 0x00000005, 0x00000011, 
0x00000001, 0x00000001, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x002D0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 
0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 
0x63206465, 0x6E65696C, 0x706F2074, 0x6C676E65, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 
0x20766E65, 0x6E65706F, 0x2F0A6C67, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x72746E65, 0x6F702D79, 0x20746E69, 0x6E69616D, 
0x696C230A, 0x3120656E, 0x0000000A, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00070005, 0x0000000D, 0x66667542, 0x6C417265, 0x5F6E6769, 
0x75727453, 0x00007463, 0x00040006, 0x0000000D, 0x00000000, 0x00003469, 0x00040006, 0x0000000D, 0x00000001, 0x00003266, 0x00040006, 0x0000000D, 
0x00000002, 0x00003162, 0x00060006, 0x0000000D, 0x00000003, 0x6461705F, 0x676E6964, 0x00000030, 0x00040006, 0x0000000D, 0x00000004, 0x00003375, 
0x00040006, 0x0000000D, 0x00000005, 0x00003169, 0x00060006, 0x0000000D, 0x00000006, 0x6461705F, 0x676E6964, 0x00000031, 0x00060006, 0x0000000D, 
0x00000007, 0x6461705F, 0x676E6964, 0x00000032, 0x00060006, 0x0000000D, 0x00000008, 0x6461705F, 0x676E6964, 0x00000033, 0x00040006, 0x0000000D, 
0x00000009, 0x00003362, 0x00040006, 0x0000000D, 0x0000000A, 0x00003166, 0x00060006, 0x0000000D, 0x0000000B, 0x6461705F, 0x676E6964, 0x00000034, 
0x00060006, 0x0000000D, 0x0000000C, 0x6461705F, 0x676E6964, 0x00000035, 0x00060006, 0x0000000D, 0x0000000D, 0x6461705F, 0x676E6964, 0x00000036, 
0x00070005, 0x0000000E, 0x66667542, 0x6C417265, 0x5F6E6769, 0x4F425353, 0x00000000, 0x00040006, 0x0000000E, 0x00000000, 0x00637273, 0x00050006, 
0x0000000E, 0x00000001, 0x31747364, 0x00000000, 0x00050006, 0x0000000E, 0x00000002, 0x32747364, 0x00000000, 0x00030005, 0x00000010, 0x00627373, 
0x00050048, 0x0000000D, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000000D, 0x00000001, 0x00000023, 0x00000010, 0x00050048, 0x0000000D, 
0x00000002, 0x00000023, 0x00000018, 0x00050048, 0x0000000D, 0x00000003, 0x00000023, 0x0000001C, 0x00050048, 0x0000000D, 0x00000004, 0x00000023, 
0x00000020, 0x00050048, 0x0000000D, 0x00000005, 0x00000023, 0x00000030, 0x00050048, 0x0000000D, 0x00000006, 0x00000023, 0x00000034, 0x00050048, 
0x0000000D, 0x00000007, 0x00000023, 0x00000038, 0x00050048, 0x0000000D, 0x00000008, 0x00000023, 0x0000003C, 0x00050048, 0x0000000D, 0x00000009, 
0x00000023, 0x00000040, 0x00050048, 0x0000000D, 0x0000000A, 0x00000023, 0x00000050, 0x00050048, 0x0000000D, 0x0000000B, 0x00000023, 0x00000054, 
0x00050048, 0x0000000D, 0x0000000C, 0x00000023, 0x00000058, 0x00050048, 0x0000000D, 0x0000000D, 0x00000023, 0x0000005C, 0x00050048, 0x0000000E, 
0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000000E, 0x00000001, 0x00000023, 0x00000060, 0x00050048, 0x0000000E, 0x00000002, 0x00000023, 
0x000000C0, 0x00030047, 0x0000000E, 0x00000003, 0x00040047, 0x00000010, 0x00000022, 0x00000000, 0x00040047, 0x00000010, 0x00000021, 0x00000000, 
0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00040015, 0x00000007, 0x00000020, 0x00000001, 0x00040017, 0x00000008, 0x00000007, 
0x00000004, 0x00030016, 0x00000009, 0x00000020, 0x00040017, 0x0000000A, 0x00000009, 0x00000002, 0x00040015, 0x0000000B, 0x00000020, 0x00000000, 
0x00040017, 0x0000000C, 0x0000000B, 0x00000004, 0x0010001E, 0x0000000D, 0x00000008, 0x0000000A, 0x0000000B, 0x00000009, 0x0000000C, 0x00000007, 
0x00000009, 0x00000009, 0x00000009, 0x0000000C, 0x00000009, 0x00000009, 0x00000009, 0x00000009, 0x0005001E, 0x0000000E, 0x0000000D, 0x0000000D, 
0x0000000D, 0x00040020, 0x0000000F, 0x00000002, 0x0000000E, 0x0004003B, 0x0000000F, 0x00000010, 0x00000002, 0x0004002B, 0x00000007, 0x00000011, 
0x00000001, 0x0004002B, 0x00000007, 0x00000012, 0x00000000, 0x00040020, 0x00000013, 0x00000002, 0x0000000D, 0x0004002B, 0x00000007, 0x00000017, 
0x00000002, 0x0004002B, 0x00000007, 0x00000018, 0xFFFFFFFE, 0x0004002B, 0x00000007, 0x00000019, 0x00000003, 0x0004002B, 0x00000007, 0x0000001A, 
0xFFFFFFFC, 0x0007002C, 0x00000008, 0x0000001B, 0x00000011, 0x00000018, 0x00000019, 0x0000001A, 0x00040020, 0x0000001C, 0x00000002, 0x00000008, 
0x0004002B, 0x00000009, 0x0000001E, 0x40466666, 0x0004002B, 0x00000009, 0x0000001F, 0x40B00000, 0x0005002C, 0x0000000A, 0x00000020, 0x0000001E, 
0x0000001F, 0x00040020, 0x00000021, 0x00000002, 0x0000000A, 0x00020014, 0x00000023, 0x00030029, 0x00000023, 0x00000024, 0x0004002B, 0x0000000B, 
0x00000025, 0x00000001, 0x0004002B, 0x0000000B, 0x00000026, 0x00000000, 0x00040020, 0x00000028, 0x00000002, 0x0000000B, 0x0004002B, 0x00000007, 
0x0000002A, 0x00000004, 0x00040017, 0x0000002B, 0x0000000B, 0x00000003, 0x0004002B, 0x0000000B, 0x0000002C, 0x00000009, 0x0004002B, 0x0000000B, 
0x0000002D, 0x00000008, 0x0004002B, 0x0000000B, 0x0000002E, 0x00000007, 0x0006002C, 0x0000002B, 0x0000002F, 0x0000002C, 0x0000002D, 0x0000002E, 
0x00040020, 0x00000030, 0x00000002, 0x0000000C, 0x0004002B, 0x00000007, 0x00000034, 0x00000005, 0x0004002B, 0x00000007, 0x00000035, 0x00123456, 
0x00040020, 0x00000036, 0x00000002, 0x00000007, 0x0004002B, 0x00000007, 0x00000038, 0x00000009, 0x00040017, 0x00000039, 0x00000023, 0x00000004, 
0x00040017, 0x0000003A, 0x00000023, 0x00000003, 0x0003002A, 0x00000023, 0x0000003B, 0x0006002C, 0x0000003A, 0x0000003C, 0x0000003B, 0x00000024, 
0x0000003B, 0x0006002C, 0x0000002B, 0x0000003D, 0x00000025, 0x00000025, 0x00000025, 0x0006002C, 0x0000002B, 0x0000003E, 0x00000026, 0x00000026, 
0x00000026, 0x0004002B, 0x00000007, 0x00000043, 0x0000000A, 0x0004002B, 0x00000009, 0x00000044, 0x3FB77CEE, 0x00040020, 0x00000045, 0x00000002, 
0x00000009, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x00040008, 0x00000001, 0x00000023, 0x00000000, 
0x00050041, 0x00000013, 0x00000014, 0x00000010, 0x00000012, 0x0004003D, 0x0000000D, 0x00000015, 0x00000014, 0x00050041, 0x00000013, 0x00000016, 
0x00000010, 0x00000011, 0x0003003E, 0x00000016, 0x00000015, 0x00040008, 0x00000001, 0x00000024, 0x00000000, 0x00060041, 0x0000001C, 0x0000001D, 
0x00000010, 0x00000017, 0x00000012, 0x0003003E, 0x0000001D, 0x0000001B, 0x00040008, 0x00000001, 0x00000025, 0x00000000, 0x00060041, 0x00000021, 
0x00000022, 0x00000010, 0x00000017, 0x00000011, 0x0003003E, 0x00000022, 0x00000020, 0x00040008, 0x00000001, 0x00000026, 0x00000000, 0x000600A9, 
0x0000000B, 0x00000027, 0x00000024, 0x00000025, 0x00000026, 0x00060041, 0x00000028, 0x00000029, 0x00000010, 0x00000017, 0x00000017, 0x0003003E, 
0x00000029, 0x00000027, 0x00040008, 0x00000001, 0x00000027, 0x00000000, 0x00060041, 0x00000030, 0x00000031, 0x00000010, 0x00000017, 0x0000002A, 
0x0004003D, 0x0000000C, 0x00000032, 0x00000031, 0x0009004F, 0x0000000C, 0x00000033, 0x00000032, 0x0000002F, 0x00000004, 0x00000005, 0x00000006, 
0x00000003, 0x0003003E, 0x00000031, 0x00000033, 0x00040008, 0x00000001, 0x00000028, 0x00000000, 0x00060041, 0x00000036, 0x00000037, 0x00000010, 
0x00000017, 0x00000034, 0x0003003E, 0x00000037, 0x00000035, 0x00040008, 0x00000001, 0x00000029, 0x00000000, 0x000600A9, 0x0000002B, 0x0000003F, 
0x0000003C, 0x0000003D, 0x0000003E, 0x00060041, 0x00000030, 0x00000040, 0x00000010, 0x00000017, 0x00000038, 0x0004003D, 0x0000000C, 0x00000041, 
0x00000040, 0x0009004F, 0x0000000C, 0x00000042, 0x00000041, 0x0000003F, 0x00000004, 0x00000005, 0x00000006, 0x00000003, 0x0003003E, 0x00000040, 
0x00000042, 0x00040008, 0x00000001, 0x0000002A, 0x00000000, 0x00060041, 0x00000045, 0x00000046, 0x00000010, 0x00000017, 0x00000043, 0x0003003E, 
0x00000046, 0x00000044, 0x000100FD, 0x00010038 });
	descr.Compute().StringCL( 
R"#(#define FORMAT( _fmt_ )
#define INOUT
#define OUT
// Functions for GLSL compatibility

#define Gen_IntTemplates( _gen_ ) \
	_gen_( int ) \
	_gen_( int2 ) \
	_gen_( int3 ) \
	_gen_( int4 )

#define Gen_UIntTemplates( _gen_ ) \
	_gen_( uint ) \
	_gen_( uint2 ) \
	_gen_( uint3 ) \
	_gen_( uint4 )
	
#define Gen_LongTemplates( _gen_ ) \
	_gen_( long ) \
	_gen_( long2 ) \
	_gen_( long3 ) \
	_gen_( long4 )
	
#define Gen_ULongTemplates( _gen_ ) \
	_gen_( ulong ) \
	_gen_( ulong2 ) \
	_gen_( ulong3 ) \
	_gen_( ulong4 )

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


// findMSB 32
#define GenTemplate_FindMSB32( _type_ ) \
	_type_ findMSB_##_type_ (_type_ x) { \
		return 31 - clz( x ); \
	}
	Gen_IntTemplates( GenTemplate_FindMSB32 )
	Gen_UIntTemplates( GenTemplate_FindMSB32 )
#undef GenTemplate_FindMSB32
	

// findMSB 64
#define GenTemplate_FindMSB64( _type_ ) \
	_type_ findMSB_##_type_ (_type_ x) { \
		return 63 - clz( x ); \
	}
	Gen_LongTemplates( GenTemplate_FindMSB64 )
	Gen_ULongTemplates( GenTemplate_FindMSB64 )
#undef GenTemplate_FindMSB64


// findLSB 32
#define GenTemplate_FindLSB32( _type_ ) \
	_type_ findLSB_##_type_ (_type_ x) { \
		return	(convert_##_type_(!!( x >> 31 )) & convert_##_type_(!( x & convert_##_type_(0x7FFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 30 )) & convert_##_type_(!( x & convert_##_type_(0x3FFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 29 )) & convert_##_type_(!( x & convert_##_type_(0x1FFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 28 )) & convert_##_type_(!( x & convert_##_type_(0xFFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 27 )) & convert_##_type_(!( x & convert_##_type_(0x7FFFFFF) ))) + \
				(convert_##_type_(!!( x >> 26 )) & convert_##_type_(!( x & convert_##_type_(0x3FFFFFF) ))) + \
				(convert_##_type_(!!( x >> 25 )) & convert_##_type_(!( x & convert_##_type_(0x1FFFFFF) ))) + \
				(convert_##_type_(!!( x >> 24 )) & convert_##_type_(!( x & convert_##_type_(0xFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 23 )) & convert_##_type_(!( x & convert_##_type_(0x7FFFFF) ))) + \
				(convert_##_type_(!!( x >> 22 )) & convert_##_type_(!( x & convert_##_type_(0x3FFFFF) ))) + \
				(convert_##_type_(!!( x >> 21 )) & convert_##_type_(!( x & convert_##_type_(0x1FFFFF) ))) + \
				(convert_##_type_(!!( x >> 20 )) & convert_##_type_(!( x & convert_##_type_(0xFFFFF) ))) + \
				(convert_##_type_(!!( x >> 19 )) & convert_##_type_(!( x & convert_##_type_(0x7FFFF) ))) + \
				(convert_##_type_(!!( x >> 18 )) & convert_##_type_(!( x & convert_##_type_(0x3FFFF) ))) + \
				(convert_##_type_(!!( x >> 17 )) & convert_##_type_(!( x & convert_##_type_(0x1FFFF) ))) + \
				(convert_##_type_(!!( x >> 16 )) & convert_##_type_(!( x & convert_##_type_(0xFFFF) ))) + \
				(convert_##_type_(!!( x >> 15 )) & convert_##_type_(!( x & convert_##_type_(0x7FFF) ))) + \
				(convert_##_type_(!!( x >> 14 )) & convert_##_type_(!( x & convert_##_type_(0x3FFF) ))) + \
				(convert_##_type_(!!( x >> 13 )) & convert_##_type_(!( x & convert_##_type_(0x1FFF) ))) + \
				(convert_##_type_(!!( x >> 12 )) & convert_##_type_(!( x & convert_##_type_(0xFFF) ))) + \
				(convert_##_type_(!!( x >> 11 )) & convert_##_type_(!( x & convert_##_type_(0x7FF) ))) + \
				(convert_##_type_(!!( x >> 10 )) & convert_##_type_(!( x & convert_##_type_(0x3FF) ))) + \
				(convert_##_type_(!!( x >>  9 )) & convert_##_type_(!( x & convert_##_type_(0x1FF) ))) + \
				(convert_##_type_(!!( x >>  8 )) & convert_##_type_(!( x & convert_##_type_(0xFF) ))) + \
				(convert_##_type_(!!( x >>  7 )) & convert_##_type_(!( x & convert_##_type_(0x7F) ))) + \
				(convert_##_type_(!!( x >>  6 )) & convert_##_type_(!( x & convert_##_type_(0x3F) ))) + \
				(convert_##_type_(!!( x >>  5 )) & convert_##_type_(!( x & convert_##_type_(0x1F) ))) + \
				(convert_##_type_(!!( x >>  4 )) & convert_##_type_(!( x & convert_##_type_(0xF) ))) + \
				(convert_##_type_(!!( x >>  3 )) & convert_##_type_(!( x & convert_##_type_(0x7) ))) + \
				(convert_##_type_(!!( x >>  2 )) & convert_##_type_(!( x & convert_##_type_(0x3) ))) + \
				(convert_##_type_(!!( x >>  1 )) & convert_##_type_(!( x & convert_##_type_(0x1) ))) + \
				convert_##_type_(!!( x )) - 1; \
	}
	Gen_IntTemplates( GenTemplate_FindLSB32 )
	Gen_UIntTemplates( GenTemplate_FindLSB32 )
#undef GenTemplate_FindLSB32
	
/*
// findLSB 64
#define GenTemplate_FindLSB64( _type_ ) \
	_type_ findLSB_##_type_ (_type_ x) { \
		return 63 - clz( rotate( x, 63 ) ); \
	}
	Gen_LongTemplates( GenTemplate_FindLSB64 )
	Gen_ULongTemplates( GenTemplate_FindLSB64 )
#undef GenTemplate_FindLSB64



// bitfieldReverse 32
#define GenTemplate_BitfieldReverse32( _type_ ) \
	_type_ bitfieldReverse_##_type_ (_type_ x) { \
		return ; \
	}
	Gen_IntTemplates( GenTemplate_BitfieldReverse32 )
	Gen_UIntTemplates( GenTemplate_BitfieldReverse32 )
#undef GenTemplate_BitfieldReverse32
*/

#undef Gen_IntTemplates
#undef Gen_UIntTemplates
#undef Gen_LongTemplates
#undef Gen_ULongTemplates
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

kernel void main (
	/*0*/__global read_write struct BufferAlign_SSBO* ssb)
{

{
	ssb->dst1 = ssb->src;
	ssb->dst2.i4 = ((int4)( 1, -2, 3, -4 ));
	ssb->dst2.f2 = ((float2)( 3.0999999f, 5.5f ));
	ssb->dst2.b1 = ((int)( true ));
	ssb->dst2.u3.xyz = ((uint3)( 9U, 8U, 7U ));
	ssb->dst2.i1 = ((int)( 1193046 ));
	ssb->dst2.b3.xyz = ((int3)( false, true, false ));
	ssb->dst2.f1 = ((float)( 1.43350005f ));
}


}
)#"_str );
#ifdef GRAPHICS_API_SOFT
	descr.Compute().FuncSW( &SWShaderLang::sw_bufferalign_comp );
#endif

};
};
