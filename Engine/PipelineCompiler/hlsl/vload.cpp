// This is generated file, don't change anything!

#include "hlsl_source_vfs.h"

namespace hlsl_vfs
{
	extern void VFS_vload (OUT String &src)
	{
		src << R"#(//

#define vload_float1( __offset, __buf )		asfloat(__buf.Load(  __offset ))
#define vload_float2( __offset, __buf )		asfloat(__buf.Load2( __offset ))
#define vload_float3( __offset, __buf )		asfloat(__buf.Load3( __offset ))
#define vload_float4( __offset, __buf )		asfloat(__buf.Load4( __offset ))

#define vload_int1( __offset, __buf )		asint(__buf.Load(  __offset ))
#define vload_int2( __offset, __buf )		asint(__buf.Load2( __offset ))
#define vload_int3( __offset, __buf )		asint(__buf.Load3( __offset ))
#define vload_int4( __offset, __buf )		asint(__buf.Load4( __offset ))

#define vload_uint1( __offset, __buf )		__buf.Load(  __offset )
#define vload_uint2( __offset, __buf )		__buf.Load2( __offset )
#define vload_uint3( __offset, __buf )		__buf.Load3( __offset )
#define vload_uint4( __offset, __buf )		__buf.Load4( __offset )

#define vload_bool1( __offset, __buf )		(__buf.Load(  __offset ) != 0)
#define vload_bool2( __offset, __buf )		(__buf.Load2( __offset ) != uint2(0,0))
#define vload_bool3( __offset, __buf )		(__buf.Load3( __offset ) != uint3(0,0,0))
#define vload_bool4( __offset, __buf )		(__buf.Load4( __offset ) != uint4(0,0,0,0))


#define vload_float2x2( __offset, __buf )	float2x2( asfloat(__buf.Load2( __offset )), asfloat(__buf.Load2( __offset + 16 )) )
#define vload_float2x3( __offset, __buf )	float2x3( asfloat(__buf.Load3( __offset )), asfloat(__buf.Load3( __offset + 16 )) )
#define vload_float2x4( __offset, __buf )	float2x4( asfloat(__buf.Load4( __offset )), asfloat(__buf.Load4( __offset + 16 )) )

#define vload_float3x2( __offset, __buf )	float3x2( asfloat(__buf.Load2( __offset )), asfloat(__buf.Load2( __offset + 16 )), asfloat(__buf.Load2( __offset + 32 )) )
#define vload_float3x3( __offset, __buf )	float3x3( asfloat(__buf.Load3( __offset )), asfloat(__buf.Load3( __offset + 16 )), asfloat(__buf.Load3( __offset + 32 )) )
#define vload_float3x4( __offset, __buf )	float3x4( asfloat(__buf.Load4( __offset )), asfloat(__buf.Load4( __offset + 16 )), asfloat(__buf.Load4( __offset + 32 )) )

#define vload_float4x2( __offset, __buf )	float4x2( asfloat(__buf.Load2( __offset )), asfloat(__buf.Load2( __offset + 16 )), asfloat(__buf.Load2( __offset + 32 )), asfloat(__buf.Load2( __offset + 48 )) )
#define vload_float4x3( __offset, __buf )	float4x3( asfloat(__buf.Load3( __offset )), asfloat(__buf.Load3( __offset + 16 )), asfloat(__buf.Load3( __offset + 32 )), asfloat(__buf.Load3( __offset + 48 )) )
#define vload_float4x4( __offset, __buf )	float4x4( asfloat(__buf.Load4( __offset )), asfloat(__buf.Load4( __offset + 16 )), asfloat(__buf.Load4( __offset + 32 )), asfloat(__buf.Load4( __offset + 48 )) )


#define vstore_float1( __data, __offset, __buf )	__buf.Store(  __offset, asuint(__data) )
#define vstore_float2( __data, __offset, __buf )	__buf.Store2( __offset, asuint(__data) )
#define vstore_float3( __data, __offset, __buf )	__buf.Store3( __offset, asuint(__data) )
#define vstore_float4( __data, __offset, __buf )	__buf.Store4( __offset, asuint(__data) )

#define vstore_int1( __data, __offset, __buf )		__buf.Store(  __offset, asuint(__data) )
#define vstore_int2( __data, __offset, __buf )		__buf.Store2( __offset, asuint(__data) )
#define vstore_int3( __data, __offset, __buf )		__buf.Store3( __offset, asuint(__data) )
#define vstore_int4( __data, __offset, __buf )		__buf.Store4( __offset, asuint(__data) )

#define vstore_uint1( __data, __offset, __buf )		__buf.Store(  __offset, __data )
#define vstore_uint2( __data, __offset, __buf )		__buf.Store2( __offset, __data )
#define vstore_uint3( __data, __offset, __buf )		__buf.Store3( __offset, __data )
#define vstore_uint4( __data, __offset, __buf )		__buf.Store4( __offset, __data )

#define vstore_bool1( __data, __offset, __buf )		__buf.Store(  __offset, uint(__data) )
#define vstore_bool2( __data, __offset, __buf )		__buf.Store2( __offset, uint2(__data) )
#define vstore_bool3( __data, __offset, __buf )		__buf.Store3( __offset, uint3(__data) )
#define vstore_bool4( __data, __offset, __buf )		__buf.Store4( __offset, uint4(__data) )


#define vstore_float2x2( __data, __offset, __buf )	{ __buf.Store2( __offset, asuint(__data[0]) ); __buf.Store2( __offset, asuint(__data[1]) ); }
#define vstore_float2x3( __data, __offset, __buf )	{ __buf.Store3( __offset, asuint(__data[0]) ); __buf.Store3( __offset, asuint(__data[1]) ); }
#define vstore_float2x4( __data, __offset, __buf )	{ __buf.Store4( __offset, asuint(__data[0]) ); __buf.Store4( __offset, asuint(__data[1]) ); }

#define vstore_float3x2( __data, __offset, __buf )	{ __buf.Store2( __offset, asuint(__data[0]) ); __buf.Store2( __offset, asuint(__data[1]) ); __buf.Store2( __offset, asuint(__data[2]) ); }
#define vstore_float3x3( __data, __offset, __buf )	{ __buf.Store3( __offset, asuint(__data[0]) ); __buf.Store3( __offset, asuint(__data[1]) ); __buf.Store3( __offset, asuint(__data[2]) ); }
#define vstore_float3x4( __data, __offset, __buf )	{ __buf.Store4( __offset, asuint(__data[0]) ); __buf.Store4( __offset, asuint(__data[1]) ); __buf.Store4( __offset, asuint(__data[2]) ); }

#define vstore_float4x2( __data, __offset, __buf )	{ __buf.Store2( __offset, asuint(__data[0]) ); __buf.Store2( __offset, asuint(__data[1]) ); __buf.Store2( __offset, asuint(__data[2]) ); __buf.Store2( __offset, asuint(__data[3]) ); }
#define vstore_float4x3( __data, __offset, __buf )	{ __buf.Store3( __offset, asuint(__data[0]) ); __buf.Store3( __offset, asuint(__data[1]) ); __buf.Store3( __offset, asuint(__data[2]) ); __buf.Store3( __offset, asuint(__data[3]) ); }
#define vstore_float4x4( __data, __offset, __buf )	{ __buf.Store4( __offset, asuint(__data[0]) ); __buf.Store4( __offset, asuint(__data[1]) ); __buf.Store4( __offset, asuint(__data[2]) ); __buf.Store4( __offset, asuint(__data[3]) ); }
)#";

	}
}	// hlsl_vfs
