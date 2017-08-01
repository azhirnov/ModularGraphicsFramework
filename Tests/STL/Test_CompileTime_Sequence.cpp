// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::CompileTime;


static void Sequence_RangeForTest ()
{
	using IdxSeq = IndexSequenceSize<10>;
	STATIC_ASSERT( IdxSeq::Count == 10 );

	usize idx = 0;

	for (auto i : IdxSeq()) {
		ASSERT( idx == i );
		++idx;
	}
	ASSERT( idx == IdxSeq::Count-1 );
}


extern void Test_CompileTime_Sequence ()
{
	Sequence_RangeForTest();

	using Typelist = TypeListFrom< int, float, uint, double >;

	const uint	count1 = CompileTime::IndexSequenceSize< Typelist::Count >::Count;
	const uint	count2 = Typelist::Count;
	STATIC_ASSERT( count1 == count2 );
}
