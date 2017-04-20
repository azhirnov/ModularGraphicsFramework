// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Platforms/Input/InputThread.h"

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
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		InputThreads_t		_threads;
		

	// methods
	public:
		InputManager (const SubSystemsRef gs, const CreateInfo::InputManager &ci);
		~InputManager ();
		
		static GModID::type		GetStaticID ()			{ return "input.mngr"_GModID; }

		
	// message handlers
	private:
		void _AddToManager (const Message< ModuleMsg::AddToManager > &);
		void _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	private:
		friend void RegisterPlatforms ();
		static void _RegisterAll (SubSystemsRef);

		static ModulePtr _CreateInputThread (SubSystemsRef, const CreateInfo::InputThread &);
		static ModulePtr _CreateInputManager (SubSystemsRef, const CreateInfo::InputManager &);
	};


}	// Platforms
}	// Engine
