// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

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

		using SupportedMessages_t	= Module::SupportedMessages_t::Append<	MessageListFrom<
											ModuleMsg::OnRegistered,
											ModuleMsg::OnUnregistered,
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
		TaskManager (const SubSystemsRef gs, const CreateInfo::TaskManager &);
		~TaskManager ();

		bool PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &msg);

		static GModID::type	GetStaticID ()		{ return "task.mngr"_GModID; }


	// message handlers
	private:
		void _Delete (const Message< ModuleMsg::Delete > &);
		void _OnRegistered (const Message< ModuleMsg::OnRegistered > &);
		void _OnUnregistered (const Message< ModuleMsg::OnUnregistered > &);
		void _AddToManager (const Message< ModuleMsg::AddToManager > &);
		void _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

	private:
		static ModulePtr _CreateTaskModule (SubSystemsRef, const CreateInfo::TaskModule &);
	};


}	// Base
}	// Engine
