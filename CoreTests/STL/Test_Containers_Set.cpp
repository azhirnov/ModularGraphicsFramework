// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"
#include "Debug.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


static void Set_Test1 ()
{
	Set<int>	s;

	for (usize i = 0; i < 10; ++i) {
		s << Random::Int<int>();
	}

	FORv( i, 1, s ) {
		TEST( s[i-1] < s[i] );
	}
}


static void Set_Test2 ()
{
	Set<int>	s;

	s << 0 << 0;
	s << 1 << 1;

	TEST( s.Count() == 2 );
}


static void Set_Test3 ()
{
	typedef TDebugInstCounter<5>	Elem1_t;
	typedef TDebugInstCounter<6>	Elem2_t;
	typedef TDebugInstCounter<7>	Elem3_t;

	Elem1_t::ClearStatistic();
	Elem2_t::ClearStatistic();
	Elem3_t::ClearStatistic();

	{
		Set<Elem1_t>			s;
		Array<Elem2_t>			a;
		std::vector<Elem3_t>	v;
	
		for (usize i = 0; i < 10; ++i)
		{
			s << Elem1_t(Random::Int<int>());
			a << Elem2_t(Random::Int<int>());
			v.push_back( Elem3_t(Random::Int<int>()) );
		}
	}

	TEST( Elem1_t::CheckStatistic() );
	TEST( Elem2_t::CheckStatistic() );
	TEST( Elem3_t::CheckStatistic() );
}


static void Set_Test4 ()
{
	Set< String >	s;
	usize			idx;

	s << "111" << "222" << "333";

	TEST( s.FindIndex( StringCRef("222"), idx ) );

	Set< String >::const_iterator	iter;

	TEST( s.Find( "333", OUT iter ) );

	TEST( idx == 1 );
}


static void MultiSet_Test1 ()
{
	MultiSet<int>	s;

	for (usize i = 0; i < 10; ++i) {
		s << Random::Int<int>();
	}

	FORv( i, 1, s ) {
		TEST( s[i-1] < s[i] );
	}
}


static void MultiSet_Test2 ()
{
	MultiSet<int>	s;
	
	s << 0 << 0;
	s << 1 << 1;

	TEST( s.Count() == 4 );
}


extern void Test_Containers_Set ()
{
	Set_Test1();
	Set_Test2();
	Set_Test3();
	Set_Test4();

	MultiSet_Test1();
	MultiSet_Test2();
}
