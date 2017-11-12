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
	// types
		using BlockingWaitFunc_t	= Delegate< void () >;

	// variables
		ReadOnce< BlockingWaitFunc_t >		wait;	// wait for complete

	// methods
		AddThreadToManager (const ModulePtr &mod, BlockingWaitFunc_t &&waitFunc) :
			AddToManager{ mod },  wait{ RVREF(waitFunc) }
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
						const TypeIdList *msgTypes,
						const TypeIdList *eventTypes);

		~ParallelThread ();
	};


}	// Base
}	// Engine
