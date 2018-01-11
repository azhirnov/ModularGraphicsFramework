// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


static void IndexedArray_Test1 ()
{
	IndexedArray<int>	arr;

	arr << 0 << 5 << 8 << 3 << 11 << 1 << 7 << 3 << 5 << 99 << 4 << 66 << 8;

	arr.Sort();

	FORv( i, 1, arr ) {
		TEST( arr[i-1] <= arr[i] );
	}
}


static void IndexedArray_Test2 ()
{
	using IMap	= _types_hidden_::BaseMap< IndexedArray, int, float, true, AutoDetectCopyStrategy<Pair<int, float>>::type, MemoryContainer<Pair<int, float>> >;

	IMap	m;

	m.Add( 0, 8.0f );
	m.Add( 4, 5.0f );
	m.Add( 2, 6.3f );
	m.Add( 3, 7.0f );

	TEST( m( 0 ) == 8.0f );
	TEST( m( 4 ) == 5.0f );
	TEST( m( 2 ) == 6.3f );
	TEST( m( 3 ) == 7.0f );
}


extern void Test_Containers_IndexedArray ()
{
	IndexedArray_Test1();
	IndexedArray_Test2();
}
