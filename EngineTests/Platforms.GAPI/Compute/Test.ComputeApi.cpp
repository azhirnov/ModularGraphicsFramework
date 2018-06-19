// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
	
extern void Test_ComputeApi (StringCRef device)
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Compute" ) );

	#ifdef GRAPHICS_API_VULKAN
	{
		CApp	app;
		app.Initialize( "VK 1.0"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif

	#ifdef GRAPHICS_API_OPENGL
	{
		CApp	app;
		app.Initialize( "GL 4.5"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
	
	#ifdef COMPUTE_API_OPENCL
	{
		CApp	app;
		app.Initialize( "CL 1.2"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
	
	#ifdef GRAPHICS_API_SOFT
	{
		CApp	app;
		app.Initialize( "SW 1.0"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
}
