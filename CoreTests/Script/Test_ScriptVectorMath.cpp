// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/Script/Common.h"


static void TestVec_Float2Ctor (ScriptEngine *se)
{
	const char	script[] = R"#(
		float main () {
			float2 f = float2(1.0f, 2.0f);
			return f.x + f.y;
		}
	)#";

	float	res = 0;
	se->Run( script, "main", OUT res );

	TEST( res == 3.0f );
}

static void TestVec_Float2Dot (ScriptEngine *se)
{
	const char	script[] = R"#(
		float main () {
			float2 f1 = float2(1.0f, 2.0f);
			float2 f2 = float2(3.0f, 4.0f);
			float f = Dot( f1, f2 );
			return f;
		}
	)#";

	float	res = 0;
	se->Run( script, "main", OUT res );

	TEST( res == 11.0f );
}

static void TestVec_Float3Cmp (ScriptEngine *se)
{
	const char	script[] = R"#(
		int main () {
			float3 f1 = float3(1.0f, 2.0f, 3.0f);
			float3 f2 = float3(1.0f, 2.0f, 3.0f);
			bool3 bb = (f1 == f2);
			return All( bb ) ? 1 : 0;
		}
	)#";

	int		res = 0;
	se->Run( script, "main", OUT res );

	TEST( res == 1 );
}


extern void Test_ScriptVectorMath ()
{
	ScriptEngine	se;

	DefaultBindings::BindVectorMath( &se );

	TestVec_Float2Ctor( &se );
	TestVec_Float2Dot( &se );
	TestVec_Float3Cmp( &se );
}
