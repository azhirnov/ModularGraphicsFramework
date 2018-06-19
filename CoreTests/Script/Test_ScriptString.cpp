// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/Script/Common.h"


static void TestString_Ctor (ScriptEngine *se)
{
	const char	script[] = R"#(
		int main () {
			String	str = "12345678";
			LogDbg( str );
			return str.Length();
		}
	)#";

	int	res = 0;
	se->Run( script, "main", OUT res );

	TEST( res == 8 );
}


static void TestString_AppendString (ScriptEngine *se)
{
	const char	script[] = R"#(
		int main () {
			String	str = "12345678";
			str += "abcd";
			LogDbg( str );
			return str.Length();
		}
	)#";

	int	res = 0;
	se->Run( script, "main", OUT res );

	TEST( res == 12 );
}


static void TestString_AppendInt (ScriptEngine *se)
{
	const char	script[] = R"#(
		int main () {
			String	str = "abcd";
			str += 1234;
			LogDbg( str );
			return str.Length();
		}
	)#";

	int	res = 0;
	se->Run( script, "main", OUT res );

	TEST( res == 8 );
}


static void TestString_AppendString2 (ScriptEngine *se)
{
	const char	script[] = R"#(
		int main () {
			String	str = "a";
			str = str + 0 + "b" + 1 + "c" + 2;
			LogDbg( str );
			return (str == "a0b1c2" ? 1 : 0);
		}
	)#";

	int	res = 0;
	se->Run( script, "main", OUT res );

	TEST( res == 1 );
}


extern void Test_ScriptString ()
{
	ScriptEngine	se;

	DefaultBindings::BindString( &se );
	DefaultBindings::BindLog( &se );

	TestString_Ctor( &se );
	TestString_AppendString( &se );
	TestString_AppendInt( &se );
	TestString_AppendString2( &se );
}
