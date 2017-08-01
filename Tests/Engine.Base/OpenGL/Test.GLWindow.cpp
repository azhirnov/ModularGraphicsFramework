// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GLApp.h"

	
extern void Test_GLWindow ()
{
	{
		GLApp	app;
	
		CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );

		app.Initialize();

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstace()->Send( Message< ModuleMsg::Delete >() );

	WARNING( "OpenGL Window test succeeded!" );
}
