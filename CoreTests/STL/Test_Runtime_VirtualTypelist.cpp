// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::CompileTime;
using namespace GX_STL::Runtime;


extern void Test_Runtime_VirtualTypelist ()
{
	using Typelist1 = TypeListFrom< int, float, ulong, bool >;

	TypeIdList	vtl = TypeIdList( UninitializedT< Typelist1 >() );

	TEST( vtl.HasType( TypeIdOf<float>() ) );
}
