// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	TaskModule - attached to thread and process async messages (short time)
					and async tasks (long time with progress).
*/

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// Task Module Create Info
	//
	struct TaskModule
	{
		// TODO: set priority or something else
		ModulePtr		manager;
	};
	

	//
	// Task Manager Create Info
	//
	struct TaskManager
	{
	};

}	// CreateInfo
}	// Engine
