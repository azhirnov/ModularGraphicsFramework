// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/Script/Common.h"


static void TestScalar_Sin (ScriptEngine *se)
{
	const char	script[] = R"#(
		float main () {
			float f = Sin( 3.14f );
			return f;
		}
	)#";

	float	res = 0;
	se->Run( script, "main", OUT res );

	const float	ref = Sin( 3.14_rad);

	TEST( res == ref );
}


extern void Test_ScriptScalarMath ()
{
	ScriptEngine	se;

	DefaultBindings::BindScalarMath( &se );

	TestScalar_Sin( &se );
}
