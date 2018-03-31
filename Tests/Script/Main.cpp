// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/Script/Common.h"

//extern void Test_SimpleScript ();

extern void Test_ScriptClass ();
extern void Test_ScriptScalarMath ();
extern void Test_ScriptVectorMath ();
extern void Test_ScriptString ();
extern void Test_Eval ();


int main ()
{
	InitializeSTLMath();

	Logger::GetInstance()->Open( "log", false );


	Test_ScriptClass();
	Test_ScriptScalarMath();
	Test_ScriptVectorMath();
	Test_ScriptString();
	Test_Eval();


	TEST( !"Tests Finished!" );

	Logger::GetInstance()->Close();
	
	DEBUG_ONLY( DbgRefCountedObject::s_ChenckNotReleasedObjects() );
	DEBUG_ONLY( RefCounter2::s_CheckAllocations() );

	return 0;
}
