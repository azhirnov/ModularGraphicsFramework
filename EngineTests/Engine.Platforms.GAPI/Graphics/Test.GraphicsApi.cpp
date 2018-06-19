// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
	
extern void Test_GraphicsApi (StringCRef device)
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Graphics" ) );

	#ifdef GRAPHICS_API_OPENGL
	{
		GApp	app;
		app.Initialize( "GL 4.5"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
	
	#ifdef GRAPHICS_API_VULKAN
	{
		GApp	app;
		app.Initialize( "VK 1.0"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
	
	#ifdef GRAPHICS_API_SOFT
	{
		GApp	app;
		app.Initialize( "SW 1.0"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
}
