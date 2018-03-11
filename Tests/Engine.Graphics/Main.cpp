// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"


int main ()
{
	Logger::GetInstance()->Open( "log", false );
	
	#ifdef GRAPHICS_API_VULKAN
	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Graphics" ) );

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
	
		CHECK( app.ms->GlobalSystems()->fileManager->FindAndSetCurrentDir( "Tests/Engine.Graphics" ) );

		app.Initialize( "GL 4.4"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstance()->Send< ModuleMsg::Delete >({});
	#endif

	return 0;
}
