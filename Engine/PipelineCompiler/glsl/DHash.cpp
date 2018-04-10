// This is generated file, don't change anything!

#include "glsl_source_vfs.h"

namespace glsl_vfs
{
	extern void VFS_Hash_DHash (OUT String &src)
	{
		src << R"#(/*
	This is generated file, don't change anything!
*/

// DHash [0;1]

float DHash11 (const float p);
float DHash12 (const float2 p);
float DHash13 (const float3 p);
float DHash14 (const float4 p);
float2 DHash21 (const float p);
float2 DHash22 (const float2 p);
float2 DHash23 (const float3 p);
float2 DHash24 (const float4 p);
float3 DHash31 (const float p);
float3 DHash32 (const float2 p);
float3 DHash33 (const float3 p);
float3 DHash34 (const float4 p);
float4 DHash41 (const float p);
float4 DHash42 (const float2 p);
float4 DHash43 (const float3 p);
float4 DHash44 (const float4 p);

double DHash11 (const double p);
double DHash12 (const double2 p);
double DHash13 (const double3 p);
double DHash14 (const double4 p);
double2 DHash21 (const double p);
double2 DHash22 (const double2 p);
double2 DHash23 (const double3 p);
double2 DHash24 (const double4 p);
double3 DHash31 (const double p);
double3 DHash32 (const double2 p);
double3 DHash33 (const double3 p);
double3 DHash34 (const double4 p);
double4 DHash41 (const double p);
double4 DHash42 (const double2 p);
double4 DHash43 (const double3 p);
double4 DHash44 (const double4 p);



//---------------------------------------------------------


// DHash from https://www.shadertoy.com/view/4djSRW 

float4 DHashScale_float ()  { return float4( 0.1031, 0.1030, 0.0973, 0.1099 ); }

float DHash11 (const float p)
{
	float3 p3 = Fract( float3(p) * DHashScale_float().x );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.x + p3.y) * p3.z );
}

float DHash12 (const float2 p)
{
	float3 p3 = Fract( float3(p.xyx) * DHashScale_float().x );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.x + p3.y) * p3.z );
}

float DHash13 (const float3 p)
{
	float3 p3 = Fract( p * DHashScale_float().x );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.x + p3.y) * p3.z );
}

float2 DHash21 (const float p)
{
	float3 p3 = Fract( float3(p) * DHashScale_float().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xx + p3.yz) * p3.zy );
}

float2 DHash22 (const float2 p)
{
	float3 p3 = Fract( float3(p.xyx) * DHashScale_float().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xx + p3.yz) * p3.zy );
}

float2 DHash23 (const float3 p)
{
	float3 p3 = Fract( p * DHashScale_float().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xx + p3.yz) * p3.zy );
}

float3 DHash31 (const float p)
{
	float3 p3 = Fract( float3(p) * DHashScale_float().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xxy + p3.yzz) * p3.zyx );
}

float3 DHash32 (const float2 p)
{
	float3 p3 = Fract( float3(p.xyx) * DHashScale_float().xyz );
	p3 += Dot( p3, p3.yxz + 19.19 );
	return Fract( (p3.xxy + p3.yzz) * p3.zyx );
}

float3 DHash33 (const float3 p)
{
	float3 p3 = Fract( p * DHashScale_float().xyz );
	p3 += Dot( p3, p3.yxz + 19.19 );
return Fract( (p3.xxy + p3.yxx) * p3.zyx );
}

float4 DHash41 (const float p)
{
	float4 p4 = Fract( float4(p) * DHashScale_float() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}

float4 DHash42 (const float2 p)
{
	float4 p4 = Fract( float4(p.xyxy) * DHashScale_float() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}

float4 DHash43 (const float3 p)
{
	float4 p4 = Fract( float4(p.xyzx) * DHashScale_float() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}

float4 DHash44 (const float4 p)
{
	float4 p4 = Fract( p * DHashScale_float() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}



double4 DHashScale_double ()  { return double4( 0.1031, 0.1030, 0.0973, 0.1099 ); }

double DHash11 (const double p)
{
	double3 p3 = Fract( double3(p) * DHashScale_double().x );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.x + p3.y) * p3.z );
}

double DHash12 (const double2 p)
{
	double3 p3 = Fract( double3(p.xyx) * DHashScale_double().x );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.x + p3.y) * p3.z );
}

double DHash13 (const double3 p)
{
	double3 p3 = Fract( p * DHashScale_double().x );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.x + p3.y) * p3.z );
}

double2 DHash21 (const double p)
{
	double3 p3 = Fract( double3(p) * DHashScale_double().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xx + p3.yz) * p3.zy );
}

double2 DHash22 (const double2 p)
{
	double3 p3 = Fract( double3(p.xyx) * DHashScale_double().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xx + p3.yz) * p3.zy );
}

double2 DHash23 (const double3 p)
{
	double3 p3 = Fract( p * DHashScale_double().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xx + p3.yz) * p3.zy );
}

double3 DHash31 (const double p)
{
	double3 p3 = Fract( double3(p) * DHashScale_double().xyz );
	p3 += Dot( p3, p3.yzx + 19.19 );
	return Fract( (p3.xxy + p3.yzz) * p3.zyx );
}

double3 DHash32 (const double2 p)
{
	double3 p3 = Fract( double3(p.xyx) * DHashScale_double().xyz );
	p3 += Dot( p3, p3.yxz + 19.19 );
	return Fract( (p3.xxy + p3.yzz) * p3.zyx );
}

double3 DHash33 (const double3 p)
{
	double3 p3 = Fract( p * DHashScale_double().xyz );
	p3 += Dot( p3, p3.yxz + 19.19 );
return Fract( (p3.xxy + p3.yxx) * p3.zyx );
}

double4 DHash41 (const double p)
{
	double4 p4 = Fract( double4(p) * DHashScale_double() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}

double4 DHash42 (const double2 p)
{
	double4 p4 = Fract( double4(p.xyxy) * DHashScale_double() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}

double4 DHash43 (const double3 p)
{
	double4 p4 = Fract( double4(p.xyzx) * DHashScale_double() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}

double4 DHash44 (const double4 p)
{
	double4 p4 = Fract( p * DHashScale_double() );
	p4 += Dot( p4, p4.wzxy + 19.19 );
	return Fract( (p4.xxyz + p4.yzzw) * p4.zywx );
}



)#";

	}
}	// glsl_vfs
