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
	// Windows Mouse Input
	//

	class WinMouseInput final : public Module
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
											ModuleMsg::InputMotion
										> >;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		uint2				_surfaceSize;
		Optional<float2>	_mouseDifference;
		float2				_mousePos;
		

	// methods
	public:
		WinMouseInput (const GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci);
		~WinMouseInput ();
		
		static OModID::type		GetStaticID ()			{ return "win.mouse"_OModID; }


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
