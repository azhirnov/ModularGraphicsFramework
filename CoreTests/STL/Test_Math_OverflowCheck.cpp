// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath::OverflowChecker;


extern void Test_Math_OverflowCheck ()
{
	TEST( not AdditionIsSafe( MinValue<int>() +  10,  -11 ) );
	TEST(     AdditionIsSafe( MinValue<int>() + 100, -100 ) );
	TEST(     AdditionIsSafe( MinValue<int>() + 100,  -99 ) );
	
	TEST( not AdditionIsSafe( MaxValue<int>() -  10,  11 ) );
	TEST(     AdditionIsSafe( MaxValue<int>() - 100, 100 ) );
	TEST(     AdditionIsSafe( MaxValue<int>() - 100,  99 ) );
	
	TEST( not SubtractionIsSafe( MinValue<int>() +  10,  11 ) );
	TEST(     SubtractionIsSafe( MinValue<int>() + 100, 100 ) );
	TEST(     SubtractionIsSafe( MinValue<int>() + 100,  99 ) );
	
	TEST( not SubtractionIsSafe( MaxValue<int>() -  10,  -11 ) );
	TEST(     SubtractionIsSafe( MaxValue<int>() - 100, -100 ) );
	TEST(     SubtractionIsSafe( MaxValue<int>() - 100,  -99 ) );

	TEST( not AdditionIsSafe( MaxValue<uint>(),    1u ) );
	TEST(     AdditionIsSafe( MaxValue<uint>()-1,  1u ) );
	TEST(     AdditionIsSafe( MaxValue<uint>()-10, 9u ) );
	
	TEST( not SubtractionIsSafe( 10u,  11u ) );
	TEST(     SubtractionIsSafe( 10u,  10u ) );
	TEST(     SubtractionIsSafe( 10u,   9u ) );
}
