// This is generated file, don't change anything!

#include "cl_source_vfs.h"

namespace cl_vfs
{
	extern void VFS_funcs (OUT String &src)
	{
		src << R"#(// Functions for GLSL compatibility

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
)#";

	}
}	// cl_vfs
