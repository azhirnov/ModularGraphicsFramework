// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/CompileTime/NewTypeInfo.h"


namespace GX_STL
{
namespace GXMath
{

	// Real
# if !defined(GX_REAL_TYPE_SIZE)

#	undef  GX_REAL_TYPE_SIZE
#	define GX_REAL_TYPE_SIZE	32
#	pragma message ("used 32bit real type!")

# endif	// GX_REAL_TYPE_SIZE

	
	// Real
# if GX_REAL_TYPE_SIZE == 16
	typedef half					real;
# elif GX_REAL_TYPE_SIZE == 64
	typedef double					real;
# else
	typedef float					real;
# endif


	constexpr real operator "" _r (long double value)	{ return real(value); }
	constexpr real operator "" _R (long double value)	{ return real(value); }



	// Vector
	template <typename T, usize I>	struct Vec;

	// Vec1
	typedef Vec< float,		1 >		float1;
	typedef Vec< double,	1 >		double1;
	typedef	Vec< byte,		1 >		byte1;
	typedef	Vec< short,		1 >		short1;
	typedef Vec< int,		1 >		int1;
	typedef Vec< ilong,		1 >		ilong1;
	typedef	Vec< ubyte,		1 >		ubyte1;
	typedef	Vec< ushort,	1 >		ushort1;
	typedef Vec< uint,		1 >		uint1;
	typedef Vec< ulong,		1 >		ulong1;
	typedef Vec< usize,		1 >		usize1;
	typedef Vec< isize,		1 >		isize1;
	typedef Vec< bool,		1 >		bool1;
	typedef Vec< real,		1 >		real1;


	// Vec2
	typedef Vec< float,		2 >		float2;
	typedef Vec< double,	2 >		double2;
	typedef	Vec< byte,		2 >		byte2;
	typedef	Vec< short,		2 >		short2;
	typedef Vec< int,		2 >		int2;
	typedef Vec< ilong,		2 >		ilong2;
	typedef	Vec< ubyte,		2 >		ubyte2;
	typedef	Vec< ushort,	2 >		ushort2;
	typedef Vec< uint,		2 >		uint2;
	typedef Vec< ulong,		2 >		ulong2;
	typedef Vec< usize,		2 >		usize2;
	typedef Vec< isize,		2 >		isize2;
	typedef Vec< bool,		2 >		bool2;
	typedef Vec< real,		2 >		real2;


	// Vec3
	typedef Vec< float,		3 >		float3;
	typedef Vec< double,	3 >		double3;
	typedef	Vec< byte,		3 >		byte3;
	typedef	Vec< short,		3 >		short3;
	typedef Vec< int,		3 >		int3;
	typedef Vec< ilong,		3 >		ilong3;
	typedef	Vec< ubyte,		3 >		ubyte3;
	typedef	Vec< ushort,	3 >		ushort3;
	typedef Vec< uint,		3 >		uint3;
	typedef Vec< ulong,		3 >		ulong3;
	typedef Vec< usize,		3 >		usize3;
	typedef Vec< isize,		3 >		isize3;
	typedef Vec< bool,		3 >		bool3;
	typedef Vec< real,		3 >		real3;


	// Vec4
	typedef Vec< float,		4 >		float4;
	typedef Vec< double,	4 >		double4;
	typedef	Vec< byte,		4 >		byte4;
	typedef	Vec< short,		4 >		short4;
	typedef Vec< int,		4 >		int4;
	typedef Vec< ilong,		4 >		ilong4;
	typedef	Vec< ubyte,		4 >		ubyte4;
	typedef	Vec< ushort,	4 >		ushort4;
	typedef Vec< uint,		4 >		uint4;
	typedef Vec< ulong,		4 >		ulong4;
	typedef Vec< usize,		4 >		usize4;
	typedef Vec< isize,		4 >		isize4;
	typedef Vec< bool,		4 >		bool4;
	typedef Vec< real,		4 >		real4;


	// Color
	struct color4u;
	struct color4f;
	

	// Matrix
	template <typename T, usize C, usize R>	struct Matrix;

	// Matrix2x2
	typedef Matrix< float,	2, 2 >		float2x2;
	typedef Matrix< double,	2, 2 >		double2x2;
	typedef Matrix< real,	2, 2 >		real2x2;
	

	// Matrix2x3
	typedef Matrix< float,	2, 3 >		float2x3;
	typedef Matrix< double,	2, 3 >		double2x3;
	typedef Matrix< real,	2, 3 >		real2x3;
	

	// Matrix2x4
	typedef Matrix< float,	2, 4 >		float2x4;
	typedef Matrix< double,	2, 4 >		double2x4;
	typedef Matrix< real,	2, 4 >		real2x4;


	// Matrix3x2
	typedef Matrix< float,	3, 2 >		float3x2;
	typedef Matrix< double,	3, 2 >		double3x2;
	typedef Matrix< real,	3, 2 >		real3x2;


	// Matrix3x3
	typedef Matrix< float,	3, 3 >		float3x3;
	typedef Matrix< double,	3, 3 >		double3x3;
	typedef Matrix< real,	3, 3 >		real3x3;
	

	// Matrix3x4
	typedef Matrix< float,	3, 4 >		float3x4;
	typedef Matrix< double,	3, 4 >		double3x4;
	typedef Matrix< real,	3, 4 >		real3x4;
	

	// Matrix4x2
	typedef Matrix< float,	4, 2 >		float4x2;
	typedef Matrix< double,	4, 2 >		double4x2;
	typedef Matrix< real,	4, 2 >		real4x2;


	// Matrix4x3
	//typedef Matrix< half,	4, 3 >		half4x3;
	typedef Matrix< float,	4, 3 >		float4x3;
	typedef Matrix< double,	4, 3 >		double4x3;
	typedef Matrix< real,	4, 3 >		real4x3;


	// Matrix4x4
	typedef Matrix< float,	4, 4 >		float4x4;
	typedef Matrix< double,	4, 4 >		double4x4;
	typedef Matrix< real,	4, 4 >		real4x4;


	// Quaternion
	template <typename T> struct Quaternion;

	typedef Quaternion< float >			fquat;
	typedef Quaternion< double >		dquat;
	typedef Quaternion< real >			quat;



	// Dimensions
	template <typename T, usize I>	using BytesVec = Vec< Bytes<T>, I >;
	template <typename T, usize I>	using BitsVec = Vec< Bits<T>, I >;


	template <typename T>	struct Radians;
	template <typename T>	struct Degrees;

	template <typename T, usize I>	using RadiansVec = Vec< Radians<T>, I >;
	template <typename T, usize I>	using DegreesVec = Vec< Degrees<T>, I >;


}	// GXMath
}	// GX_STL
