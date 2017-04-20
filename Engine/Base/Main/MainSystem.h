// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	MainSystem - this class contains all global systems.
*/

#pragma once

#include "Engine/Base/Modules/ModulesFactory.h"
#include "Engine/Base/Tasks/TaskManager.h"
#include "Engine/Base/Threads/ThreadManager.h"

namespace Engine
{
namespace Base
{


	//
	// Main System
	//

	class MainSystem final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::FindModule,
											ModuleMsg::Link,
											ModuleMsg::Compose,
											ModuleMsg::Update,
											ModuleMsg::Delete
										>;
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose,
											ModuleMsg::Update
										>;

		struct _FinalChecks {
			~_FinalChecks ();
		};

		
	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		_FinalChecks		_finalChecks;

		EngineSubSystems	_globalSystems;

		ModulesFactory		_factory;
		TaskManager			_taskMngr;
		ThreadManager		_threadMngr;


	// methods
	public:
		explicit MainSystem (const SubSystemsRef gs);
		~MainSystem ();

		static GModID::type		GetStaticID ()		{ return "main"_GModID; }
		

	// message handlers
	private:
		void _Delete (const Message< ModuleMsg::Delete > &);

	private:
		static ModulePtr _CreateThreadManager (SubSystemsRef, const CreateInfo::ThreadManager &);
		static ModulePtr _CreateTaskManager (SubSystemsRef, const CreateInfo::TaskManager &);
	};


}	// Base
}	// Engine
