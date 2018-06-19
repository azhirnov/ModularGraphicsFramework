// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


static void TestRange1 ()
{
	Array<int>			arr1;		arr1.Resize( 1000 );
	const Array<int>	arr2{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	int					arr3[100] = {};
	const int			arr4[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	Array<int>			arr5;


	usize idx = 0;
	for (auto& a : Range(arr1))
	{
		a = int(++idx);
	}
	TEST( idx == arr1.Count() );


	idx = 0;
	for (auto& a : Range(arr2))
	{
		TEST( a == arr1[idx++] );
	}
	TEST( idx == arr2.Count() );


	idx = 0;
	for (auto& b : Range(arr3))
	{
		b = arr1[idx++];
	}
	TEST( idx == CountOf(arr3) );


	idx = 0;
	for (auto& b : Range(arr4))
	{
		TEST( b == arr3[idx++] );
	}
	TEST( idx == CountOf(arr4) );

	for (auto& a : Range(arr5))
	{
		a = 1;
	}
}


static void TestRange2 ()
{
	Array<int>			arr1;		arr1.Resize( 100 );
	const Array<int>	arr2{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };


	usize idx = 0;
	for (auto& a : RevRange(arr1))
	{
		a = int(arr1.Count() - (idx++));
	}
	TEST( idx == arr1.Count() );


	idx = 0;
	for (auto& a : RevRange(arr2))
	{
		TEST( a == arr1[ arr2.Count() - (++idx) ] );
	}
	TEST( idx == arr2.Count() );
}


static void TestRange3 ()
{
	const int	arr[100] = {};
	
	usize	cnt = 0;
	for (auto a : Range(arr, 0, 3))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == 3 );

	cnt = 0;
	for (auto a : Range(arr, 4, 3))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == 3 );

	cnt = 0;
	for (auto a : Range(arr, 0, UMax))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(arr) );

	cnt = 0;
	for (auto a : Range(arr, UMax, 0))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == 0 );

	cnt = 0;
	for (auto a : Range(arr, 1, UMax))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt+1 == CountOf(arr) );

	cnt = 0;
	for (auto a : Range(arr, 2, UMax))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt+2 == CountOf(arr) );
}


static void TestRange4 ()
{
	const HashSet<int>	set{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	usize	cnt = 0;
	for (auto a : set)
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(set) );

	cnt = 0;
	int	sum = 0;
	for (auto a : Range(set)) {
		sum += a;
	}
	TEST( sum == 45 );

	cnt = 0;
	for (auto a : Range(set, 0, UMax))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(set) );

	cnt = 0;
	for (auto a : Range(set, UMax, 0))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == 0 );
}


static void TestRange5 ()
{
	HashMap<String, int>	map{ {"1",1}, {"2",2}, {"3",3}, {"4",4}, {"5",5}, {"6",6}, {"7",7}, {"8",8}, {"9",9} };

	usize	cnt = 0;
	for (auto& a : Range(map))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(map) );

	cnt = 0;
	int	sum = 0;
	for (auto& a : Range(map)) {
		sum += a.second;
	}
	TEST( sum == 45 );

	cnt = 0;
	for (auto& a : Range(map, 0, UMax))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(map) );

	cnt = 0;
	for (auto& a : Range(map, UMax, 0))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == 0 );
}


static void TestRange6 ()
{
	Set<int>	set{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	usize	cnt = 0;
	for (auto a : set)
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(set) );

	cnt = 0;
	int	sum = 0;
	for (auto a : Range(set)) {
		sum += a;
	}
	TEST( sum == 45 );

	cnt = 0;
	for (auto a : Range(set, 0, UMax))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(set) );

	cnt = 0;
	for (auto a : Range(set, UMax, 0))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == 0 );
}


static void TestRange7 ()
{
	const Map<String, int>	map{ {"1",1}, {"2",2}, {"3",3}, {"4",4}, {"5",5}, {"6",6}, {"7",7}, {"8",8}, {"9",9} };

	usize	cnt = 0;
	for (auto& a : Range(map))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(map) );

	cnt = 0;
	int	sum = 0;
	for (auto& a : Range(map)) {
		sum += a.second;
	}
	TEST( sum == 45 );

	cnt = 0;
	for (auto& a : Range(map, 0, UMax))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == CountOf(map) );

	cnt = 0;
	for (auto& a : Range(map, UMax, 0))
	{
		++cnt;
		GX_UNUSED( a );
	}
	TEST( cnt == 0 );
}


extern void Test_Algorithms_Range ()
{
	TestRange1();
	TestRange2();
	TestRange3();
	TestRange4();
	TestRange5();
	TestRange6();
	TestRange7();
}
