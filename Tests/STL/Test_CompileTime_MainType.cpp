// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::CompileTime;


extern void Test_CompileTime_MainType ()
{
	STATIC_ASSERT( (IsSameTypes< int,		MainTypeStd< byte, byte > >) );
	STATIC_ASSERT( (IsSameTypes< int,		MainTypeStd< byte, ubyte > >) );
	STATIC_ASSERT( (IsSameTypes< int,		MainTypeStd< short, byte > >) );
	STATIC_ASSERT( (IsSameTypes< int,		MainTypeStd< byte, int > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		MainTypeStd< uint, byte > >) );
	STATIC_ASSERT( (IsSameTypes< ilong,		MainTypeStd< byte, ilong > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		MainTypeStd< ulong, byte > >) );
	STATIC_ASSERT( (IsSameTypes< float,		MainTypeStd< byte, float > >) );
	STATIC_ASSERT( (IsSameTypes< double,	MainTypeStd< double, byte > >) );
	STATIC_ASSERT( (IsSameTypes< double,	MainTypeStd< float, double > >) );
	STATIC_ASSERT( (IsSameTypes< double,	MainTypeStd< double, float > >) );
	STATIC_ASSERT( (IsSameTypes< float,		MainTypeStd< ulong, float > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		MainTypeStd< int, uint > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		MainTypeStd< ulong, ilong > >) );

	STATIC_ASSERT( (IsSameTypes< byte,		MainType< byte, byte > >) );
	STATIC_ASSERT( (IsSameTypes< ubyte,		MainType< byte, ubyte > >) );
	STATIC_ASSERT( (IsSameTypes< short,		MainType< byte, short > >) );
	STATIC_ASSERT( (IsSameTypes< ushort,	MainType< byte, ushort > >) );
	STATIC_ASSERT( (IsSameTypes< int,		MainType< byte, int > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		MainType< byte, uint > >) );
	STATIC_ASSERT( (IsSameTypes< ilong,		MainType< byte, ilong > >) );
	STATIC_ASSERT( (IsSameTypes< float,		MainType< byte, float > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		MainType< uint, int > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		MainType< int, uint > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		MainType< ulong, ilong > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		MainType< ilong, ulong > >) );
	STATIC_ASSERT( (IsSameTypes< double,	MainType< float, double > >) );
	STATIC_ASSERT( (IsSameTypes< double,	MainType< double, float > >) );
	STATIC_ASSERT( (IsSameTypes< float,		MainType< ulong, float > >) );
	STATIC_ASSERT( (IsSameTypes< float,		MainType< half, byte > >) );
	//STATIC_ASSERT( (IsSameTypes< float,	MainType< Fixed16_t, int > >) );
	//STATIC_ASSERT( (IsSameTypes< double,	MainType< half, Fixed64_t > >) );
	
	// to check C++ spec
	auto a = int(1) * uint(2);		STATIC_ASSERT(( IsSameTypes< decltype(a),	MainTypeStd< int, uint > > ));
	auto b = float(1) * ilong(2);	STATIC_ASSERT(( IsSameTypes< decltype(b),	MainTypeStd< float, ilong > > ));
	auto c = ulong(1) * float(2);	STATIC_ASSERT(( IsSameTypes< decltype(c),	MainTypeStd< ulong, float > > ));
	auto d = ilong(1) * ubyte(2);	STATIC_ASSERT(( IsSameTypes< decltype(d),	MainTypeStd< ilong, ubyte > > ));
	auto e = byte(1) * ubyte(2);	STATIC_ASSERT(( IsSameTypes< decltype(e),	MainTypeStd< byte, ubyte > > ));
	auto f = byte(1) * byte(2);		STATIC_ASSERT(( IsSameTypes< decltype(f),	MainTypeStd< byte, byte > > ));


	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	MainType< GXMath::Radians<float>, float > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	MainType< float, GXMath::Radians<float> > > ));

	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<double>,	MainType< double, GXMath::Radians<float> > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<double>,	MainType< double, GXMath::Radians<float> > > ));

	STATIC_ASSERT(( IsSameTypes< GXMath::float3,	MainType< GXMath::float3, float > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::float3,	MainType< float, GXMath::float3 > > ));


	STATIC_ASSERT(( IsSameTypes< GXMath::float4,	MainType< GXMath::uint4, GXMath::float3 > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::float4,	MainType< GXMath::uint4, float > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::ulong4,	MainType< GXMath::uint4, ulong > > ));

	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<uint>,	MainType< GXTypes::Bytes<uint>, uint > > ));
	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<uint>,	MainType< uint,  GXTypes::Bytes<uint> > > ));
	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<uint>,	MainType< int,   GXTypes::Bytes<uint> > > ));
	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<ulong>,	MainType< ulong, GXTypes::Bytes<uint> > > ));

	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	MainType< GXMath::Radians<float>, GXTypes::Bytes<uint>   > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	MainType< GXTypes::Bytes<uint>,   GXMath::Radians<float> > > ));


	STATIC_ASSERT( not IsWrapper<float> );
	STATIC_ASSERT( IsWrapper< GXMath::Radians<float> > );
	STATIC_ASSERT( IsScalar< float > );
	STATIC_ASSERT( not IsVector< float > );
	STATIC_ASSERT( IsVector< GXMath::float3 > );
	STATIC_ASSERT( not IsScalar< GXMath::float3 > );
	STATIC_ASSERT( IsArithmetic< float > );
	STATIC_ASSERT( IsArithmetic< GXMath::float3 > );
}
