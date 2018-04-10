// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Common_Defines (OUT String &src)
	{
		src << R"#(/*
	Some helper macro.
*/


// to mark 'out' and 'inout' argument in function call
// in function argument list use defined by GLSL qualificators: in, out, inout
#define OUT
#define INOUT

#define CountOf( _arr_ )	(_arr_).length


// for each
#define FOR( _counter_, _array_ ) \
	for (int _counter_ = 0; i < CountOf(_array_); ++i)

// reverse for each
#define FOR_rev( _counter_, _array_ ) \
	for (int _counter_ = CountOf(_array_)-1; i >= 0; --i)


// image
// example: DECL_IMAGE( image2D, rgba8, readonly, inImage )
//	_access_: readonly, writeonly, coherent, volatile, restrict

//#define DECL_IMAGE( _imgType_, _format_, _access_, _name_ )	
//	layout(_format_) _access_ uniform _imgType_   _name_;


// texture
// example: DECL_TEXTURE( sampler2D, inTexture )

//#define DECL_TEXTURE( _texType_, _name_ ) 
//	uniform _texType_  _name_;


// shader storage buffer
// example: DECL_STORAGE( std140, struct Vertex {}, readonly, inBuffer )
//	_access_  : readonly, writeonly, coherent, volatile, restrict
//	_packType : shared, packed, std140, std430
//	_format_  : float, struct {...}

//#define DECL_STORAGE( _packType_, _format_, _access_, _name_ ) 
//	layout(_packType_) _access_ buffer SSBO_##_name_ { _format_  _name_[]; };

//#define DECL_BUFFER( _packType_, _format_, _access_, _name_ ) 
//	DECL_STORAGE( _packType_, _format_, _access_, _name_ )


// uniform buffer (readonly, fixed size, fast access)
// example: DECL_UBLOCK( std140, struct Block {}, UB )
//	_packType : shared, packed, std140

//#define DECL_UBLOCK( _packType, _format_, _name_ ) 
//	layout(_packType_) uniform UBO_##_name_ { _format_ _name_; }


#define and		&&
#define or		||
#define not		!


#if !defined(GX_REAL_TYPE_SIZE)
#	define GX_REAL_TYPE_SIZE	32
#endif

#if GX_REAL_TYPE_SIZE == 32

#	define real			float
#	define real2		float2
#	define real3		float3
#	define real4		float4
#	define real2x2		float2x2
#	define real2x3		float2x3
#	define real2x4		float2x4
#	define real3x2		float3x2
#	define real3x3		float3x3
#	define real3x4		float3x4
#	define real4x2		float4x2
#	define real4x3		float4x3
#	define real4x4		float4x4

#elif GX_REAL_TYPE_SIZE == 64

#	define real			double
#	define real2		double2
#	define real3		double3
#	define real4		double4
#	define real2x2		double2x2
#	define real2x3		double2x3
#	define real2x4		double2x4
#	define real3x2		double3x2
#	define real3x3		double3x3
#	define real3x4		double3x4
#	define real4x2		double4x2
#	define real4x3		double4x3
#	define real4x4		double4x4

#else
#	error GX_REAL_TYPE_SIZE incorrect value
#endif

)#";

	}
}	// glsl_vfs
