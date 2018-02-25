// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::CompileTime;


extern void Test_CompileTime_MainType ()
{
	STATIC_ASSERT( (IsSameTypes< int,		GenTypeStd< byte, byte > >) );
	STATIC_ASSERT( (IsSameTypes< int,		GenTypeStd< byte, ubyte > >) );
	STATIC_ASSERT( (IsSameTypes< int,		GenTypeStd< short, byte > >) );
	STATIC_ASSERT( (IsSameTypes< int,		GenTypeStd< byte, int > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		GenTypeStd< uint, byte > >) );
	STATIC_ASSERT( (IsSameTypes< ilong,		GenTypeStd< byte, ilong > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		GenTypeStd< ulong, byte > >) );
	STATIC_ASSERT( (IsSameTypes< float,		GenTypeStd< byte, float > >) );
	STATIC_ASSERT( (IsSameTypes< double,	GenTypeStd< double, byte > >) );
	STATIC_ASSERT( (IsSameTypes< double,	GenTypeStd< float, double > >) );
	STATIC_ASSERT( (IsSameTypes< double,	GenTypeStd< double, float > >) );
	STATIC_ASSERT( (IsSameTypes< float,		GenTypeStd< ulong, float > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		GenTypeStd< int, uint > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		GenTypeStd< ulong, ilong > >) );

	STATIC_ASSERT( (IsSameTypes< byte,		GenType< byte, byte > >) );
	STATIC_ASSERT( (IsSameTypes< ubyte,		GenType< byte, ubyte > >) );
	STATIC_ASSERT( (IsSameTypes< short,		GenType< byte, short > >) );
	STATIC_ASSERT( (IsSameTypes< ushort,	GenType< byte, ushort > >) );
	STATIC_ASSERT( (IsSameTypes< int,		GenType< byte, int > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		GenType< byte, uint > >) );
	STATIC_ASSERT( (IsSameTypes< ilong,		GenType< byte, ilong > >) );
	STATIC_ASSERT( (IsSameTypes< float,		GenType< byte, float > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		GenType< uint, int > >) );
	STATIC_ASSERT( (IsSameTypes< uint,		GenType< int, uint > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		GenType< ulong, ilong > >) );
	STATIC_ASSERT( (IsSameTypes< ulong,		GenType< ilong, ulong > >) );
	STATIC_ASSERT( (IsSameTypes< double,	GenType< float, double > >) );
	STATIC_ASSERT( (IsSameTypes< double,	GenType< double, float > >) );
	STATIC_ASSERT( (IsSameTypes< float,		GenType< ulong, float > >) );
	STATIC_ASSERT( (IsSameTypes< float,		GenType< half, byte > >) );
	//STATIC_ASSERT( (IsSameTypes< float,	GenType< Fixed16_t, int > >) );
	//STATIC_ASSERT( (IsSameTypes< double,	GenType< half, Fixed64_t > >) );
	
	// to check C++ spec
	auto a = int(1) * uint(2);		STATIC_ASSERT(( IsSameTypes< decltype(a),	GenTypeStd< int, uint > > ));
	auto b = float(1) * ilong(2);	STATIC_ASSERT(( IsSameTypes< decltype(b),	GenTypeStd< float, ilong > > ));
	auto c = ulong(1) * float(2);	STATIC_ASSERT(( IsSameTypes< decltype(c),	GenTypeStd< ulong, float > > ));
	auto d = ilong(1) * ubyte(2);	STATIC_ASSERT(( IsSameTypes< decltype(d),	GenTypeStd< ilong, ubyte > > ));
	auto e = byte(1) * ubyte(2);	STATIC_ASSERT(( IsSameTypes< decltype(e),	GenTypeStd< byte, ubyte > > ));
	auto f = byte(1) * byte(2);		STATIC_ASSERT(( IsSameTypes< decltype(f),	GenTypeStd< byte, byte > > ));


	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	GenType< GXMath::Radians<float>, float > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	GenType< float, GXMath::Radians<float> > > ));

	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<double>,	GenType< double, GXMath::Radians<float> > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<double>,	GenType< double, GXMath::Radians<float> > > ));

	STATIC_ASSERT(( IsSameTypes< GXMath::float3,	GenType< GXMath::float3, float > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::float3,	GenType< float, GXMath::float3 > > ));


	STATIC_ASSERT(( IsSameTypes< GXMath::float4,	GenType< GXMath::uint4, GXMath::float3 > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::float4,	GenType< GXMath::uint4, float > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::ulong4,	GenType< GXMath::uint4, ulong > > ));

	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<uint>,	GenType< GXTypes::Bytes<uint>, uint > > ));
	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<uint>,	GenType< uint,  GXTypes::Bytes<uint> > > ));
	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<uint>,	GenType< int,   GXTypes::Bytes<uint> > > ));
	STATIC_ASSERT(( IsSameTypes< GXTypes::Bytes<ulong>,	GenType< ulong, GXTypes::Bytes<uint> > > ));

	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	GenType< GXMath::Radians<float>, GXTypes::Bytes<uint>   > > ));
	STATIC_ASSERT(( IsSameTypes< GXMath::Radians<float>,	GenType< GXTypes::Bytes<uint>,   GXMath::Radians<float> > > ));


	STATIC_ASSERT( not IsWrapper<float> );
	STATIC_ASSERT( IsWrapper< GXMath::Radians<float> > );
	STATIC_ASSERT( IsScalar< float > );
	STATIC_ASSERT( not IsVector< float > );
	STATIC_ASSERT( IsVector< GXMath::float3 > );
	STATIC_ASSERT( not IsScalar< GXMath::float3 > );
	STATIC_ASSERT( IsArithmetic< float > );
	STATIC_ASSERT( IsArithmetic< GXMath::float3 > );
}
