// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Engine.STL.h"
#include "Engine/Config/Engine.Config.h"
#include "Engine/Config/Engine.Version.h"


namespace Engine
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;

	namespace Base
	{
		class Module;
		class MessageHandler;
		class TaskModule;
		class ParallelThread;
		class ModulesFactory;
		class ThreadManager;
		class MainSystem;
		class AsyncMessage;

		// TODO: FileManager
		// TODO: Serializer

		SHARED_POINTER( Module );
		SHARED_POINTER( TaskModule );
		SHARED_POINTER( ModulesFactory );

		Ptr<Module>  GetMainSystemInstace ();
	}
	
	namespace ModuleMsg
	{
		using namespace Base;
	}

	namespace CreateInfo
	{
		//using namespace Base;
	}
	
	namespace Platforms
	{
		void RegisterPlatforms ();
	}
	
	namespace Profilers
	{
	}
	
	namespace Validators
	{
	}
	
	namespace Graphics
	{
	}
	
	namespace Audio
	{
	}
	
	namespace UI
	{
	}

}	// Engine
