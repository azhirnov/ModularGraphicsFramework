// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/MessageHelpers.h"
#include "Engine/Base/Common/Enums.h"

namespace Engine
{
namespace CreateInfo
{
	template <typename T>	using SingleRead	= ModuleMsg::SingleRead<T>;

	using Engine::Base::ModulePtr;

	

	//
	// Thread Create Info
	//
	struct Thread
	{
		String			name;
		ModulePtr		manager;
	};
	

	//
	// Task Module Create Info
	//
	struct TaskModule
	{
		// TODO: set priority or something else
		ModulePtr		manager;
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
