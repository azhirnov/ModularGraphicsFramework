// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Input.h"

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
											ModuleMsg::InputKeyBind,
											ModuleMsg::InputMotionBind,
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputKey,		// not recomended to use
											ModuleMsg::InputMotion		// not recomended to use
										> >;

		using KeyCollback_t			= ModuleMsg::InputKeyBind::Callback_t;
		using KeyBinds_t			= MultiMap< KeyID::type, Pair< EKeyState, KeyCollback_t > >;

		using MotionCallback_t		= ModuleMsg::InputMotionBind::Callback_t;
		using MotionBinds_t			= MultiMap< MotionID::type, MotionCallback_t >;

		using ModulesSet_t			= Set< ModulePtr >;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		ModulesSet_t	_inputs;

		KeyBinds_t		_keyBinds;
		MotionBinds_t	_motionBinds;
		

	// methods
	public:
		InputThread (const GlobalSystemsRef gs, const CreateInfo::InputThread &ci);
		~InputThread ();
		
		static TModID::type		GetStaticID ()			{ return "input.thrd"_TModID; }
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _InputKey (const Message< ModuleMsg::InputKey > &);
		bool _InputMotion (const Message< ModuleMsg::InputMotion > &);
		bool _InputKeyBind (const Message< ModuleMsg::InputKeyBind > &);
		bool _InputMotionBind (const Message< ModuleMsg::InputMotionBind > &);
	};


}	// Platforms
}	// Engine
