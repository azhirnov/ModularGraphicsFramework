// This is generated file
// Origin file: 'Compute/Pipelines/DynamicBuffer.ppln'
#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang {
namespace {

#	define INOUT
#	define IN
#	define OUT

	struct DynamicBuffer_Struct
	{
		Int2 i2;
		Bool b1;
		Float _padding0;
		Float2 f2;
		Float2 _padding1;
		Int4 i3;
		Bool2 b2;
		Float2 _padding2;
	
		DynamicBuffer_Struct () {}
		DynamicBuffer_Struct (DynamicBuffer_Struct &&) = default;
		DynamicBuffer_Struct (const DynamicBuffer_Struct &) = default;
		explicit DynamicBuffer_Struct(const Int2 &i2, const Bool &b1, const Float &_padding0, const Float2 &f2, const Float2 &_padding1, const Int4 &i3, const Bool2 &b2, const Float2 &_padding2) : i2{i2}, b1{b1}, _padding0{_padding0}, f2{f2}, _padding1{_padding1}, i3{i3}, b2{b2}, _padding2{_padding2} {}
	
		DynamicBuffer_Struct& operator = (DynamicBuffer_Struct &&) = default;
		DynamicBuffer_Struct& operator = (const DynamicBuffer_Struct &) = default;
		Bool operator == (const DynamicBuffer_Struct &right) const {
			return	all( i2 == right.i2 ) &&
					all( b1 == right.b1 ) &&
					all( _padding0 == right._padding0 ) &&
					all( f2 == right.f2 ) &&
					all( _padding1 == right._padding1 ) &&
					all( i3 == right.i3 ) &&
					all( b2 == right.b2 ) &&
					all( _padding2 == right._padding2 );
		}
		Bool operator != (const DynamicBuffer_Struct &right) const { return !(*this == right); }
	
	};
	
	struct DynamicBuffer_SSBO
	{
		Float2 f2;
		Float2 _padding0;
		Int4 i4;
		DynamicBuffer_Struct arr[];
	};
	
	
	//---------------------------------
	
	static void sw_dynamicbuffer_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::StorageBuffer< DynamicBuffer_SSBO, Impl::EStorageAccess::Coherent >  ssb;    _helper_.GetStorageBuffer( 0, ssb );
		auto& gl_GlobalInvocationID = _helper_.GetComputeShaderState().inGlobalInvocationID;
	
		// shader
		{
			const Float2 xD_f2 = ssb->f2;
			;
			const Int4 xD_i4 = ssb->i4;
			;
			Bool xD_exit = Bool(false);
			;
			do {
				Int xD_idx = Int(gl_GlobalInvocationID.x);
				;
				((ssb->arr[xD_idx]).i2 += Int2(xD_i4.x, xD_idx));
				(ssb->arr[xD_idx]).b1 = ((xD_idx & Int(1)) == Int(0));
				((ssb->arr[xD_idx]).f2 -= Float2(xD_f2));
				((ssb->arr[xD_idx]).i3.xy *= Int(2));
				(ssb->arr[xD_idx]).i3.z = ~((ssb->arr[xD_idx]).i3.z);
				(ssb->arr[xD_idx]).b2 = not((ssb->arr[xD_idx]).b2);
			} while ( Bool(false));
			;
		}
	}
	
}		// anonymous namespace
}		// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_dynamicbuffer (PipelineTemplateDescription& descr)
{
	descr = PipelineTemplateDescription();
	descr.supportedShaders = EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescription::Builder()
			.AddStorageBuffer( "ssb", 32_b, 64_b, EShaderMemoryModel::Coherent, 0u, 0u, EShader::Compute )
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

struct DynamicBuffer_Struct
{
	ivec2  i2;  // offset: 0, align: 8
	bool  b1;  // offset: 8, align: 4
	float  _padding0;  // offset: 12, align: 4
	vec2  f2;  // offset: 16, align: 8
	vec2  _padding1;  // offset: 24, align: 8
	ivec4  i3;  // offset: 32, align: 16
	bvec2  b2;  // offset: 48, align: 8
	vec2  _padding2;  // offset: 56, align: 8
};


layout(binding=0) layout(std430) coherent buffer DynamicBuffer_SSBO {
	layout(offset=0, align=8) vec2  f2;
	layout(offset=8, align=8) vec2  _padding0;
	layout(offset=16, align=16) ivec4  i4;
	layout(offset=32, align=16) DynamicBuffer_Struct  arr [];

} ssb;


layout (local_size_x=1, local_size_y=1, local_size_z=1) in;


//---------------------------------

// void SomeFunc (in DynamicBuffer_Struct arr[], const vec2 f2, const ivec4 i4);	// will be inlined

//---------------------------------

void main ()
{
	
	// SomeFunc(struct-DynamicBuffer_Struct-vi2-b1-vf2-vi3-vb21[0];vf2;vi4;
	const vec2 xD_f2 = ssb.f2;
	const ivec4 xD_i4 = ssb.i4;
	bool xD_exit = bool( false );
	do {
		int xD_idx = int( gl_GlobalInvocationID.x );
		;
		((ssb.arr[xD_idx]).i2 += ivec2( xD_i4.x, xD_idx ));
		(ssb.arr[xD_idx]).b1 = ((xD_idx & int( 1 )) == int( 0 ));
		((ssb.arr[xD_idx]).f2 -= vec2( xD_f2 ));
		((ssb.arr[xD_idx]).i3.xy *= int( 2 ));
		(ssb.arr[xD_idx]).i3.z = ~( (ssb.arr[xD_idx]).i3.z );
		(ssb.arr[xD_idx]).b2 = not( (ssb.arr[xD_idx]).b2 );
	
	} while(false);
	// end SomeFunc(struct-DynamicBuffer_Struct-vi2-b1-vf2-vi3-vb21[0];vf2;vi4;;
	 ;
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
struct DynamicBuffer_Struct
{
	int2 i2;
	int b1;
	float _padding0;
	float2 f2;
	float2 _padding1;
	int4 i3;
	int2 b2;
	float2 _padding2;
};

struct DynamicBuffer_SSBO
{
	float2 f2;
	float2 _padding0;
	int4 i4;
	struct DynamicBuffer_Struct arr [];
};


//---------------------------------

kernel void Main (
	/*0*/__global  struct DynamicBuffer_SSBO* ssb)
{

	{
		const float2 xD_f2 = ssb->f2;
		;
		const int4 xD_i4 = ssb->i4;
		;
		int xD_exit = ((int)( false ));
		;
		do {
			int xD_idx = convert_int( ((uint3)(get_global_id(0),  get_global_id(1),  get_global_id(2))).x );
			;
			((ssb->arr[xD_idx]).i2 += ((int2)( xD_i4.x, xD_idx )));
			(ssb->arr[xD_idx]).b1 = ((xD_idx & ((int)( 1 ))) == ((int)( 0 )));
			((ssb->arr[xD_idx]).f2 -= ((float2)( xD_f2 )));
			((ssb->arr[xD_idx]).i3.xy *= ((int)( 2 )));
			(ssb->arr[xD_idx]).i3.z = ~( (ssb->arr[xD_idx]).i3.z );
			(ssb->arr[xD_idx]).b2 = !( (ssb->arr[xD_idx]).b2 );
		} while ( ((int)( false )));
		;
	}
}

)#"_str );

	descr.Compute().AddSpirv( EShaderLangFormat::Vulkan_100 | EShaderLangFormat::SPIRV, { 
0x07230203, 0x00010000, 0x00080006, 0x0000006D, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0006000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x0000002B, 0x00060010, 0x00000005, 
0x00000011, 0x00000001, 0x00000001, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x00410003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 
0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 
0x73736563, 0x61206465, 0x2D6F7475, 0x2D70616D, 0x61636F6C, 0x6E6F6974, 0x2F2F0A73, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x61206465, 
0x2D6F7475, 0x2D70616D, 0x646E6962, 0x73676E69, 0x202F2F0A, 0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6C632064, 0x746E6569, 0x6C757620, 
0x316E616B, 0x2F0A3030, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x67726174, 0x652D7465, 0x7620766E, 0x616B6C75, 0x302E316E, 
0x202F2F0A, 0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6E652064, 0x2D797274, 0x6E696F70, 0x616D2074, 0x230A6E69, 0x656E696C, 0x000A3120, 
0x00090004, 0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00090004, 0x415F4C47, 0x735F4252, 
0x69646168, 0x6C5F676E, 0x75676E61, 0x5F656761, 0x70303234, 0x006B6361, 0x000A0004, 0x475F4C47, 0x4C474F4F, 0x70635F45, 0x74735F70, 0x5F656C79, 
0x656E696C, 0x7269645F, 0x69746365, 0x00006576, 0x00080004, 0x475F4C47, 0x4C474F4F, 0x6E695F45, 0x64756C63, 0x69645F65, 0x74636572, 0x00657669, 
0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00040005, 0x0000000A, 0x665F4478, 0x00000032, 0x00080005, 0x00000010, 0x616E7944, 0x4263696D, 
0x65666675, 0x74535F72, 0x74637572, 0x00000000, 0x00040006, 0x00000010, 0x00000000, 0x00003269, 0x00040006, 0x00000010, 0x00000001, 0x00003162, 
0x00060006, 0x00000010, 0x00000002, 0x6461705F, 0x676E6964, 0x00000030, 0x00040006, 0x00000010, 0x00000003, 0x00003266, 0x00060006, 0x00000010, 
0x00000004, 0x6461705F, 0x676E6964, 0x00000031, 0x00040006, 0x00000010, 0x00000005, 0x00003369, 0x00040006, 0x00000010, 0x00000006, 0x00003262, 
0x00060006, 0x00000010, 0x00000007, 0x6461705F, 0x676E6964, 0x00000032, 0x00070005, 0x00000012, 0x616E7944, 0x4263696D, 0x65666675, 0x53535F72, 
0x00004F42, 0x00040006, 0x00000012, 0x00000000, 0x00003266, 0x00060006, 0x00000012, 0x00000001, 0x6461705F, 0x676E6964, 0x00000030, 0x00040006, 
0x00000012, 0x00000002, 0x00003469, 0x00040006, 0x00000012, 0x00000003, 0x00727261, 0x00030005, 0x00000014, 0x00627373, 0x00040005, 0x0000001A, 
0x695F4478, 0x00000034, 0x00040005, 0x00000021, 0x655F4478, 0x00746978, 0x00040005, 0x00000028, 0x695F4478, 0x00007864, 0x00080005, 0x0000002B, 
0x475F6C67, 0x61626F6C, 0x766E496C, 0x7461636F, 0x496E6F69, 0x00000044, 0x00050048, 0x00000010, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 
0x00000010, 0x00000001, 0x00000023, 0x00000008, 0x00050048, 0x00000010, 0x00000002, 0x00000023, 0x0000000C, 0x00050048, 0x00000010, 0x00000003, 
0x00000023, 0x00000010, 0x00050048, 0x00000010, 0x00000004, 0x00000023, 0x00000018, 0x00050048, 0x00000010, 0x00000005, 0x00000023, 0x00000020, 
0x00050048, 0x00000010, 0x00000006, 0x00000023, 0x00000030, 0x00050048, 0x00000010, 0x00000007, 0x00000023, 0x00000038, 0x00040047, 0x00000011, 
0x00000006, 0x00000040, 0x00040048, 0x00000012, 0x00000000, 0x00000017, 0x00050048, 0x00000012, 0x00000000, 0x00000023, 0x00000000, 0x00040048, 
0x00000012, 0x00000001, 0x00000017, 0x00050048, 0x00000012, 0x00000001, 0x00000023, 0x00000008, 0x00040048, 0x00000012, 0x00000002, 0x00000017, 
0x00050048, 0x00000012, 0x00000002, 0x00000023, 0x00000010, 0x00040048, 0x00000012, 0x00000003, 0x00000017, 0x00050048, 0x00000012, 0x00000003, 
0x00000023, 0x00000020, 0x00030047, 0x00000012, 0x00000003, 0x00040047, 0x00000014, 0x00000022, 0x00000000, 0x00040047, 0x00000014, 0x00000021, 
0x00000000, 0x00040047, 0x0000002B, 0x0000000B, 0x0000001C, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00030016, 0x00000007, 
0x00000020, 0x00040017, 0x00000008, 0x00000007, 0x00000002, 0x00040020, 0x00000009, 0x00000007, 0x00000008, 0x00040015, 0x0000000B, 0x00000020, 
0x00000001, 0x00040017, 0x0000000C, 0x0000000B, 0x00000004, 0x00040017, 0x0000000D, 0x0000000B, 0x00000002, 0x00040015, 0x0000000E, 0x00000020, 
0x00000000, 0x00040017, 0x0000000F, 0x0000000E, 0x00000002, 0x000A001E, 0x00000010, 0x0000000D, 0x0000000E, 0x00000007, 0x00000008, 0x00000008, 
0x0000000C, 0x0000000F, 0x00000008, 0x0003001D, 0x00000011, 0x00000010, 0x0006001E, 0x00000012, 0x00000008, 0x00000008, 0x0000000C, 0x00000011, 
0x00040020, 0x00000013, 0x00000002, 0x00000012, 0x0004003B, 0x00000013, 0x00000014, 0x00000002, 0x0004002B, 0x0000000B, 0x00000015, 0x00000000, 
0x00040020, 0x00000016, 0x00000002, 0x00000008, 0x00040020, 0x00000019, 0x00000007, 0x0000000C, 0x0004002B, 0x0000000B, 0x0000001B, 0x00000002, 
0x00040020, 0x0000001C, 0x00000002, 0x0000000C, 0x00020014, 0x0000001F, 0x00040020, 0x00000020, 0x00000007, 0x0000001F, 0x0003002A, 0x0000001F, 
0x00000022, 0x00040020, 0x00000027, 0x00000007, 0x0000000B, 0x00040017, 0x00000029, 0x0000000E, 0x00000003, 0x00040020, 0x0000002A, 0x00000001, 
0x00000029, 0x0004003B, 0x0000002A, 0x0000002B, 0x00000001, 0x0004002B, 0x0000000E, 0x0000002C, 0x00000000, 0x00040020, 0x0000002D, 0x00000001, 
0x0000000E, 0x0004002B, 0x0000000B, 0x00000031, 0x00000003, 0x00040020, 0x00000037, 0x00000002, 0x0000000D, 0x0004002B, 0x0000000B, 0x0000003D, 
0x00000001, 0x0004002B, 0x0000000E, 0x00000041, 0x00000001, 0x00040020, 0x00000043, 0x00000002, 0x0000000E, 0x0004002B, 0x0000000B, 0x0000004F, 
0x00000005, 0x0004002B, 0x0000000E, 0x0000005A, 0x00000002, 0x00040020, 0x0000005B, 0x00000002, 0x0000000B, 0x0004002B, 0x0000000B, 0x00000061, 
0x00000006, 0x00040020, 0x00000063, 0x00000002, 0x0000000F, 0x00040017, 0x00000066, 0x0000001F, 0x00000002, 0x0005002C, 0x0000000F, 0x00000067, 
0x0000002C, 0x0000002C, 0x0005002C, 0x0000000F, 0x0000006A, 0x00000041, 0x00000041, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 
0x000200F8, 0x00000006, 0x0004003B, 0x00000009, 0x0000000A, 0x00000007, 0x0004003B, 0x00000019, 0x0000001A, 0x00000007, 0x0004003B, 0x00000020, 
0x00000021, 0x00000007, 0x0004003B, 0x00000027, 0x00000028, 0x00000007, 0x00040008, 0x00000001, 0x0000000E, 0x00000000, 0x00050041, 0x00000016, 
0x00000017, 0x00000014, 0x00000015, 0x0004003D, 0x00000008, 0x00000018, 0x00000017, 0x0003003E, 0x0000000A, 0x00000018, 0x00040008, 0x00000001, 
0x0000000F, 0x00000000, 0x00050041, 0x0000001C, 0x0000001D, 0x00000014, 0x0000001B, 0x0004003D, 0x0000000C, 0x0000001E, 0x0000001D, 0x0003003E, 
0x0000001A, 0x0000001E, 0x00040008, 0x00000001, 0x00000010, 0x00000000, 0x0003003E, 0x00000021, 0x00000022, 0x000200F9, 0x00000023, 0x00040008, 
0x00000001, 0x0000001B, 0x00000000, 0x000200F8, 0x00000023, 0x000400F6, 0x00000025, 0x00000026, 0x00000000, 0x000200F9, 0x00000024, 0x000200F8, 
0x00000024, 0x00040008, 0x00000001, 0x00000012, 0x00000000, 0x00050041, 0x0000002D, 0x0000002E, 0x0000002B, 0x0000002C, 0x0004003D, 0x0000000E, 
0x0000002F, 0x0000002E, 0x0004007C, 0x0000000B, 0x00000030, 0x0000002F, 0x0003003E, 0x00000028, 0x00000030, 0x00040008, 0x00000001, 0x00000014, 
0x00000000, 0x0004003D, 0x0000000B, 0x00000032, 0x00000028, 0x00050041, 0x00000027, 0x00000033, 0x0000001A, 0x0000002C, 0x0004003D, 0x0000000B, 
0x00000034, 0x00000033, 0x0004003D, 0x0000000B, 0x00000035, 0x00000028, 0x00050050, 0x0000000D, 0x00000036, 0x00000034, 0x00000035, 0x00070041, 
0x00000037, 0x00000038, 0x00000014, 0x00000031, 0x00000032, 0x00000015, 0x0004003D, 0x0000000D, 0x00000039, 0x00000038, 0x00050080, 0x0000000D, 
0x0000003A, 0x00000039, 0x00000036, 0x00070041, 0x00000037, 0x0000003B, 0x00000014, 0x00000031, 0x00000032, 0x00000015, 0x0003003E, 0x0000003B, 
0x0000003A, 0x00040008, 0x00000001, 0x00000015, 0x00000000, 0x0004003D, 0x0000000B, 0x0000003C, 0x00000028, 0x0004003D, 0x0000000B, 0x0000003E, 
0x00000028, 0x000500C7, 0x0000000B, 0x0000003F, 0x0000003E, 0x0000003D, 0x000500AA, 0x0000001F, 0x00000040, 0x0000003F, 0x00000015, 0x000600A9, 
0x0000000E, 0x00000042, 0x00000040, 0x00000041, 0x0000002C, 0x00070041, 0x00000043, 0x00000044, 0x00000014, 0x00000031, 0x0000003C, 0x0000003D, 
0x0003003E, 0x00000044, 0x00000042, 0x00040008, 0x00000001, 0x00000016, 0x00000000, 0x0004003D, 0x0000000B, 0x00000045, 0x00000028, 0x0004003D, 
0x00000008, 0x00000046, 0x0000000A, 0x00050051, 0x00000007, 0x00000047, 0x00000046, 0x00000000, 0x00050051, 0x00000007, 0x00000048, 0x00000046, 
0x00000001, 0x00050050, 0x00000008, 0x00000049, 0x00000047, 0x00000048, 0x00070041, 0x00000016, 0x0000004A, 0x00000014, 0x00000031, 0x00000045, 
0x00000031, 0x0004003D, 0x00000008, 0x0000004B, 0x0000004A, 0x00050083, 0x00000008, 0x0000004C, 0x0000004B, 0x00000049, 0x00070041, 0x00000016, 
0x0000004D, 0x00000014, 0x00000031, 0x00000045, 0x00000031, 0x0003003E, 0x0000004D, 0x0000004C, 0x00040008, 0x00000001, 0x00000017, 0x00000000, 
0x0004003D, 0x0000000B, 0x0000004E, 0x00000028, 0x00070041, 0x0000001C, 0x00000050, 0x00000014, 0x00000031, 0x0000004E, 0x0000004F, 0x0004003D, 
0x0000000C, 0x00000051, 0x00000050, 0x0007004F, 0x0000000D, 0x00000052, 0x00000051, 0x00000051, 0x00000000, 0x00000001, 0x00050050, 0x0000000D, 
0x00000053, 0x0000001B, 0x0000001B, 0x00050084, 0x0000000D, 0x00000054, 0x00000052, 0x00000053, 0x00070041, 0x0000001C, 0x00000055, 0x00000014, 
0x00000031, 0x0000004E, 0x0000004F, 0x0004003D, 0x0000000C, 0x00000056, 0x00000055, 0x0009004F, 0x0000000C, 0x00000057, 0x00000056, 0x00000054, 
0x00000004, 0x00000005, 0x00000002, 0x00000003, 0x0003003E, 0x00000055, 0x00000057, 0x00040008, 0x00000001, 0x00000018, 0x00000000, 0x0004003D, 
0x0000000B, 0x00000058, 0x00000028, 0x0004003D, 0x0000000B, 0x00000059, 0x00000028, 0x00080041, 0x0000005B, 0x0000005C, 0x00000014, 0x00000031, 
0x00000059, 0x0000004F, 0x0000005A, 0x0004003D, 0x0000000B, 0x0000005D, 0x0000005C, 0x000400C8, 0x0000000B, 0x0000005E, 0x0000005D, 0x00080041, 
0x0000005B, 0x0000005F, 0x00000014, 0x00000031, 0x00000058, 0x0000004F, 0x0000005A, 0x0003003E, 0x0000005F, 0x0000005E, 0x00040008, 0x00000001, 
0x00000019, 0x00000000, 0x0004003D, 0x0000000B, 0x00000060, 0x00000028, 0x0004003D, 0x0000000B, 0x00000062, 0x00000028, 0x00070041, 0x00000063, 
0x00000064, 0x00000014, 0x00000031, 0x00000062, 0x00000061, 0x0004003D, 0x0000000F, 0x00000065, 0x00000064, 0x000500AB, 0x00000066, 0x00000068, 
0x00000065, 0x00000067, 0x000400A8, 0x00000066, 0x00000069, 0x00000068, 0x000600A9, 0x0000000F, 0x0000006B, 0x00000069, 0x0000006A, 0x00000067, 
0x00070041, 0x00000063, 0x0000006C, 0x00000014, 0x00000031, 0x00000060, 0x00000061, 0x0003003E, 0x0000006C, 0x0000006B, 0x000200F9, 0x00000026, 
0x000200F8, 0x00000026, 0x000400FA, 0x00000022, 0x00000023, 0x00000025, 0x000200F8, 0x00000025, 0x000100FD, 0x00010038 });

#ifdef GRAPHICS_API_SOFT
	descr.Compute().AddInvocable( EShaderLangFormat::Software_100 | EShaderLangFormat::CPP_Invocable, &SWShaderLang::sw_dynamicbuffer_comp );
#endif


};
}	// Pipelines
