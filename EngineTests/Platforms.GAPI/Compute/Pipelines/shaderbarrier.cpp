// This is generated file
// Origin file: 'Compute/Pipelines/ShaderBarrier.ppln'
#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang {
namespace {

#	define INOUT
#	define IN
#	define OUT

	static Float MaxMag (const Float a, const Float b);
	static Float4 MaxMag (const Float4 a1, const Float4 b1);
	
	//---------------------------------
	
	static void sw_shaderbarrier_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::Image2D< vec4, Impl::EStorageAccess::WriteOnly >  un_DstImage;    _helper_.GetImage( 0, un_DstImage );
		Impl::Image2D< vec4, Impl::EStorageAccess::ReadOnly >  un_SrcImage;    _helper_.GetImage( 1, un_SrcImage );
		Impl::SharedMemory< Float4 >	sharedMemory;	_helper_.GetShared( 0, 16, sharedMemory );
		Impl::Barrier __barrier_obj0;	_helper_.InitBarrier( 0, __barrier_obj0 );
		auto& gl_LocalInvocationIndex = _helper_.GetComputeShaderState().inLocalInvocationIndex;
		auto& gl_GlobalInvocationID = _helper_.GetComputeShaderState().inGlobalInvocationID;
	
		// shader
		{
			Int2 coord = Int3(gl_GlobalInvocationID).xy;
			;
			Int idx = Int(gl_LocalInvocationIndex);
			;
			(sharedMemory[idx]) = imageLoad(un_SrcImage, coord);
			__barrier_obj0.Wait();
			Float4 value = Float4(0.0f);
			;
			for (Int y = Int(0); (UInt(y) < UInt(4u)); ++(y))
			{
				for (Int x = Int(0); (UInt(x) < UInt(4u)); ++(x))
				{
					const Float4 diff = ((sharedMemory[idx]) - (sharedMemory[(UInt(x) + (UInt(y) * UInt(4u)))]));
					;
					
					// MaxMag(vf4;vf4;
					const Float4 xU_a1 = value;
					const Float4 xU_b1 = diff;
					Float4 xU_return = Float4(0);
					Bool xU_exit = Bool(false);
					do {
						
						// MaxMag(f1;f1;
						const Float xM_a = xU_a1.x;
						const Float xM_b = xU_b1.x;
						Float xM_return = Float(0);
						Bool xM_exit = Bool(false);
						do {
							if ((glm::abs(xM_a) > glm::abs(xM_b)))
							{
								{
									xM_return = xM_a;
									xM_exit = true;
									break;
								};
							;
							}
							;
							if ((glm::abs(xM_a) < glm::abs(xM_b)))
							{
								{
									xM_return = xM_b;
									xM_exit = true;
									break;
								};
							;
							}
							;
							{
								xM_return = glm::max(xM_a, xM_b);
								xM_exit = true;
								break;
							};
						
						} while(false);
						// end MaxMag(f1;f1;;
						
						// MaxMag(f1;f1;
						const Float xV_a = xU_a1.y;
						const Float xV_b = xU_b1.y;
						Float xV_return = Float(0);
						Bool xV_exit = Bool(false);
						do {
							if ((glm::abs(xV_a) > glm::abs(xV_b)))
							{
								{
									xV_return = xV_a;
									xV_exit = true;
									break;
								};
							;
							}
							;
							if ((glm::abs(xV_a) < glm::abs(xV_b)))
							{
								{
									xV_return = xV_b;
									xV_exit = true;
									break;
								};
							;
							}
							;
							{
								xV_return = glm::max(xV_a, xV_b);
								xV_exit = true;
								break;
							};
						
						} while(false);
						// end MaxMag(f1;f1;;
						
						// MaxMag(f1;f1;
						const Float xI_a = xU_a1.z;
						const Float xI_b = xU_b1.z;
						Float xI_return = Float(0);
						Bool xI_exit = Bool(false);
						do {
							if ((glm::abs(xI_a) > glm::abs(xI_b)))
							{
								{
									xI_return = xI_a;
									xI_exit = true;
									break;
								};
							;
							}
							;
							if ((glm::abs(xI_a) < glm::abs(xI_b)))
							{
								{
									xI_return = xI_b;
									xI_exit = true;
									break;
								};
							;
							}
							;
							{
								xI_return = glm::max(xI_a, xI_b);
								xI_exit = true;
								break;
							};
						
						} while(false);
						// end MaxMag(f1;f1;;
						
						// MaxMag(f1;f1;
						const Float xJ_a = xU_a1.w;
						const Float xJ_b = xU_b1.w;
						Float xJ_return = Float(0);
						Bool xJ_exit = Bool(false);
						do {
							if ((glm::abs(xJ_a) > glm::abs(xJ_b)))
							{
								{
									xJ_return = xJ_a;
									xJ_exit = true;
									break;
								};
							;
							}
							;
							if ((glm::abs(xJ_a) < glm::abs(xJ_b)))
							{
								{
									xJ_return = xJ_b;
									xJ_exit = true;
									break;
								};
							;
							}
							;
							{
								xJ_return = glm::max(xJ_a, xJ_b);
								xJ_exit = true;
								break;
							};
						
						} while(false);
						// end MaxMag(f1;f1;;
						{
							xU_return = Float4(xM_return, xV_return, xI_return, xJ_return);
							xU_exit = true;
							break;
						};
					
					} while(false);
					// end MaxMag(vf4;vf4;;
					value = xU_return;
				}
				;
			}
			;
			imageStore(un_DstImage, coord, value);
		}
	}
	
}		// anonymous namespace
}		// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_shaderbarrier (PipelineTemplateDescription& descr)
{
	descr = PipelineTemplateDescription();
	descr.supportedShaders = EShader::Compute;

	descr.localGroupSize = uint3(4, 4, 1);
	descr.layout = PipelineLayoutDescription::Builder()
			.AddImage( "un_DstImage", EImage::Tex2D, EPixelFormat::RGBA32F, EShaderMemoryModel::WriteOnly, 0u, 0u, EShader::Compute )
			.AddImage( "un_SrcImage", EImage::Tex2D, EPixelFormat::RGBA32F, EShaderMemoryModel::ReadOnly, 1u, 1u, EShader::Compute )
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

layout(binding=1) layout(rgba32f) readonly uniform image2D un_SrcImage;


layout (local_size_x=4, local_size_y=4, local_size_z=1) in;

shared vec4 sharedMemory[16];

//---------------------------------

vec4 MaxMag (const vec4 a1, const vec4 b1);
float MaxMag (const float a, const float b);

//---------------------------------

float MaxMag (const float a, const float b)
{
	if ((abs( a ) > abs( b )))
	{
		return a;
	}
	;
	if ((abs( a ) < abs( b )))
	{
		return b;
	}
	;
	return max( a, b );
}


vec4 MaxMag (const vec4 a1, const vec4 b1)
{
	return vec4( MaxMag(a1.x, b1.x), MaxMag(a1.y, b1.y), MaxMag(a1.z, b1.z), MaxMag(a1.w, b1.w) );
}


void main ()
{
	ivec2 coord = ivec3( gl_GlobalInvocationID ).xy;
	;
	int idx = int( gl_LocalInvocationIndex );
	;
	(sharedMemory[idx]) = imageLoad( un_SrcImage, coord );
	barrier();
	vec4 value = vec4( 0.0f );
	;
	for (int y = int( 0 ); (uint( y ) < uint( 4u )); ++( y ))
	{
		for (int x = int( 0 ); (uint( x ) < uint( 4u )); ++( x ))
		{
			const vec4 diff = ((sharedMemory[idx]) - (sharedMemory[(uint( x ) + (uint( y ) * uint( 4u )))]));
			;
			value = MaxMag(value, diff);
		}
		;
	}
	;
	imageStore( un_DstImage, coord, value );
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
float MaxMag (const float a, const float b);
float4 MaxMag_1 (const float4 a1, const float4 b1);

//---------------------------------

kernel void Main (
	/*0*/write_only FORMAT(rgba32f) image2d_t un_DstImage,
	/*1*/read_only FORMAT(rgba32f) image2d_t un_SrcImage)
{
__local float4 sharedMemory [16];

	{
		int2 coord = convert_int3( ((uint3)(get_global_id(0),  get_global_id(1),  get_global_id(2))) ).xy;
		;
		int idx = convert_int( ((uint)((get_local_id(2) * get_local_size(1) * get_local_size(0)) + (get_local_id(1) * get_local_size(0)) + get_local_id(0))) );
		;
		(sharedMemory[idx]) = read_imagef(un_SrcImage, coord);
		barrier(CLK_LOCAL_MEM_FENCE);
		float4 value = ((float4)( 0.0f ));
		;
		for (int y = ((int)( 0 )); (convert_uint( y ) < ((uint)( 4u ))); ++( y ))
		{
			for (int x = ((int)( 0 )); (convert_uint( x ) < ((uint)( 4u ))); ++( x ))
			{
				const float4 diff = ((sharedMemory[idx]) - (sharedMemory[(convert_uint( x ) + (convert_uint( y ) * ((uint)( 4u ))))]));
				;
				
				// MaxMag(vf4;vf4;
				const float4 xU_a1 = value;
				const float4 xU_b1 = diff;
				float4 xU_return = ((float4)( 0 ));
				int xU_exit = ((int)( false ));
				do {
					
					// MaxMag(f1;f1;
					const float xM_a = xU_a1.x;
					const float xM_b = xU_b1.x;
					float xM_return = ((float)( 0 ));
					int xM_exit = ((int)( false ));
					do {
						if ((fabs( xM_a ) > fabs( xM_b )))
						{
							{
								xM_return = xM_a;
								xM_exit = true;
								break;
							};
						;
						}
						;
						if ((fabs( xM_a ) < fabs( xM_b )))
						{
							{
								xM_return = xM_b;
								xM_exit = true;
								break;
							};
						;
						}
						;
						{
							xM_return = max( xM_a, xM_b );
							xM_exit = true;
							break;
						};
					
					} while(false);
					// end MaxMag(f1;f1;;
					
					// MaxMag(f1;f1;
					const float xV_a = xU_a1.y;
					const float xV_b = xU_b1.y;
					float xV_return = ((float)( 0 ));
					int xV_exit = ((int)( false ));
					do {
						if ((fabs( xV_a ) > fabs( xV_b )))
						{
							{
								xV_return = xV_a;
								xV_exit = true;
								break;
							};
						;
						}
						;
						if ((fabs( xV_a ) < fabs( xV_b )))
						{
							{
								xV_return = xV_b;
								xV_exit = true;
								break;
							};
						;
						}
						;
						{
							xV_return = max( xV_a, xV_b );
							xV_exit = true;
							break;
						};
					
					} while(false);
					// end MaxMag(f1;f1;;
					
					// MaxMag(f1;f1;
					const float xI_a = xU_a1.z;
					const float xI_b = xU_b1.z;
					float xI_return = ((float)( 0 ));
					int xI_exit = ((int)( false ));
					do {
						if ((fabs( xI_a ) > fabs( xI_b )))
						{
							{
								xI_return = xI_a;
								xI_exit = true;
								break;
							};
						;
						}
						;
						if ((fabs( xI_a ) < fabs( xI_b )))
						{
							{
								xI_return = xI_b;
								xI_exit = true;
								break;
							};
						;
						}
						;
						{
							xI_return = max( xI_a, xI_b );
							xI_exit = true;
							break;
						};
					
					} while(false);
					// end MaxMag(f1;f1;;
					
					// MaxMag(f1;f1;
					const float xJ_a = xU_a1.w;
					const float xJ_b = xU_b1.w;
					float xJ_return = ((float)( 0 ));
					int xJ_exit = ((int)( false ));
					do {
						if ((fabs( xJ_a ) > fabs( xJ_b )))
						{
							{
								xJ_return = xJ_a;
								xJ_exit = true;
								break;
							};
						;
						}
						;
						if ((fabs( xJ_a ) < fabs( xJ_b )))
						{
							{
								xJ_return = xJ_b;
								xJ_exit = true;
								break;
							};
						;
						}
						;
						{
							xJ_return = max( xJ_a, xJ_b );
							xJ_exit = true;
							break;
						};
					
					} while(false);
					// end MaxMag(f1;f1;;
					{
						xU_return = ((float4)( xM_return, xV_return, xI_return, xJ_return ));
						xU_exit = true;
						break;
					};
				
				} while(false);
				// end MaxMag(vf4;vf4;;
				value = xU_return;
			}
			;
		}
		;
		write_imagef(un_DstImage, coord, value);
	}
}

)#"_str );

	descr.Compute().AddSpirv( EShaderLangFormat::Vulkan_100 | EShaderLangFormat::SPIRV, { 
0x07230203, 0x00010000, 0x00080006, 0x00000088, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000005, 0x00000005, 0x6E69616D, 0x00000000, 0x0000003D, 0x00000045, 0x00060010, 
0x00000005, 0x00000011, 0x00000004, 0x00000004, 0x00000001, 0x00030007, 0x00000001, 0x00000000, 0x00410003, 0x00000002, 0x000001C2, 0x00000001, 
0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 
0x6F725065, 0x73736563, 0x61206465, 0x2D6F7475, 0x2D70616D, 0x61636F6C, 0x6E6F6974, 0x2F2F0A73, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 
0x61206465, 0x2D6F7475, 0x2D70616D, 0x646E6962, 0x73676E69, 0x202F2F0A, 0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6C632064, 0x746E6569, 
0x6C757620, 0x316E616B, 0x2F0A3030, 0x704F202F, 0x75646F4D, 0x7250656C, 0x7365636F, 0x20646573, 0x67726174, 0x652D7465, 0x7620766E, 0x616B6C75, 
0x302E316E, 0x202F2F0A, 0x6F4D704F, 0x656C7564, 0x636F7250, 0x65737365, 0x6E652064, 0x2D797274, 0x6E696F70, 0x616D2074, 0x230A6E69, 0x656E696C, 
0x000A3120, 0x00090004, 0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00090004, 0x415F4C47, 
0x735F4252, 0x69646168, 0x6C5F676E, 0x75676E61, 0x5F656761, 0x70303234, 0x006B6361, 0x000A0004, 0x475F4C47, 0x4C474F4F, 0x70635F45, 0x74735F70, 
0x5F656C79, 0x656E696C, 0x7269645F, 0x69746365, 0x00006576, 0x00080004, 0x475F4C47, 0x4C474F4F, 0x6E695F45, 0x64756C63, 0x69645F65, 0x74636572, 
0x00657669, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00060005, 0x0000000B, 0x4D78614D, 0x66286761, 0x31663B31, 0x0000003B, 0x00030005, 
0x00000009, 0x00000061, 0x00030005, 0x0000000A, 0x00000062, 0x00060005, 0x00000011, 0x4D78614D, 0x76286761, 0x763B3466, 0x003B3466, 0x00030005, 
0x0000000F, 0x00003161, 0x00030005, 0x00000010, 0x00003162, 0x00040005, 0x0000003A, 0x726F6F63, 0x00000064, 0x00080005, 0x0000003D, 0x475F6C67, 
0x61626F6C, 0x766E496C, 0x7461636F, 0x496E6F69, 0x00000044, 0x00030005, 0x00000043, 0x00786469, 0x00080005, 0x00000045, 0x4C5F6C67, 0x6C61636F, 
0x6F766E49, 0x69746163, 0x6E496E6F, 0x00786564, 0x00060005, 0x0000004B, 0x72616873, 0x654D6465, 0x79726F6D, 0x00000000, 0x00050005, 0x0000004F, 
0x535F6E75, 0x6D496372, 0x00656761, 0x00040005, 0x00000057, 0x756C6176, 0x00000065, 0x00030005, 0x0000005A, 0x00000079, 0x00030005, 0x00000065, 
0x00000078, 0x00040005, 0x0000006E, 0x66666964, 0x00000000, 0x00050005, 0x00000083, 0x445F6E75, 0x6D497473, 0x00656761, 0x00040047, 0x0000003D, 
0x0000000B, 0x0000001C, 0x00040047, 0x00000045, 0x0000000B, 0x0000001D, 0x00040047, 0x0000004F, 0x00000022, 0x00000000, 0x00040047, 0x0000004F, 
0x00000021, 0x00000001, 0x00030047, 0x0000004F, 0x00000018, 0x00040047, 0x00000083, 0x00000022, 0x00000000, 0x00040047, 0x00000083, 0x00000021, 
0x00000000, 0x00030047, 0x00000083, 0x00000019, 0x00040047, 0x00000087, 0x0000000B, 0x00000019, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 
0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00050021, 0x00000008, 0x00000007, 0x00000007, 0x00000007, 0x00040017, 0x0000000D, 0x00000007, 
0x00000004, 0x00050021, 0x0000000E, 0x0000000D, 0x0000000D, 0x0000000D, 0x00020014, 0x00000015, 0x00040015, 0x00000023, 0x00000020, 0x00000000, 
0x0004002B, 0x00000023, 0x00000024, 0x00000000, 0x0004002B, 0x00000023, 0x00000028, 0x00000001, 0x0004002B, 0x00000023, 0x0000002C, 0x00000002, 
0x0004002B, 0x00000023, 0x00000030, 0x00000003, 0x00040015, 0x00000037, 0x00000020, 0x00000001, 0x00040017, 0x00000038, 0x00000037, 0x00000002, 
0x00040020, 0x00000039, 0x00000007, 0x00000038, 0x00040017, 0x0000003B, 0x00000023, 0x00000003, 0x00040020, 0x0000003C, 0x00000001, 0x0000003B, 
0x0004003B, 0x0000003C, 0x0000003D, 0x00000001, 0x00040017, 0x0000003F, 0x00000037, 0x00000003, 0x00040020, 0x00000042, 0x00000007, 0x00000037, 
0x00040020, 0x00000044, 0x00000001, 0x00000023, 0x0004003B, 0x00000044, 0x00000045, 0x00000001, 0x0004002B, 0x00000023, 0x00000048, 0x00000010, 
0x0004001C, 0x00000049, 0x0000000D, 0x00000048, 0x00040020, 0x0000004A, 0x00000004, 0x00000049, 0x0004003B, 0x0000004A, 0x0000004B, 0x00000004, 
0x00090019, 0x0000004D, 0x00000007, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000001, 0x00040020, 0x0000004E, 0x00000000, 
0x0000004D, 0x0004003B, 0x0000004E, 0x0000004F, 0x00000000, 0x00040020, 0x00000053, 0x00000004, 0x0000000D, 0x0004002B, 0x00000023, 0x00000055, 
0x00000108, 0x00040020, 0x00000056, 0x00000007, 0x0000000D, 0x0004002B, 0x00000007, 0x00000058, 0x00000000, 0x0007002C, 0x0000000D, 0x00000059, 
0x00000058, 0x00000058, 0x00000058, 0x00000058, 0x0004002B, 0x00000037, 0x0000005B, 0x00000000, 0x0004002B, 0x00000023, 0x00000063, 0x00000004, 
0x0004002B, 0x00000037, 0x0000007F, 0x00000001, 0x0004003B, 0x0000004E, 0x00000083, 0x00000000, 0x0006002C, 0x0000003B, 0x00000087, 0x00000063, 
0x00000063, 0x00000028, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x0004003B, 0x00000039, 0x0000003A, 
0x00000007, 0x0004003B, 0x00000042, 0x00000043, 0x00000007, 0x0004003B, 0x00000056, 0x00000057, 0x00000007, 0x0004003B, 0x00000042, 0x0000005A, 
0x00000007, 0x0004003B, 0x00000042, 0x00000065, 0x00000007, 0x0004003B, 0x00000056, 0x0000006E, 0x00000007, 0x00040008, 0x00000001, 0x00000024, 
0x00000000, 0x0004003D, 0x0000003B, 0x0000003E, 0x0000003D, 0x0004007C, 0x0000003F, 0x00000040, 0x0000003E, 0x0007004F, 0x00000038, 0x00000041, 
0x00000040, 0x00000040, 0x00000000, 0x00000001, 0x0003003E, 0x0000003A, 0x00000041, 0x00040008, 0x00000001, 0x00000026, 0x00000000, 0x0004003D, 
0x00000023, 0x00000046, 0x00000045, 0x0004007C, 0x00000037, 0x00000047, 0x00000046, 0x0003003E, 0x00000043, 0x00000047, 0x00040008, 0x00000001, 
0x00000028, 0x00000000, 0x0004003D, 0x00000037, 0x0000004C, 0x00000043, 0x0004003D, 0x0000004D, 0x00000050, 0x0000004F, 0x0004003D, 0x00000038, 
0x00000051, 0x0000003A, 0x00050062, 0x0000000D, 0x00000052, 0x00000050, 0x00000051, 0x00050041, 0x00000053, 0x00000054, 0x0000004B, 0x0000004C, 
0x0003003E, 0x00000054, 0x00000052, 0x00040008, 0x00000001, 0x00000029, 0x00000000, 0x000400E0, 0x0000002C, 0x0000002C, 0x00000055, 0x00040008, 
0x00000001, 0x0000002A, 0x00000000, 0x0003003E, 0x00000057, 0x00000059, 0x00040008, 0x00000001, 0x0000002C, 0x00000000, 0x0003003E, 0x0000005A, 
0x0000005B, 0x000200F9, 0x0000005C, 0x000200F8, 0x0000005C, 0x000400F6, 0x0000005E, 0x0000005F, 0x00000000, 0x000200F9, 0x00000060, 0x000200F8, 
0x00000060, 0x0004003D, 0x00000037, 0x00000061, 0x0000005A, 0x0004007C, 0x00000023, 0x00000062, 0x00000061, 0x000500B0, 0x00000015, 0x00000064, 
0x00000062, 0x00000063, 0x000400FA, 0x00000064, 0x0000005D, 0x0000005E, 0x000200F8, 0x0000005D, 0x00040008, 0x00000001, 0x0000002E, 0x00000000, 
0x0003003E, 0x00000065, 0x0000005B, 0x000200F9, 0x00000066, 0x000200F8, 0x00000066, 0x000400F6, 0x00000068, 0x00000069, 0x00000000, 0x000200F9, 
0x0000006A, 0x000200F8, 0x0000006A, 0x0004003D, 0x00000037, 0x0000006B, 0x00000065, 0x0004007C, 0x00000023, 0x0000006C, 0x0000006B, 0x000500B0, 
0x00000015, 0x0000006D, 0x0000006C, 0x00000063, 0x000400FA, 0x0000006D, 0x00000067, 0x00000068, 0x000200F8, 0x00000067, 0x00040008, 0x00000001, 
0x00000030, 0x00000000, 0x0004003D, 0x00000037, 0x0000006F, 0x00000043, 0x00050041, 0x00000053, 0x00000070, 0x0000004B, 0x0000006F, 0x0004003D, 
0x0000000D, 0x00000071, 0x00000070, 0x0004003D, 0x00000037, 0x00000072, 0x00000065, 0x0004007C, 0x00000023, 0x00000073, 0x00000072, 0x0004003D, 
0x00000037, 0x00000074, 0x0000005A, 0x0004007C, 0x00000023, 0x00000075, 0x00000074, 0x00050084, 0x00000023, 0x00000076, 0x00000075, 0x00000063, 
0x00050080, 0x00000023, 0x00000077, 0x00000073, 0x00000076, 0x00050041, 0x00000053, 0x00000078, 0x0000004B, 0x00000077, 0x0004003D, 0x0000000D, 
0x00000079, 0x00000078, 0x00050083, 0x0000000D, 0x0000007A, 0x00000071, 0x00000079, 0x0003003E, 0x0000006E, 0x0000007A, 0x00040008, 0x00000001, 
0x00000032, 0x00000000, 0x0004003D, 0x0000000D, 0x0000007B, 0x00000057, 0x0004003D, 0x0000000D, 0x0000007C, 0x0000006E, 0x00060039, 0x0000000D, 
0x0000007D, 0x00000011, 0x0000007B, 0x0000007C, 0x0003003E, 0x00000057, 0x0000007D, 0x000200F9, 0x00000069, 0x000200F8, 0x00000069, 0x00040008, 
0x00000001, 0x0000002E, 0x00000000, 0x0004003D, 0x00000037, 0x0000007E, 0x00000065, 0x00050080, 0x00000037, 0x00000080, 0x0000007E, 0x0000007F, 
0x0003003E, 0x00000065, 0x00000080, 0x000200F9, 0x00000066, 0x000200F8, 0x00000068, 0x000200F9, 0x0000005F, 0x000200F8, 0x0000005F, 0x00040008, 
0x00000001, 0x0000002C, 0x00000000, 0x0004003D, 0x00000037, 0x00000081, 0x0000005A, 0x00050080, 0x00000037, 0x00000082, 0x00000081, 0x0000007F, 
0x0003003E, 0x0000005A, 0x00000082, 0x000200F9, 0x0000005C, 0x000200F8, 0x0000005E, 0x00040008, 0x00000001, 0x00000037, 0x00000000, 0x0004003D, 
0x0000004D, 0x00000084, 0x00000083, 0x0004003D, 0x00000038, 0x00000085, 0x0000003A, 0x0004003D, 0x0000000D, 0x00000086, 0x00000057, 0x00040063, 
0x00000084, 0x00000085, 0x00000086, 0x000100FD, 0x00010038, 0x00050036, 0x00000007, 0x0000000B, 0x00000000, 0x00000008, 0x00030037, 0x00000007, 
0x00000009, 0x00030037, 0x00000007, 0x0000000A, 0x000200F8, 0x0000000C, 0x00040008, 0x00000001, 0x0000000E, 0x00000000, 0x0006000C, 0x00000007, 
0x00000013, 0x00000002, 0x00000004, 0x00000009, 0x0006000C, 0x00000007, 0x00000014, 0x00000002, 0x00000004, 0x0000000A, 0x000500BA, 0x00000015, 
0x00000016, 0x00000013, 0x00000014, 0x000300F7, 0x00000018, 0x00000000, 0x000400FA, 0x00000016, 0x00000017, 0x00000018, 0x000200F8, 0x00000017, 
0x00040008, 0x00000001, 0x00000010, 0x00000000, 0x000200FE, 0x00000009, 0x000200F8, 0x00000018, 0x00040008, 0x00000001, 0x00000013, 0x00000000, 
0x0006000C, 0x00000007, 0x0000001A, 0x00000002, 0x00000004, 0x00000009, 0x0006000C, 0x00000007, 0x0000001B, 0x00000002, 0x00000004, 0x0000000A, 
0x000500B8, 0x00000015, 0x0000001C, 0x0000001A, 0x0000001B, 0x000300F7, 0x0000001E, 0x00000000, 0x000400FA, 0x0000001C, 0x0000001D, 0x0000001E, 
0x000200F8, 0x0000001D, 0x00040008, 0x00000001, 0x00000015, 0x00000000, 0x000200FE, 0x0000000A, 0x000200F8, 0x0000001E, 0x00040008, 0x00000001, 
0x00000018, 0x00000000, 0x0007000C, 0x00000007, 0x00000020, 0x00000002, 0x00000028, 0x00000009, 0x0000000A, 0x000200FE, 0x00000020, 0x00010038, 
0x00050036, 0x0000000D, 0x00000011, 0x00000000, 0x0000000E, 0x00030037, 0x0000000D, 0x0000000F, 0x00030037, 0x0000000D, 0x00000010, 0x000200F8, 
0x00000012, 0x00040008, 0x00000001, 0x0000001E, 0x00000000, 0x00050051, 0x00000007, 0x00000025, 0x0000000F, 0x00000000, 0x00050051, 0x00000007, 
0x00000026, 0x00000010, 0x00000000, 0x00060039, 0x00000007, 0x00000027, 0x0000000B, 0x00000025, 0x00000026, 0x00050051, 0x00000007, 0x00000029, 
0x0000000F, 0x00000001, 0x00050051, 0x00000007, 0x0000002A, 0x00000010, 0x00000001, 0x00060039, 0x00000007, 0x0000002B, 0x0000000B, 0x00000029, 
0x0000002A, 0x00050051, 0x00000007, 0x0000002D, 0x0000000F, 0x00000002, 0x00050051, 0x00000007, 0x0000002E, 0x00000010, 0x00000002, 0x00060039, 
0x00000007, 0x0000002F, 0x0000000B, 0x0000002D, 0x0000002E, 0x00050051, 0x00000007, 0x00000031, 0x0000000F, 0x00000003, 0x00050051, 0x00000007, 
0x00000032, 0x00000010, 0x00000003, 0x00060039, 0x00000007, 0x00000033, 0x0000000B, 0x00000031, 0x00000032, 0x00070050, 0x0000000D, 0x00000034, 
0x00000027, 0x0000002B, 0x0000002F, 0x00000033, 0x000200FE, 0x00000034, 0x00010038 });

#ifdef GRAPHICS_API_SOFT
	descr.Compute().AddInvocable( EShaderLangFormat::Software_100 | EShaderLangFormat::CPP_Invocable, &SWShaderLang::sw_shaderbarrier_comp );
#endif


};
}	// Pipelines
