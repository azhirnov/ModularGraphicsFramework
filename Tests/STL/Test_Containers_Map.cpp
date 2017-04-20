// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"
#include "Debug.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


static void Map_Test1 ()
{
	Map<int, String>	m;

	for (usize i = 0; i < 10; ++i) {
		m.Add( Random::Int<int>(), String() << i );
	}

	for (usize i = 1; i < m.Count(); ++i) {
		ASSERT( m[i-1].first < m[i].first );
	}
}


static void Map_Test2 ()
{
	typedef TDebugInstCounter<5>	Elem1_t;
	typedef TDebugInstCounter<6>	Elem2_t;
	typedef TDebugInstCounter<7>	Elem3_t;

	Elem1_t::ClearStatistic();
	Elem2_t::ClearStatistic();
	Elem3_t::ClearStatistic();

	{
		Map<Elem1_t, String>	m;
		Array<Elem2_t>			a;
		std::vector<Elem3_t>	v;
	
		for (usize i = 0; i < 10; ++i)
		{
			m << Pair<Elem1_t, String>( Elem1_t(Random::Int<int>()), "" );
			a << Elem2_t(Random::Int<int>());
			v.push_back( Elem3_t(Random::Int<int>()) );
		}
	}

	ASSERT( Elem1_t::CheckStatistic() );
	ASSERT( Elem2_t::CheckStatistic() );
	ASSERT( Elem3_t::CheckStatistic() );
}


static void Map_Test3 ()
{
	struct ValueWithUnion
	{
		String						s;
		Union<int, float, double>	u;
		uint						idx;

		ValueWithUnion (int i, String s) : s(s), idx(0) { u.Create( i ); }
		ValueWithUnion (float f, String s) : s(s), idx(1) { u.Create( f ); }
		ValueWithUnion (double d, String s) : s(s), idx(2) { u.Create( d ); }
	};
	typedef StaticString< 64 >					StString_t;
	typedef Map< StString_t, ValueWithUnion >	Map_t;

	Map_t	m;

	m.Add( "111", ValueWithUnion( 1, "aaa" ) );
	m.Add( "222", ValueWithUnion( 2.0f, "bbb" ) );
	m.Add( "333", ValueWithUnion( 4.0, "ccc" ) );

	Map_t::iterator	iter;
	ASSERT( m.Find( "222", iter ) );
	ASSERT( iter->second.u.Is<float>() );
	ASSERT( iter->second.s == "bbb" );
}


extern void Test_Containers_Map ()
{
	Map_Test1();
	Map_Test2();
	Map_Test3();
}
