// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


extern void Test_Types_Cast ()
{
	const float a = Cast<float>( 10 );

	const int b = CompileTime::ValueToType< int, Cast<int>( 6u ) >::value;
}
