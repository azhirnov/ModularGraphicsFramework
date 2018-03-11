// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Profilers/Impl/ProfilerObjectsConstructor.h"
#include "Engine/Profilers/Public/GpuStatistic.h"

namespace Engine
{
namespace Profilers
{

/*
=================================================
	RegisterProfilers
=================================================
*/
	void RegisterProfilers ()
	{
		using POC = ProfilerObjectsConstructor;
		
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( FPSCounterModuleID, &POC::CreateFPSCounter ) );
	}
	
/*
=================================================
	UnregisterProfilers
=================================================
*/
	void UnregisterProfilers ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( FPSCounterModuleID );
	}

}	// Profilers
}	// Engine