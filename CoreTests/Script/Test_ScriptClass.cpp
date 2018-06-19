// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/Script/Common.h"
#include "Core/STL/Types/Bitfield.h"


// class
static void TestScript_ClassRef (ScriptEngine &se)
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

		int F () {
			return i;
		}
	};

	ClassBinder<CL> binder( &se, "CL" );

	binder.CreateRef();
	binder.AddMethod( &CL::F, "F" );

	const char	script[] = R"#(
		int main (int i) {
			CL@ c = CL();
			return c.F() + i;
		}
	)#";

	int	res = 0;
	se.Run( script, "main", OUT res, 1 );

	TEST( res == 3+1 );
}


// value class
struct Value
{
	int i;

	Value () : i(4) {}
	Value (int i) : i(i) {}

	void operator = (const Value &) {}

	int F () const {
		return i;
	}

	static int Append (Value &self, int x) {
		self.i += x;
		return self.i;
	}
};

namespace GXScript {
	GX_DECL_SCRIPT_TYPE( Value,	"Value" );
}

static void TestScript_ValueClass (ScriptEngine &se)
{
	ClassBinder<Value>	binder{ &se };

	binder.CreateClassValue();
	binder.AddMethod( &Value::F, "F" );
	binder.AddMethodFromGlobal( &Value::Append, "Append" );

	const char	script[] = R"#(
		int main (int i) {
			Value p = Value();
			p.Append( i );
			return p.F();
		}
	)#";

	int	res = 0;
	se.Run( script, "main", OUT res, 2 );

	TEST( res == 4+2 );
}


// enum
struct EEnum
{
	enum type : uint
	{
		Value1 = 1,
		Value2,
		Value3,
		Value4,

		_Count
	};

	GX_ENUM_BITFIELD( EEnum );
};

namespace GXScript {
	GX_DECL_SCRIPT_TYPE( EEnum::type,	"EEnum" );
}

static void TestScript_Enum (ScriptEngine &se)
{
	EnumBinder<EEnum::type>		binder{ &se };

	binder.Create();
	binder.AddValue( "Value1",	EEnum::Value1 );
	binder.AddValue( "Value2",	EEnum::Value2 );
	binder.AddValue( "Value3",	EEnum::Value3 );
	binder.AddValue( "Value4",	EEnum::Value4 );

	static const char script[] = R"#(
		int main () {
			EEnum e = EEnum_Value1;
			return e + EEnum_Value2;
		}
	)#";
	
	int	res = 0;
	se.Run( script, "main", OUT res );

	TEST( res == 3 );
}


// enum bitfield
/*static void TestScript_EnumBitfield (ScriptEngine &se)
{
	EnumBitfieldBinder<EEnum::bits>		binder{ se };

	binder.BindDefaults();

	static const char script[] = R"#(
		int main () {
		}
	)#";

	int res = 0;
	se.Run();
}*/


extern void Test_ScriptClass ()
{
	ScriptEngine	se;

	TestScript_ClassRef( se );
	TestScript_ValueClass( se );
	TestScript_Enum( se );

}
