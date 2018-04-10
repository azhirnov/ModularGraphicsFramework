// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Common_Cmp (OUT String &src)
	{
		src << R"#(/*
	This is generated file, don't change anything!
*/

#define AllLess( _a_, _b_ )			All( Less( (_a_), (_b_) ) )
#define AllLessEqual( _a_, _b_ )	All( LessEqual( (_a_), (_b_) ) )

#define AllGreater( _a_, _b_ )		All( Greater( (_a_), (_b_) ) )
#define AllGreaterEqual( _a_, _b_ )	All( GreaterEqual( (_a_), (_b_) ) )

#define AnyLess( _a_, _b_ )			Any( Less( (_a_), (_b_) ) )
#define AnyLessEqual( _a_, _b_ )	Any( LessEqual( (_a_), (_b_) ) )

#define AnyGreater( _a_, _b_ )		Any( Greater( (_a_), (_b_) ) )
#define AnyGreaterEqual( _a_, _b_ )	Any( GreaterEqual( (_a_), (_b_) ) )

#define AllEqual( _a_, _b_ )		All( Equals( (_a_), (_b_) ) )
#define AnyEqual( _a_, _b_ )		Any( Equals( (_a_), (_b_) ) )

#define AllNotEqual( _a_, _b_ )		All( Not( Equals( (_a_), (_b_) ) ) )
#define AnyNotEqual( _a_, _b_ )		Any( Not( Equals( (_a_), (_b_) ) ) )

#define NotAllEqual( _a_, _b_ )		not All( Equals( (_a_), (_b_) ) )
#define NotAnyEqual( _a_, _b_ )		not Any( Equals( (_a_), (_b_) ) )


// Less, Greater, LessEqual, GreaterEqual
bool Less (const float x, const float y);
bool Greater (const float x, const float y);
bool LessEqual (const float x, const float y);
bool GreaterEqual (const float x, const float y);
bool2 Less (const float2 x, const float2 y);
bool2 Greater (const float2 x, const float2 y);
bool2 LessEqual (const float2 x, const float2 y);
bool2 GreaterEqual (const float2 x, const float2 y);
bool3 Less (const float3 x, const float3 y);
bool3 Greater (const float3 x, const float3 y);
bool3 LessEqual (const float3 x, const float3 y);
bool3 GreaterEqual (const float3 x, const float3 y);
bool4 Less (const float4 x, const float4 y);
bool4 Greater (const float4 x, const float4 y);
bool4 LessEqual (const float4 x, const float4 y);
bool4 GreaterEqual (const float4 x, const float4 y);

bool Less (const double x, const double y);
bool Greater (const double x, const double y);
bool LessEqual (const double x, const double y);
bool GreaterEqual (const double x, const double y);
bool2 Less (const double2 x, const double2 y);
bool2 Greater (const double2 x, const double2 y);
bool2 LessEqual (const double2 x, const double2 y);
bool2 GreaterEqual (const double2 x, const double2 y);
bool3 Less (const double3 x, const double3 y);
bool3 Greater (const double3 x, const double3 y);
bool3 LessEqual (const double3 x, const double3 y);
bool3 GreaterEqual (const double3 x, const double3 y);
bool4 Less (const double4 x, const double4 y);
bool4 Greater (const double4 x, const double4 y);
bool4 LessEqual (const double4 x, const double4 y);
bool4 GreaterEqual (const double4 x, const double4 y);

bool Less (const int x, const int y);
bool Greater (const int x, const int y);
bool LessEqual (const int x, const int y);
bool GreaterEqual (const int x, const int y);
bool2 Less (const int2 x, const int2 y);
bool2 Greater (const int2 x, const int2 y);
bool2 LessEqual (const int2 x, const int2 y);
bool2 GreaterEqual (const int2 x, const int2 y);
bool3 Less (const int3 x, const int3 y);
bool3 Greater (const int3 x, const int3 y);
bool3 LessEqual (const int3 x, const int3 y);
bool3 GreaterEqual (const int3 x, const int3 y);
bool4 Less (const int4 x, const int4 y);
bool4 Greater (const int4 x, const int4 y);
bool4 LessEqual (const int4 x, const int4 y);
bool4 GreaterEqual (const int4 x, const int4 y);

bool Less (const uint x, const uint y);
bool Greater (const uint x, const uint y);
bool LessEqual (const uint x, const uint y);
bool GreaterEqual (const uint x, const uint y);
bool2 Less (const uint2 x, const uint2 y);
bool2 Greater (const uint2 x, const uint2 y);
bool2 LessEqual (const uint2 x, const uint2 y);
bool2 GreaterEqual (const uint2 x, const uint2 y);
bool3 Less (const uint3 x, const uint3 y);
bool3 Greater (const uint3 x, const uint3 y);
bool3 LessEqual (const uint3 x, const uint3 y);
bool3 GreaterEqual (const uint3 x, const uint3 y);
bool4 Less (const uint4 x, const uint4 y);
bool4 Greater (const uint4 x, const uint4 y);
bool4 LessEqual (const uint4 x, const uint4 y);
bool4 GreaterEqual (const uint4 x, const uint4 y);

#ifdef ARB_gpu_shader_int64_enabled
bool Less (const ilong x, const ilong y);
bool Greater (const ilong x, const ilong y);
bool LessEqual (const ilong x, const ilong y);
bool GreaterEqual (const ilong x, const ilong y);
bool2 Less (const ilong2 x, const ilong2 y);
bool2 Greater (const ilong2 x, const ilong2 y);
bool2 LessEqual (const ilong2 x, const ilong2 y);
bool2 GreaterEqual (const ilong2 x, const ilong2 y);
bool3 Less (const ilong3 x, const ilong3 y);
bool3 Greater (const ilong3 x, const ilong3 y);
bool3 LessEqual (const ilong3 x, const ilong3 y);
bool3 GreaterEqual (const ilong3 x, const ilong3 y);
bool4 Less (const ilong4 x, const ilong4 y);
bool4 Greater (const ilong4 x, const ilong4 y);
bool4 LessEqual (const ilong4 x, const ilong4 y);
bool4 GreaterEqual (const ilong4 x, const ilong4 y);
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool Less (const ulong x, const ulong y);
bool Greater (const ulong x, const ulong y);
bool LessEqual (const ulong x, const ulong y);
bool GreaterEqual (const ulong x, const ulong y);
bool2 Less (const ulong2 x, const ulong2 y);
bool2 Greater (const ulong2 x, const ulong2 y);
bool2 LessEqual (const ulong2 x, const ulong2 y);
bool2 GreaterEqual (const ulong2 x, const ulong2 y);
bool3 Less (const ulong3 x, const ulong3 y);
bool3 Greater (const ulong3 x, const ulong3 y);
bool3 LessEqual (const ulong3 x, const ulong3 y);
bool3 GreaterEqual (const ulong3 x, const ulong3 y);
bool4 Less (const ulong4 x, const ulong4 y);
bool4 Greater (const ulong4 x, const ulong4 y);
bool4 LessEqual (const ulong4 x, const ulong4 y);
bool4 GreaterEqual (const ulong4 x, const ulong4 y);
#endif


// IsZero, Equals (with error)
float  EpsilonF ();
double EpsilonD ();
real   Epsilon ();

bool IsZero (const float x);
bool2 IsZero (const float2 x);
bool3 IsZero (const float3 x);
bool4 IsZero (const float4 x);

bool IsZero (const double x);
bool2 IsZero (const double2 x);
bool3 IsZero (const double3 x);
bool4 IsZero (const double4 x);

bool IsZero (const int x);
bool2 IsZero (const int2 x);
bool3 IsZero (const int3 x);
bool4 IsZero (const int4 x);

bool IsZero (const uint x);
bool2 IsZero (const uint2 x);
bool3 IsZero (const uint3 x);
bool4 IsZero (const uint4 x);

bool IsZero (const bool x);
bool2 IsZero (const bool2 x);
bool3 IsZero (const bool3 x);
bool4 IsZero (const bool4 x);

#ifdef ARB_gpu_shader_int64_enabled
bool IsZero (const ilong x);
bool2 IsZero (const ilong2 x);
bool3 IsZero (const ilong3 x);
bool4 IsZero (const ilong4 x);
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool IsZero (const ulong x);
bool2 IsZero (const ulong2 x);
bool3 IsZero (const ulong3 x);
bool4 IsZero (const ulong4 x);
#endif

bool Equals (const float left, const float right);
bool2 Equals (const float2 left, const float2 right);
bool3 Equals (const float3 left, const float3 right);
bool4 Equals (const float4 left, const float4 right);

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const double left, const double right);
bool2 Equals (const double2 left, const double2 right);
bool3 Equals (const double3 left, const double3 right);
bool4 Equals (const double4 left, const double4 right);
#endif

bool Equals (const int left, const int right);
bool2 Equals (const int2 left, const int2 right);
bool3 Equals (const int3 left, const int3 right);
bool4 Equals (const int4 left, const int4 right);

bool Equals (const uint left, const uint right);
bool2 Equals (const uint2 left, const uint2 right);
bool3 Equals (const uint3 left, const uint3 right);
bool4 Equals (const uint4 left, const uint4 right);

bool Equals (const bool left, const bool right);
bool2 Equals (const bool2 left, const bool2 right);
bool3 Equals (const bool3 left, const bool3 right);
bool4 Equals (const bool4 left, const bool4 right);

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const ilong left, const ilong right);
bool2 Equals (const ilong2 left, const ilong2 right);
bool3 Equals (const ilong3 left, const ilong3 right);
bool4 Equals (const ilong4 left, const ilong4 right);
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const ulong left, const ulong right);
bool2 Equals (const ulong2 left, const ulong2 right);
bool3 Equals (const ulong3 left, const ulong3 right);
bool4 Equals (const ulong4 left, const ulong4 right);
#endif

bool Equals (const float left, const float right, const uint accuracyInBits);
bool2 Equals (const float2 left, const float2 right, const uint accuracyInBits);
bool3 Equals (const float3 left, const float3 right, const uint accuracyInBits);
bool4 Equals (const float4 left, const float4 right, const uint accuracyInBits);

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const double left, const double right, const uint accuracyInBits);
bool2 Equals (const double2 left, const double2 right, const uint accuracyInBits);
bool3 Equals (const double3 left, const double3 right, const uint accuracyInBits);
bool4 Equals (const double4 left, const double4 right, const uint accuracyInBits);
#endif


// All, Any, Not
bool All (const bool x);
bool Any (const bool x);
bool Not (const bool x);
bool All (const bool2 x);
bool Any (const bool2 x);
bool2 Not (const bool2 x);
bool All (const bool3 x);
bool Any (const bool3 x);
bool3 Not (const bool3 x);
bool All (const bool4 x);
bool Any (const bool4 x);
bool4 Not (const bool4 x);


//---------------------------------------------------------


// Less, Greater, LessEqual, GreaterEqual
bool Less (const float x, const float y)  { return x < y; }
bool Greater (const float x, const float y)  { return x > y; }
bool LessEqual (const float x, const float y)  { return x <= y; }
bool GreaterEqual (const float x, const float y)  { return x >= y; }
bool2 Less (const float2 x, const float2 y)  { return lessThan( x, y ); }
bool2 Greater (const float2 x, const float2 y)  { return greaterThan( x, y ); }
bool2 LessEqual (const float2 x, const float2 y)  { return lessThanEqual( x, y ); }
bool2 GreaterEqual (const float2 x, const float2 y)  { return greaterThanEqual( x, y ); }
bool3 Less (const float3 x, const float3 y)  { return lessThan( x, y ); }
bool3 Greater (const float3 x, const float3 y)  { return greaterThan( x, y ); }
bool3 LessEqual (const float3 x, const float3 y)  { return lessThanEqual( x, y ); }
bool3 GreaterEqual (const float3 x, const float3 y)  { return greaterThanEqual( x, y ); }
bool4 Less (const float4 x, const float4 y)  { return lessThan( x, y ); }
bool4 Greater (const float4 x, const float4 y)  { return greaterThan( x, y ); }
bool4 LessEqual (const float4 x, const float4 y)  { return lessThanEqual( x, y ); }
bool4 GreaterEqual (const float4 x, const float4 y)  { return greaterThanEqual( x, y ); }

bool Less (const double x, const double y)  { return x < y; }
bool Greater (const double x, const double y)  { return x > y; }
bool LessEqual (const double x, const double y)  { return x <= y; }
bool GreaterEqual (const double x, const double y)  { return x >= y; }
bool2 Less (const double2 x, const double2 y)  { return lessThan( x, y ); }
bool2 Greater (const double2 x, const double2 y)  { return greaterThan( x, y ); }
bool2 LessEqual (const double2 x, const double2 y)  { return lessThanEqual( x, y ); }
bool2 GreaterEqual (const double2 x, const double2 y)  { return greaterThanEqual( x, y ); }
bool3 Less (const double3 x, const double3 y)  { return lessThan( x, y ); }
bool3 Greater (const double3 x, const double3 y)  { return greaterThan( x, y ); }
bool3 LessEqual (const double3 x, const double3 y)  { return lessThanEqual( x, y ); }
bool3 GreaterEqual (const double3 x, const double3 y)  { return greaterThanEqual( x, y ); }
bool4 Less (const double4 x, const double4 y)  { return lessThan( x, y ); }
bool4 Greater (const double4 x, const double4 y)  { return greaterThan( x, y ); }
bool4 LessEqual (const double4 x, const double4 y)  { return lessThanEqual( x, y ); }
bool4 GreaterEqual (const double4 x, const double4 y)  { return greaterThanEqual( x, y ); }

bool Less (const int x, const int y)  { return x < y; }
bool Greater (const int x, const int y)  { return x > y; }
bool LessEqual (const int x, const int y)  { return x <= y; }
bool GreaterEqual (const int x, const int y)  { return x >= y; }
bool2 Less (const int2 x, const int2 y)  { return lessThan( x, y ); }
bool2 Greater (const int2 x, const int2 y)  { return greaterThan( x, y ); }
bool2 LessEqual (const int2 x, const int2 y)  { return lessThanEqual( x, y ); }
bool2 GreaterEqual (const int2 x, const int2 y)  { return greaterThanEqual( x, y ); }
bool3 Less (const int3 x, const int3 y)  { return lessThan( x, y ); }
bool3 Greater (const int3 x, const int3 y)  { return greaterThan( x, y ); }
bool3 LessEqual (const int3 x, const int3 y)  { return lessThanEqual( x, y ); }
bool3 GreaterEqual (const int3 x, const int3 y)  { return greaterThanEqual( x, y ); }
bool4 Less (const int4 x, const int4 y)  { return lessThan( x, y ); }
bool4 Greater (const int4 x, const int4 y)  { return greaterThan( x, y ); }
bool4 LessEqual (const int4 x, const int4 y)  { return lessThanEqual( x, y ); }
bool4 GreaterEqual (const int4 x, const int4 y)  { return greaterThanEqual( x, y ); }

bool Less (const uint x, const uint y)  { return x < y; }
bool Greater (const uint x, const uint y)  { return x > y; }
bool LessEqual (const uint x, const uint y)  { return x <= y; }
bool GreaterEqual (const uint x, const uint y)  { return x >= y; }
bool2 Less (const uint2 x, const uint2 y)  { return lessThan( x, y ); }
bool2 Greater (const uint2 x, const uint2 y)  { return greaterThan( x, y ); }
bool2 LessEqual (const uint2 x, const uint2 y)  { return lessThanEqual( x, y ); }
bool2 GreaterEqual (const uint2 x, const uint2 y)  { return greaterThanEqual( x, y ); }
bool3 Less (const uint3 x, const uint3 y)  { return lessThan( x, y ); }
bool3 Greater (const uint3 x, const uint3 y)  { return greaterThan( x, y ); }
bool3 LessEqual (const uint3 x, const uint3 y)  { return lessThanEqual( x, y ); }
bool3 GreaterEqual (const uint3 x, const uint3 y)  { return greaterThanEqual( x, y ); }
bool4 Less (const uint4 x, const uint4 y)  { return lessThan( x, y ); }
bool4 Greater (const uint4 x, const uint4 y)  { return greaterThan( x, y ); }
bool4 LessEqual (const uint4 x, const uint4 y)  { return lessThanEqual( x, y ); }
bool4 GreaterEqual (const uint4 x, const uint4 y)  { return greaterThanEqual( x, y ); }

#ifdef ARB_gpu_shader_int64_enabled
bool Less (const ilong x, const ilong y)  { return x < y; }
bool Greater (const ilong x, const ilong y)  { return x > y; }
bool LessEqual (const ilong x, const ilong y)  { return x <= y; }
bool GreaterEqual (const ilong x, const ilong y)  { return x >= y; }
bool2 Less (const ilong2 x, const ilong2 y)  { return lessThan( x, y ); }
bool2 Greater (const ilong2 x, const ilong2 y)  { return greaterThan( x, y ); }
bool2 LessEqual (const ilong2 x, const ilong2 y)  { return lessThanEqual( x, y ); }
bool2 GreaterEqual (const ilong2 x, const ilong2 y)  { return greaterThanEqual( x, y ); }
bool3 Less (const ilong3 x, const ilong3 y)  { return lessThan( x, y ); }
bool3 Greater (const ilong3 x, const ilong3 y)  { return greaterThan( x, y ); }
bool3 LessEqual (const ilong3 x, const ilong3 y)  { return lessThanEqual( x, y ); }
bool3 GreaterEqual (const ilong3 x, const ilong3 y)  { return greaterThanEqual( x, y ); }
bool4 Less (const ilong4 x, const ilong4 y)  { return lessThan( x, y ); }
bool4 Greater (const ilong4 x, const ilong4 y)  { return greaterThan( x, y ); }
bool4 LessEqual (const ilong4 x, const ilong4 y)  { return lessThanEqual( x, y ); }
bool4 GreaterEqual (const ilong4 x, const ilong4 y)  { return greaterThanEqual( x, y ); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool Less (const ulong x, const ulong y)  { return x < y; }
bool Greater (const ulong x, const ulong y)  { return x > y; }
bool LessEqual (const ulong x, const ulong y)  { return x <= y; }
bool GreaterEqual (const ulong x, const ulong y)  { return x >= y; }
bool2 Less (const ulong2 x, const ulong2 y)  { return lessThan( x, y ); }
)#";

		src << R"#(bool2 Greater (const ulong2 x, const ulong2 y)  { return greaterThan( x, y ); }
bool2 LessEqual (const ulong2 x, const ulong2 y)  { return lessThanEqual( x, y ); }
bool2 GreaterEqual (const ulong2 x, const ulong2 y)  { return greaterThanEqual( x, y ); }
bool3 Less (const ulong3 x, const ulong3 y)  { return lessThan( x, y ); }
bool3 Greater (const ulong3 x, const ulong3 y)  { return greaterThan( x, y ); }
bool3 LessEqual (const ulong3 x, const ulong3 y)  { return lessThanEqual( x, y ); }
bool3 GreaterEqual (const ulong3 x, const ulong3 y)  { return greaterThanEqual( x, y ); }
bool4 Less (const ulong4 x, const ulong4 y)  { return lessThan( x, y ); }
bool4 Greater (const ulong4 x, const ulong4 y)  { return greaterThan( x, y ); }
bool4 LessEqual (const ulong4 x, const ulong4 y)  { return lessThanEqual( x, y ); }
bool4 GreaterEqual (const ulong4 x, const ulong4 y)  { return greaterThanEqual( x, y ); }
#endif


// IsZero, Equals (with error)
float  EpsilonF ()  { return 1.0e-5; }
double EpsilonD ()  { return 1.0e-7LF; }

real   Epsilon ()
{
# if GX_REAL_TYPE_SIZE == 32
	return EpsilonF();
# elif GX_REAL_TYPE_SIZE == 64
	return EpsilonD();
# endif
}

bool IsZero (const float x)  { return abs(x) < 1.0e-5; }
bool2 IsZero (const float2 x)  { return bool2( IsZero(x[0]), IsZero(x[1]) ); }
bool3 IsZero (const float3 x)  { return bool3( IsZero(x[0]), IsZero(x[1]), IsZero(x[2]) ); }
bool4 IsZero (const float4 x)  { return bool4( IsZero(x[0]), IsZero(x[1]), IsZero(x[2]), IsZero(x[3]) ); }

bool IsZero (const double x)  { return abs(x) < 1.0e-7LF; }
bool2 IsZero (const double2 x)  { return bool2( IsZero(x[0]), IsZero(x[1]) ); }
bool3 IsZero (const double3 x)  { return bool3( IsZero(x[0]), IsZero(x[1]), IsZero(x[2]) ); }
bool4 IsZero (const double4 x)  { return bool4( IsZero(x[0]), IsZero(x[1]), IsZero(x[2]), IsZero(x[3]) ); }

bool IsZero (const int x)  { return Equals( x, int(0) ); }
bool2 IsZero (const int2 x)  { return Equals( x, int2(0) ); }
bool3 IsZero (const int3 x)  { return Equals( x, int3(0) ); }
bool4 IsZero (const int4 x)  { return Equals( x, int4(0) ); }

bool IsZero (const uint x)  { return Equals( x, uint(0) ); }
bool2 IsZero (const uint2 x)  { return Equals( x, uint2(0) ); }
bool3 IsZero (const uint3 x)  { return Equals( x, uint3(0) ); }
bool4 IsZero (const uint4 x)  { return Equals( x, uint4(0) ); }

bool IsZero (const bool x)  { return Equals( x, bool(0) ); }
bool2 IsZero (const bool2 x)  { return Equals( x, bool2(0) ); }
bool3 IsZero (const bool3 x)  { return Equals( x, bool3(0) ); }
bool4 IsZero (const bool4 x)  { return Equals( x, bool4(0) ); }

#ifdef ARB_gpu_shader_int64_enabled
bool IsZero (const ilong x)  { return Equals( x, ilong(0) ); }
bool2 IsZero (const ilong2 x)  { return Equals( x, ilong2(0) ); }
bool3 IsZero (const ilong3 x)  { return Equals( x, ilong3(0) ); }
bool4 IsZero (const ilong4 x)  { return Equals( x, ilong4(0) ); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool IsZero (const ulong x)  { return Equals( x, ulong(0) ); }
bool2 IsZero (const ulong2 x)  { return Equals( x, ulong2(0) ); }
bool3 IsZero (const ulong3 x)  { return Equals( x, ulong3(0) ); }
bool4 IsZero (const ulong4 x)  { return Equals( x, ulong4(0) ); }
#endif

bool Equals (const float left, const float right)  { return Equals( left, right, 10 ); }
bool2 Equals (const float2 left, const float2 right)  { return Equals( left, right, 10 ); }
bool3 Equals (const float3 left, const float3 right)  { return Equals( left, right, 10 ); }
bool4 Equals (const float4 left, const float4 right)  { return Equals( left, right, 10 ); }

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const double left, const double right)  { return Equals( left, right, 42 ); }
bool2 Equals (const double2 left, const double2 right)  { return Equals( left, right, 42 ); }
bool3 Equals (const double3 left, const double3 right)  { return Equals( left, right, 42 ); }
bool4 Equals (const double4 left, const double4 right)  { return Equals( left, right, 42 ); }
#endif

bool Equals (const int left, const int right)  { return left == right; }
bool2 Equals (const int2 left, const int2 right)  { return equal( left, right ); }
bool3 Equals (const int3 left, const int3 right)  { return equal( left, right ); }
bool4 Equals (const int4 left, const int4 right)  { return equal( left, right ); }

bool Equals (const uint left, const uint right)  { return left == right; }
bool2 Equals (const uint2 left, const uint2 right)  { return equal( left, right ); }
bool3 Equals (const uint3 left, const uint3 right)  { return equal( left, right ); }
bool4 Equals (const uint4 left, const uint4 right)  { return equal( left, right ); }

bool Equals (const bool left, const bool right)  { return left == right; }
bool2 Equals (const bool2 left, const bool2 right)  { return equal( left, right ); }
bool3 Equals (const bool3 left, const bool3 right)  { return equal( left, right ); }
bool4 Equals (const bool4 left, const bool4 right)  { return equal( left, right ); }

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const ilong left, const ilong right)  { return left == right; }
bool2 Equals (const ilong2 left, const ilong2 right)  { return equal( left, right ); }
bool3 Equals (const ilong3 left, const ilong3 right)  { return equal( left, right ); }
bool4 Equals (const ilong4 left, const ilong4 right)  { return equal( left, right ); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const ulong left, const ulong right)  { return left == right; }
bool2 Equals (const ulong2 left, const ulong2 right)  { return equal( left, right ); }
bool3 Equals (const ulong3 left, const ulong3 right)  { return equal( left, right ); }
bool4 Equals (const ulong4 left, const ulong4 right)  { return equal( left, right ); }
#endif

bool Equals (const float left, const float right, const uint accuracyInBits)
{
	int ileft = floatBitsToInt(left);
	int iright = floatBitsToInt(right);
	ileft = ileft < 0 ? (-2147483648 - ileft) : ileft;
	iright = iright < 0 ? (-2147483648 - iright) : iright;
	return abs( ileft - iright ) <= (1 << (accuracyInBits & 31));
}

bool2 Equals (const float2 left, const float2 right, const uint accuracyInBits)
{
	return bool2(
		Equals( left[0], right[0], accuracyInBits ),
		Equals( left[1], right[1], accuracyInBits ) );
}

bool3 Equals (const float3 left, const float3 right, const uint accuracyInBits)
{
	return bool3(
		Equals( left[0], right[0], accuracyInBits ),
		Equals( left[1], right[1], accuracyInBits ),
		Equals( left[2], right[2], accuracyInBits ) );
}

bool4 Equals (const float4 left, const float4 right, const uint accuracyInBits)
{
	return bool4(
		Equals( left[0], right[0], accuracyInBits ),
		Equals( left[1], right[1], accuracyInBits ),
		Equals( left[2], right[2], accuracyInBits ),
		Equals( left[3], right[3], accuracyInBits ) );
}

#ifdef ARB_gpu_shader_int64_enabled
bool Equals (const double left, const double right, const uint accuracyInBits)
{
	ilong ileft = doubleBitsToInt64(left);
	ilong iright = doubleBitsToInt64(right);
	ileft = ileft < 0 ? (-9223372036854775808L - ileft) : ileft;
	iright = iright < 0 ? (-9223372036854775808L - iright) : iright;
	return abs( ileft - iright ) <= (1 << (accuracyInBits & 63));
}

bool2 Equals (const double2 left, const double2 right, const uint accuracyInBits)
{
	return bool2(
		Equals( left[0], right[0], accuracyInBits ),
		Equals( left[1], right[1], accuracyInBits ) );
}

bool3 Equals (const double3 left, const double3 right, const uint accuracyInBits)
{
	return bool3(
		Equals( left[0], right[0], accuracyInBits ),
		Equals( left[1], right[1], accuracyInBits ),
		Equals( left[2], right[2], accuracyInBits ) );
}

bool4 Equals (const double4 left, const double4 right, const uint accuracyInBits)
{
	return bool4(
		Equals( left[0], right[0], accuracyInBits ),
		Equals( left[1], right[1], accuracyInBits ),
		Equals( left[2], right[2], accuracyInBits ),
		Equals( left[3], right[3], accuracyInBits ) );
}

#endif

// All, Any, Not
#undef not

bool All (const bool x)  { return x; }
bool Any (const bool x)  { return x; }
bool Not (const bool x)  { return !x; }
bool All (const bool2 x)  { return all( x ); }
bool Any (const bool2 x)  { return any( x ); }
bool2 Not (const bool2 x)  { return not( x ); }
bool All (const bool3 x)  { return all( x ); }
bool Any (const bool3 x)  { return any( x ); }
bool3 Not (const bool3 x)  { return not( x ); }
bool All (const bool4 x)  { return all( x ); }
bool Any (const bool4 x)  { return any( x ); }
bool4 Not (const bool4 x)  { return not( x ); }
#define not	!

)#";

	}
}	// glsl_vfs
