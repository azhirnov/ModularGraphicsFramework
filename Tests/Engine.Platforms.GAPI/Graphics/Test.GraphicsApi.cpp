// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
	
extern void Test_GraphicsApi ()
{
	#ifdef GRAPHICS_API_OPENGL
	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Graphics" ) );

		app.Initialize( "GL 4.4"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	#ifdef GRAPHICS_API_VULKAN
	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Graphics" ) );

		app.Initialize( "VK 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	#if 0 //def GRAPHICS_API_SOFT
	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Graphics" ) );

		app.Initialize( "Soft 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
}
