// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "SApp.h"
	
extern void Test_Sharing (StringCRef device)
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Sharing" ) );

	#if defined(GRAPHICS_API_OPENGL) and defined(COMPUTE_API_OPENCL)
	{
		LOG( "Test OpenGL - OpenCL sharing", ELog::Info );

		SApp	app;
		app.Initialize( "GL 4.5"_GAPI, "CL 1.2"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	/*#if defined(GRAPHICS_API_OPENGL) and defined(GRAPHICS_API_SOFT)
	{
		LOG( "Test Soft - OpenGL sharing", ELog::Info );

		SApp	app;
		app.Initialize( "SW 1.0"_GAPI, "GL 4.5"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	#if defined(COMPUTE_API_OPENCL) and defined(GRAPHICS_API_SOFT)
	{
		LOG( "Test Soft - OpenCL sharing", ELog::Info );

		SApp	app;
		app.Initialize( "SW 1.0"_GAPI, "CL 1.2"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif
	
	#if defined(GRAPHICS_API_VULKAN) and defined(GRAPHICS_API_SOFT)
	{
		LOG( "Test Soft - Vulkan sharing", ELog::Info );

		SApp	app;
		app.Initialize( "SW 1.0"_GAPI, "VK 1.0"_GAPI, device );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif*/
}
