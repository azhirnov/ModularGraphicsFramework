// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"
#include "Debug.h"
#include <unordered_set>
#include <list>

using namespace GX_STL;
using namespace GX_STL::GXTypes;

typedef TDebugInstCounter<8>	Elem_t;

static void Test_Func1 (OUT ContainerAdaptors::Appendable<int> arr)
{
	arr.PushBack( 1 );
	arr.PushBack( 2 );
}

static void Test_Adaptor1 ()
{
	Array<int>				arr;
	FixedSizeArray<int, 8>	farr;
	Set<int>				set;
	std::vector<int>		vec;
	std::unordered_set<int>	uset;
	std::list<int>			list;

	Test_Func1( arr );
	TEST( arr[0] == 1 );
	TEST( arr[1] == 2 );
	
	Test_Func1( farr );
	TEST( farr[0] == 1 );
	TEST( farr[1] == 2 );

	Test_Func1( set );
	TEST( set.IsExist( 1 ) );
	TEST( set.IsExist( 2 ) );

	Test_Func1( vec );
	TEST( vec[0] == 1 );
	TEST( vec[1] == 2 );

	Test_Func1( uset );
	TEST( uset.find( 1 ) != uset.end() );
	TEST( uset.find( 2 ) != uset.end() );

	Test_Func1( list );
	TEST( *(list.begin()) == 1 );
	TEST( *(++list.begin()) == 2 );
}


static void Test_Func2 (OUT ContainerAdaptors::Appendable<Elem_t> arr)
{
	Array<Elem_t>	movable;

	arr.PushBack( Elem_t() );
	arr.PushBack( Elem_t() );
	arr.AppendMovable( movable );
	arr.PushBack( Elem_t() );
}

static void Test_Adaptor2 ()
{
	Array<Elem_t>				arr;
	FixedSizeArray<Elem_t, 16>	farr;
	Set<Elem_t>					set;
	std::vector<Elem_t>			vec;
	std::unordered_set<Elem_t>	uset;
	std::list<Elem_t>			list;

	Test_Func2( arr );
	Test_Func2( farr );
	Test_Func2( set );
	Test_Func2( vec );
	Test_Func2( uset );
	Test_Func2( list );
}


static void Test_Func3 (OUT ContainerAdaptors::Erasable<const int> arr)
{
	FOR( i, arr )
	{
		if ( arr[i] == 2 )
		{
			arr.Erase( i );
			--i;
		}
	}
}

static void Test_Adaptor3 ()
{
	Array<int>			arr = { 0, 1, 2, 3, 4, 5 };
	std::vector<int>	vec = { 0, 1, 2, 3, 4, 5 };
	Set<int>			set = { 0, 1, 2, 3, 4, 5 };
	//HashSet<int>		hset = { 7, 3, 5, 2, 8, 9 };
	
	TEST( arr.IsExist( 2 ) );
	Test_Func3( arr );
	TEST( not arr.IsExist( 2 ) );

	TEST( std::find( vec.begin(), vec.end(), 2 ) != vec.end() );
	Test_Func3( vec );
	TEST( std::find( vec.begin(), vec.end(), 2 ) == vec.end() );

	TEST( set.IsExist( 2 ) );
	Test_Func3( set );
	TEST( not set.IsExist( 2 ) );

	//TEST( hset.IsExist( 2 ) );
	//Test_Func3( hset );
	//TEST( not hset.IsExist( 2 ) );
}


extern void Test_Containers_Adaptors ()
{
	Test_Adaptor1();

	Elem_t::ClearStatistic();
	Test_Adaptor2();
	TEST( Elem_t::CheckStatistic() );

	Test_Adaptor3();
}
