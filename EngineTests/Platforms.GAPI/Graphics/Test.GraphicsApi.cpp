// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
	
extern void Test_GraphicsApi (StringCRef device, bool debug)
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "EngineTests/Platforms.GAPI/Graphics" ) );

	#if 0 //def GRAPHICS_API_OPENGL
	{
		GApp	app;
		app.Initialize( "GL 4.5"_GAPI, device, debug );

		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
	
	#ifdef GRAPHICS_API_VULKAN
	{
		GApp	app;
		app.Initialize( "VK 1.0"_GAPI, device, debug );

		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
	
	#ifdef GRAPHICS_API_SOFT
	{
		GApp	app;
		app.Initialize( "SW 1.0"_GAPI, device, debug );

		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
}
