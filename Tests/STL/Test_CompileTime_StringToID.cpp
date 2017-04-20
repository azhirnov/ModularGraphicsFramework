// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::CompileTime;


DECL_STRING_TO_ID( MyID, 1 );
DECL_STRING_TO_ID( MyID2, 15 );


extern void Test_CompileTime_StringToID ()
{
	STATIC_ASSERT( MyID::_ID == 1 );
	STATIC_ASSERT( MyID2::_ID == 15 );

	auto	a0 = "qwertyuiopas"_str;	// max size
	auto	a1 = MyID::FromString( a0 );
	auto	a2 = MyID::ToString( a1 );
	ASSERT( a2.StartsWithIC( a0 ) );
	
	auto	b0 = "023459"_str;			// max size for numbers
	auto	b1 = MyID::FromString( b0 );
	auto	b2 = MyID::ToString( b1 );
	ASSERT( b2.StartsWithIC( b0 ) );

	auto	c0 = "abc1.-/*"_str;
	auto	c1 = MyID::FromString( c0 );
	auto	c2 = MyID::ToString( c1 );
	ASSERT( c2.StartsWithIC( c0 ) );

	auto	d0 = MyID2::FromString( c0 );
	ASSERT( d0 != c1 );
	ASSERT( (d0 & ~MyID::_IDMask) == (c1 & ~MyID::_IDMask) );
}
