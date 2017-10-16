// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace ModuleMsg
{

	//
	// Add Thread To Manager
	//
	struct AddThreadToManager : AddToManager
	{
		SingleRead< Delegate<void ()> >		wait;	// wait for complete

		AddThreadToManager (const ModulePtr &mod, Delegate<void ()> &&waitFunc) :
			AddToManager{ mod }, wait{ RVREF(waitFunc) }
		{}
	};

}	// ModuleMsg

namespace Base
{

	//
	// Parallel Thread
	//

	class ParallelThread : public Module
	{
	public:
		ParallelThread (const GlobalSystemsRef gs,
						const ModuleConfig &config,
						const Runtime::VirtualTypeList *msgTypes,
						const Runtime::VirtualTypeList *eventTypes);

		~ParallelThread ();
	};


}	// Base
}	// Engine
