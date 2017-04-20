// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_Color ()
{
	color4u c;
	ubyte R = 0x11;
	ubyte G = 0x22;
	ubyte B = 0x33;
	ubyte A = 0x44;

	c.FromIntRGB( (R<<16) | (G<<8) | B );
	ASSERT( c.R() == R );
	ASSERT( c.G() == G );
	ASSERT( c.B() == B );
	
	c.FromIntRGBA( (R<<24) | (G<<16) | (B<<8) | A );
	ASSERT( c.R() == R );
	ASSERT( c.G() == G );
	ASSERT( c.B() == B );
	ASSERT( c.A() == A );
	
	c.FromIntABGR( (A<<24) | (B<<16) | (G<<8) | R );
	ASSERT( c.R() == R );
	ASSERT( c.G() == G );
	ASSERT( c.B() == B );
	ASSERT( c.A() == A );
	
	c.FromIntARGB( (A<<24) | (R<<16) | (G<<8) | B );
	ASSERT( c.R() == R );
	ASSERT( c.G() == G );
	ASSERT( c.B() == B );
	ASSERT( c.A() == A );
}
