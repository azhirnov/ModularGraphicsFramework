// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Math_Math (OUT String &src)
	{
		src << R"#(/*
	This is generated file, don't change anything!
*/

#include <Common/Cmp.glsl>
#include <Math/MathDef.glsl>

// Trigonometry
float2  SinCos (const float x);
float2  ASinCos (const float x);
float2  SinCosH (const float x);
float2  ASinCosH (const float x);

float CoTan (const float x);
float2 CoTan (const float2 x);
float3 CoTan (const float3 x);
float4 CoTan (const float4 x);

float CoTanH (const float x);
float2 CoTanH (const float2 x);
float3 CoTanH (const float3 x);
float4 CoTanH (const float4 x);

float ACoTan (const float x);
float2 ACoTan (const float2 x);
float3 ACoTan (const float3 x);
float4 ACoTan (const float4 x);

float ACoTanH (const float x);
float2 ACoTanH (const float2 x);
float3 ACoTanH (const float3 x);
float4 ACoTanH (const float4 x);


// Sign (1 or -1)
float Sign (const float x);
float2 Sign (const float2 x);
float3 Sign (const float3 x);
float4 Sign (const float4 x);

double Sign (const double x);
double2 Sign (const double2 x);
double3 Sign (const double3 x);
double4 Sign (const double4 x);

int Sign (const int x);
int2 Sign (const int2 x);
int3 Sign (const int3 x);
int4 Sign (const int4 x);

#ifdef ARB_gpu_shader_int64_enabled
ilong Sign (const ilong x);
ilong2 Sign (const ilong2 x);
ilong3 Sign (const ilong3 x);
ilong4 Sign (const ilong4 x);
#endif


// ClampOut
float ClampOut (const float x, const float minVal, const float maxVal);
float2 ClampOut (const float2 x, const float minVal, const float maxVal);
float2 ClampOut (const float2 x, const float2 minVal, const float2 maxVal);
float3 ClampOut (const float3 x, const float minVal, const float maxVal);
float3 ClampOut (const float3 x, const float3 minVal, const float3 maxVal);
float4 ClampOut (const float4 x, const float minVal, const float maxVal);
float4 ClampOut (const float4 x, const float4 minVal, const float4 maxVal);

double ClampOut (const double x, const double minVal, const double maxVal);
double2 ClampOut (const double2 x, const double minVal, const double maxVal);
double2 ClampOut (const double2 x, const double2 minVal, const double2 maxVal);
double3 ClampOut (const double3 x, const double minVal, const double maxVal);
double3 ClampOut (const double3 x, const double3 minVal, const double3 maxVal);
double4 ClampOut (const double4 x, const double minVal, const double maxVal);
double4 ClampOut (const double4 x, const double4 minVal, const double4 maxVal);

int ClampOut (const int x, const int minVal, const int maxVal);
int2 ClampOut (const int2 x, const int minVal, const int maxVal);
int2 ClampOut (const int2 x, const int2 minVal, const int2 maxVal);
int3 ClampOut (const int3 x, const int minVal, const int maxVal);
int3 ClampOut (const int3 x, const int3 minVal, const int3 maxVal);
int4 ClampOut (const int4 x, const int minVal, const int maxVal);
int4 ClampOut (const int4 x, const int4 minVal, const int4 maxVal);

uint ClampOut (const uint x, const uint minVal, const uint maxVal);
uint2 ClampOut (const uint2 x, const uint minVal, const uint maxVal);
uint2 ClampOut (const uint2 x, const uint2 minVal, const uint2 maxVal);
uint3 ClampOut (const uint3 x, const uint minVal, const uint maxVal);
uint3 ClampOut (const uint3 x, const uint3 minVal, const uint3 maxVal);
uint4 ClampOut (const uint4 x, const uint minVal, const uint maxVal);
uint4 ClampOut (const uint4 x, const uint4 minVal, const uint4 maxVal);

#ifdef ARB_gpu_shader_int64_enabled
ilong ClampOut (const ilong x, const ilong minVal, const ilong maxVal);
ilong2 ClampOut (const ilong2 x, const ilong minVal, const ilong maxVal);
ilong2 ClampOut (const ilong2 x, const ilong2 minVal, const ilong2 maxVal);
ilong3 ClampOut (const ilong3 x, const ilong minVal, const ilong maxVal);
ilong3 ClampOut (const ilong3 x, const ilong3 minVal, const ilong3 maxVal);
ilong4 ClampOut (const ilong4 x, const ilong minVal, const ilong maxVal);
ilong4 ClampOut (const ilong4 x, const ilong4 minVal, const ilong4 maxVal);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong ClampOut (const ulong x, const ulong minVal, const ulong maxVal);
ulong2 ClampOut (const ulong2 x, const ulong minVal, const ulong maxVal);
ulong2 ClampOut (const ulong2 x, const ulong2 minVal, const ulong2 maxVal);
ulong3 ClampOut (const ulong3 x, const ulong minVal, const ulong maxVal);
ulong3 ClampOut (const ulong3 x, const ulong3 minVal, const ulong3 maxVal);
ulong4 ClampOut (const ulong4 x, const ulong minVal, const ulong maxVal);
ulong4 ClampOut (const ulong4 x, const ulong4 minVal, const ulong4 maxVal);
#endif


// Wrap
float Wrap (const float x, const float minVal, const float maxVal);
float2 Wrap (const float2 x, const float2 minVal, const float2 maxVal);
float2 Wrap (const float2 x, const float minVal, const float maxVal);
float3 Wrap (const float3 x, const float3 minVal, const float3 maxVal);
float3 Wrap (const float3 x, const float minVal, const float maxVal);
float4 Wrap (const float4 x, const float4 minVal, const float4 maxVal);
float4 Wrap (const float4 x, const float minVal, const float maxVal);

double Wrap (const double x, const double minVal, const double maxVal);
double2 Wrap (const double2 x, const double2 minVal, const double2 maxVal);
double2 Wrap (const double2 x, const double minVal, const double maxVal);
double3 Wrap (const double3 x, const double3 minVal, const double3 maxVal);
double3 Wrap (const double3 x, const double minVal, const double maxVal);
double4 Wrap (const double4 x, const double4 minVal, const double4 maxVal);
double4 Wrap (const double4 x, const double minVal, const double maxVal);

int Wrap (const int x, const int minVal, const int maxVal);
int2 Wrap (const int2 x, const int2 minVal, const int2 maxVal);
int2 Wrap (const int2 x, const int minVal, const int maxVal);
int3 Wrap (const int3 x, const int3 minVal, const int3 maxVal);
int3 Wrap (const int3 x, const int minVal, const int maxVal);
int4 Wrap (const int4 x, const int4 minVal, const int4 maxVal);
int4 Wrap (const int4 x, const int minVal, const int maxVal);

uint Wrap (const uint x, const uint minVal, const uint maxVal);
uint2 Wrap (const uint2 x, const uint2 minVal, const uint2 maxVal);
uint2 Wrap (const uint2 x, const uint minVal, const uint maxVal);
uint3 Wrap (const uint3 x, const uint3 minVal, const uint3 maxVal);
uint3 Wrap (const uint3 x, const uint minVal, const uint maxVal);
uint4 Wrap (const uint4 x, const uint4 minVal, const uint4 maxVal);
uint4 Wrap (const uint4 x, const uint minVal, const uint maxVal);

#ifdef ARB_gpu_shader_int64_enabled
ilong Wrap (const ilong x, const ilong minVal, const ilong maxVal);
ilong2 Wrap (const ilong2 x, const ilong2 minVal, const ilong2 maxVal);
ilong2 Wrap (const ilong2 x, const ilong minVal, const ilong maxVal);
ilong3 Wrap (const ilong3 x, const ilong3 minVal, const ilong3 maxVal);
ilong3 Wrap (const ilong3 x, const ilong minVal, const ilong maxVal);
ilong4 Wrap (const ilong4 x, const ilong4 minVal, const ilong4 maxVal);
ilong4 Wrap (const ilong4 x, const ilong minVal, const ilong maxVal);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong Wrap (const ulong x, const ulong minVal, const ulong maxVal);
ulong2 Wrap (const ulong2 x, const ulong2 minVal, const ulong2 maxVal);
ulong2 Wrap (const ulong2 x, const ulong minVal, const ulong maxVal);
ulong3 Wrap (const ulong3 x, const ulong3 minVal, const ulong3 maxVal);
ulong3 Wrap (const ulong3 x, const ulong minVal, const ulong maxVal);
ulong4 Wrap (const ulong4 x, const ulong4 minVal, const ulong4 maxVal);
ulong4 Wrap (const ulong4 x, const ulong minVal, const ulong maxVal);
#endif


// Log, Log10, Exp10
float Log (const float x, const float base);
float2 Log (const float2 x, const float2 base);
float3 Log (const float3 x, const float3 base);
float4 Log (const float4 x, const float4 base);

float Log10 (const float x);
float2 Log10 (const float2 x);
float3 Log10 (const float3 x);
float4 Log10 (const float4 x);

float Exp10 (const float x);
float2 Exp10 (const float2 x);
float3 Exp10 (const float3 x);
float4 Exp10 (const float4 x);


// Square
float Square (const float x);
float2 Square (const float2 x);
float3 Square (const float3 x);
float4 Square (const float4 x);

double Square (const double x);
double2 Square (const double2 x);
double3 Square (const double3 x);
double4 Square (const double4 x);

int Square (const int x);
int2 Square (const int2 x);
int3 Square (const int3 x);
int4 Square (const int4 x);

uint Square (const uint x);
uint2 Square (const uint2 x);
uint3 Square (const uint3 x);
uint4 Square (const uint4 x);

#ifdef ARB_gpu_shader_int64_enabled
ilong Square (const ilong x);
ilong2 Square (const ilong2 x);
ilong3 Square (const ilong3 x);
ilong4 Square (const ilong4 x);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong Square (const ulong x);
ulong2 Square (const ulong2 x);
ulong3 Square (const ulong3 x);
ulong4 Square (const ulong4 x);
#endif


// RoundToInt, RoundToUInt, RoundTo
int RoundToInt (const float x);
uint RoundToUInt (const float x);
int2 RoundToInt (const float2 x);
uint2 RoundToUInt (const float2 x);
int3 RoundToInt (const float3 x);
uint3 RoundToUInt (const float3 x);
int4 RoundToInt (const float4 x);
uint4 RoundToUInt (const float4 x);

#ifdef ARB_gpu_shader_int64_enabled
ilong RoundToInt (const double x);
ulong RoundToUInt (const double x);
ilong2 RoundToInt (const double2 x);
ulong2 RoundToUInt (const double2 x);
ilong3 RoundToInt (const double3 x);
ulong3 RoundToUInt (const double3 x);
ilong4 RoundToInt (const double4 x);
ulong4 RoundToUInt (const double4 x);
#endif

float RoundTo (const float x, const float base);
float2 RoundTo (const float2 x, const float base);
float3 RoundTo (const float3 x, const float base);
float4 RoundTo (const float4 x, const float base);

double RoundTo (const double x, const double base);
double2 RoundTo (const double2 x, const double base);
double3 RoundTo (const double3 x, const double base);
double4 RoundTo (const double4 x, const double base);


// SafeDiv
float SafeDiv (const float left, const float right, const float defVal);
float2 SafeDiv (const float2 left, const float right, const float defVal);
float2 SafeDiv (const float2 left, const float2 right, const float defVal);
float3 SafeDiv (const float3 left, const float right, const float defVal);
float3 SafeDiv (const float3 left, const float3 right, const float defVal);
float4 SafeDiv (const float4 left, const float right, const float defVal);
float4 SafeDiv (const float4 left, const float4 right, const float defVal);

double SafeDiv (const double left, const double right, const double defVal);
double2 SafeDiv (const double2 left, const double right, const double defVal);
double2 SafeDiv (const double2 left, const double2 right, const double defVal);
double3 SafeDiv (const double3 left, const double right, const double defVal);
double3 SafeDiv (const double3 left, const double3 right, const double defVal);
double4 SafeDiv (const double4 left, const double right, const double defVal);
double4 SafeDiv (const double4 left, const double4 right, const double defVal);


// IsOdd, IsEven
bool IsOdd (const int x);
bool2 IsOdd (const int2 x);
bool3 IsOdd (const int3 x);
bool4 IsOdd (const int4 x);

bool IsOdd (const uint x);
bool2 IsOdd (const uint2 x);
bool3 IsOdd (const uint3 x);
bool4 IsOdd (const uint4 x);

#ifdef ARB_gpu_shader_int64_enabled
bool IsOdd (const ilong x);
bool2 IsOdd (const ilong2 x);
bool3 IsOdd (const ilong3 x);
bool4 IsOdd (const ilong4 x);
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool IsOdd (const ulong x);
bool2 IsOdd (const ulong2 x);
bool3 IsOdd (const ulong3 x);
bool4 IsOdd (const ulong4 x);
#endif

bool IsEven (const int x);
bool2 IsEven (const int2 x);
bool3 IsEven (const int3 x);
bool4 IsEven (const int4 x);

bool IsEven (const uint x);
bool2 IsEven (const uint2 x);
bool3 IsEven (const uint3 x);
bool4 IsEven (const uint4 x);

#ifdef ARB_gpu_shader_int64_enabled
bool IsEven (const ilong x);
bool2 IsEven (const ilong2 x);
bool3 IsEven (const ilong3 x);
bool4 IsEven (const ilong4 x);
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool IsEven (const ulong x);
bool2 IsEven (const ulong2 x);
bool3 IsEven (const ulong3 x);
bool4 IsEven (const ulong4 x);
#endif


// ToUNorm, ToSNorm
float ToUNorm (const float x);
float2 ToUNorm (const float2 x);
float3 ToUNorm (const float3 x);
float4 ToUNorm (const float4 x);

double ToUNorm (const double x);
double2 ToUNorm (const double2 x);
double3 ToUNorm (const double3 x);
double4 ToUNorm (const double4 x);

float ToSNorm (const float x);
float2 ToSNorm (const float2 x);
float3 ToSNorm (const float3 x);
float4 ToSNorm (const float4 x);

double ToSNorm (const double x);
double2 ToSNorm (const double2 x);
double3 ToSNorm (const double3 x);
double4 ToSNorm (const double4 x);


// InvLerp, BiLerp
float InvLerp (const float x, const float y, const float interpolated);
float2 InvLerp (const float2 x, const float2 y, const float interpolated);
float3 InvLerp (const float3 x, const float3 y, const float interpolated);
float4 InvLerp (const float4 x, const float4 y, const float interpolated);

double InvLerp (const double x, const double y, const double interpolated);
double2 InvLerp (const double2 x, const double2 y, const double interpolated);
double3 InvLerp (const double3 x, const double3 y, const double interpolated);
double4 InvLerp (const double4 x, const double4 y, const double interpolated);

float BiLerp (const float v0, const float v1, const float v2, const float v3, const float2 factor);
float2 BiLerp (const float2 v0, const float2 v1, const float2 v2, const float2 v3, const float2 factor);
float3 BiLerp (const float3 v0, const float3 v1, const float3 v2, const float3 v3, const float2 factor);
float4 BiLerp (const float4 v0, const float4 v1, const float4 v2, const float4 v3, const float2 factor);

double BiLerp (const double v0, const double v1, const double v2, const double v3, const double2 factor);
double2 BiLerp (const double2 v0, const double2 v1, const double2 v2, const double2 v3, const double2 factor);
double3 BiLerp (const double3 v0, const double3 v1, const double3 v2, const double3 v3, const double2 factor);
double4 BiLerp (const double4 v0, const double4 v1, const double4 v2, const double4 v3, const double2 factor);


// LinearStep, BumpStep, SmoothBumpStep
float LinearStep (const float x, const float edge0, const float edge1);
float2 LinearStep (const float2 x, const float2 edge0, const float2 edge1);
float2 LinearStep (const float2 x, const float edge0, const float edge1);
float3 LinearStep (const float3 x, const float3 edge0, const float3 edge1);
float3 LinearStep (const float3 x, const float edge0, const float edge1);
float4 LinearStep (const float4 x, const float4 edge0, const float4 edge1);
float4 LinearStep (const float4 x, const float edge0, const float edge1);

double LinearStep (const double x, const double edge0, const double edge1);
double2 LinearStep (const double2 x, const double2 edge0, const double2 edge1);
double2 LinearStep (const double2 x, const double edge0, const double edge1);
double3 LinearStep (const double3 x, const double3 edge0, const double3 edge1);
double3 LinearStep (const double3 x, const double edge0, const double edge1);
double4 LinearStep (const double4 x, const double4 edge0, const double4 edge1);
double4 LinearStep (const double4 x, const double edge0, const double edge1);

float BumpStep (const float x, const float edge0, const float edge1);
float2 BumpStep (const float2 x, const float2 edge0, const float2 edge1);
float2 BumpStep (const float2 x, const float edge0, const float edge1);
float3 BumpStep (const float3 x, const float3 edge0, const float3 edge1);
float3 BumpStep (const float3 x, const float edge0, const float edge1);
float4 BumpStep (const float4 x, const float4 edge0, const float4 edge1);
float4 BumpStep (const float4 x, const float edge0, const float edge1);

double BumpStep (const double x, const double edge0, const double edge1);
double2 BumpStep (const double2 x, const double2 edge0, const double2 edge1);
double2 BumpStep (const double2 x, const double edge0, const double edge1);
double3 B)#";

		src << R"#(umpStep (const double3 x, const double3 edge0, const double3 edge1);
double3 BumpStep (const double3 x, const double edge0, const double edge1);
double4 BumpStep (const double4 x, const double4 edge0, const double4 edge1);
double4 BumpStep (const double4 x, const double edge0, const double edge1);

float SmoothBumpStep (const float x, const float edge0, const float edge1);
float2 SmoothBumpStep (const float2 x, const float2 edge0, const float2 edge1);
float2 SmoothBumpStep (const float2 x, const float edge0, const float edge1);
float3 SmoothBumpStep (const float3 x, const float3 edge0, const float3 edge1);
float3 SmoothBumpStep (const float3 x, const float edge0, const float edge1);
float4 SmoothBumpStep (const float4 x, const float4 edge0, const float4 edge1);
float4 SmoothBumpStep (const float4 x, const float edge0, const float edge1);

double SmoothBumpStep (const double x, const double edge0, const double edge1);
double2 SmoothBumpStep (const double2 x, const double2 edge0, const double2 edge1);
double2 SmoothBumpStep (const double2 x, const double edge0, const double edge1);
double3 SmoothBumpStep (const double3 x, const double3 edge0, const double3 edge1);
double3 SmoothBumpStep (const double3 x, const double edge0, const double edge1);
double4 SmoothBumpStep (const double4 x, const double4 edge0, const double4 edge1);
double4 SmoothBumpStep (const double4 x, const double edge0, const double edge1);



//---------------------------------------------------------

float Const_PiF ()  { return float( 3.14159265358979323846 ); }
double Const_PiD ()  { return double( 3.14159265358979323846LF ); }

float Const_InvPiF ()  { return float( 0.3183098861837906715377 ); }
double Const_InvPiD ()  { return double( 0.3183098861837906715377LF ); }

float Const_EulerF ()  { return float( 2.71828182845904523536 ); }
double Const_EulerD ()  { return double( 2.71828182845904523536LF ); }

float Const_InvLnOf10F ()  { return float( 0.4342944819032518 ); }
double Const_InvLnOf10D ()  { return double( 0.4342944819032518LF ); }

// Trigonometry
float2  SinCos (const float x)     { return float2( Sin(x), Cos(x) ); }
float2  ASinCos (const float x)    { return float2( ASin(x), ACos(x) ); }
float2  SinCosH (const float x)    { return float2( SinH(x), CosH(x) ); }
float2  ASinCosH (const float x)   { return float2( ASinH(x), ACosH(x) ); }

float CoTan (const float x)  { return SafeDiv( float(1.0), Tan( x ), 0.0 ); }
float2 CoTan (const float2 x)  { return SafeDiv( float2(1.0), Tan( x ), 0.0 ); }
float3 CoTan (const float3 x)  { return SafeDiv( float3(1.0), Tan( x ), 0.0 ); }
float4 CoTan (const float4 x)  { return SafeDiv( float4(1.0), Tan( x ), 0.0 ); }

float CoTanH (const float x)  { return SafeDiv( float(1.0), TanH( x ), 0.0 ); }
float2 CoTanH (const float2 x)  { return SafeDiv( float2(1.0), TanH( x ), 0.0 ); }
float3 CoTanH (const float3 x)  { return SafeDiv( float3(1.0), TanH( x ), 0.0 ); }
float4 CoTanH (const float4 x)  { return SafeDiv( float4(1.0), TanH( x ), 0.0 ); }

float ACoTan (const float x)  { return SafeDiv( float(1.0), ATan( x ), 0.0 ); }
float2 ACoTan (const float2 x)  { return SafeDiv( float2(1.0), ATan( x ), 0.0 ); }
float3 ACoTan (const float3 x)  { return SafeDiv( float3(1.0), ATan( x ), 0.0 ); }
float4 ACoTan (const float4 x)  { return SafeDiv( float4(1.0), ATan( x ), 0.0 ); }

float ACoTanH (const float x)  { return SafeDiv( float(1.0), ATanH( x ), 0.0 ); }
float2 ACoTanH (const float2 x)  { return SafeDiv( float2(1.0), ATanH( x ), 0.0 ); }
float3 ACoTanH (const float3 x)  { return SafeDiv( float3(1.0), ATanH( x ), 0.0 ); }
float4 ACoTanH (const float4 x)  { return SafeDiv( float4(1.0), ATanH( x ), 0.0 ); }


// Sign (1 or -1)
float Sign (const float x)  { return  x < 0.0 ? -1.0 : 1.0; }
float2 Sign (const float2 x)  { return float2( Sign(x[0]), Sign(x[1]) ); }
float3 Sign (const float3 x)  { return float3( Sign(x[0]), Sign(x[1]), Sign(x[2]) ); }
float4 Sign (const float4 x)  { return float4( Sign(x[0]), Sign(x[1]), Sign(x[2]), Sign(x[3]) ); }

double Sign (const double x)  { return  x < 0.0 ? -1.0 : 1.0; }
double2 Sign (const double2 x)  { return double2( Sign(x[0]), Sign(x[1]) ); }
double3 Sign (const double3 x)  { return double3( Sign(x[0]), Sign(x[1]), Sign(x[2]) ); }
double4 Sign (const double4 x)  { return double4( Sign(x[0]), Sign(x[1]), Sign(x[2]), Sign(x[3]) ); }

int Sign (const int x)  { return  x < 0 ? -1 : 1; }
int2 Sign (const int2 x)  { return int2( Sign(x[0]), Sign(x[1]) ); }
int3 Sign (const int3 x)  { return int3( Sign(x[0]), Sign(x[1]), Sign(x[2]) ); }
int4 Sign (const int4 x)  { return int4( Sign(x[0]), Sign(x[1]), Sign(x[2]), Sign(x[3]) ); }

#ifdef ARB_gpu_shader_int64_enabled
ilong Sign (const ilong x)  { return  x < 0 ? -1 : 1; }
ilong2 Sign (const ilong2 x)  { return ilong2( Sign(x[0]), Sign(x[1]) ); }
ilong3 Sign (const ilong3 x)  { return ilong3( Sign(x[0]), Sign(x[1]), Sign(x[2]) ); }
ilong4 Sign (const ilong4 x)  { return ilong4( Sign(x[0]), Sign(x[1]), Sign(x[2]), Sign(x[3]) ); }
#endif


// ClampOut
float ClampOut (const float x, const float minVal, const float maxVal)
{
	float mid = (minVal + maxVal) * 0.5;
	return x < mid ? (x < minVal ? x : minVal) : (x > maxVal ? x : maxVal);
}

float2 ClampOut (const float2 x, const float minVal, const float maxVal)  { return ClampOut( x, float2( minVal ), float2( maxVal ) ); }
float2 ClampOut (const float2 x, const float2 minVal, const float2 maxVal)
{
	return float2(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ) );
}

float3 ClampOut (const float3 x, const float minVal, const float maxVal)  { return ClampOut( x, float3( minVal ), float3( maxVal ) ); }
float3 ClampOut (const float3 x, const float3 minVal, const float3 maxVal)
{
	return float3(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ) );
}

float4 ClampOut (const float4 x, const float minVal, const float maxVal)  { return ClampOut( x, float4( minVal ), float4( maxVal ) ); }
float4 ClampOut (const float4 x, const float4 minVal, const float4 maxVal)
{
	return float4(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ),
		ClampOut( x[3], minVal[3], maxVal[3] ) );
}

double ClampOut (const double x, const double minVal, const double maxVal)
{
	double mid = (minVal + maxVal) * 0.5;
	return x < mid ? (x < minVal ? x : minVal) : (x > maxVal ? x : maxVal);
}

double2 ClampOut (const double2 x, const double minVal, const double maxVal)  { return ClampOut( x, double2( minVal ), double2( maxVal ) ); }
double2 ClampOut (const double2 x, const double2 minVal, const double2 maxVal)
{
	return double2(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ) );
}

double3 ClampOut (const double3 x, const double minVal, const double maxVal)  { return ClampOut( x, double3( minVal ), double3( maxVal ) ); }
double3 ClampOut (const double3 x, const double3 minVal, const double3 maxVal)
{
	return double3(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ) );
}

double4 ClampOut (const double4 x, const double minVal, const double maxVal)  { return ClampOut( x, double4( minVal ), double4( maxVal ) ); }
double4 ClampOut (const double4 x, const double4 minVal, const double4 maxVal)
{
	return double4(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ),
		ClampOut( x[3], minVal[3], maxVal[3] ) );
}

int ClampOut (const int x, const int minVal, const int maxVal)
{
	int mid = (minVal+1)/2 + (maxVal+1)/2;
	return x < mid ? (x < minVal ? x : minVal) : (x > maxVal ? x : maxVal);
}

int2 ClampOut (const int2 x, const int minVal, const int maxVal)  { return ClampOut( x, int2( minVal ), int2( maxVal ) ); }
int2 ClampOut (const int2 x, const int2 minVal, const int2 maxVal)
{
	return int2(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ) );
}

int3 ClampOut (const int3 x, const int minVal, const int maxVal)  { return ClampOut( x, int3( minVal ), int3( maxVal ) ); }
int3 ClampOut (const int3 x, const int3 minVal, const int3 maxVal)
{
	return int3(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ) );
}

int4 ClampOut (const int4 x, const int minVal, const int maxVal)  { return ClampOut( x, int4( minVal ), int4( maxVal ) ); }
int4 ClampOut (const int4 x, const int4 minVal, const int4 maxVal)
{
	return int4(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ),
		ClampOut( x[3], minVal[3], maxVal[3] ) );
}

uint ClampOut (const uint x, const uint minVal, const uint maxVal)
{
	uint mid = (minVal+1)/2 + (maxVal+1)/2;
	return x < mid ? (x < minVal ? x : minVal) : (x > maxVal ? x : maxVal);
}

uint2 ClampOut (const uint2 x, const uint minVal, const uint maxVal)  { return ClampOut( x, uint2( minVal ), uint2( maxVal ) ); }
uint2 ClampOut (const uint2 x, const uint2 minVal, const uint2 maxVal)
{
	return uint2(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ) );
}

uint3 ClampOut (const uint3 x, const uint minVal, const uint maxVal)  { return ClampOut( x, uint3( minVal ), uint3( maxVal ) ); }
uint3 ClampOut (const uint3 x, const uint3 minVal, const uint3 maxVal)
{
	return uint3(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ) );
}

uint4 ClampOut (const uint4 x, const uint minVal, const uint maxVal)  { return ClampOut( x, uint4( minVal ), uint4( maxVal ) ); }
uint4 ClampOut (const uint4 x, const uint4 minVal, const uint4 maxVal)
{
	return uint4(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ),
		ClampOut( x[3], minVal[3], maxVal[3] ) );
}

#ifdef ARB_gpu_shader_int64_enabled
ilong ClampOut (const ilong x, const ilong minVal, const ilong maxVal)
{
	ilong mid = (minVal+1)/2 + (maxVal+1)/2;
	return x < mid ? (x < minVal ? x : minVal) : (x > maxVal ? x : maxVal);
}

ilong2 ClampOut (const ilong2 x, const ilong minVal, const ilong maxVal)  { return ClampOut( x, ilong2( minVal ), ilong2( maxVal ) ); }
ilong2 ClampOut (const ilong2 x, const ilong2 minVal, const ilong2 maxVal)
{
	return ilong2(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ) );
}

ilong3 ClampOut (const ilong3 x, const ilong minVal, const ilong maxVal)  { return ClampOut( x, ilong3( minVal ), ilong3( maxVal ) ); }
ilong3 ClampOut (const ilong3 x, const ilong3 minVal, const ilong3 maxVal)
{
	return ilong3(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ) );
}

ilong4 ClampOut (const ilong4 x, const ilong minVal, const ilong maxVal)  { return ClampOut( x, ilong4( minVal ), ilong4( maxVal ) ); }
ilong4 ClampOut (const ilong4 x, const ilong4 minVal, const ilong4 maxVal)
{
	return ilong4(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ),
		ClampOut( x[3], minVal[3], maxVal[3] ) );
}

#endif
#ifdef ARB_gpu_shader_int64_enabled
ulong ClampOut (const ulong x, const ulong minVal, const ulong maxVal)
{
	ulong mid = (minVal+1)/2 + (maxVal+1)/2;
	return x < mid ? (x < minVal ? x : minVal) : (x > maxVal ? x : maxVal);
}

ulong2 ClampOut (const ulong2 x, const ulong minVal, const ulong maxVal)  { return ClampOut( x, ulong2( minVal ), ulong2( maxVal ) ); }
ulong2 ClampOut (const ulong2 x, const ulong2 minVal, const ulong2 maxVal)
{
	return ulong2(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ) );
}

ulong3 ClampOut (const ulong3 x, const ulong minVal, const ulong maxVal)  { return ClampOut( x, ulong3( minVal ), ulong3( maxVal ) ); }
ulong3 ClampOut (const ulong3 x, const ulong3 minVal, const ulong3 maxVal)
{
	return ulong3(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ) );
}

ulong4 ClampOut (const ulong4 x, const ulong minVal, const ulong maxVal)  { return ClampOut( x, ulong4( minVal ), ulong4( maxVal ) ); }
ulong4 ClampOut (const ulong4 x, const ulong4 minVal, const ulong4 maxVal)
{
	return ulong4(
		ClampOut( x[0], minVal[0], maxVal[0] ),
		ClampOut( x[1], minVal[1], maxVal[1] ),
		ClampOut( x[2], minVal[2], maxVal[2] ),
		ClampOut( x[3], minVal[3], maxVal[3] ) );
}

#endif

// Wrap
float Wrap (const float x, const float minVal, const float maxVal)
{
	if ( maxVal < minVal ) return minVal;
	float size = maxVal - minVal;
	float res = minVal + Mod( x - minVal, size );
	if ( res < minVal ) return res + size;
	return res;
}
float2 Wrap (const float2 x, const float minVal, const float maxVal)  { return Wrap( x, float2( minVal ), float2( maxVal ) ); }
float2 Wrap (const float2 x, const float2 minVal, const float2 maxVal)
{
	return float2(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ) );
}

float3 Wrap (const float3 x, const float minVal, const float maxVal)  { return Wrap( x, float3( minVal ), float3( maxVal ) ); }
float3 Wrap (const float3 x, const float3 minVal, const float3 maxVal)
{
	return float3(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ) );
}

float4 Wrap (const float4 x, const float minVal, const float maxVal)  { return Wrap( x, float4( minVal ), float4( maxVal ) ); }
float4 Wrap (const float4 x, const float4 minVal, const float4 maxVal)
{
	return float4(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ),
		Wrap( x[3], minVal[3], maxVal[3] ) );
}


double Wrap (const double x, const double minVal, const double maxVal)
{
	if ( maxVal < minVal ) return minVal;
	double size = maxVal - minVal;
	double res = minVal + Mod( x - minVal, size );
	if ( res < minVal ) return res + size;
	return res;
}
double2 Wrap (const double2 x, const double minVal, const double maxVal)  { return Wrap( x, double2( minVal ), double2( maxVal ) ); }
double2 Wrap (const double2 x, const double2 minVal, const double2 maxVal)
{
	return double2(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ) );
}

double3 Wrap (const double3 x, const double minVal, const double maxVal)  { return Wrap( x, double3( minVal ), double3( maxVal ) ); }
double3 Wrap (const double3 x, const double3 minVal, const double3 maxVal)
{
	return double3(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ) );
}

double4 Wrap (const double4 x, const double minVal, const double maxVal)  { return Wrap( x, double4( minVal ), double4( maxVal ) ); }
double4 Wrap (const double4 x, const double4 minVal, const double4 maxVal)
{
	return double4(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ),
		Wrap( x[3], minVal[3], maxVal[3] ) );
}


int Wrap (const int x, const int minVal, const int maxVal)
{
	if ( maxVal < minVal ) return minVal;
	int size = maxVal+1 - minVal;
	int res = minVal + ((x - minVal) % size);
	if ( res < minVal ) return res + size;
	return res;
}
int2 Wrap (const int2 x, const int minVal, const int maxVal)  { return Wrap( x, int2( minVal ), int2( maxVal ) ); }
int2 Wrap (const int2 x, const int2 minVal, const int2 maxVal)
{
	return int2(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ) );
}

int3 Wrap (const int3 x, const int minVal, const int maxVal)  { return Wrap( x, int3( minVal ), int3( maxVal ) ); }
int3 Wrap (const int3 x, const int3 minVal, const int3 maxVal)
{
	return int3(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal)#";

		src << R"#([2], maxVal[2] ) );
}

int4 Wrap (const int4 x, const int minVal, const int maxVal)  { return Wrap( x, int4( minVal ), int4( maxVal ) ); }
int4 Wrap (const int4 x, const int4 minVal, const int4 maxVal)
{
	return int4(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ),
		Wrap( x[3], minVal[3], maxVal[3] ) );
}


uint Wrap (const uint x, const uint minVal, const uint maxVal)
{
	if ( maxVal < minVal ) return minVal;
	uint size = maxVal+1 - minVal;
	uint res = minVal + ((x - minVal) % size);
	if ( res < minVal ) return res + size;
	return res;
}
uint2 Wrap (const uint2 x, const uint minVal, const uint maxVal)  { return Wrap( x, uint2( minVal ), uint2( maxVal ) ); }
uint2 Wrap (const uint2 x, const uint2 minVal, const uint2 maxVal)
{
	return uint2(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ) );
}

uint3 Wrap (const uint3 x, const uint minVal, const uint maxVal)  { return Wrap( x, uint3( minVal ), uint3( maxVal ) ); }
uint3 Wrap (const uint3 x, const uint3 minVal, const uint3 maxVal)
{
	return uint3(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ) );
}

uint4 Wrap (const uint4 x, const uint minVal, const uint maxVal)  { return Wrap( x, uint4( minVal ), uint4( maxVal ) ); }
uint4 Wrap (const uint4 x, const uint4 minVal, const uint4 maxVal)
{
	return uint4(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ),
		Wrap( x[3], minVal[3], maxVal[3] ) );
}


#ifdef ARB_gpu_shader_int64_enabled
ilong Wrap (const ilong x, const ilong minVal, const ilong maxVal)
{
	if ( maxVal < minVal ) return minVal;
	ilong size = maxVal+1 - minVal;
	ilong res = minVal + ((x - minVal) % size);
	if ( res < minVal ) return res + size;
	return res;
}
ilong2 Wrap (const ilong2 x, const ilong minVal, const ilong maxVal)  { return Wrap( x, ilong2( minVal ), ilong2( maxVal ) ); }
ilong2 Wrap (const ilong2 x, const ilong2 minVal, const ilong2 maxVal)
{
	return ilong2(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ) );
}

ilong3 Wrap (const ilong3 x, const ilong minVal, const ilong maxVal)  { return Wrap( x, ilong3( minVal ), ilong3( maxVal ) ); }
ilong3 Wrap (const ilong3 x, const ilong3 minVal, const ilong3 maxVal)
{
	return ilong3(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ) );
}

ilong4 Wrap (const ilong4 x, const ilong minVal, const ilong maxVal)  { return Wrap( x, ilong4( minVal ), ilong4( maxVal ) ); }
ilong4 Wrap (const ilong4 x, const ilong4 minVal, const ilong4 maxVal)
{
	return ilong4(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ),
		Wrap( x[3], minVal[3], maxVal[3] ) );
}

#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong Wrap (const ulong x, const ulong minVal, const ulong maxVal)
{
	if ( maxVal < minVal ) return minVal;
	ulong size = maxVal+1 - minVal;
	ulong res = minVal + ((x - minVal) % size);
	if ( res < minVal ) return res + size;
	return res;
}
ulong2 Wrap (const ulong2 x, const ulong minVal, const ulong maxVal)  { return Wrap( x, ulong2( minVal ), ulong2( maxVal ) ); }
ulong2 Wrap (const ulong2 x, const ulong2 minVal, const ulong2 maxVal)
{
	return ulong2(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ) );
}

ulong3 Wrap (const ulong3 x, const ulong minVal, const ulong maxVal)  { return Wrap( x, ulong3( minVal ), ulong3( maxVal ) ); }
ulong3 Wrap (const ulong3 x, const ulong3 minVal, const ulong3 maxVal)
{
	return ulong3(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ) );
}

ulong4 Wrap (const ulong4 x, const ulong minVal, const ulong maxVal)  { return Wrap( x, ulong4( minVal ), ulong4( maxVal ) ); }
ulong4 Wrap (const ulong4 x, const ulong4 minVal, const ulong4 maxVal)
{
	return ulong4(
		Wrap( x[0], minVal[0], maxVal[0] ),
		Wrap( x[1], minVal[1], maxVal[1] ),
		Wrap( x[2], minVal[2], maxVal[2] ),
		Wrap( x[3], minVal[3], maxVal[3] ) );
}

#endif


// Log, Log10, Exp10
float Log (const float x, const float base)  { return Ln(x) / Ln(base); }
float2 Log (const float2 x, const float2 base)  { return Ln(x) / Ln(base); }
float3 Log (const float3 x, const float3 base)  { return Ln(x) / Ln(base); }
float4 Log (const float4 x, const float4 base)  { return Ln(x) / Ln(base); }

float Log10 (const float x)  { return Ln(x) * float(Const_InvLnOf10F()); }
float2 Log10 (const float2 x)  { return Ln(x) * float2(Const_InvLnOf10F()); }
float3 Log10 (const float3 x)  { return Ln(x) * float3(Const_InvLnOf10F()); }
float4 Log10 (const float4 x)  { return Ln(x) * float4(Const_InvLnOf10F()); }

float Exp10 (const float x)  { return Pow( float(10.0), x ); }
float2 Exp10 (const float2 x)  { return Pow( float2(10.0), x ); }
float3 Exp10 (const float3 x)  { return Pow( float3(10.0), x ); }
float4 Exp10 (const float4 x)  { return Pow( float4(10.0), x ); }


// Square
float Square (const float x)  { return x * x; }
float2 Square (const float2 x)  { return x * x; }
float3 Square (const float3 x)  { return x * x; }
float4 Square (const float4 x)  { return x * x; }

double Square (const double x)  { return x * x; }
double2 Square (const double2 x)  { return x * x; }
double3 Square (const double3 x)  { return x * x; }
double4 Square (const double4 x)  { return x * x; }

int Square (const int x)  { return x * x; }
int2 Square (const int2 x)  { return x * x; }
int3 Square (const int3 x)  { return x * x; }
int4 Square (const int4 x)  { return x * x; }

uint Square (const uint x)  { return x * x; }
uint2 Square (const uint2 x)  { return x * x; }
uint3 Square (const uint3 x)  { return x * x; }
uint4 Square (const uint4 x)  { return x * x; }

#ifdef ARB_gpu_shader_int64_enabled
ilong Square (const ilong x)  { return x * x; }
ilong2 Square (const ilong2 x)  { return x * x; }
ilong3 Square (const ilong3 x)  { return x * x; }
ilong4 Square (const ilong4 x)  { return x * x; }
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong Square (const ulong x)  { return x * x; }
ulong2 Square (const ulong2 x)  { return x * x; }
ulong3 Square (const ulong3 x)  { return x * x; }
ulong4 Square (const ulong4 x)  { return x * x; }
#endif


// RoundToInt, RoundToUInt, RoundTo
int RoundToInt (const float x)  { return int( round( x ) ); }
uint RoundToUInt (const float x)  { return uint( round( x ) ); }
int2 RoundToInt (const float2 x)  { return int2( round( x ) ); }
uint2 RoundToUInt (const float2 x)  { return uint2( round( x ) ); }
int3 RoundToInt (const float3 x)  { return int3( round( x ) ); }
uint3 RoundToUInt (const float3 x)  { return uint3( round( x ) ); }
int4 RoundToInt (const float4 x)  { return int4( round( x ) ); }
uint4 RoundToUInt (const float4 x)  { return uint4( round( x ) ); }

#ifdef ARB_gpu_shader_int64_enabled
ilong RoundToInt (const double x)  { return ilong( round( x ) ); }
ulong RoundToUInt (const double x)  { return ulong( round( x ) ); }
ilong2 RoundToInt (const double2 x)  { return ilong2( round( x ) ); }
ulong2 RoundToUInt (const double2 x)  { return ulong2( round( x ) ); }
ilong3 RoundToInt (const double3 x)  { return ilong3( round( x ) ); }
ulong3 RoundToUInt (const double3 x)  { return ulong3( round( x ) ); }
ilong4 RoundToInt (const double4 x)  { return ilong4( round( x ) ); }
ulong4 RoundToUInt (const double4 x)  { return ulong4( round( x ) ); }
#endif

float RoundTo (const float x, const float base)  { return Round( x / base ) * base; }
float2 RoundTo (const float2 x, const float base)  { return Round( x / base ) * base; }
float3 RoundTo (const float3 x, const float base)  { return Round( x / base ) * base; }
float4 RoundTo (const float4 x, const float base)  { return Round( x / base ) * base; }

double RoundTo (const double x, const double base)  { return Round( x / base ) * base; }
double2 RoundTo (const double2 x, const double base)  { return Round( x / base ) * base; }
double3 RoundTo (const double3 x, const double base)  { return Round( x / base ) * base; }
double4 RoundTo (const double4 x, const double base)  { return Round( x / base ) * base; }


// SafeDiv
float SafeDiv (const float left, const float right, const float defVal)  { return not IsZero( right ) ? (left / right) : float(defVal); }
float2 SafeDiv (const float2 left, const float right, const float defVal)  { return not IsZero( right ) ? (left / right) : float2(defVal); }
float2 SafeDiv (const float2 left, const float2 right, const float defVal)
{
	return float2(
		SafeDiv( left[0], right[0], defVal ),
		SafeDiv( left[1], right[1], defVal ) );
}
float3 SafeDiv (const float3 left, const float right, const float defVal)  { return not IsZero( right ) ? (left / right) : float3(defVal); }
float3 SafeDiv (const float3 left, const float3 right, const float defVal)
{
	return float3(
		SafeDiv( left[0], right[0], defVal ),
		SafeDiv( left[1], right[1], defVal ),
		SafeDiv( left[2], right[2], defVal ) );
}
float4 SafeDiv (const float4 left, const float right, const float defVal)  { return not IsZero( right ) ? (left / right) : float4(defVal); }
float4 SafeDiv (const float4 left, const float4 right, const float defVal)
{
	return float4(
		SafeDiv( left[0], right[0], defVal ),
		SafeDiv( left[1], right[1], defVal ),
		SafeDiv( left[2], right[2], defVal ),
		SafeDiv( left[3], right[3], defVal ) );
}

double SafeDiv (const double left, const double right, const double defVal)  { return not IsZero( right ) ? (left / right) : double(defVal); }
double2 SafeDiv (const double2 left, const double right, const double defVal)  { return not IsZero( right ) ? (left / right) : double2(defVal); }
double2 SafeDiv (const double2 left, const double2 right, const double defVal)
{
	return double2(
		SafeDiv( left[0], right[0], defVal ),
		SafeDiv( left[1], right[1], defVal ) );
}
double3 SafeDiv (const double3 left, const double right, const double defVal)  { return not IsZero( right ) ? (left / right) : double3(defVal); }
double3 SafeDiv (const double3 left, const double3 right, const double defVal)
{
	return double3(
		SafeDiv( left[0], right[0], defVal ),
		SafeDiv( left[1], right[1], defVal ),
		SafeDiv( left[2], right[2], defVal ) );
}
double4 SafeDiv (const double4 left, const double right, const double defVal)  { return not IsZero( right ) ? (left / right) : double4(defVal); }
double4 SafeDiv (const double4 left, const double4 right, const double defVal)
{
	return double4(
		SafeDiv( left[0], right[0], defVal ),
		SafeDiv( left[1], right[1], defVal ),
		SafeDiv( left[2], right[2], defVal ),
		SafeDiv( left[3], right[3], defVal ) );
}


// IsOdd, IsEven
bool IsOdd (const int x)  { return Equals( (x & 1), int(1) ); }
bool2 IsOdd (const int2 x)  { return Equals( (x & 1), int2(1) ); }
bool3 IsOdd (const int3 x)  { return Equals( (x & 1), int3(1) ); }
bool4 IsOdd (const int4 x)  { return Equals( (x & 1), int4(1) ); }

bool IsOdd (const uint x)  { return Equals( (x & 1), uint(1) ); }
bool2 IsOdd (const uint2 x)  { return Equals( (x & 1), uint2(1) ); }
bool3 IsOdd (const uint3 x)  { return Equals( (x & 1), uint3(1) ); }
bool4 IsOdd (const uint4 x)  { return Equals( (x & 1), uint4(1) ); }

#ifdef ARB_gpu_shader_int64_enabled
bool IsOdd (const ilong x)  { return Equals( (x & 1), ilong(1) ); }
bool2 IsOdd (const ilong2 x)  { return Equals( (x & 1), ilong2(1) ); }
bool3 IsOdd (const ilong3 x)  { return Equals( (x & 1), ilong3(1) ); }
bool4 IsOdd (const ilong4 x)  { return Equals( (x & 1), ilong4(1) ); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool IsOdd (const ulong x)  { return Equals( (x & 1), ulong(1) ); }
bool2 IsOdd (const ulong2 x)  { return Equals( (x & 1), ulong2(1) ); }
bool3 IsOdd (const ulong3 x)  { return Equals( (x & 1), ulong3(1) ); }
bool4 IsOdd (const ulong4 x)  { return Equals( (x & 1), ulong4(1) ); }
#endif

bool IsEven (const int x)  { return Equals( (x & 1), int(0) ); }
bool2 IsEven (const int2 x)  { return Equals( (x & 1), int2(0) ); }
bool3 IsEven (const int3 x)  { return Equals( (x & 1), int3(0) ); }
bool4 IsEven (const int4 x)  { return Equals( (x & 1), int4(0) ); }

bool IsEven (const uint x)  { return Equals( (x & 1), uint(0) ); }
bool2 IsEven (const uint2 x)  { return Equals( (x & 1), uint2(0) ); }
bool3 IsEven (const uint3 x)  { return Equals( (x & 1), uint3(0) ); }
bool4 IsEven (const uint4 x)  { return Equals( (x & 1), uint4(0) ); }

#ifdef ARB_gpu_shader_int64_enabled
bool IsEven (const ilong x)  { return Equals( (x & 1), ilong(0) ); }
bool2 IsEven (const ilong2 x)  { return Equals( (x & 1), ilong2(0) ); }
bool3 IsEven (const ilong3 x)  { return Equals( (x & 1), ilong3(0) ); }
bool4 IsEven (const ilong4 x)  { return Equals( (x & 1), ilong4(0) ); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
bool IsEven (const ulong x)  { return Equals( (x & 1), ulong(0) ); }
bool2 IsEven (const ulong2 x)  { return Equals( (x & 1), ulong2(0) ); }
bool3 IsEven (const ulong3 x)  { return Equals( (x & 1), ulong3(0) ); }
bool4 IsEven (const ulong4 x)  { return Equals( (x & 1), ulong4(0) ); }
#endif


// ToUNorm, ToSNorm
float ToUNorm (const float x)  { return x * 0.5 + 0.5; }
float2 ToUNorm (const float2 x)  { return x * 0.5 + 0.5; }
float3 ToUNorm (const float3 x)  { return x * 0.5 + 0.5; }
float4 ToUNorm (const float4 x)  { return x * 0.5 + 0.5; }

double ToUNorm (const double x)  { return x * 0.5 + 0.5; }
double2 ToUNorm (const double2 x)  { return x * 0.5 + 0.5; }
double3 ToUNorm (const double3 x)  { return x * 0.5 + 0.5; }
double4 ToUNorm (const double4 x)  { return x * 0.5 + 0.5; }

float ToSNorm (const float x)  { return x * 2.0 - 1.0; }
float2 ToSNorm (const float2 x)  { return x * 2.0 - 1.0; }
float3 ToSNorm (const float3 x)  { return x * 2.0 - 1.0; }
float4 ToSNorm (const float4 x)  { return x * 2.0 - 1.0; }

double ToSNorm (const double x)  { return x * 2.0 - 1.0; }
double2 ToSNorm (const double2 x)  { return x * 2.0 - 1.0; }
double3 ToSNorm (const double3 x)  { return x * 2.0 - 1.0; }
double4 ToSNorm (const double4 x)  { return x * 2.0 - 1.0; }


// InvLerp, BiLerp
float InvLerp (const float x, const float y, const float interpolated)
{
	return (interpolated - x) / (y - x);
}
float2 InvLerp (const float2 x, const float2 y, const float interpolated)
{
	return (interpolated - x) / (y - x);
}
float3 InvLerp (const float3 x, const float3 y, const float interpolated)
{
	return (interpolated - x) / (y - x);
}
float4 InvLerp (const float4 x, const float4 y, const float interpolated)
{
	return (interpolated - x) / (y - x);
}

double InvLerp (const double x, const double y, const double interpolated)
{
	return (interpolated - x) / (y - x);
}
double2 InvLerp (const double2 x, const double2 y, const double interpolated)
{
	return (interpolated - x) / (y - x);
}
double3 InvLerp (const double3 x, const double3 y, const double interpolated)
{
	return (interpolated - x) / (y - x);
}
double4 InvLerp (const double4 x, const double4 y, const double interpolated)
{
	return (interpolated - x) / (y - x);
}

float BiLerp (const float v0, const float v1, const float v2, const float v3, const float2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}
float2 BiLerp (const float2 v0, const float2 v1, const float2 v2, const float2 v3, const float2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}
float3 BiLerp (const float3 v0, const float3 v1, const float3 v2, const float3 v3, const float2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}
float4 BiLerp (const float4 v0, const float4 v1, const float4 v2, const float4 v3, const float2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}

double BiLerp (const double v0, const double v1, const double v2, const double v3, const double2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}
double2 BiLerp (const)#";

		src << R"#( double2 v0, const double2 v1, const double2 v2, const double2 v3, const double2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}
double3 BiLerp (const double3 v0, const double3 v1, const double3 v2, const double3 v3, const double2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}
double4 BiLerp (const double4 v0, const double4 v1, const double4 v2, const double4 v3, const double2 factor)
{
	return Lerp( Lerp( v0, v3, factor.x ), Lerp( v1, v2, factor.x ), factor.y );
}


// LinearStep, BumpStep, SmoothBumpStep
float LinearStep (const float x, const float edge0, const float edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
float2 LinearStep (const float2 x, const float2 edge0, const float2 edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
float2 LinearStep (const float2 x, const float edge0, const float edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
float3 LinearStep (const float3 x, const float3 edge0, const float3 edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
float3 LinearStep (const float3 x, const float edge0, const float edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
float4 LinearStep (const float4 x, const float4 edge0, const float4 edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
float4 LinearStep (const float4 x, const float edge0, const float edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }

double LinearStep (const double x, const double edge0, const double edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
double2 LinearStep (const double2 x, const double2 edge0, const double2 edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
double2 LinearStep (const double2 x, const double edge0, const double edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
double3 LinearStep (const double3 x, const double3 edge0, const double3 edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
double3 LinearStep (const double3 x, const double edge0, const double edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
double4 LinearStep (const double4 x, const double4 edge0, const double4 edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }
double4 LinearStep (const double4 x, const double edge0, const double edge1)  { return Clamp( (x - edge0) / (edge1 - edge0), 0.0, 1.0 ); }

float BumpStep (const float x, const float edge0, const float edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
float2 BumpStep (const float2 x, const float2 edge0, const float2 edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
float2 BumpStep (const float2 x, const float edge0, const float edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
float3 BumpStep (const float3 x, const float3 edge0, const float3 edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
float3 BumpStep (const float3 x, const float edge0, const float edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
float4 BumpStep (const float4 x, const float4 edge0, const float4 edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
float4 BumpStep (const float4 x, const float edge0, const float edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }

double BumpStep (const double x, const double edge0, const double edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
double2 BumpStep (const double2 x, const double2 edge0, const double2 edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
double2 BumpStep (const double2 x, const double edge0, const double edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
double3 BumpStep (const double3 x, const double3 edge0, const double3 edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
double3 BumpStep (const double3 x, const double edge0, const double edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
double4 BumpStep (const double4 x, const double4 edge0, const double4 edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }
double4 BumpStep (const double4 x, const double edge0, const double edge1)  { return 1.0 - Abs( LinearStep( x, edge0, edge1 ) - 0.5 ) * 2.0; }

float SmoothBumpStep (const float x, const float edge0, const float edge1)
{
		float t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
float2 SmoothBumpStep (const float2 x, const float2 edge0, const float2 edge1)
{
		float2 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
float2 SmoothBumpStep (const float2 x, const float edge0, const float edge1)
{
		float2 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
float3 SmoothBumpStep (const float3 x, const float3 edge0, const float3 edge1)
{
		float3 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
float3 SmoothBumpStep (const float3 x, const float edge0, const float edge1)
{
		float3 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
float4 SmoothBumpStep (const float4 x, const float4 edge0, const float4 edge1)
{
		float4 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
float4 SmoothBumpStep (const float4 x, const float edge0, const float edge1)
{
		float4 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}

double SmoothBumpStep (const double x, const double edge0, const double edge1)
{
		double t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
double2 SmoothBumpStep (const double2 x, const double2 edge0, const double2 edge1)
{
		double2 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
double2 SmoothBumpStep (const double2 x, const double edge0, const double edge1)
{
		double2 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
double3 SmoothBumpStep (const double3 x, const double3 edge0, const double3 edge1)
{
		double3 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
double3 SmoothBumpStep (const double3 x, const double edge0, const double edge1)
{
		double3 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
double4 SmoothBumpStep (const double4 x, const double4 edge0, const double4 edge1)
{
		double4 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}
double4 SmoothBumpStep (const double4 x, const double edge0, const double edge1)
{
		double4 t = BumpStep( x, edge0, edge1 );
		return t * t * (3.0 - 2.0 * t);
}

)#";

	}
}	// glsl_vfs
