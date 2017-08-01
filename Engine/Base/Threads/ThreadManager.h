// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		SHARED_POINTER( ParallelThread );
		SHARED_POINTER( ThreadManager );

		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
											//ModuleMsg::FindThread
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using Threads_t				= Map< ThreadID, ModulePtr >;

		using CallInThreadFunc_t	= void (*) (const ThreadManagerPtr &mngr, const ModulePtr &thread, const ModulePtr &task);
		

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
		ThreadManager (const GlobalSystemsRef gs, const CreateInfo::ThreadManager &info);
		~ThreadManager ();

		bool SendToAllThreads (CallInThreadFunc_t func);

		static GModID::type	GetStaticID ()		{ return "thread.mngr"_GModID; }
		
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);


	// message handlers
	private:
		//bool _FindThread (const Message< ModuleMsg::FindThread > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);


	private:
		static ModulePtr _CreateParallelThread (GlobalSystemsRef, const CreateInfo::Thread &);
		static ModulePtr _CreateThreadManager (GlobalSystemsRef, const CreateInfo::ThreadManager &);

		static void _RunAsync (void *);
	};


}	// Base
}	// Engine
