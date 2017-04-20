// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Base/Engine.Base.h"
#include "Engine/Platforms/Engine.Platforms.h"

using namespace Engine;
using namespace Engine::Base;



void main ()
{
	class App : public StaticRefCountedObject
	{
	public:
		Ptr<Module>		ms;
		bool			looping	= true;

		App ()
		{
			ms = GetMainSystemInstace();
		}

		void OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &)
		{
			looping = false;

			ms->Send( Message< ModuleMsg::Delete >() );
		}

		bool Update ()
		{
			ms->Send( Message< ModuleMsg::Update >() );
			return looping;
		}
	};

	Logger::GetInstance()->Open( "log", false );

	App	app;

	Platforms::RegisterPlatforms();

	app.ms->AddModule( "win.platform"_GModID, CreateInfo::Platform() );
	//app.ms->AddModule( "vulkan.ctx"_GModID, CreateInfo::GpuContext() );

	auto	thread	= app.ms->GlobalSystems()->Get< ParallelThread >();
	
	thread->AddModule( "win.window"_TModID, CreateInfo::Window() );
	//thread->AddModule( "win.keys"_TModID, CreateInfo::Input() );
	//thread->AddModule( "win.mouse"_TModID, CreateInfo::Input() );
	//thread->AddModule( "vulkan.thrd"_TModID, CreateInfo::GpuThread() );

	thread->GetModule( "win.window"_TModID )->Subscribe( &app, &App::OnWindowClosed );
	thread = null;


	// finish initialization
	app.ms->Send( Message< ModuleMsg::Link >() );
	app.ms->Send( Message< ModuleMsg::Compose >() );

	// main loop
	for (; app.ms->Send( Message< ModuleMsg::Update >() );)
	{
	}

	app.ms->Send( Message< ModuleMsg::Delete >() );
}
