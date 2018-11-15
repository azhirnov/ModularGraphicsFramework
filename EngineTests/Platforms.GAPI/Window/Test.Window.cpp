// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Window1.h"
	
extern void Test_Window ()
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "EngineTests/Platforms.GAPI/Window" ) );

	Test_WindowApp1();
}
