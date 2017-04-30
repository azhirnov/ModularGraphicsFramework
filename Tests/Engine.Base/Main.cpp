// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Engine.Base.h"
#include "Engine/Platforms/Engine.Platforms.h"

using namespace Engine;
using namespace Engine::Base;
using namespace Engine::Platforms;

#if 0
#	include "GLApp.h"
	using App = GLApp;
#else
#	include "VkApp.h"
	using App = VkApp;
#endif


void main ()
{
	Logger::GetInstance()->Open( "log", false );

	App	app;
	
	CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );

	app.Initialize();

	// main loop
	for (; app.Update();) {}

	app.Quit();
}
