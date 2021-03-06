// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Threads/ParallelThreadImpl.h"

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
		SHARED_POINTER( ParallelThreadImpl );
		SHARED_POINTER( ThreadManager );

		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::AddThreadToManager,
											ModuleMsg::RemoveFromManager
											//ModuleMsg::FindThread
										>;
		using SupportedEvents_t		= Module::SupportedEvents_t;
		
		using TaskMsgList_t			= MessageListFrom< ModuleMsg::Update, ModuleMsg::PushAsyncMessage >;

		using CallInThreadFunc_t	= void (*) (const ThreadManagerPtr &mngr, const ModulePtr &thread, GlobalSystemsRef gs);
		using BlockingWaitThread_t	= ModuleMsg::AddThreadToManager::BlockingWaitFunc_t;

		struct ThreadInfo
		{
			ModulePtr				thread;
			BlockingWaitThread_t	wait;
		};

		using Threads_t				= Map< ThreadID, ThreadInfo >;

		struct CreateParallelThreadData;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		Threads_t				_threads;
		ParallelThreadImplPtr	_currentThread;
		Mutex					_lock;


	// methods
	public:
		ThreadManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ThreadManager &info);
		~ThreadManager ();
		
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);


	// message handlers
	private:
		//bool _FindThread (const ModuleMsg::FindThread &);
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _Update (const ModuleMsg::Update &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _AddThreadToManager (const ModuleMsg::AddThreadToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);


	private:
		bool _SendToAllThreads (CallInThreadFunc_t func, bool exceptMain = false);

		static ModulePtr _CreateParallelThread (UntypedID_t, GlobalSystemsRef, const CreateInfo::Thread &);
		static ModulePtr _CreateThreadManager (UntypedID_t, GlobalSystemsRef, const CreateInfo::ThreadManager &);

		static void _RunAsync (void *);
	};


}	// Base
}	// Engine
