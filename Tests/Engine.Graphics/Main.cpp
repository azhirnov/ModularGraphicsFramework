// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"


void main ()
{
	Logger::GetInstance()->Open( "log", false );
	
	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Graphics" ) );

		app.Initialize( "VK 1.0"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstace()->Send< ModuleMsg::Delete >({});

	{
		GApp	app;
	
		CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Graphics" ) );

		app.Initialize( "GL 4.4"_GAPI );

		// main loop
		for (; app.Update();) {}

		app.Quit();
	}
	GetMainSystemInstace()->Send< ModuleMsg::Delete >({});
}
