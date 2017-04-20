// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Platforms/Shared/Input.h"

namespace Engine
{
namespace Platforms
{

	//
	// Input Thread
	//

	class InputThread final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::InputKey,
											ModuleMsg::InputMotion
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputKey,		// not recomended to use
											ModuleMsg::InputMotion,		// not recomended to use
											ModuleMsg::InputKeyBind,
											ModuleMsg::InputMotionBind
										> >;

		using KeyCollback_t			= ModuleMsg::InputKeyBind::Callback_t;
		using KeyBinds_t			= MultiMap< KeyID::type, Pair< EKeyState, KeyCollback_t > >;

		using MotionCallback_t		= ModuleMsg::InputMotionBind::Callback_t;
		using MotionBinds_t			= MultiMap< MotionID::type, MotionCallback_t >;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		KeyBinds_t		_keyBinds;
		MotionBinds_t	_motionBinds;
		

	// methods
	public:
		InputThread (const SubSystemsRef gs, const CreateInfo::InputThread &ci);
		~InputThread ();
		
		static TModID::type		GetStaticID ()			{ return "input.thrd"_TModID; }
		

	// message handlers
	private:
		void _Delete (const Message< ModuleMsg::Delete > &);
		void _InputKey (const Message< ModuleMsg::InputKey > &);
		void _InputMotion (const Message< ModuleMsg::InputMotion > &);
	};


}	// Platforms
}	// Engine
