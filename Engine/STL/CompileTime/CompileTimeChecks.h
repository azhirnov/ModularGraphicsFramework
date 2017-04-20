// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Engine.STL.h"


namespace GX_STL
{
namespace GXTypes
{

	STATIC_ASSERT( sizeof(char) == 1 );
	STATIC_ASSERT( sizeof(bool) == sizeof(char) );
	STATIC_ASSERT( sizeof(byte) == sizeof(char) );
	STATIC_ASSERT( sizeof(ubyte) == sizeof(char) );
	
	STATIC_ASSERT( sizeof(short) == sizeof(byte)*2 );
	STATIC_ASSERT( sizeof(ushort) == sizeof(ubyte)*2 );
	
	STATIC_ASSERT( sizeof(int) == sizeof(short)*2 );
	STATIC_ASSERT( sizeof(uint) == sizeof(ushort)*2 );
	
	STATIC_ASSERT( sizeof(ilong) == sizeof(int)*2 );
	STATIC_ASSERT( sizeof(ulong) == sizeof(uint)*2 );

	STATIC_ASSERT( sizeof(usize) == sizeof(void *) );
	STATIC_ASSERT( sizeof(isize) == sizeof(void *) );

	STATIC_ASSERT( sizeof(float)*2 == sizeof(double) );
	STATIC_ASSERT( sizeof(long double) >= sizeof(double) );

}	// GXTypes
}	// GX_STL
