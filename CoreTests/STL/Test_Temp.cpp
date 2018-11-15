// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Temporary tests.
*/

#include "CoreTests/STL/Common.h"
#include "Debug.h"
#include <array>

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;



extern void Test_Temp ()
{
	int i = 0;
	
	auto& j = i;

	const auto	lambda = [j] ()
	{
		STATIC_ASSERT( not TypeTraits::IsAnyReference< decltype(j) > );

		LOG( typeid(j).raw_name(), ELog::Info );
	};

	lambda();


	WARNING( "!!!" );
}
