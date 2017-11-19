// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
	
extern void Test_GWindow ()
{
	#ifdef GRAPHICS_API_OPENGL
	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );

		app.Initialize( "GL 4.4"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstace()->Send< ModuleMsg::Delete >({});
	#endif
	
	#ifdef GRAPHICS_API_VULKAN
	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );

		app.Initialize( "VK 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstace()->Send< ModuleMsg::Delete >({});
	#endif
}
