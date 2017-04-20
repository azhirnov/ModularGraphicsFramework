// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	TaskModule - attached to thread and process async messages (short time)
					and async tasks (long time with progress).
*/

#pragma once

#include "Engine/Base/Modules/Module.h"
#include "Engine/Base/Tasks/AsyncMessage.h"

namespace Engine
{
namespace Base
{

	//
	// Async Task Module
	//

	class TaskModule final : public Module
	{
		friend class TaskManager;

	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::PushAsyncMessage
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;
		using MsgQueue_t			= MtQueue< CircularQueue< AsyncMessage > >;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		MsgQueue_t		_msgQueue;


	// methods
	public:
		TaskModule (const SubSystemsRef gs, const CreateInfo::TaskModule &);
		~TaskModule ();

		static TModID::type		GetStaticID ()			{ return "task.mod"_TModID; }


	// message handlers
	private:
		void _Update (const Message< ModuleMsg::Update > &);
		void _Delete (const Message< ModuleMsg::Delete > &);
		void _PushAsyncMessage (const Message< ModuleMsg::PushAsyncMessage > &);


	private:
		usize _Push (AsyncMessage &&op);
		usize _Flush ();
		usize _ProcessMessages ();

		static constexpr usize _MaxPendingQueueSize ()	{ return 1024; }
	};

	
}	// Base
}	// Engine
