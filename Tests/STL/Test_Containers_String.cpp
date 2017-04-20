// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


static void String_StartsWith_EndsWith ()
{
	const StringCRef	s0 = "123456789";
	const StringCRef	s1 = "QwErTyUiOp";

	ASSERT( s0.StartsWith( "123" ) );
	ASSERT( s0.EndsWith( "789" ) );

	ASSERT( s1.StartsWithIC( "qwer" ) );
	ASSERT( s1.EndsWithIC( "UIOP" ) );

	String	ss0( s0 );	ss0.EraseFromBack( s0.Length() );	ASSERT( ss0.Empty() );
}


static void String_LessThan_SortLikeInFileSystem ()
{
	struct SortComparator {
		bool operator () (const String &left, const String &right) const {
			return right.LessThan( left );
		}
	};

	Array< String >	arr;

	arr << "aaa" << "bbb" << "aaaa";

	Sort( arr );
	Sort( arr, SortComparator() );

	ASSERT( arr[0] == "aaa" );
	ASSERT( arr[1] == "aaaa" );
	ASSERT( arr[2] == "bbb" );
}


extern void Test_Containers_String ()
{
	String_StartsWith_EndsWith();
	String_LessThan_SortLikeInFileSystem();
}
