// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Input.h"

namespace Engine
{
namespace Platforms
{

	//
	// Input Manger
	//

	class InputManager final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using InputThreads_t		= Set< ModulePtr >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		InputThreads_t		_threads;
		

	// methods
	public:
		InputManager (GlobalSystemsRef gs, const CreateInfo::InputManager &ci);
		~InputManager ();
		
		static void Register ();
		static void Unregister ();

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	private:
		static ModulePtr _CreateInputThread (GlobalSystemsRef, const CreateInfo::InputThread &);
		static ModulePtr _CreateInputManager (GlobalSystemsRef, const CreateInfo::InputManager &);
	};


}	// Platforms
}	// Engine
