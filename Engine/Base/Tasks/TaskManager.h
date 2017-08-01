// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Tasks/TaskModule.h"

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
		SHARED_POINTER( TaskModule );

		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;
		using TaskThreads_t			= Map< ThreadID, TaskModulePtr >;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		TaskThreads_t	_threads;
		OS::Mutex		_lock;


	// methods
	public:
		TaskManager (const GlobalSystemsRef gs, const CreateInfo::TaskManager &);
		~TaskManager ();

		bool PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &msg);

		static GModID::type	GetStaticID ()		{ return "task.mngr"_GModID; }

		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

	private:
		static ModulePtr _CreateTaskModule (GlobalSystemsRef, const CreateInfo::TaskModule &);
		static ModulePtr _CreateTaskManager (GlobalSystemsRef, const CreateInfo::TaskManager &);
	};


}	// Base
}	// Engine
