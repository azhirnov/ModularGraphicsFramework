// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"
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

	for (usize i = 1; i < s.Count(); ++i) {
		ASSERT( s[i-1] < s[i] );
	}
}


static void Set_Test2 ()
{
	Set<int>	s;

	s << 0 << 0;
	s << 1 << 1;

	ASSERT( s.Count() == 2 );
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

	ASSERT( Elem1_t::CheckStatistic() );
	ASSERT( Elem2_t::CheckStatistic() );
	ASSERT( Elem3_t::CheckStatistic() );
}


static void Set_Test4 ()
{
	Set< String >	s;
	usize			idx;

	s << "111" << "222" << "333";

	ASSERT( s.FindIndex( StringCRef("222"), idx ) );

	Set< String >::const_iterator	iter;

	ASSERT( s.Find( "333", OUT iter ) );

	ASSERT( idx == 1 );
}


static void MultiSet_Test1 ()
{
	MultiSet<int>	s;

	for (usize i = 0; i < 10; ++i) {
		s << Random::Int<int>();
	}

	for (usize i = 1; i < s.Count(); ++i) {
		ASSERT( s[i-1] < s[i] );
	}
}


static void MultiSet_Test2 ()
{
	MultiSet<int>	s;
	
	s << 0 << 0;
	s << 1 << 1;

	ASSERT( s.Count() == 4 );
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
