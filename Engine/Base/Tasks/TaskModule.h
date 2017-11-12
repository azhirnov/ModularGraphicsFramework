// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	TaskModule - attached to thread and process async messages (short time)
					and async tasks (long time with progress).
*/

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{

	//
	// Task Module
	//

	class TaskModule : public Module
	{
	public:
		TaskModule (const GlobalSystemsRef gs,
					const ModuleConfig &config,
					const TypeIdList *msgTypes,
					const TypeIdList *eventTypes);

		~TaskModule ();
	};
	
}	// Base
}	// Engine
