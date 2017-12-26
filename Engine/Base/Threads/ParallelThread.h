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


namespace CreateInfo
{

	//
	// Thread Create Info
	//
	struct Thread
	{
	// types
		using OnStartThreadFunc_t	= std::function< void (GlobalSystemsRef) >;

	// variables
		String								name;
		ModulePtr							manager;
		ReadOnce< OnStartThreadFunc_t >		onStarted;		// this function must be as fast as possible

	// methods
		Thread (StringCRef name, const ModulePtr &mngr) :
			name( name ),
			manager( mngr ),
			onStarted( OnStartThreadFunc_t() )
		{}
		
		template <typename FN, typename ...Args>
		Thread (StringCRef name, const ModulePtr &mngr, FN func, Args&& ...args) :
			name{ name },
			manager{ mngr },
			onStarted{std::bind( func, FW<Args>(args)..., std::placeholders::_1 )}
		{}
	};
	

	//
	// Thread Manager Create Info
	//
	struct ThreadManager
	{
	};

}	// CreateInfo


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
