// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;
		
		using OnStartThreadFunc_t	= CreateInfo::Thread::OnStartThreadFunc_t;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		OS::Thread				_thread;
		OnStartThreadFunc_t		_onStarted;
		TimeProfilerD			_timer;
		bool					_isLooping;


	// methods
	public:
		ParallelThread (const GlobalSystemsRef gs, const CreateInfo::Thread &);
		~ParallelThread ();

		static TModID::type		GetStaticID ()			{ return "thread"_TModID; }


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		void _OnEnter ();
		void _Loop ();
		void _OnExit ();
		void _SyncUpdate ();
	};


}	// Base
}	// Engine
