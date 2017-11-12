// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::CompileTime;


extern void Test_CompileTime_Map ()
{
	using map1 = CreateMap< int, ValueToType<int, 0> >;
	using map2 = map1::Add< float, ValueToType<int, 2> >;

	STATIC_ASSERT( map2::Get< int >::value == 0 );
	STATIC_ASSERT( map2::Get< float >::value == 2 );
	STATIC_ASSERT( map2::GetValue< int > == 0 );
}
