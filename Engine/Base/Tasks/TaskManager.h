// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/IDs.h"
#include "Engine/Base/Public/TaskModule.h"
#include "Engine/Base/Public/AsyncMessage.h"

namespace Engine
{
namespace Base
{

	//
	// Async Tasks Manager
	//

	class TaskManager final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::AddTaskSchedulerToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::PushAsyncMessage
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using AsyncPushFunc_t		= ModuleMsg::AddTaskSchedulerToManager::Func_t;


		struct TaskModuleInfo
		{
			ModulePtr			module;
			AsyncPushFunc_t		asyncPushMsg;
		};
		using TaskThreads_t			= Map< ThreadID, TaskModuleInfo >;
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		TaskThreads_t	_threads;
		OS::Mutex		_lock;


	// methods
	public:
		TaskManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::TaskManager &);
		~TaskManager ();

		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);


	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);
		bool _AddTaskSchedulerToManager (const ModuleMsg::AddTaskSchedulerToManager &);
		bool _PushAsyncMessage (const ModuleMsg::PushAsyncMessage &msg) noexcept;

	private:
		static ModulePtr _CreateTaskModule (UntypedID_t, GlobalSystemsRef, const CreateInfo::TaskModule &);
		static ModulePtr _CreateTaskManager (UntypedID_t, GlobalSystemsRef, const CreateInfo::TaskManager &);
	};


}	// Base
}	// Engine
