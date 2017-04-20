// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::CompileTime;
using namespace GX_STL::Runtime;


extern void Test_Runtime_VirtualTypelist ()
{
	using Typelist1 = TypeListFrom< int, float, ulong, bool >;

	VirtualTypeList	vtl = VirtualTypeList( UninitializedT< Typelist1 >() );

	ASSERT( vtl.HasType( TypeIdOf<float>() ) );
}
