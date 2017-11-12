// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/MessageHelpers.h"
#include "Engine/Base/Common/ModuleMessages.h"
#include "Engine/Base/Common/Enums.h"
#include "Engine/Base/Common/EngineSubSystems.h"

namespace Engine
{
namespace CreateInfo
{
	template <typename T>	using ReadOnce	= ModuleMsg::ReadOnce<T>;

	using ModulePtr		= Engine::Base::ModulePtr;
	using UntypedID_t	= Engine::ModuleMsg::UntypedID_t;
	


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
