// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

extern void Test_Window ();
extern void Test_GWindow ();
extern void Test_CWindow ();


int main ()
{
	Logger::GetInstance()->Open( "log", false );

	//Test_Window();
	Test_GWindow();
	//Test_CWindow();

	return 0;
}
