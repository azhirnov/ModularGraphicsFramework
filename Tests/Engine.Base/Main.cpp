// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

extern void Test_Window ();
extern void Test_GLWindow ();
extern void Test_VkWindow ();


void main ()
{
	Logger::GetInstance()->Open( "log", false );

	//Test_Window();
	//Test_GLWindow();
	Test_VkWindow();
}
