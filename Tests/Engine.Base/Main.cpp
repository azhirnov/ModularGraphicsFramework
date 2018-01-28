// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

extern void Test_Window ();
extern void Test_CLCompute ();
extern void Test_GWindow ();


int main ()
{
	Logger::GetInstance()->Open( "log", false );

	//Test_Window();
	//Test_CLCompute();
	Test_GWindow();

	return 0;
}
