// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
	
extern void Test_ComputeApi ()
{
	#ifdef GRAPHICS_API_VULKAN
	{
		CApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Compute" ) );

		app.Initialize( "VK 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif

	#ifdef GRAPHICS_API_OPENGL
	{
		CApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Compute" ) );

		app.Initialize( "GL 4.5"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	#ifdef COMPUTE_API_OPENCL
	{
		CApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Compute" ) );

		app.Initialize( "CL 1.2"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	#ifdef GRAPHICS_API_SOFT
	{
		CApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Compute" ) );

		app.Initialize( "SW 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
}
