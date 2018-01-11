// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{

	// Streams
	static constexpr GModID::type	StreamManagerModuleID	= "stream.mngr"_GModID;
	static constexpr OModID::type	InputStreamModuleID		= "in-stream"_OModID;
	static constexpr OModID::type	OutputStreamModuleID	= "out-stream"_OModID;

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
