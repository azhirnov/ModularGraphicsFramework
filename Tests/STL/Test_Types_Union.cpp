// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


class RefClass : public RefCountedObject<>
{
	int i;
};
SHARED_POINTER( RefClass );


typedef Union< byte, short, int, ilong, float, double, RefClassPtr, String >	Union_t;


static void Union_Test1 ()
{
	float		f1 = 1.74323f;
	Union_t		u;	u.Create( f1 );
	float		f2 = u.Get<float>();

	TEST( f1 == f2 );
}


static void Union_Test2 ()
{
	STATIC_ASSERT( CompileTime::IsCtorAvailable< Union_t > );
	STATIC_ASSERT( CompileTime::IsDtorAvailable< Union_t > );

	RefClassPtr	p = new RefClass();
	
	Array<Union_t>	arr;

	arr.Resize( 10 );

	arr[0].Create( p );
	arr[1].Create( 0.0f );
	arr[2].Create( 777 );
}


static void Union_Test3 ()
{
	Union_t un2;
	{
		Union_t		un1;
		un1.Create( String("1234567890") );

		un2 = un1;
	}

	String s = un2.Get<String>();
	TEST( s == "1234567890" );
}


static void Union_Test4 ()
{
	StaticArray<Union_t, 4>	au2;
	{
		StaticArray<Union_t, 4> au1;

		au1[0].Create( "000000000000000000000"_str );
		au1[1].Create( "111111111111111111111"_str );
		au1[2].Create( "222222222222222222222"_str );
		au1[3].Create( "333333333333333333333"_str );

		au2 = au1;
	}

	String	s0 = au2[0].Get< String >();	TEST( s0 == "000000000000000000000" );
	String	s1 = au2[1].Get< String >();	TEST( s1 == "111111111111111111111" );
	String	s2 = au2[2].Get< String >();	TEST( s2 == "222222222222222222222" );
	String	s3 = au2[3].Get< String >();	TEST( s3 == "333333333333333333333" );
}


extern void Test_Types_Union ()
{
	Union_Test1();
	Union_Test2();
	Union_Test3();
	Union_Test4();
}
