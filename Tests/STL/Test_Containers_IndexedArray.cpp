// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


static void IndexedArray_Test1 ()
{
	IndexedArray<int>	arr;

	arr << 0 << 5 << 8 << 3 << 11 << 1 << 7 << 3 << 5 << 99 << 4 << 66 << 8;

	arr.Sort();

	FORv( i, 1, arr ) {
		ASSERT( arr[i-1] <= arr[i] );
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

	ASSERT( m( 0 ) == 8.0f );
	ASSERT( m( 4 ) == 5.0f );
	ASSERT( m( 2 ) == 6.3f );
	ASSERT( m( 3 ) == 7.0f );
}


extern void Test_Containers_IndexedArray ()
{
	IndexedArray_Test1();
	IndexedArray_Test2();
}
