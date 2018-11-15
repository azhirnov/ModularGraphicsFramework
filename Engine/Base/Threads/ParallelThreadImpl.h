// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/IDs.h"
#include "Engine/Base/Public/ParallelThread.h"

namespace Engine
{
namespace Base
{

	//
	// Parallel Thread
	//

	class ParallelThreadImpl final : public Module
	{
		friend class ThreadManager;

	// types
	private:
		using SupportedEvents_t		= Module::SupportedEvents_t;
		
		using OnStartThreadFunc_t	= CreateInfo::Thread::OnStartThreadFunc_t;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		OS::Thread				_thread;
		OnStartThreadFunc_t		_onStarted;
		TimeProfilerL			_timer;
		bool					_isLooping;


	// methods
	public:
		ParallelThreadImpl (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Thread &);
		~ParallelThreadImpl ();
		
		void SetDebugName (StringCRef name) override;


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);

	private:
		void _OnEnter ();
		void _Loop ();
		void _OnExit ();
		void _Wait ();
		
		void _SyncUpdate ();
		void _NoWait ();
	};


}	// Base
}	// Engine
