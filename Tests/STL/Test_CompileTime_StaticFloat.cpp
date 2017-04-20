// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace CompileTime;


extern void Test_CompileTime_StaticFloat ()
{
#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

	typedef StaticFloat< 1, 0 >		f0;
	typedef StaticFloat< 2, 0 >		f1;

	STATIC_ASSERT( not f0::Equal< f1 >::value );
	STATIC_ASSERT( f0::NotEqual< f1 >::value );
	
	STATIC_ASSERT( f0::FastEqual< f0 >::value );
	STATIC_ASSERT( not f0::FastEqual< f1 >::value );

	STATIC_ASSERT( f0::LessThan< f1 >::value );

	typedef f1::Div< f0 >	f2;
	STATIC_ASSERT( f2::Equal< f1 >::value );

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
}
