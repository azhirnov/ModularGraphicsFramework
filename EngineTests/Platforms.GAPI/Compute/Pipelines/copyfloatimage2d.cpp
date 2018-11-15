// This is generated file
// Origin file: 'Compute/Pipelines/CopyFloatImage2D.ppln'
#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang {
namespace {

#	define INOUT
#	define IN
#	define OUT

	static void sw_copyfloatimage2d_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::Image2D< vec4, Impl::EStorageAccess::WriteOnly >  un_DstImage;    _helper_.GetImage( 0, un_DstImage );
		Impl::Image2D< vec4, Impl::EStorageAccess::ReadOnly >  un_SrcImage;    _helper_.GetImage( 1, un_SrcImage );
		auto& gl_GlobalInvocationID = _helper_.GetComputeShaderState().inGlobalInvocationID;
	
		// shader
		{
			Int2 coord = Int3(gl_GlobalInvocationID).xy;
			;
			Float4 color = imageLoad(un_SrcImage, coord);
			;
			imageStore(un_DstImage, coord, color);
		}
	}
	
}		// anonymous namespace
}		// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_copyfloatimage2d (PipelineTemplateDescription& descr)
{
	descr = PipelineTemplateDescription();
	descr.supportedShaders = EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescription::Builder()
			.AddImage( "un_DstImage", EImage::Tex2D, EPixelFormat::RGBA32F, EShaderMemoryModel::WriteOnly, 0u, 0u, EShader::Compute )
			.AddImage( "un_SrcImage", EImage::Tex2D, EPixelFormat::RGBA8_UNorm, EShaderMemoryModel::ReadOnly, 1u, 1u, EShader::Compute )
			.Finish();

	descr.Compute().AddSource( EShaderLangFormat::OpenGL_450 | EShaderLangFormat::HighLevel, 
R"#(#version 450 core


#define SH_VERTEX           (1<<0)
#define SH_TESS_CONTROL     (1<<1)
#define SH_TESS_EVALUATION  (1<<2)
#define SH_GEOMETRY         (1<<3)
#define SH_FRAGMENT         (1<<4)
#define SH_COMPUTE          (1<<5)
		
#define SHADER	SH_COMPUTE

layout(binding=0) layout(rgba32f) writeonly uniform image2D un_DstImage;

layout(binding=1) layout(rgba8) readonly uniform image2D un_SrcImage;


layout (local_size_x=1, local_size_y=1, local_size_z=1) in;

void main ()
{
	ivec2 coord = ivec3( gl_GlobalInvocationID ).xy;
	;
	vec4 color = imageLoad( un_SrcImage, coord );
	;
	imageStore( un_DstImage, coord, color );
}



)#"_str );

	descr.Compute().AddSource( EShaderLangFormat::OpenCL_120 | EShaderLangFormat::HighLevel, 
R"#(#define FORMAT( _fmt_ )
#define INOUT
#define OUT
// Functions for GLSL compatibility

#define Gen_IntTemplates( _gen_ )     _gen_( int )     _gen_( int2 )     _gen_( int3 )     _gen_( int4 )
#define Gen_UIntTemplates( _gen_ )    _gen_( uint )    _gen_( uint2 )    _gen_( uint3 )    _gen_( uint4 )
#define Gen_LongTemplates( _gen_ )    _gen_( long )    _gen_( long2 )    _gen_( long3 )    _gen_( long4 )
#define Gen_ULongTemplates( _gen_ )   _gen_( ulong )   _gen_( ulong2 )   _gen_( ulong3 )   _gen_( ulong4 )
#define Gen_FloatTemplates( _gen_ )   _gen_( float )   _gen_( float2 )   _gen_( float3 )   _gen_( float4 )
#define Gen_DoubleTemplates( _gen_ )  _gen_( double )  _gen_( double2 )  _gen_( double3 )  _gen_( double4 )

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
	/*0*/write_only FORMAT(rgba32f) image2d_t un_DstImage,
	/*1*/read_only FORMAT(rgba8_unorm) image2d_t un_SrcImage)
{

	{
		int2 coord = convert_int3( ((uint3)(get_global_id(0),  get_global_id(1),  get_global_id(2))) ).xy;
		;
		float4 color = read_imagef(un_SrcImage, coord);
		;
		write_imagef(un_DstImage, coord, color);
	}
}

)#"_str );

	descr.Compute().AddSpirv( EShaderLangFormat::Vulkan_100 | EShaderLangFormat::SPIRV, { 
0x07230203, 0x00010000, 0x00080006, 0x00000023, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0006000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000E, 0x00060010, 0x00000005, 
0x00000011, 0x00000001, 0x00000001, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x00410003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 
0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 
0x73736563, 0x61206465, 0x2D6F7475, 0x2D70616D, 0x61636F6C, 0x6E6F6974, 0x2F2F0A73, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x61206465, 
0x2D6F7475, 0x2D70616D, 0x646E6962, 0x73676E69, 0x202F2F0A, 0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6C632064, 0x746E6569, 0x6C757620, 
0x316E616B, 0x2F0A3030, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x67726174, 0x652D7465, 0x7620766E, 0x616B6C75, 0x302E316E, 
0x202F2F0A, 0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6E652064, 0x2D797274, 0x6E696F70, 0x616D2074, 0x230A6E69, 0x656E696C, 0x000A3120, 
0x00090004, 0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00090004, 0x415F4C47, 0x735F4252, 
0x69646168, 0x6C5F676E, 0x75676E61, 0x5F656761, 0x70303234, 0x006B6361, 0x000A0004, 0x475F4C47, 0x4C474F4F, 0x70635F45, 0x74735F70, 0x5F656C79, 
0x656E696C, 0x7269645F, 0x69746365, 0x00006576, 0x00080004, 0x475F4C47, 0x4C474F4F, 0x6E695F45, 0x64756C63, 0x69645F65, 0x74636572, 0x00657669, 
0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00040005, 0x0000000A, 0x726F6F63, 0x00000064, 0x00080005, 0x0000000E, 0x475F6C67, 0x61626F6C, 
0x766E496C, 0x7461636F, 0x496E6F69, 0x00000044, 0x00040005, 0x00000016, 0x6F6C6F63, 0x00000072, 0x00050005, 0x00000019, 0x535F6E75, 0x6D496372, 
0x00656761, 0x00050005, 0x0000001F, 0x445F6E75, 0x6D497473, 0x00656761, 0x00040047, 0x0000000E, 0x0000000B, 0x0000001C, 0x00040047, 0x00000019, 
0x00000022, 0x00000000, 0x00040047, 0x00000019, 0x00000021, 0x00000001, 0x00030047, 0x00000019, 0x00000018, 0x00040047, 0x0000001F, 0x00000022, 
0x00000000, 0x00040047, 0x0000001F, 0x00000021, 0x00000000, 0x00030047, 0x0000001F, 0x00000019, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 
0x00000003, 0x00040015, 0x00000007, 0x00000020, 0x00000001, 0x00040017, 0x00000008, 0x00000007, 0x00000002, 0x00040020, 0x00000009, 0x00000007, 
0x00000008, 0x00040015, 0x0000000B, 0x00000020, 0x00000000, 0x00040017, 0x0000000C, 0x0000000B, 0x00000003, 0x00040020, 0x0000000D, 0x00000001, 
0x0000000C, 0x0004003B, 0x0000000D, 0x0000000E, 0x00000001, 0x00040017, 0x00000010, 0x00000007, 0x00000003, 0x00030016, 0x00000013, 0x00000020, 
0x00040017, 0x00000014, 0x00000013, 0x00000004, 0x00040020, 0x00000015, 0x00000007, 0x00000014, 0x00090019, 0x00000017, 0x00000013, 0x00000001, 
0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000004, 0x00040020, 0x00000018, 0x00000000, 0x00000017, 0x0004003B, 0x00000018, 0x00000019, 
0x00000000, 0x00090019, 0x0000001D, 0x00000013, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000001, 0x00040020, 0x0000001E, 
0x00000000, 0x0000001D, 0x0004003B, 0x0000001E, 0x0000001F, 0x00000000, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 
0x00000006, 0x0004003B, 0x00000009, 0x0000000A, 0x00000007, 0x0004003B, 0x00000015, 0x00000016, 0x00000007, 0x00040008, 0x00000001, 0x00000005, 
0x00000000, 0x0004003D, 0x0000000C, 0x0000000F, 0x0000000E, 0x0004007C, 0x00000010, 0x00000011, 0x0000000F, 0x0007004F, 0x00000008, 0x00000012, 
0x00000011, 0x00000011, 0x00000000, 0x00000001, 0x0003003E, 0x0000000A, 0x00000012, 0x00040008, 0x00000001, 0x00000007, 0x00000000, 0x0004003D, 
0x00000017, 0x0000001A, 0x00000019, 0x0004003D, 0x00000008, 0x0000001B, 0x0000000A, 0x00050062, 0x00000014, 0x0000001C, 0x0000001A, 0x0000001B, 
0x0003003E, 0x00000016, 0x0000001C, 0x00040008, 0x00000001, 0x00000009, 0x00000000, 0x0004003D, 0x0000001D, 0x00000020, 0x0000001F, 0x0004003D, 
0x00000008, 0x00000021, 0x0000000A, 0x0004003D, 0x00000014, 0x00000022, 0x00000016, 0x00040063, 0x00000020, 0x00000021, 0x00000022, 0x000100FD, 
0x00010038 });

#ifdef GRAPHICS_API_SOFT
	descr.Compute().AddInvocable( EShaderLangFormat::Software_100 | EShaderLangFormat::CPP_Invocable, &SWShaderLang::sw_copyfloatimage2d_comp );
#endif


};
}	// Pipelines
