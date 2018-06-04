// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"


int main ()
{
	Logger::GetInstance()->Open( "log", false );
	
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Tests/Engine.Graphics" ) );

	#ifdef GRAPHICS_API_VULKAN
	{
		GApp	app;
		app.Initialize( "VK 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	#ifdef GRAPHICS_API_OPENGL
	{
		GApp	app;
		app.Initialize( "GL 4.5"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif

	return 0;
}
