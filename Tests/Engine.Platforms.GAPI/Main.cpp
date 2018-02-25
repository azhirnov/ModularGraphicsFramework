// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

extern void Test_ComputeApi ();
extern void Test_GraphicsApi ();


int main ()
{
	Logger::GetInstance()->Open( "log", false );

	Test_ComputeApi();
	//Test_GraphicsApi();

	return 0;
}
