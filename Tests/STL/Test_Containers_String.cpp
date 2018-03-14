// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


static void String_StartsWith_EndsWith ()
{
	const StringCRef	s0 = "123456789";
	const StringCRef	s1 = "QwErTyUiOp";

	TEST( s0.StartsWith( "123" ) );
	TEST( s0.EndsWith( "789" ) );

	TEST( s1.StartsWithIC( "qwer" ) );
	TEST( s1.EndsWithIC( "UIOP" ) );

	String	ss0( s0 );	ss0.EraseFromBack( s0.Length() );	TEST( ss0.Empty() );
}


static void String_LessThan_SortLikeInFileSystem ()
{
	struct SortComparator {
		bool operator () (const String &left, const String &right) const {
			return right.LessThan( left );
		}
	};

	Array< String >	arr;

	arr << "111/aaa" << "111/bbb" << "111/aaaa.x" << "22/aaa.x" << "22/aaa.y" << "22/aa";

	//Sort( arr );
	Sort( arr, SortComparator() );

	TEST( arr[0] == "111/aaa" );
	TEST( arr[1] == "111/aaaa.x" );
	TEST( arr[2] == "111/bbb" );
	TEST( arr[3] == "22/aa" );
	TEST( arr[4] == "22/aaa.x" );
	TEST( arr[5] == "22/aaa.y" );
}


static void String_Find1 ()
{
	String	s1 = "12345\n6789\n";
	String	s2 = "\n";
	usize	p = 0;

	TEST( s1.Find( s2, OUT p, 0 ) );
	TEST( s1.Find( s2, OUT p, p+1 ) );

	p = 0;
	TEST( s1.Find( s2[0], OUT p, 0 ) );
	TEST( s1.Find( s2[0], OUT p, p+1 ) );
}


static void String_Find2 ()
{
	String	s1 = "1234aaaa56789aaa";
	String	s2 = "aaaa";
	usize	p = 0;

	TEST( s1.Find( s2, OUT p, 0 ) );
	TEST( not s1.Find( s2, OUT p, p+1 ) );
}


static void String_Erase ()
{
	String	s0 = "01234aaaa56789";
	s0.Erase( 0, 5 );
	TEST( s0 == "aaaa56789" );

	String	s1 = "0123456789abcdefghijklmnop";
	s1.Erase( 0, 21 );
	TEST( s1 == "lmnop" );
}


static void String_FormatI ()
{
	String	a0 = String().FormatI( 1234567890, 10 );
	TEST( a0 == "1234567890" );

	String	a1 = String().FormatI( 0x12345678, 16 );
	TEST( a1 == "12345678" );

	String	a2 = String().FormatI( 0x20408010, 2 );
	TEST( a2 == "100000010000001000000000010000" );
}


static void String_FormatF ()
{
	float	f0 = 12345678.0f;
	String	a0 = String().FormatF( f0, StringFormatF().Fmt(0,6).CutZeros() );
	TEST( a0 == "12345678.0" );

	float	f1 = 1234.5678f;
	String	a1 = String().FormatF( f1, StringFormatF().Fmt(0,5).CutZeros() );
	TEST( a1 == "1234.56775" );

	float	f2 = 1.234e+12f;
	String	a2 = String().FormatF( f2, StringFormatF().Fmt(0,4).Exp().CutZeros() );
	TEST( (a2 == "1.2340e+12") or (a2 == "1.2340e+012") );


	double	d0 = 123456789.123456;
	String	b0 = String().FormatF( d0, StringFormatF().Fmt(0,6).CutZeros() );
	TEST( b0 == "123456789.123456" );

	double	d1 = 0.12345678912345678;
	String	b1 = String().FormatF( d1, StringFormatF().Fmt(0,17).CutZeros() );
	TEST( b1 == "0.12345678912345678" );

	double	d2 = 1.234567e+27;
	String	b2 = String().FormatF( d2, StringFormatF().Fmt(0,9).Exp().CutZeros() );
	TEST( (b2 == "1.234567000e+27") or (b2 == "1.234567000e+027") );
	
	double	d3 = 2.74234e-134;
	String	b3 = String().FormatF( d3, StringFormatF().Fmt(0,10) );
	TEST( b3 == "2.7423400000e-134" );
}


extern void Test_Containers_String ()
{
	String_StartsWith_EndsWith();
	String_LessThan_SortLikeInFileSystem();
	String_Find1();
	String_Find2();
	String_Erase();
	String_FormatI();
	String_FormatF();
}
