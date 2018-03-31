// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/Script/Common.h"


class Script
{
public:
	static ScriptEngine *	engine;

	int Run (int value)
	{
		const char	script[] = R"#(
			int main (int x) {
				return 10 + x;
			}
		)#";

		ScriptModulePtr	mod = New<ScriptModule>( engine );
		mod->Create( "def1" );

		int	res = 0;
		mod->Run( script, "main", OUT res, value );

		return res;
	}
};

ScriptEngine *	Script::engine = null;


static void Test_ScriptInScript (ScriptEngine &se)
{
	const char	script[] = R"#(
		int main ()
		{
			Script sc;
			return sc.Run( 1 );
		}
	)#";

	Script::engine = &se;

	ClassBinder<Script>		binder{ &se, "Script" };

	binder.CreateClassValue();
	binder.AddMethod( &Script::Run, "Run" );


	int	res = 0;
	se.Run( script, "main", OUT res );

	TEST( res == 11 );
}


extern void Test_Eval ()
{
	ScriptEngine	se;

	Test_ScriptInScript( se );
}
