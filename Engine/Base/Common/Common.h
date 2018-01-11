// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Engine.STL.h"
#include "Engine/Config/Engine.Config.h"
#include "Engine/Config/Engine.Version.h"
#include "Engine/Base/Common/Defines.h"

namespace Engine
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;

	namespace Base
	{
		class BaseObject;
		class Module;
		class MessageHandler;
		class TaskModule;
		class ParallelThread;
		class ModulesFactory;
		class ThreadManager;
		class MainSystem;
		class AsyncMessage;
		class FileManager;

		// TODO: FileManager
		// TODO: Serializer

		WEAK_POINTER( Module );
		SHARED_POINTER( Module );
		SHARED_POINTER( TaskModule );
		SHARED_POINTER( ModulesFactory );

		Ptr<Module>  GetMainSystemInstance ();

		template <typename T> struct Message;
		
		using TypeIdList = Runtime::TypeIdList;
	}
	
	namespace ModuleMsg
	{
		// never use 'using namespace ModuleMsg' !!!
	}

	namespace ProfilingMsg
	{
		// never use 'using namespace ProfilingMsg' !!!
	}

	namespace CreateInfo
	{
		// never use 'using namespace CreateInfo' !!!
	}
	
	namespace Platforms
	{
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
	
	namespace Scene
	{
	}

	namespace Audio
	{
	}
	
	namespace UI
	{
	}

}	// Engine
