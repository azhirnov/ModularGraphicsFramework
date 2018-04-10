// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Common_TypeInfo (OUT String &src)
	{
		src << R"#(/*
	This is generated file, don't change anything!
*/

// MinValue, MaxValue
float MinValue (const float x);
float MaxValue (const float x);
float2 MinValue (const float2 x);
float2 MaxValue (const float2 x);
float3 MinValue (const float3 x);
float3 MaxValue (const float3 x);
float4 MinValue (const float4 x);
float4 MaxValue (const float4 x);

double MinValue (const double x);
double MaxValue (const double x);
double2 MinValue (const double2 x);
double2 MaxValue (const double2 x);
double3 MinValue (const double3 x);
double3 MaxValue (const double3 x);
double4 MinValue (const double4 x);
double4 MaxValue (const double4 x);

int MinValue (const int x);
int MaxValue (const int x);
int2 MinValue (const int2 x);
int2 MaxValue (const int2 x);
int3 MinValue (const int3 x);
int3 MaxValue (const int3 x);
int4 MinValue (const int4 x);
int4 MaxValue (const int4 x);

uint MinValue (const uint x);
uint MaxValue (const uint x);
uint2 MinValue (const uint2 x);
uint2 MaxValue (const uint2 x);
uint3 MinValue (const uint3 x);
uint3 MaxValue (const uint3 x);
uint4 MinValue (const uint4 x);
uint4 MaxValue (const uint4 x);

#ifdef ARB_gpu_shader_int64_enabled
ilong MinValue (const ilong x);
ilong MaxValue (const ilong x);
ilong2 MinValue (const ilong2 x);
ilong2 MaxValue (const ilong2 x);
ilong3 MinValue (const ilong3 x);
ilong3 MaxValue (const ilong3 x);
ilong4 MinValue (const ilong4 x);
ilong4 MaxValue (const ilong4 x);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong MinValue (const ulong x);
ulong MaxValue (const ulong x);
ulong2 MinValue (const ulong2 x);
ulong2 MaxValue (const ulong2 x);
ulong3 MinValue (const ulong3 x);
ulong3 MaxValue (const ulong3 x);
ulong4 MinValue (const ulong4 x);
ulong4 MaxValue (const ulong4 x);
#endif



//---------------------------------------------------------

// MinValue, MaxValue
float MinValue_float () { return 1.175494e-38F; }
float MaxValue_float () { return 3.402823e+38F; }
float MinValue (const float x) { return MinValue_float(); }
float MaxValue (const float x) { return MaxValue_float(); }
float2 MinValue (const float2 x) { return float2( MinValue_float(), MinValue_float()); }
float2 MaxValue (const float2 x) { return float2( MaxValue_float(), MaxValue_float()); }
float3 MinValue (const float3 x) { return float3( MinValue_float(), MinValue_float(), MinValue_float()); }
float3 MaxValue (const float3 x) { return float3( MaxValue_float(), MaxValue_float(), MaxValue_float()); }
float4 MinValue (const float4 x) { return float4( MinValue_float(), MinValue_float(), MinValue_float(), MinValue_float()); }
float4 MaxValue (const float4 x) { return float4( MaxValue_float(), MaxValue_float(), MaxValue_float(), MaxValue_float()); }

double MinValue_double () { return 2.225074e-308LF; }
double MaxValue_double () { return 1.797693e+308LF; }
double MinValue (const double x) { return MinValue_double(); }
double MaxValue (const double x) { return MaxValue_double(); }
double2 MinValue (const double2 x) { return double2( MinValue_double(), MinValue_double()); }
double2 MaxValue (const double2 x) { return double2( MaxValue_double(), MaxValue_double()); }
double3 MinValue (const double3 x) { return double3( MinValue_double(), MinValue_double(), MinValue_double()); }
double3 MaxValue (const double3 x) { return double3( MaxValue_double(), MaxValue_double(), MaxValue_double()); }
double4 MinValue (const double4 x) { return double4( MinValue_double(), MinValue_double(), MinValue_double(), MinValue_double()); }
double4 MaxValue (const double4 x) { return double4( MaxValue_double(), MaxValue_double(), MaxValue_double(), MaxValue_double()); }

int MinValue_int () { return -2147483648; }
int MaxValue_int () { return 2147483647; }
int MinValue (const int x) { return MinValue_int(); }
int MaxValue (const int x) { return MaxValue_int(); }
int2 MinValue (const int2 x) { return int2( MinValue_int(), MinValue_int()); }
int2 MaxValue (const int2 x) { return int2( MaxValue_int(), MaxValue_int()); }
int3 MinValue (const int3 x) { return int3( MinValue_int(), MinValue_int(), MinValue_int()); }
int3 MaxValue (const int3 x) { return int3( MaxValue_int(), MaxValue_int(), MaxValue_int()); }
int4 MinValue (const int4 x) { return int4( MinValue_int(), MinValue_int(), MinValue_int(), MinValue_int()); }
int4 MaxValue (const int4 x) { return int4( MaxValue_int(), MaxValue_int(), MaxValue_int(), MaxValue_int()); }

uint MinValue_uint () { return 0x0U; }
uint MaxValue_uint () { return 0xFFFFFFFFU; }
uint MinValue (const uint x) { return MinValue_uint(); }
uint MaxValue (const uint x) { return MaxValue_uint(); }
uint2 MinValue (const uint2 x) { return uint2( MinValue_uint(), MinValue_uint()); }
uint2 MaxValue (const uint2 x) { return uint2( MaxValue_uint(), MaxValue_uint()); }
uint3 MinValue (const uint3 x) { return uint3( MinValue_uint(), MinValue_uint(), MinValue_uint()); }
uint3 MaxValue (const uint3 x) { return uint3( MaxValue_uint(), MaxValue_uint(), MaxValue_uint()); }
uint4 MinValue (const uint4 x) { return uint4( MinValue_uint(), MinValue_uint(), MinValue_uint(), MinValue_uint()); }
uint4 MaxValue (const uint4 x) { return uint4( MaxValue_uint(), MaxValue_uint(), MaxValue_uint(), MaxValue_uint()); }

#ifdef ARB_gpu_shader_int64_enabled
ilong MinValue_ilong () { return -9223372036854775808L; }
ilong MaxValue_ilong () { return 9223372036854775807L; }
ilong MinValue (const ilong x) { return MinValue_ilong(); }
ilong MaxValue (const ilong x) { return MaxValue_ilong(); }
ilong2 MinValue (const ilong2 x) { return ilong2( MinValue_ilong(), MinValue_ilong()); }
ilong2 MaxValue (const ilong2 x) { return ilong2( MaxValue_ilong(), MaxValue_ilong()); }
ilong3 MinValue (const ilong3 x) { return ilong3( MinValue_ilong(), MinValue_ilong(), MinValue_ilong()); }
ilong3 MaxValue (const ilong3 x) { return ilong3( MaxValue_ilong(), MaxValue_ilong(), MaxValue_ilong()); }
ilong4 MinValue (const ilong4 x) { return ilong4( MinValue_ilong(), MinValue_ilong(), MinValue_ilong(), MinValue_ilong()); }
ilong4 MaxValue (const ilong4 x) { return ilong4( MaxValue_ilong(), MaxValue_ilong(), MaxValue_ilong(), MaxValue_ilong()); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong MinValue_ulong () { return 0x0UL; }
ulong MaxValue_ulong () { return 0xFFFFFFFFFFFFFFFFUL; }
ulong MinValue (const ulong x) { return MinValue_ulong(); }
ulong MaxValue (const ulong x) { return MaxValue_ulong(); }
ulong2 MinValue (const ulong2 x) { return ulong2( MinValue_ulong(), MinValue_ulong()); }
ulong2 MaxValue (const ulong2 x) { return ulong2( MaxValue_ulong(), MaxValue_ulong()); }
ulong3 MinValue (const ulong3 x) { return ulong3( MinValue_ulong(), MinValue_ulong(), MinValue_ulong()); }
ulong3 MaxValue (const ulong3 x) { return ulong3( MaxValue_ulong(), MaxValue_ulong(), MaxValue_ulong()); }
ulong4 MinValue (const ulong4 x) { return ulong4( MinValue_ulong(), MinValue_ulong(), MinValue_ulong(), MinValue_ulong()); }
ulong4 MaxValue (const ulong4 x) { return ulong4( MaxValue_ulong(), MaxValue_ulong(), MaxValue_ulong(), MaxValue_ulong()); }
#endif

)#";

	}
}	// glsl_vfs
