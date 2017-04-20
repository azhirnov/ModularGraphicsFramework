// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{

	//
	// Parallel Thread
	//

	class ParallelThread final : public Module
	{
		friend class ThreadManager;

	// types
	private:
		class AddToManagerTask;
		class RemoveFromManagerTask;

		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		OS::Thread			_thread;
		TimeProfilerD		_timer;
		bool				_isLooping;


	// methods
	public:
		ParallelThread (const SubSystemsRef gs, const CreateInfo::Thread &);
		~ParallelThread ();

		static TModID::type		GetStaticID ()			{ return "thread"_TModID; }


	// message handlers
	private:
		void _Link (const Message< ModuleMsg::Link > &);
		void _Compose (const Message< ModuleMsg::Compose > &);
		void _Delete (const Message< ModuleMsg::Delete > &);

	private:
		void _OnEnter ();
		void _Loop ();
		void _OnExit ();
		void _SyncUpdate ();
	};


}	// Base
}	// Engine
