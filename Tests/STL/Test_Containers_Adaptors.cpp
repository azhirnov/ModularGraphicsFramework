// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"
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
	ASSERT( arr[0] == 1 );
	ASSERT( arr[1] == 2 );
	
	Test_Func1( farr );
	ASSERT( farr[0] == 1 );
	ASSERT( farr[1] == 2 );

	Test_Func1( set );
	ASSERT( set.IsExist( 1 ) );
	ASSERT( set.IsExist( 2 ) );

	Test_Func1( vec );
	ASSERT( vec[0] == 1 );
	ASSERT( vec[1] == 2 );

	Test_Func1( uset );
	ASSERT( uset.find( 1 ) != uset.end() );
	ASSERT( uset.find( 2 ) != uset.end() );

	Test_Func1( list );
	ASSERT( *(list.begin()) == 1 );
	ASSERT( *(++list.begin()) == 2 );
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
	
	ASSERT( arr.IsExist( 2 ) );
	Test_Func3( arr );
	ASSERT( not arr.IsExist( 2 ) );

	ASSERT( std::find( vec.begin(), vec.end(), 2 ) != vec.end() );
	Test_Func3( vec );
	ASSERT( std::find( vec.begin(), vec.end(), 2 ) == vec.end() );

	ASSERT( set.IsExist( 2 ) );
	Test_Func3( set );
	ASSERT( not set.IsExist( 2 ) );

	//ASSERT( hset.IsExist( 2 ) );
	//Test_Func3( hset );
	//ASSERT( not hset.IsExist( 2 ) );
}


extern void Test_Containers_Adaptors ()
{
	Test_Adaptor1();

	Elem_t::ClearStatistic();
	Test_Adaptor2();
	ASSERT( Elem_t::CheckStatistic() );

	Test_Adaptor3();
}
