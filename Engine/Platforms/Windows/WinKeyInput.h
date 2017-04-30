// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Input.h"
#include "Engine/Platforms/Windows/WinWindow.h"

#if defined( PLATFORM_WINDOWS )

namespace Engine
{
namespace Platforms
{

	//
	// Windows Key Input
	//

	class WinKeyInput final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											ModuleMsg::WindowDescriptorChanged,
											ModuleMsg::WindowCreated,
											ModuleMsg::WindowBeforeDestroy,
											ModuleMsg::WindowRawMessage
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputKey
										> >;

		using Keys_t	= Array< ModuleMsg::InputKey >;	// TODO: use static array


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		Keys_t		_pendingKeys;	// store keys and send in Update


	// methods
	public:
		WinKeyInput (const GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci);
		~WinKeyInput ();
		
		static OModID::type		GetStaticID ()			{ return "win.keys"_OModID; }


	// message handlers
	private:
		bool _WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &);
		bool _WindowCreated (const Message< ModuleMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &);
		bool _WindowRawMessage (const Message< ModuleMsg::WindowRawMessage > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
	};


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
