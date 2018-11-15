// This is generated file
// Origin file: 'Compute/Pipelines/BufferAlign.ppln'
#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang {
namespace {

#	define INOUT
#	define IN
#	define OUT

	struct BufferAlign_Struct
	{
		Int4 i4;
		Float2 f2;
		Bool b1;
		Float _padding0;
		UInt3 u3;
		Int i1;
		Bool3 b3;
		Float f1;
	
		BufferAlign_Struct () {}
		BufferAlign_Struct (BufferAlign_Struct &&) = default;
		BufferAlign_Struct (const BufferAlign_Struct &) = default;
		explicit BufferAlign_Struct(const Int4 &i4, const Float2 &f2, const Bool &b1, const Float &_padding0, const UInt3 &u3, const Int &i1, const Bool3 &b3, const Float &f1) : i4{i4}, f2{f2}, b1{b1}, _padding0{_padding0}, u3{u3}, i1{i1}, b3{b3}, f1{f1} {}
	
		BufferAlign_Struct& operator = (BufferAlign_Struct &&) = default;
		BufferAlign_Struct& operator = (const BufferAlign_Struct &) = default;
		Bool operator == (const BufferAlign_Struct &right) const {
			return	all( i4 == right.i4 ) &&
					all( f2 == right.f2 ) &&
					all( b1 == right.b1 ) &&
					all( _padding0 == right._padding0 ) &&
					all( u3 == right.u3 ) &&
					all( i1 == right.i1 ) &&
					all( b3 == right.b3 ) &&
					all( f1 == right.f1 );
		}
		Bool operator != (const BufferAlign_Struct &right) const { return !(*this == right); }
	
	};
	
	struct BufferAlign_SSBO
	{
		BufferAlign_Struct src;
		BufferAlign_Struct dst1;
		BufferAlign_Struct dst2;
	
		BufferAlign_SSBO () {}
		BufferAlign_SSBO (BufferAlign_SSBO &&) = default;
		BufferAlign_SSBO (const BufferAlign_SSBO &) = default;
		explicit BufferAlign_SSBO(const BufferAlign_Struct &src, const BufferAlign_Struct &dst1, const BufferAlign_Struct &dst2) : src{src}, dst1{dst1}, dst2{dst2} {}
	
		BufferAlign_SSBO& operator = (BufferAlign_SSBO &&) = default;
		BufferAlign_SSBO& operator = (const BufferAlign_SSBO &) = default;
		Bool operator == (const BufferAlign_SSBO &right) const {
			return	all( src == right.src ) &&
					all( dst1 == right.dst1 ) &&
					all( dst2 == right.dst2 );
		}
		Bool operator != (const BufferAlign_SSBO &right) const { return !(*this == right); }
	
	};
	
	
	//---------------------------------
	
	static void sw_bufferalign_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::StorageBuffer< BufferAlign_SSBO, Impl::EStorageAccess::Coherent >  ssb;    _helper_.GetStorageBuffer( 0, ssb );
	
		// shader
		{
			ssb->dst1 = ssb->src;
			ssb->dst2.i4 = Int4(1, -2, 3, -4);
			ssb->dst2.f2 = Float2(3.0999999f, 5.5f);
			ssb->dst2.b1 = Bool(true);
			ssb->dst2.u3 = UInt3(9u, 8u, 7u);
			ssb->dst2.i1 = Int(1193046);
			ssb->dst2.b3 = Bool3(false, true, false);
			ssb->dst2.f1 = Float(1.43350005f);
		}
	}
	
}		// anonymous namespace
}		// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_bufferalign (PipelineTemplateDescription& descr)
{
	descr = PipelineTemplateDescription();
	descr.supportedShaders = EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescription::Builder()
			.AddStorageBuffer( "ssb", 192_b, 0_b, EShaderMemoryModel::Default, 0u, 0u, EShader::Compute )
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

struct BufferAlign_Struct
{
	ivec4  i4;  // offset: 0, align: 16
	vec2  f2;  // offset: 16, align: 8
	bool  b1;  // offset: 24, align: 4
	float  _padding0;  // offset: 28, align: 4
	uvec3  u3;  // offset: 32, align: 16
	int  i1;  // offset: 44, align: 4
	bvec3  b3;  // offset: 48, align: 16
	float  f1;  // offset: 60, align: 4
};


layout(binding=0) layout(std430)  buffer BufferAlign_SSBO {
	layout(offset=0, align=16) readonly BufferAlign_Struct  src;
	layout(offset=64, align=16) writeonly BufferAlign_Struct  dst1;
	layout(offset=128, align=16) writeonly BufferAlign_Struct  dst2;

} ssb;


layout (local_size_x=1, local_size_y=1, local_size_z=1) in;


//---------------------------------

void main ()
{
	ssb.dst1 = ssb.src;
	ssb.dst2.i4 = ivec4( 1, -2, 3, -4 );
	ssb.dst2.f2 = vec2( 3.0999999f, 5.5f );
	ssb.dst2.b1 = bool( true );
	ssb.dst2.u3 = uvec3( 9u, 8u, 7u );
	ssb.dst2.i1 = int( 1193046 );
	ssb.dst2.b3 = bvec3( false, true, false );
	ssb.dst2.f1 = float( 1.43350005f );
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
union U_int4_float4 {
	int4  fst;
	float4  sec;
};

union U_uint4_int4 {
	uint4  fst;
	int4  sec;
};

struct BufferAlign_Struct
{
	int4 i4;
	float2 f2;
	int b1;
	float _padding0;
	union U_uint4_int4 u3_i1;
	union U_int4_float4 b3_f1;
};

struct BufferAlign_SSBO
{
	struct BufferAlign_Struct src;
	struct BufferAlign_Struct dst1;
	struct BufferAlign_Struct dst2;
};


//---------------------------------

kernel void Main (
	/*0*/__global  struct BufferAlign_SSBO* ssb)
{

	{
		ssb->dst1 = ssb->src;
		ssb->dst2.i4 = ((int4)( 1, -2, 3, -4 ));
		ssb->dst2.f2 = ((float2)( 3.0999999f, 5.5f ));
		ssb->dst2.b1 = ((int)( true ));
		ssb->dst2.u3_i1.fst.xyz = ((uint3)( 9u, 8u, 7u ));
		ssb->dst2.u3_i1.sec.w = ((int)( 1193046 ));
		ssb->dst2.b3_f1.fst.xyz = ((int3)( false, true, false ));
		ssb->dst2.b3_f1.sec.w = ((float)( 1.43350005f ));
	}
}

)#"_str );

	descr.Compute().AddSpirv( EShaderLangFormat::Vulkan_100 | EShaderLangFormat::SPIRV, { 
0x07230203, 0x00010000, 0x00080006, 0x00000041, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0005000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x00060010, 0x00000005, 0x00000011, 
0x00000001, 0x00000001, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x00410003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 
0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 
0x61206465, 0x2D6F7475, 0x2D70616D, 0x61636F6C, 0x6E6F6974, 0x2F2F0A73, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x61206465, 0x2D6F7475, 
0x2D70616D, 0x646E6962, 0x73676E69, 0x202F2F0A, 0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6C632064, 0x746E6569, 0x6C757620, 0x316E616B, 
0x2F0A3030, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x67726174, 0x652D7465, 0x7620766E, 0x616B6C75, 0x302E316E, 0x202F2F0A, 
0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6E652064, 0x2D797274, 0x6E696F70, 0x616D2074, 0x230A6E69, 0x656E696C, 0x000A3120, 0x00090004, 
0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00090004, 0x415F4C47, 0x735F4252, 0x69646168, 
0x6C5F676E, 0x75676E61, 0x5F656761, 0x70303234, 0x006B6361, 0x000A0004, 0x475F4C47, 0x4C474F4F, 0x70635F45, 0x74735F70, 0x5F656C79, 0x656E696C, 
0x7269645F, 0x69746365, 0x00006576, 0x00080004, 0x475F4C47, 0x4C474F4F, 0x6E695F45, 0x64756C63, 0x69645F65, 0x74636572, 0x00657669, 0x00040005, 
0x00000005, 0x6E69616D, 0x00000000, 0x00070005, 0x0000000D, 0x66667542, 0x6C417265, 0x5F6E6769, 0x75727453, 0x00007463, 0x00040006, 0x0000000D, 
0x00000000, 0x00003469, 0x00040006, 0x0000000D, 0x00000001, 0x00003266, 0x00040006, 0x0000000D, 0x00000002, 0x00003162, 0x00060006, 0x0000000D, 
0x00000003, 0x6461705F, 0x676E6964, 0x00000030, 0x00040006, 0x0000000D, 0x00000004, 0x00003375, 0x00040006, 0x0000000D, 0x00000005, 0x00003169, 
0x00040006, 0x0000000D, 0x00000006, 0x00003362, 0x00040006, 0x0000000D, 0x00000007, 0x00003166, 0x00070005, 0x0000000E, 0x66667542, 0x6C417265, 
0x5F6E6769, 0x4F425353, 0x00000000, 0x00040006, 0x0000000E, 0x00000000, 0x00637273, 0x00050006, 0x0000000E, 0x00000001, 0x31747364, 0x00000000, 
0x00050006, 0x0000000E, 0x00000002, 0x32747364, 0x00000000, 0x00030005, 0x00000010, 0x00627373, 0x00050048, 0x0000000D, 0x00000000, 0x00000023, 
0x00000000, 0x00050048, 0x0000000D, 0x00000001, 0x00000023, 0x00000010, 0x00050048, 0x0000000D, 0x00000002, 0x00000023, 0x00000018, 0x00050048, 
0x0000000D, 0x00000003, 0x00000023, 0x0000001C, 0x00050048, 0x0000000D, 0x00000004, 0x00000023, 0x00000020, 0x00050048, 0x0000000D, 0x00000005, 
0x00000023, 0x0000002C, 0x00050048, 0x0000000D, 0x00000006, 0x00000023, 0x00000030, 0x00050048, 0x0000000D, 0x00000007, 0x00000023, 0x0000003C, 
0x00040048, 0x0000000E, 0x00000000, 0x00000018, 0x00050048, 0x0000000E, 0x00000000, 0x00000023, 0x00000000, 0x00040048, 0x0000000E, 0x00000001, 
0x00000019, 0x00050048, 0x0000000E, 0x00000001, 0x00000023, 0x00000040, 0x00040048, 0x0000000E, 0x00000002, 0x00000019, 0x00050048, 0x0000000E, 
0x00000002, 0x00000023, 0x00000080, 0x00030047, 0x0000000E, 0x00000003, 0x00040047, 0x00000010, 0x00000022, 0x00000000, 0x00040047, 0x00000010, 
0x00000021, 0x00000000, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00040015, 0x00000007, 0x00000020, 0x00000001, 0x00040017, 
0x00000008, 0x00000007, 0x00000004, 0x00030016, 0x00000009, 0x00000020, 0x00040017, 0x0000000A, 0x00000009, 0x00000002, 0x00040015, 0x0000000B, 
0x00000020, 0x00000000, 0x00040017, 0x0000000C, 0x0000000B, 0x00000003, 0x000A001E, 0x0000000D, 0x00000008, 0x0000000A, 0x0000000B, 0x00000009, 
0x0000000C, 0x00000007, 0x0000000C, 0x00000009, 0x0005001E, 0x0000000E, 0x0000000D, 0x0000000D, 0x0000000D, 0x00040020, 0x0000000F, 0x00000002, 
0x0000000E, 0x0004003B, 0x0000000F, 0x00000010, 0x00000002, 0x0004002B, 0x00000007, 0x00000011, 0x00000001, 0x0004002B, 0x00000007, 0x00000012, 
0x00000000, 0x00040020, 0x00000013, 0x00000002, 0x0000000D, 0x0004002B, 0x00000007, 0x00000017, 0x00000002, 0x0004002B, 0x00000007, 0x00000018, 
0xFFFFFFFE, 0x0004002B, 0x00000007, 0x00000019, 0x00000003, 0x0004002B, 0x00000007, 0x0000001A, 0xFFFFFFFC, 0x0007002C, 0x00000008, 0x0000001B, 
0x00000011, 0x00000018, 0x00000019, 0x0000001A, 0x00040020, 0x0000001C, 0x00000002, 0x00000008, 0x0004002B, 0x00000009, 0x0000001E, 0x40466666, 
0x0004002B, 0x00000009, 0x0000001F, 0x40B00000, 0x0005002C, 0x0000000A, 0x00000020, 0x0000001E, 0x0000001F, 0x00040020, 0x00000021, 0x00000002, 
0x0000000A, 0x00020014, 0x00000023, 0x00030029, 0x00000023, 0x00000024, 0x0004002B, 0x0000000B, 0x00000025, 0x00000001, 0x0004002B, 0x0000000B, 
0x00000026, 0x00000000, 0x00040020, 0x00000028, 0x00000002, 0x0000000B, 0x0004002B, 0x00000007, 0x0000002A, 0x00000004, 0x0004002B, 0x0000000B, 
0x0000002B, 0x00000009, 0x0004002B, 0x0000000B, 0x0000002C, 0x00000008, 0x0004002B, 0x0000000B, 0x0000002D, 0x00000007, 0x0006002C, 0x0000000C, 
0x0000002E, 0x0000002B, 0x0000002C, 0x0000002D, 0x00040020, 0x0000002F, 0x00000002, 0x0000000C, 0x0004002B, 0x00000007, 0x00000031, 0x00000005, 
0x0004002B, 0x00000007, 0x00000032, 0x00123456, 0x00040020, 0x00000033, 0x00000002, 0x00000007, 0x0004002B, 0x00000007, 0x00000035, 0x00000006, 
0x00040017, 0x00000036, 0x00000023, 0x00000003, 0x0003002A, 0x00000023, 0x00000037, 0x0006002C, 0x00000036, 0x00000038, 0x00000037, 0x00000024, 
0x00000037, 0x0006002C, 0x0000000C, 0x00000039, 0x00000025, 0x00000025, 0x00000025, 0x0006002C, 0x0000000C, 0x0000003A, 0x00000026, 0x00000026, 
0x00000026, 0x0004002B, 0x00000007, 0x0000003D, 0x00000007, 0x0004002B, 0x00000009, 0x0000003E, 0x3FB77CEE, 0x00040020, 0x0000003F, 0x00000002, 
0x00000009, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x00040008, 0x00000001, 0x00000008, 0x00000000, 
0x00050041, 0x00000013, 0x00000014, 0x00000010, 0x00000012, 0x0004003D, 0x0000000D, 0x00000015, 0x00000014, 0x00050041, 0x00000013, 0x00000016, 
0x00000010, 0x00000011, 0x0003003E, 0x00000016, 0x00000015, 0x00040008, 0x00000001, 0x00000009, 0x00000000, 0x00060041, 0x0000001C, 0x0000001D, 
0x00000010, 0x00000017, 0x00000012, 0x0003003E, 0x0000001D, 0x0000001B, 0x00040008, 0x00000001, 0x0000000A, 0x00000000, 0x00060041, 0x00000021, 
0x00000022, 0x00000010, 0x00000017, 0x00000011, 0x0003003E, 0x00000022, 0x00000020, 0x00040008, 0x00000001, 0x0000000B, 0x00000000, 0x000600A9, 
0x0000000B, 0x00000027, 0x00000024, 0x00000025, 0x00000026, 0x00060041, 0x00000028, 0x00000029, 0x00000010, 0x00000017, 0x00000017, 0x0003003E, 
0x00000029, 0x00000027, 0x00040008, 0x00000001, 0x0000000C, 0x00000000, 0x00060041, 0x0000002F, 0x00000030, 0x00000010, 0x00000017, 0x0000002A, 
0x0003003E, 0x00000030, 0x0000002E, 0x00040008, 0x00000001, 0x0000000D, 0x00000000, 0x00060041, 0x00000033, 0x00000034, 0x00000010, 0x00000017, 
0x00000031, 0x0003003E, 0x00000034, 0x00000032, 0x00040008, 0x00000001, 0x0000000E, 0x00000000, 0x000600A9, 0x0000000C, 0x0000003B, 0x00000038, 
0x00000039, 0x0000003A, 0x00060041, 0x0000002F, 0x0000003C, 0x00000010, 0x00000017, 0x00000035, 0x0003003E, 0x0000003C, 0x0000003B, 0x00040008, 
0x00000001, 0x0000000F, 0x00000000, 0x00060041, 0x0000003F, 0x00000040, 0x00000010, 0x00000017, 0x0000003D, 0x0003003E, 0x00000040, 0x0000003E, 
0x000100FD, 0x00010038 });

#ifdef GRAPHICS_API_SOFT
	descr.Compute().AddInvocable( EShaderLangFormat::Software_100 | EShaderLangFormat::CPP_Invocable, &SWShaderLang::sw_bufferalign_comp );
#endif


};
}	// Pipelines
