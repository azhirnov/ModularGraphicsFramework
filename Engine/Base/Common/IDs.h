// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{

	// Data Providers
	static constexpr GModID::type	DataProviderManagerModuleID			= "data.mngr"_GModID;
	static constexpr TModID::type	LocalStorageDataProviderModuleID	= "ls.dprov"_TModID;
	static constexpr TModID::type	InMemoryDataProviderModuleID		= "mem.dprov"_TModID;
	static constexpr TModID::type	InternetDataProviderModuleID		= "net.dprov"_TModID;
	static constexpr TModID::type	BuiltinStorageDataProviderModuleID	= "bs.dprov"_TModID;

	static constexpr OModID::type	FileInputStreamModuleID				= "in-fstream"_OModID;
	static constexpr OModID::type	FileOutputStreamModuleID			= "out-fstream"_OModID;
	static constexpr OModID::type	FileDataInputModuleID				= "in-file"_OModID;
	static constexpr OModID::type	FileDataOutputModuleID				= "out-file"_OModID;


	// Tasks
	static constexpr GModID::type	TaskManagerModuleID		= "task.mngr"_GModID;
	static constexpr TModID::type	TaskModuleModuleID		= "task.mod"_TModID;


	// Threads
	static constexpr GModID::type	ThreadManagerModuleID	= "thread.mngr"_GModID;
	static constexpr TModID::type	ParallelThreadModuleID	= "thread"_TModID;


	// Main
	static constexpr GModID::type	MainSystemModuleID		= "main"_GModID;


}	// Base
}	// Engine
