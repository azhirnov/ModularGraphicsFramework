// This is generated file
// Origin file: 'Compute/Pipelines/Image2DBilinearFilter.ppln'
#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang {
namespace {

#	define INOUT
#	define IN
#	define OUT

	static void sw_image2dbilinearfilter_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::Texture2D< vec4 >  un_SrcTexture;    _helper_.GetTexture( 0, un_SrcTexture );
		Impl::Image2D< vec4, Impl::EStorageAccess::WriteOnly >  un_DstImage;    _helper_.GetImage( 1, un_DstImage );
		auto& gl_GlobalInvocationID = _helper_.GetComputeShaderState().inGlobalInvocationID;
		auto& gl_NumWorkGroups = _helper_.GetComputeShaderState().inNumWorkGroups;
	
		// shader
		{
			Int2 coord = Int3(gl_GlobalInvocationID).xy;
			;
			Int2 size = Int3((gl_NumWorkGroups * UInt3(1u))).xy;
			;
			Float2 texc = (Float2(coord) / Float2((size - Int(1))));
			;
			Float4 color = texture(un_SrcTexture, texc);
			;
			imageStore(un_DstImage, coord, color);
		}
	}
	
}		// anonymous namespace
}		// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_image2dbilinearfilter (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	descr.supportedShaders = EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddTexture( "un_SrcTexture", EImage::Tex2D, EPixelFormatClass::AnyColorChannels | EPixelFormatClass::LinearColorSpace | EPixelFormatClass::AnyFloat | EPixelFormatClass::AnyNorm, 0u, 0u, EShader::Compute )
			.AddImage( "un_DstImage", EImage::Tex2D, EPixelFormat::RGBA8_UNorm, EShaderMemoryModel::WriteOnly, 0u, 1u, EShader::Compute )
			.Finish();

	descr.Compute().StringGLSL( 
R"#(#version 450 core
layout (local_size_x=1, local_size_y=1, local_size_z=1) in;

layout(binding=0) uniform sampler2D un_SrcTexture;
layout(binding=0) layout(rgba8) writeonly uniform image2D un_DstImage;

//---------------------------------

void main ()
{
	ivec2 coord = ivec3( gl_GlobalInvocationID ).xy;
	;
	ivec2 size = ivec3( (gl_NumWorkGroups * uvec3( 1u )) ).xy;
	;
	vec2 texc = (vec2( coord ) / vec2( (size - int( 1 )) ));
	;
	vec4 color = texture( un_SrcTexture, texc );
	;
	imageStore( un_DstImage, coord, color );
}


)#"_str );
	descr.Compute().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080006, 0x00000038, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000E, 0x00000014, 0x00060010, 
0x00000005, 0x00000011, 0x00000001, 0x00000001, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x002D0003, 0x00000002, 0x000001C2, 0x00000001, 
0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 
0x6F725065, 0x73736563, 0x63206465, 0x6E65696C, 0x706F2074, 0x6C676E65, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 
0x72617420, 0x2D746567, 0x20766E65, 0x6E65706F, 0x2F0A6C67, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x72746E65, 0x6F702D79, 
0x20746E69, 0x6E69616D, 0x696C230A, 0x3120656E, 0x0000000A, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00040005, 0x0000000A, 0x726F6F63, 
0x00000064, 0x00080005, 0x0000000E, 0x475F6C67, 0x61626F6C, 0x766E496C, 0x7461636F, 0x496E6F69, 0x00000044, 0x00040005, 0x00000013, 0x657A6973, 
0x00000000, 0x00070005, 0x00000014, 0x4E5F6C67, 0x6F576D75, 0x72476B72, 0x7370756F, 0x00000000, 0x00040005, 0x0000001E, 0x63786574, 0x00000000, 
0x00040005, 0x00000029, 0x6F6C6F63, 0x00000072, 0x00060005, 0x0000002D, 0x535F6E75, 0x65546372, 0x72757478, 0x00000065, 0x00050005, 0x00000034, 
0x445F6E75, 0x6D497473, 0x00656761, 0x00040047, 0x0000000E, 0x0000000B, 0x0000001C, 0x00040047, 0x00000014, 0x0000000B, 0x00000018, 0x00040047, 
0x0000002D, 0x00000022, 0x00000000, 0x00040047, 0x0000002D, 0x00000021, 0x00000000, 0x00040047, 0x00000034, 0x00000022, 0x00000000, 0x00040047, 
0x00000034, 0x00000021, 0x00000001, 0x00030047, 0x00000034, 0x00000019, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00040015, 
0x00000007, 0x00000020, 0x00000001, 0x00040017, 0x00000008, 0x00000007, 0x00000002, 0x00040020, 0x00000009, 0x00000007, 0x00000008, 0x00040015, 
0x0000000B, 0x00000020, 0x00000000, 0x00040017, 0x0000000C, 0x0000000B, 0x00000003, 0x00040020, 0x0000000D, 0x00000001, 0x0000000C, 0x0004003B, 
0x0000000D, 0x0000000E, 0x00000001, 0x00040017, 0x00000010, 0x00000007, 0x00000003, 0x0004003B, 0x0000000D, 0x00000014, 0x00000001, 0x0004002B, 
0x0000000B, 0x00000016, 0x00000001, 0x0006002C, 0x0000000C, 0x00000017, 0x00000016, 0x00000016, 0x00000016, 0x00030016, 0x0000001B, 0x00000020, 
0x00040017, 0x0000001C, 0x0000001B, 0x00000002, 0x00040020, 0x0000001D, 0x00000007, 0x0000001C, 0x0004002B, 0x00000007, 0x00000022, 0x00000001, 
0x00040017, 0x00000027, 0x0000001B, 0x00000004, 0x00040020, 0x00000028, 0x00000007, 0x00000027, 0x00090019, 0x0000002A, 0x0000001B, 0x00000001, 
0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x0003001B, 0x0000002B, 0x0000002A, 0x00040020, 0x0000002C, 0x00000000, 0x0000002B, 
0x0004003B, 0x0000002C, 0x0000002D, 0x00000000, 0x0004002B, 0x0000001B, 0x00000030, 0x00000000, 0x00090019, 0x00000032, 0x0000001B, 0x00000001, 
0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000004, 0x00040020, 0x00000033, 0x00000000, 0x00000032, 0x0004003B, 0x00000033, 0x00000034, 
0x00000000, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x0004003B, 0x00000009, 0x0000000A, 0x00000007, 
0x0004003B, 0x00000009, 0x00000013, 0x00000007, 0x0004003B, 0x0000001D, 0x0000001E, 0x00000007, 0x0004003B, 0x00000028, 0x00000029, 0x00000007, 
0x00040008, 0x00000001, 0x0000000B, 0x00000000, 0x0004003D, 0x0000000C, 0x0000000F, 0x0000000E, 0x0004007C, 0x00000010, 0x00000011, 0x0000000F, 
0x0007004F, 0x00000008, 0x00000012, 0x00000011, 0x00000011, 0x00000000, 0x00000001, 0x0003003E, 0x0000000A, 0x00000012, 0x00040008, 0x00000001, 
0x0000000D, 0x00000000, 0x0004003D, 0x0000000C, 0x00000015, 0x00000014, 0x00050084, 0x0000000C, 0x00000018, 0x00000015, 0x00000017, 0x0004007C, 
0x00000010, 0x00000019, 0x00000018, 0x0007004F, 0x00000008, 0x0000001A, 0x00000019, 0x00000019, 0x00000000, 0x00000001, 0x0003003E, 0x00000013, 
0x0000001A, 0x00040008, 0x00000001, 0x0000000F, 0x00000000, 0x0004003D, 0x00000008, 0x0000001F, 0x0000000A, 0x0004006F, 0x0000001C, 0x00000020, 
0x0000001F, 0x0004003D, 0x00000008, 0x00000021, 0x00000013, 0x00050050, 0x00000008, 0x00000023, 0x00000022, 0x00000022, 0x00050082, 0x00000008, 
0x00000024, 0x00000021, 0x00000023, 0x0004006F, 0x0000001C, 0x00000025, 0x00000024, 0x00050088, 0x0000001C, 0x00000026, 0x00000020, 0x00000025, 
0x0003003E, 0x0000001E, 0x00000026, 0x00040008, 0x00000001, 0x00000011, 0x00000000, 0x0004003D, 0x0000002B, 0x0000002E, 0x0000002D, 0x0004003D, 
0x0000001C, 0x0000002F, 0x0000001E, 0x00070058, 0x00000027, 0x00000031, 0x0000002E, 0x0000002F, 0x00000002, 0x00000030, 0x0003003E, 0x00000029, 
0x00000031, 0x00040008, 0x00000001, 0x00000013, 0x00000000, 0x0004003D, 0x00000032, 0x00000035, 0x00000034, 0x0004003D, 0x00000008, 0x00000036, 
0x0000000A, 0x0004003D, 0x00000027, 0x00000037, 0x00000029, 0x00040063, 0x00000035, 0x00000036, 0x00000037, 0x000100FD, 0x00010038 });
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
		return	(convert_##_type_(!!( x >> 31 )) & convert_##_type_(!( x & (_type_)(0x7FFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 30 )) & convert_##_type_(!( x & (_type_)(0x3FFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 29 )) & convert_##_type_(!( x & (_type_)(0x1FFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 28 )) & convert_##_type_(!( x & (_type_)(0xFFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 27 )) & convert_##_type_(!( x & (_type_)(0x7FFFFFF) ))) + \
				(convert_##_type_(!!( x >> 26 )) & convert_##_type_(!( x & (_type_)(0x3FFFFFF) ))) + \
				(convert_##_type_(!!( x >> 25 )) & convert_##_type_(!( x & (_type_)(0x1FFFFFF) ))) + \
				(convert_##_type_(!!( x >> 24 )) & convert_##_type_(!( x & (_type_)(0xFFFFFF) ))) + \
				(convert_##_type_(!!( x >> 23 )) & convert_##_type_(!( x & (_type_)(0x7FFFFF) ))) + \
				(convert_##_type_(!!( x >> 22 )) & convert_##_type_(!( x & (_type_)(0x3FFFFF) ))) + \
				(convert_##_type_(!!( x >> 21 )) & convert_##_type_(!( x & (_type_)(0x1FFFFF) ))) + \
				(convert_##_type_(!!( x >> 20 )) & convert_##_type_(!( x & (_type_)(0xFFFFF) ))) + \
				(convert_##_type_(!!( x >> 19 )) & convert_##_type_(!( x & (_type_)(0x7FFFF) ))) + \
				(convert_##_type_(!!( x >> 18 )) & convert_##_type_(!( x & (_type_)(0x3FFFF) ))) + \
				(convert_##_type_(!!( x >> 17 )) & convert_##_type_(!( x & (_type_)(0x1FFFF) ))) + \
				(convert_##_type_(!!( x >> 16 )) & convert_##_type_(!( x & (_type_)(0xFFFF) ))) + \
				(convert_##_type_(!!( x >> 15 )) & convert_##_type_(!( x & (_type_)(0x7FFF) ))) + \
				(convert_##_type_(!!( x >> 14 )) & convert_##_type_(!( x & (_type_)(0x3FFF) ))) + \
				(convert_##_type_(!!( x >> 13 )) & convert_##_type_(!( x & (_type_)(0x1FFF) ))) + \
				(convert_##_type_(!!( x >> 12 )) & convert_##_type_(!( x & (_type_)(0xFFF) ))) + \
				(convert_##_type_(!!( x >> 11 )) & convert_##_type_(!( x & (_type_)(0x7FF) ))) + \
				(convert_##_type_(!!( x >> 10 )) & convert_##_type_(!( x & (_type_)(0x3FF) ))) + \
				(convert_##_type_(!!( x >>  9 )) & convert_##_type_(!( x & (_type_)(0x1FF) ))) + \
				(convert_##_type_(!!( x >>  8 )) & convert_##_type_(!( x & (_type_)(0xFF) ))) + \
				(convert_##_type_(!!( x >>  7 )) & convert_##_type_(!( x & (_type_)(0x7F) ))) + \
				(convert_##_type_(!!( x >>  6 )) & convert_##_type_(!( x & (_type_)(0x3F) ))) + \
				(convert_##_type_(!!( x >>  5 )) & convert_##_type_(!( x & (_type_)(0x1F) ))) + \
				(convert_##_type_(!!( x >>  4 )) & convert_##_type_(!( x & (_type_)(0xF) ))) + \
				(convert_##_type_(!!( x >>  3 )) & convert_##_type_(!( x & (_type_)(0x7) ))) + \
				(convert_##_type_(!!( x >>  2 )) & convert_##_type_(!( x & (_type_)(0x3) ))) + \
				(convert_##_type_(!!( x >>  1 )) & convert_##_type_(!( x & (_type_)(0x1) ))) + \
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

kernel void Main (
	/*0*/read_only image2d_t un_SrcTexture,
	/*1*/write_only FORMAT(rgba8_unorm) image2d_t un_DstImage,
	/*2*/sampler_t un_SrcTexture_sampler)
{

	{
		int2 coord = convert_int3( ((uint3)(get_global_id(0),  get_global_id(1),  get_global_id(2))) ).xy;
		;
		int2 size = convert_int3( (((uint3)(get_num_groups(0), get_num_groups(1), get_num_groups(2))) * ((uint3)( 1u ))) ).xy;
		;
		float2 texc = (convert_float2( coord ) / convert_float2( (size - ((int)( 1 ))) ));
		;
		float4 color = read_imagef(un_SrcTexture, un_SrcTexture_sampler, texc);
		;
		write_imagef(un_DstImage, coord, color);
	}
}

)#"_str );
	descr.Compute().StringHLSL( 
R"#(cbuffer ComputeBuiltins : register(b0)
{
	uint3		dx_NumWorkGroups;
};

Texture2D<float4> un_SrcTexture : register(t0);
SamplerState un_SrcTextureSampler : register(s0);
globallycoherent RWTexture2D<float4> un_DstImage : register(u1);

//---------------------------------

[numthreads(1, 1, 1)]
void main (uint3 dx_DispatchThreadID : SV_DispatchThreadID, uint3 dx_GroupThreadID : SV_GroupThreadID, uint3 dx_GroupID : SV_GroupID)
{
	int2 coord = int3( dx_DispatchThreadID ).xy;
	;
	int2 size = int3( (dx_NumWorkGroups * uint3( 1u, 1u, 1u )) ).xy;
	;
	float2 texc = (float2( coord ) / float2( (size - (int( 1 )).xx) ));
	;
	float4 color = un_SrcTexture.SampleLevel(un_SrcTextureSampler, texc, 0.0f);
	;
	(un_DstImage)[coord] = color;
}


)#"_str );
#ifdef GRAPHICS_API_SOFT
	descr.Compute().FuncSW( &SWShaderLang::sw_image2dbilinearfilter_comp );
#endif

};
};
