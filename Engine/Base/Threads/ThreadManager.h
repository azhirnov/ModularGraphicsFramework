// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Base/Threads/ParallelThread.h"

namespace Engine
{
namespace Base
{

	//
	// Thread Manager
	//

	class ThreadManager final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::OnRegistered,
											ModuleMsg::OnUnregistered,
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
											//ModuleMsg::FindThread
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using Threads_t				= Map< ThreadID, ModulePtr >;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		Threads_t			_threads;
		ParallelThread		_currentThread;
		OS::Mutex			_lock;


	// methods
	public:
		ThreadManager (const SubSystemsRef gs, const CreateInfo::ThreadManager &info);
		~ThreadManager ();

		bool SendToAllThreads (AsyncMessage &&msg);

		static GModID::type	GetStaticID ()		{ return "thread.mngr"_GModID; }


	// message handlers
	private:
		void _OnRegistered (const Message< ModuleMsg::OnRegistered > &);
		void _OnUnregistered (const Message< ModuleMsg::OnUnregistered > &);
		//void _FindThread (const Message< ModuleMsg::FindThread > &);
		void _Link (const Message< ModuleMsg::Link > &);
		void _Compose (const Message< ModuleMsg::Compose > &);
		void _Delete (const Message< ModuleMsg::Delete > &);
		void _Update (const Message< ModuleMsg::Update > &);
		void _AddToManager (const Message< ModuleMsg::AddToManager > &);
		void _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);


	private:
		static ModulePtr _CreateParallelThread (SubSystemsRef, const CreateInfo::Thread &);

		static void _RunAsync (void *);
	};


}	// Base
}	// Engine
