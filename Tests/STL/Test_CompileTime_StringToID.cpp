// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;
using namespace GX_STL::CompileTime;


DECL_STRING_TO_ID( MyID, 1 );
DECL_STRING_TO_ID( MyID2, 15 );


extern void Test_CompileTime_StringToID ()
{
	STATIC_ASSERT( MyID::_ID == 1 );
	STATIC_ASSERT( MyID2::_ID == 15 );
	
	STATIC_ASSERT( ("_"_MyID >> 9) == 0 );

	STATIC_ASSERT( ("*"_MyID >> 14) == 0 );
	STATIC_ASSERT( (":"_MyID >> 14) == 0 );
	STATIC_ASSERT( (";"_MyID >> 14) == 0 );
	STATIC_ASSERT( ("="_MyID >> 14) == 0 );
	STATIC_ASSERT( ("%"_MyID >> 14) == 0 );

	const ulong aa1 = "%"_MyID;
	const ulong aa3 = ("%"_MyID) >> 5;
	const ulong aa4 = ("%"_MyID) >> 10;

	const ulong aa2 = "AAA"_MyID;

	switch ( "*"_MyID )
	{
		case "*"_MyID : break;
		default :		WARNING( "----" );
	}

	auto	a0 = "qwertyuiopas"_str;	// max size
	auto	a1 = MyID::FromString( a0 );
	auto	a2 = MyID::ToString( a1 );
	TEST( a2.StartsWithIC( a0 ) );
	
	auto	b0 = "023459"_str;			// max size for numbers
	auto	b1 = MyID::FromString( b0 );
	auto	b2 = MyID::ToString( b1 );
	TEST( b2.StartsWithIC( b0 ) );

	auto	c0 = "abZ1.-+_"_str;
	auto	c1 = MyID::FromString( c0 );
	auto	c2 = MyID::ToString( c1 );
	TEST( c2.StartsWithIC( c0 ) );

	auto	d0 = MyID2::FromString( c0 );
	TEST( d0 != c1 );
	TEST( (d0 & ~MyID::_IDMask) == (c1 & ~MyID::_IDMask) );

	auto	e0 = "*/!?~\""_str;
	auto	e1 = MyID::FromString( e0 );
	auto	e2 = MyID::ToString( e1 );
	TEST( e2.StartsWithIC( e0 ) );
	
	auto	f0 = "[]()\\$"_str;
	auto	f1 = MyID::FromString( f0 );
	auto	f2 = MyID::ToString( f1 );
	TEST( f2.StartsWithIC( f0 ) );

	auto	g0 = "&|^<>:"_str;
	auto	g1 = MyID::FromString( g0 );
	auto	g2 = MyID::ToString( g1 );
	TEST( g2.StartsWithIC( g0 ) );

	auto	i0 = ";=%189"_str;
	auto	i1 = MyID::FromString( i0 );
	auto	i2 = MyID::ToString( i1 );
	TEST( i2.StartsWithIC( i0 ) );

	const uint	sw0 = _math_hidden_::_ParseSwizzle( "RRRR"_Swizzle );
	STATIC_ASSERT( sw0 == 0x1111 );
	
	const uint	sw1 = _math_hidden_::_ParseSwizzle( " RRRR"_Swizzle );
	STATIC_ASSERT( sw1 == ~0u );
	
	const uint	sw2 = _math_hidden_::_ParseSwizzle( "RGBA"_Swizzle );
	STATIC_ASSERT( sw2 == 0x4321 );
	
	const uint	sw3 = _math_hidden_::_ParseSwizzle( "YX"_Swizzle );
	STATIC_ASSERT( sw3 == 0x12 );

	const uint	sw4 = _math_hidden_::_ParseSwizzle( "R0"_Swizzle );
	STATIC_ASSERT( sw4 == 0x51 );
	
	const uint	sw5 = _math_hidden_::_ParseSwizzle( "R01G"_Swizzle );
	STATIC_ASSERT( sw5 == 0x2651 );

	const uint	sw6 = _math_hidden_::_ParseSwizzle( "R "_Swizzle );
	STATIC_ASSERT( sw6 == 0x1 );

	const uint	sw7 = _math_hidden_::_ParseSwizzle( "R G "_Swizzle );
	STATIC_ASSERT( sw7 == 0x21 );
}
