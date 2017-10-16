// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::CompileTime;


static void Sequence_RangeForTest ()
{
	using IdxSeq = IndexSequenceFromSize<10>;
	STATIC_ASSERT( IdxSeq::Count == 10 );

	usize idx = 0;

	for (auto i : IdxSeq()) {
		ASSERT( idx == i );
		++idx;
	}
	ASSERT( idx == IdxSeq::Count );

	
	using IdxSeq2 = IndexSequenceFromRange<2, 8>;
	STATIC_ASSERT( IdxSeq2::Count == 7 );

	idx = 0;

	for (auto i : IdxSeq2()) {
		ASSERT( idx + 2 == i );
		++idx;
	}
	ASSERT( idx == IdxSeq2::Count );
}


static void Sequence_SizeTest ()
{
	using Typelist = TypeListFrom< int, float, uint, double >;

	const uint	count1 = CompileTime::IndexSequenceFromSize< Typelist::Count >::Count;
	const uint	count2 = Typelist::Count;
	STATIC_ASSERT( count1 == count2 );
}


static void Sequence_BoolTest ()
{
	int idx = 0;

	for (bool b : CompileTime::Sequence<bool, true, false>())
	{
		ASSERT( b == (idx ? false : true) );
		++idx;
	}
	ASSERT( idx == 2 );
}


extern void Test_CompileTime_Sequence ()
{
	Sequence_RangeForTest();
	Sequence_SizeTest();
	Sequence_BoolTest();
}
