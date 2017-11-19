// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Engine.STL.h"
#include "Engine/Config/Engine.Config.h"
#include "Engine/Config/Engine.Version.h"
#include "Engine/Base/Common/Defines.h"

#ifdef __GX_BUILD_LIB__
#	define _ENGINE_BASE_EXPORT_
#elif defined( __GX_ENGINE_BASE_BUILD__ )
#	define _ENGINE_BASE_EXPORT_		GX_DLL_EXPORT
#else
#	define _ENGINE_BASE_EXPORT_		GX_DLL_IMPORT
#endif

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
		class FileManager;

		// TODO: FileManager
		// TODO: Serializer

		WEAK_POINTER( Module );
		SHARED_POINTER( Module );
		SHARED_POINTER( TaskModule );
		SHARED_POINTER( ModulesFactory );

		_ENGINE_BASE_EXPORT_ Ptr<Module>  GetMainSystemInstace ();

		template <typename T> struct Message;
	}
	
	namespace ModuleMsg
	{
		// never use 'using namespace ModuleMsg' !!!
	}

	namespace CreateInfo
	{
		// never use 'using namespace CreateInfo' !!!
	}
	
	namespace Platforms
	{
		//void RegisterPlatforms ();
		//void UnregisterPlatforms ();
	}
	
	namespace Profilers
	{
	}
	
	namespace Validators
	{
	}
	
	namespace Graphics
	{
		//void RegisterGraphics ();
		//void UnregisterGraphics ();
	}
	
	namespace Audio
	{
	}
	
	namespace UI
	{
	}

}	// Engine
