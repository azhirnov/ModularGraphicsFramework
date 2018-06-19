// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


static void TupleTest1 ()
{
	using T0 = Tuple< int >;

	T0	t { 1 };

	TEST( t.Get<0>() == 1 );
	TEST( t.Count() == 1 );
}

static void TupleTest2 ()
{
	using T0 = Tuple< int, float >;

	T0	t { 1, 2.0f };

	TEST( t.Get<0>() == 1 );
	TEST( t.Get<1>() == 2.0f );
	TEST( t.Count() == 2 );
}

static void TupleTest3 ()
{
	using T0 = Tuple< CompileTime::TypeListFrom< int, float, double > >;

	T0	t { 1, 2.0f, 3.0 };

	TEST( t.Get<0>() == 1 );
	TEST( t.Get<1>() == 2.0f );
	TEST( t.Get<2>() == 3.0 );
	TEST( t.Count() == 3 );
}

static void TupleTest4 ()
{
	using T0 = Tuple< int, float >;

	T0	t {};

	TEST( t.Get<0>() == 0 );
	TEST( t.Get<1>() == 0.0f );
	TEST( t.Count() == 2 );

	T0	t2 { 1, 2.0f };
	t = t2;

	TEST( t.Get<0>() == 1 );
	TEST( t.Get<1>() == 2.0f );
}


extern void Test_Containers_Tuple ()
{
	TupleTest1();
	TupleTest2();
	TupleTest3();
	TupleTest4();
}
