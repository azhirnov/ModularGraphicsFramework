// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Base/Common/Common.h"

namespace Engine
{
namespace CreateInfo
{
	

	//
	// Thread Create Info
	//
	struct Thread
	{
		String				name;
		Base::ModulePtr		manager;
	};
	

	//
	// Task Module Create Info
	//
	struct TaskModule
	{
		// TODO: set priority or something else
		Base::ModulePtr		manager;
	};
	

	//
	// Thread Manager Create Info
	//
	struct ThreadManager
	{
	};
	

	//
	// Task Manager Create Info
	//
	struct TaskManager
	{
	};


}	// CreateInfo
}	// Engine
