// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


class RefClass : public RefCountedObject
{
	int i;
};
SHARED_POINTER( RefClass );


typedef Union< byte, short, int, ilong, float, double, RefClassPtr >	Union_t;


static void Union_Test1 ()
{
	float		f1 = 1.74323f;
	Union_t		u;	u.Create( f1 );
	float		f2 = u.Get<float>();

	ASSERT( f1 == f2 );
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


extern void Test_Types_Union ()
{
	Union_Test1();
	Union_Test2();
}
