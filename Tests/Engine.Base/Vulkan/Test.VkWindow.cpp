// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "VkApp.h"
	
extern void Test_VkWindow ()
{
	{
		VkApp	app;
	
		CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );

		app.Initialize();

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstace()->Send( Message< ModuleMsg::Delete >() );

	WARNING( "Vulkan Window test succeeded!" );
}
