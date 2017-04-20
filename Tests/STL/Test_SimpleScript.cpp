// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

#ifdef GX_ANGELSCRIPT_ENABLED

using namespace GX_STL;
using namespace GXScript;
using namespace GXTypes;
using namespace GXMath;


static void SimpleScript_ScriptFunction (ScriptEngine &se)
{
	const char	script[] = TOSTRING(
		int main (int i) {
			return i + 1;
		}
	);

	int	res = 0;

	se.Run<int, int>( script, "main", 0, res );

	ASSERT( res == 1 );
}


// Global Function
static int GlobalFuncForTesting (int input)
{
	return input * 10;
}

static void SimpleScript_GlobalFunction (ScriptEngine &se)
{
	const char	script[] = TOSTRING(
		int main (int i) {
			return gFunc( i + 1 );
		}
	);

	se.AddFunction( &GlobalFuncForTesting, "gFunc" );
	
	int	res = 0;

	se.Run<int, int>( script, "main", 1, res );

	ASSERT( res == 20 );
}


// Global Variable
static void SimpleScript_GlobalVariable (ScriptEngine &se)
{
	int	gVar = 11;
	int gConst = 10;

	const char	script[] = TOSTRING(
		int main (int i) {
			gVar = 9;
			return ( gVar + i ) * gConst;
		}
	);

	se.AddProperty( gVar, "gVar" );
	se.AddConstProperty( gConst, "gConst" );
	
	int	res = 0;

	gVar = 3;
	gConst = 5;

	se.Run<int, int>( script, "main", 5, res );

	ASSERT( res == (gVar + 5)*gConst );
}


// Class Binding
static void SimpleScript_ClassBinding (ScriptEngine &se)
{
	DefaultBindings::BindVector( se.Get() );

	const char	script[] = TOSTRING(
		float main (const float2 &in v) {
			float2	w = float2( 3.0f, 5.0f ) * float2(3.0f);
			return v.x + v.y + w.x + w.y;
		}
	);
	
	float	res = 0;
	float2	v( 5, 11 );

	se.Run< const float2 *, float >( script, "main", &v, res );

	ASSERT( res == v.x + v.y + (3+5)*3 );
}


// std::string
static void SimpleScript_String (ScriptEngine &se)
{
	const char	script[] = TOSTRING(
		int main (const string &in s) {
			return s.length();
		}
	);

	AngelScript::RegisterStdString( se.Get() );

	std::string	s("test");
	int			res = 0;

	se.Run< const std::string *, int >( script, "main", &s, res );

	ASSERT( res == s.length() );
}


// class
static void SimpleScript_ClassRef (ScriptEngine &se)
{
	struct CL : AngelScriptHelper::SimpleRefCounter
	{
		int i;

		CL() : i(3)
		{}

		CL(int i): i(i)
		{}

		void operator = (const CL &)
		{}

		int F() {
			return i;
		}
	};

	ClassBinder<CL> binder( se.Get(), "CL" );

	binder.CreateRef();
	binder.AddMethod( &CL::F, "F" );

	const char	script[] = TOSTRING(
		int main (int i) {\n
			CL@ c = CL();\n
			return c.F() + i;\n
		}\n
	);

	int	res = 0;

	se.Run< int, int >( script, "main", 1, res );

	ASSERT( res == 3+1 );
}


// enum
/*#define ENUM_LIST( build ) \
	build( ONE, ) \
	build( TWO, ) \
	build( THREE, =3 )

GX_SCRIPT_ENUM( TestEnum, ENUM_LIST, );
#undef ENUM_LIST
*/

static void SimpleScript_Enum (ScriptEngine &se)
{
	//TODO( "" );
}


extern void Test_SimpleScript ()
{
	ScriptEngine	se;
	
	SimpleScript_Enum( se );
	SimpleScript_ScriptFunction( se );
	SimpleScript_GlobalFunction( se );
	SimpleScript_GlobalVariable( se );
	SimpleScript_ClassBinding( se );
	SimpleScript_String( se );
	SimpleScript_ClassRef( se );
}

#else

extern void Test_SimpleScript ()
{
}

#endif	// GX_ANGELSCRIPT_ENABLED
