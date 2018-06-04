// This is generated file
// Origin file: 'Compiler/Pipelines/InlineAll.ppln'
#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang {
namespace {

#	define INOUT
#	define IN
#	define OUT

	struct InlineAll_SSBO
	{
		SArr<UInt,16> results;
	
		InlineAll_SSBO () {}
		InlineAll_SSBO (InlineAll_SSBO &&) = default;
		InlineAll_SSBO (const InlineAll_SSBO &) = default;
		explicit InlineAll_SSBO(const SArr<UInt,16> &results) : results{results} {}
	
		InlineAll_SSBO& operator = (InlineAll_SSBO &&) = default;
		InlineAll_SSBO& operator = (const InlineAll_SSBO &) = default;
		Bool operator == (const InlineAll_SSBO &right) const {
			return	all( results == right.results );
		}
		Bool operator != (const InlineAll_SSBO &right) const { return !(*this == right); }
	
	};
	
	
	//---------------------------------
	
	static UInt Hash (IN UInt value);
	static void Loop (IN UInt value1, OUT UInt &result);
	
	//---------------------------------
	
	static void sw_inlineall_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::StorageBuffer< InlineAll_SSBO, Impl::EStorageAccess::WriteOnly >  ssb;    _helper_.GetStorageBuffer( 0, ssb );
		auto& gl_LocalInvocationIndex = _helper_.GetComputeShaderState().inLocalInvocationIndex;
	
		// shader
		{
			UInt index = gl_LocalInvocationIndex;
			;
			UInt result1 = UInt(0u);
			;
			if ((index > UInt(0u)))
			{
				
				// Hash(u1;
				UInt xK_value = (index * UInt(17u));
				UInt xK_return = UInt(0);
				Bool xK_exit = Bool(false);
				do {
					UInt xK_res = ((xK_value << Int(7)) | (xK_value << Int(11)));
					;
					{
						xK_return = xK_res;
						xK_exit = true;
						break;
					};
				
				} while(false);
				// end Hash(u1;;
				(result1 |= xK_return);
				
				// Hash(u1;
				UInt xR_value = (index * UInt(211u));
				UInt xR_return = UInt(0);
				Bool xR_exit = Bool(false);
				do {
					UInt xR_res = ((xR_value << Int(7)) | (xR_value << Int(11)));
					;
					{
						xR_return = xR_res;
						xR_exit = true;
						break;
					};
				
				} while(false);
				// end Hash(u1;;
				(result1 |= xR_return);
			;
			}
			else
			{
				
				// Hash(u1;
				UInt xW_value = UInt(456u);
				UInt xW_return = UInt(0);
				Bool xW_exit = Bool(false);
				do {
					UInt xW_res = ((xW_value << Int(7)) | (xW_value << Int(11)));
					;
					{
						xW_return = xW_res;
						xW_exit = true;
						break;
					};
				
				} while(false);
				// end Hash(u1;;
				(result1 |= xW_return);
				UInt temp = UInt(1u);
				;
				
				// Loop(u1;u1;
				UInt xV_value1 = (result1 ^ (result1 << Int(4)));
				Bool xV_exit = Bool(false);
				do {
					for (UInt j = UInt(0u); (!xV_exit && (j < UInt(4u))); ++(j))
					{
						for (UInt i = UInt(0u); (!xV_exit && (i < UInt(16u))); ++(i))
						{
							(xV_value1 ^= UInt((Int(3) << i)));
							if ((((xV_value1 >> Int(13)) & UInt(1u)) == UInt(1u)))
							{
								temp = UInt(0u);
								xV_exit = true;
								break;
							;
							}
							;
						}
						if (xV_exit) break;
						;
						++(temp);
					}
					if (xV_exit) break;
					;
					temp = (xV_value1 + UInt(4u));
				
				} while(false);
				// end Loop(u1;u1;;
				 ;
				(result1 |= temp);
			;
			}
			;
			(ssb->results[index]) = result1;
		}
	}
	
}		// anonymous namespace
}		// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_inlineall (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	descr.supportedShaders = EShader::Compute;

	descr.localGroupSize = uint3(4, 4, 1);
	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddStorageBuffer( "ssb", 64_b, 0_b, EShaderMemoryModel::WriteOnly, 0u, 0u, EShader::Compute )
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
		
#define SHADER	SH_COMPUTE


layout(binding=0) layout(std430) writeonly buffer InlineAll_SSBO {
	layout(offset=0, align=4) uint  results[16];

} ssb;


layout (local_size_x=4, local_size_y=4, local_size_z=1) in;

uint Hash (in uint value);
void Loop (in uint value1, out uint result);

//---------------------------------

uint Hash (in uint value)
{
	uint res = ((value << int( 7 )) | (value << int( 11 )));
	;
	return res;
}


void Loop (in uint value1, out uint result)
{
	for (uint j = uint( 0u ); (j < uint( 4u )); ++( j ))
	{
		for (uint i = uint( 0u ); (i < uint( 16u )); ++( i ))
		{
			(value1 ^= uint( (int( 3 ) << i) ));
			if ((((value1 >> int( 13 )) & uint( 1u )) == uint( 1u )))
			{
				result = uint( 0u );
				return;
			;
			}
			;
		}
		;
		++( result );
	}
	;
	result = (value1 + uint( 4u ));
}


void main ()
{
	uint index = gl_LocalInvocationIndex;
	;
	uint result1 = uint( 0u );
	;
	if ((index > uint( 0u )))
	{
		(result1 |= Hash((index * uint( 17u ))));
		(result1 |= Hash((index * uint( 211u ))));
	;
	}
	else
	{
		(result1 |= Hash(uint( 456u )));
		uint temp = uint( 1u );
		;
		Loop((result1 ^ (result1 << int( 4 ))), temp);
		(result1 |= temp);
	;
	}
	;
	(ssb.results[index]) = result1;
}



)#"_str );
	descr.Compute().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080006, 0x0000007F, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0006000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x0000004C, 0x00060010, 0x00000005, 
0x00000011, 0x00000004, 0x00000004, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x002D0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 
0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 
0x73736563, 0x63206465, 0x6E65696C, 0x706F2074, 0x6C676E65, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 
0x2D746567, 0x20766E65, 0x6E65706F, 0x2F0A6C67, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x72746E65, 0x6F702D79, 0x20746E69, 
0x6E69616D, 0x696C230A, 0x3120656E, 0x0000000A, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 
0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 
0x00050005, 0x0000000B, 0x68736148, 0x3B317528, 0x00000000, 0x00040005, 0x0000000A, 0x756C6176, 0x00000065, 0x00050005, 0x00000010, 0x706F6F4C, 
0x3B317528, 0x003B3175, 0x00040005, 0x0000000E, 0x756C6176, 0x00003165, 0x00040005, 0x0000000F, 0x75736572, 0x0000746C, 0x00030005, 0x00000012, 
0x00736572, 0x00030005, 0x0000001E, 0x0000006A, 0x00030005, 0x00000029, 0x00000069, 0x00040005, 0x0000004A, 0x65646E69, 0x00000078, 0x00080005, 
0x0000004C, 0x4C5F6C67, 0x6C61636F, 0x6F766E49, 0x69746163, 0x6E496E6F, 0x00786564, 0x00040005, 0x0000004E, 0x75736572, 0x0031746C, 0x00040005, 
0x00000056, 0x61726170, 0x0000006D, 0x00040005, 0x0000005D, 0x61726170, 0x0000006D, 0x00040005, 0x00000063, 0x61726170, 0x0000006D, 0x00040005, 
0x00000067, 0x706D6574, 0x00000000, 0x00040005, 0x0000006D, 0x61726170, 0x0000006D, 0x00040005, 0x0000006E, 0x61726170, 0x0000006D, 0x00060005, 
0x00000075, 0x696C6E49, 0x6C41656E, 0x53535F6C, 0x00004F42, 0x00050006, 0x00000075, 0x00000000, 0x75736572, 0x0073746C, 0x00030005, 0x00000077, 
0x00627373, 0x00040047, 0x0000004C, 0x0000000B, 0x0000001D, 0x00040047, 0x00000074, 0x00000006, 0x00000004, 0x00040048, 0x00000075, 0x00000000, 
0x00000019, 0x00050048, 0x00000075, 0x00000000, 0x00000023, 0x00000000, 0x00030047, 0x00000075, 0x00000003, 0x00040047, 0x00000077, 0x00000022, 
0x00000000, 0x00040047, 0x00000077, 0x00000021, 0x00000000, 0x00040047, 0x0000007E, 0x0000000B, 0x00000019, 0x00020013, 0x00000003, 0x00030021, 
0x00000004, 0x00000003, 0x00040015, 0x00000007, 0x00000020, 0x00000000, 0x00040020, 0x00000008, 0x00000007, 0x00000007, 0x00040021, 0x00000009, 
0x00000007, 0x00000008, 0x00050021, 0x0000000D, 0x00000003, 0x00000008, 0x00000008, 0x00040015, 0x00000014, 0x00000020, 0x00000001, 0x0004002B, 
0x00000014, 0x00000015, 0x00000007, 0x0004002B, 0x00000014, 0x00000018, 0x0000000B, 0x0004002B, 0x00000007, 0x0000001F, 0x00000000, 0x0004002B, 
0x00000007, 0x00000026, 0x00000004, 0x00020014, 0x00000027, 0x0004002B, 0x00000007, 0x00000030, 0x00000010, 0x0004002B, 0x00000014, 0x00000032, 
0x00000003, 0x0004002B, 0x00000014, 0x00000039, 0x0000000D, 0x0004002B, 0x00000007, 0x0000003B, 0x00000001, 0x0004002B, 0x00000014, 0x00000042, 
0x00000001, 0x00040020, 0x0000004B, 0x00000001, 0x00000007, 0x0004003B, 0x0000004B, 0x0000004C, 0x00000001, 0x0004002B, 0x00000007, 0x00000054, 
0x00000011, 0x0004002B, 0x00000007, 0x0000005B, 0x000000D3, 0x0004002B, 0x00000007, 0x00000062, 0x000001C8, 0x0004002B, 0x00000014, 0x0000006A, 
0x00000004, 0x0004001C, 0x00000074, 0x00000007, 0x00000030, 0x0003001E, 0x00000075, 0x00000074, 0x00040020, 0x00000076, 0x00000002, 0x00000075, 
0x0004003B, 0x00000076, 0x00000077, 0x00000002, 0x0004002B, 0x00000014, 0x00000078, 0x00000000, 0x00040020, 0x0000007B, 0x00000002, 0x00000007, 
0x00040017, 0x0000007D, 0x00000007, 0x00000003, 0x0006002C, 0x0000007D, 0x0000007E, 0x00000026, 0x00000026, 0x0000003B, 0x00050036, 0x00000003, 
0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x0004003B, 0x00000008, 0x0000004A, 0x00000007, 0x0004003B, 0x00000008, 0x0000004E, 
0x00000007, 0x0004003B, 0x00000008, 0x00000056, 0x00000007, 0x0004003B, 0x00000008, 0x0000005D, 0x00000007, 0x0004003B, 0x00000008, 0x00000063, 
0x00000007, 0x0004003B, 0x00000008, 0x00000067, 0x00000007, 0x0004003B, 0x00000008, 0x0000006D, 0x00000007, 0x0004003B, 0x00000008, 0x0000006E, 
0x00000007, 0x00040008, 0x00000001, 0x00000080, 0x00000000, 0x0004003D, 0x00000007, 0x0000004D, 0x0000004C, 0x0003003E, 0x0000004A, 0x0000004D, 
0x00040008, 0x00000001, 0x00000082, 0x00000000, 0x0003003E, 0x0000004E, 0x0000001F, 0x00040008, 0x00000001, 0x00000084, 0x00000000, 0x0004003D, 
0x00000007, 0x0000004F, 0x0000004A, 0x000500AC, 0x00000027, 0x00000050, 0x0000004F, 0x0000001F, 0x000300F7, 0x00000052, 0x00000000, 0x000400FA, 
0x00000050, 0x00000051, 0x00000061, 0x000200F8, 0x00000051, 0x00040008, 0x00000001, 0x00000086, 0x00000000, 0x0004003D, 0x00000007, 0x00000053, 
0x0000004A, 0x00050084, 0x00000007, 0x00000055, 0x00000053, 0x00000054, 0x0003003E, 0x00000056, 0x00000055, 0x00050039, 0x00000007, 0x00000057, 
0x0000000B, 0x00000056, 0x0004003D, 0x00000007, 0x00000058, 0x0000004E, 0x000500C5, 0x00000007, 0x00000059, 0x00000058, 0x00000057, 0x0003003E, 
0x0000004E, 0x00000059, 0x00040008, 0x00000001, 0x00000087, 0x00000000, 0x0004003D, 0x00000007, 0x0000005A, 0x0000004A, 0x00050084, 0x00000007, 
0x0000005C, 0x0000005A, 0x0000005B, 0x0003003E, 0x0000005D, 0x0000005C, 0x00050039, 0x00000007, 0x0000005E, 0x0000000B, 0x0000005D, 0x0004003D, 
0x00000007, 0x0000005F, 0x0000004E, 0x000500C5, 0x00000007, 0x00000060, 0x0000005F, 0x0000005E, 0x0003003E, 0x0000004E, 0x00000060, 0x000200F9, 
0x00000052, 0x000200F8, 0x00000061, 0x00040008, 0x00000001, 0x0000008C, 0x00000000, 0x0003003E, 0x00000063, 0x00000062, 0x00050039, 0x00000007, 
0x00000064, 0x0000000B, 0x00000063, 0x0004003D, 0x00000007, 0x00000065, 0x0000004E, 0x000500C5, 0x00000007, 0x00000066, 0x00000065, 0x00000064, 
0x0003003E, 0x0000004E, 0x00000066, 0x00040008, 0x00000001, 0x0000008D, 0x00000000, 0x0003003E, 0x00000067, 0x0000003B, 0x00040008, 0x00000001, 
0x0000008F, 0x00000000, 0x0004003D, 0x00000007, 0x00000068, 0x0000004E, 0x0004003D, 0x00000007, 0x00000069, 0x0000004E, 0x000500C4, 0x00000007, 
0x0000006B, 0x00000069, 0x0000006A, 0x000500C6, 0x00000007, 0x0000006C, 0x00000068, 0x0000006B, 0x0003003E, 0x0000006D, 0x0000006C, 0x00060039, 
0x00000003, 0x0000006F, 0x00000010, 0x0000006D, 0x0000006E, 0x0004003D, 0x00000007, 0x00000070, 0x0000006E, 0x0003003E, 0x00000067, 0x00000070, 
0x00040008, 0x00000001, 0x00000090, 0x00000000, 0x0004003D, 0x00000007, 0x00000071, 0x00000067, 0x0004003D, 0x00000007, 0x00000072, 0x0000004E, 
0x000500C5, 0x00000007, 0x00000073, 0x00000072, 0x00000071, 0x0003003E, 0x0000004E, 0x00000073, 0x000200F9, 0x00000052, 0x000200F8, 0x00000052, 
0x00040008, 0x00000001, 0x00000094, 0x00000000, 0x0004003D, 0x00000007, 0x00000079, 0x0000004A, 0x0004003D, 0x00000007, 0x0000007A, 0x0000004E, 
0x00060041, 0x0000007B, 0x0000007C, 0x00000077, 0x00000078, 0x00000079, 0x0003003E, 0x0000007C, 0x0000007A, 0x000100FD, 0x00010038, 0x00050036, 
0x00000007, 0x0000000B, 0x00000000, 0x00000009, 0x00030037, 0x00000008, 0x0000000A, 0x000200F8, 0x0000000C, 0x0004003B, 0x00000008, 0x00000012, 
0x00000007, 0x00040008, 0x00000001, 0x00000061, 0x00000000, 0x0004003D, 0x00000007, 0x00000013, 0x0000000A, 0x000500C4, 0x00000007, 0x00000016, 
0x00000013, 0x00000015, 0x0004003D, 0x00000007, 0x00000017, 0x0000000A, 0x000500C4, 0x00000007, 0x00000019, 0x00000017, 0x00000018, 0x000500C5, 
0x00000007, 0x0000001A, 0x00000016, 0x00000019, 0x0003003E, 0x00000012, 0x0000001A, 0x00040008, 0x00000001, 0x00000063, 0x00000000, 0x0004003D, 
0x00000007, 0x0000001B, 0x00000012, 0x000200FE, 0x0000001B, 0x00010038, 0x00050036, 0x00000003, 0x00000010, 0x00000000, 0x0000000D, 0x00030037, 
0x00000008, 0x0000000E, 0x00030037, 0x00000008, 0x0000000F, 0x000200F8, 0x00000011, 0x0004003B, 0x00000008, 0x0000001E, 0x00000007, 0x0004003B, 
0x00000008, 0x00000029, 0x00000007, 0x00040008, 0x00000001, 0x00000069, 0x00000000, 0x0003003E, 0x0000001E, 0x0000001F, 0x000200F9, 0x00000020, 
0x000200F8, 0x00000020, 0x000400F6, 0x00000022, 0x00000023, 0x00000000, 0x000200F9, 0x00000024, 0x000200F8, 0x00000024, 0x0004003D, 0x00000007, 
0x00000025, 0x0000001E, 0x000500B0, 0x00000027, 0x00000028, 0x00000025, 0x00000026, 0x000400FA, 0x00000028, 0x00000021, 0x00000022, 0x000200F8, 
0x00000021, 0x00040008, 0x00000001, 0x0000006B, 0x00000000, 0x0003003E, 0x00000029, 0x0000001F, 0x000200F9, 0x0000002A, 0x000200F8, 0x0000002A, 
0x000400F6, 0x0000002C, 0x0000002D, 0x00000000, 0x000200F9, 0x0000002E, 0x000200F8, 0x0000002E, 0x0004003D, 0x00000007, 0x0000002F, 0x00000029, 
0x000500B0, 0x00000027, 0x00000031, 0x0000002F, 0x00000030, 0x000400FA, 0x00000031, 0x0000002B, 0x0000002C, 0x000200F8, 0x0000002B, 0x00040008, 
0x00000001, 0x0000006D, 0x00000000, 0x0004003D, 0x00000007, 0x00000033, 0x00000029, 0x000500C4, 0x00000014, 0x00000034, 0x00000032, 0x00000033, 
0x0004007C, 0x00000007, 0x00000035, 0x00000034, 0x0004003D, 0x00000007, 0x00000036, 0x0000000E, 0x000500C6, 0x00000007, 0x00000037, 0x00000036, 
0x00000035, 0x0003003E, 0x0000000E, 0x00000037, 0x00040008, 0x00000001, 0x0000006E, 0x00000000, 0x0004003D, 0x00000007, 0x00000038, 0x0000000E, 
0x000500C2, 0x00000007, 0x0000003A, 0x00000038, 0x00000039, 0x000500C7, 0x00000007, 0x0000003C, 0x0000003A, 0x0000003B, 0x000500AA, 0x00000027, 
0x0000003D, 0x0000003C, 0x0000003B, 0x000300F7, 0x0000003F, 0x00000000, 0x000400FA, 0x0000003D, 0x0000003E, 0x0000003F, 0x000200F8, 0x0000003E, 
0x00040008, 0x00000001, 0x00000070, 0x00000000, 0x0003003E, 0x0000000F, 0x0000001F, 0x00040008, 0x00000001, 0x00000071, 0x00000000, 0x000100FD, 
0x000200F8, 0x0000003F, 0x000200F9, 0x0000002D, 0x000200F8, 0x0000002D, 0x00040008, 0x00000001, 0x0000006B, 0x00000000, 0x0004003D, 0x00000007, 
0x00000041, 0x00000029, 0x00050080, 0x00000007, 0x00000043, 0x00000041, 0x00000042, 0x0003003E, 0x00000029, 0x00000043, 0x000200F9, 0x0000002A, 
0x000200F8, 0x0000002C, 0x00040008, 0x00000001, 0x00000077, 0x00000000, 0x0004003D, 0x00000007, 0x00000044, 0x0000000F, 0x00050080, 0x00000007, 
0x00000045, 0x00000044, 0x00000042, 0x0003003E, 0x0000000F, 0x00000045, 0x000200F9, 0x00000023, 0x000200F8, 0x00000023, 0x00040008, 0x00000001, 
0x00000069, 0x00000000, 0x0004003D, 0x00000007, 0x00000046, 0x0000001E, 0x00050080, 0x00000007, 0x00000047, 0x00000046, 0x00000042, 0x0003003E, 
0x0000001E, 0x00000047, 0x000200F9, 0x00000020, 0x000200F8, 0x00000022, 0x00040008, 0x00000001, 0x0000007A, 0x00000000, 0x0004003D, 0x00000007, 
0x00000048, 0x0000000E, 0x00050080, 0x00000007, 0x00000049, 0x00000048, 0x00000026, 0x0003003E, 0x0000000F, 0x00000049, 0x000100FD, 0x00010038 });
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

struct InlineAll_SSBO
{
	uint results [16];
};


//---------------------------------

uint Hash (uint value);
void Loop (uint value1, OUT uint *result);

//---------------------------------

kernel void Main (
	/*0*/__global  struct InlineAll_SSBO* ssb)
{

	{
		uint index = ((uint)((get_local_id(2) * get_local_size(1) * get_local_size(0)) + (get_local_id(1) * get_local_size(0)) + get_local_id(0)));
		;
		uint result1 = ((uint)( 0u ));
		;
		if ((index > ((uint)( 0u ))))
		{
			
			// Hash(u1;
			uint xK_value = (index * ((uint)( 17u )));
			uint xK_return = ((uint)( 0 ));
			int xK_exit = ((int)( false ));
			do {
				uint xK_res = ((xK_value << ((int)( 7 ))) | (xK_value << ((int)( 11 ))));
				;
				{
					xK_return = xK_res;
					xK_exit = true;
					break;
				};
			
			} while(false);
			// end Hash(u1;;
			(result1 |= xK_return);
			
			// Hash(u1;
			uint xR_value = (index * ((uint)( 211u )));
			uint xR_return = ((uint)( 0 ));
			int xR_exit = ((int)( false ));
			do {
				uint xR_res = ((xR_value << ((int)( 7 ))) | (xR_value << ((int)( 11 ))));
				;
				{
					xR_return = xR_res;
					xR_exit = true;
					break;
				};
			
			} while(false);
			// end Hash(u1;;
			(result1 |= xR_return);
		;
		}
		else
		{
			
			// Hash(u1;
			uint xW_value = ((uint)( 456u ));
			uint xW_return = ((uint)( 0 ));
			int xW_exit = ((int)( false ));
			do {
				uint xW_res = ((xW_value << ((int)( 7 ))) | (xW_value << ((int)( 11 ))));
				;
				{
					xW_return = xW_res;
					xW_exit = true;
					break;
				};
			
			} while(false);
			// end Hash(u1;;
			(result1 |= xW_return);
			uint temp = ((uint)( 1u ));
			;
			
			// Loop(u1;u1;
			uint xV_value1 = (result1 ^ (result1 << ((int)( 4 ))));
			int xV_exit = ((int)( false ));
			do {
				for (uint j = ((uint)( 0u )); (!xV_exit && (j < ((uint)( 4u )))); ++( j ))
				{
					for (uint i = ((uint)( 0u )); (!xV_exit && (i < ((uint)( 16u )))); ++( i ))
					{
						(xV_value1 ^= convert_uint( (((int)( 3 )) << i) ));
						if ((((xV_value1 >> ((int)( 13 ))) & ((uint)( 1u ))) == ((uint)( 1u ))))
						{
							temp = ((uint)( 0u ));
							xV_exit = true;
							break;
						;
						}
						;
					}
					if (xV_exit) break;
					;
					++( temp );
				}
				if (xV_exit) break;
				;
				temp = (xV_value1 + ((uint)( 4u )));
			
			} while(false);
			// end Loop(u1;u1;;
			 ;
			(result1 |= temp);
		;
		}
		;
		(ssb->results[index]) = result1;
	}
}

)#"_str );
	descr.Compute().StringHLSL( 
R"#(cbuffer ComputeBuiltins : register(b0)
{
	uint3		dx_NumWorkGroups;
};

struct InlineAll_SSBO{
	uint results[16];
};
RWByteAddressBuffer<InlineAll_SSBO> ssb : register(u0);

//---------------------------------

uint Hash (in uint value);
void Loop (in uint value1, out uint result);

//---------------------------------

[numthreads(4, 4, 1)]
void main (uint3 dx_DispatchThreadID : SV_DispatchThreadID, uint3 dx_GroupThreadID : SV_GroupThreadID, uint3 dx_GroupID : SV_GroupID)
{
	uint index = (dx_GroupThreadID.x + dx_GroupThreadID.y * 4 + dx_GroupThreadID.z * 16);
	;
	uint result1 = uint( 0u );
	;
	if ((index > uint( 0u )))
	{
		
		// Hash(u1;
		uint xK_value = (index * uint( 17u ));
		uint xK_return = uint( 0 );
		int xK_exit = int( false );
		do {
			uint xK_res = ((xK_value << int( 7 )) | (xK_value << int( 11 )));
			;
			{
				xK_return = xK_res;
				xK_exit = true;
				break;
			};
		
		} while(false);
		// end Hash(u1;;
		(result1 |= xK_return);
		
		// Hash(u1;
		uint xR_value = (index * uint( 211u ));
		uint xR_return = uint( 0 );
		int xR_exit = int( false );
		do {
			uint xR_res = ((xR_value << int( 7 )) | (xR_value << int( 11 )));
			;
			{
				xR_return = xR_res;
				xR_exit = true;
				break;
			};
		
		} while(false);
		// end Hash(u1;;
		(result1 |= xR_return);
	;
	}
	else
	{
		
		// Hash(u1;
		uint xW_value = uint( 456u );
		uint xW_return = uint( 0 );
		int xW_exit = int( false );
		do {
			uint xW_res = ((xW_value << int( 7 )) | (xW_value << int( 11 )));
			;
			{
				xW_return = xW_res;
				xW_exit = true;
				break;
			};
		
		} while(false);
		// end Hash(u1;;
		(result1 |= xW_return);
		uint temp = uint( 1u );
		;
		
		// Loop(u1;u1;
		uint xV_value1 = (result1 ^ (result1 << int( 4 )));
		int xV_exit = int( false );
		do {
			for (uint j = uint( 0u ); (!xV_exit && (j < uint( 4u ))); ++( j ))
			{
				for (uint i = uint( 0u ); (!xV_exit && (i < uint( 16u ))); ++( i ))
				{
					(xV_value1 ^= uint( (int( 3 ) << i) ));
					if ((((xV_value1 >> int( 13 )) & uint( 1u )) == uint( 1u )))
					{
						temp = uint( 0u );
						xV_exit = true;
						break;
					;
					}
					;
				}
				if (xV_exit) break;
				;
				++( temp );
			}
			if (xV_exit) break;
			;
			temp = (xV_value1 + uint( 4u ));
		
		} while(false);
		// end Loop(u1;u1;;
		 ;
		(result1 |= temp);
	;
	}
	;
	(((ssb).Load1(/*results*/0))[index]) = result1;
}


)#"_str );
#ifdef GRAPHICS_API_SOFT
	descr.Compute().FuncSW( &SWShaderLang::sw_inlineall_comp );
#endif

};
};
