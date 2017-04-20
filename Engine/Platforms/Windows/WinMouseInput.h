// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Platforms/Shared/Input.h"
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
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
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
		WinMouseInput (const SubSystemsRef gs, const CreateInfo::RawInputHandler &ci);
		~WinMouseInput ();
		
		static OModID::type		GetStaticID ()			{ return "win.mouse"_OModID; }


	// message handlers
	private:
		void _WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &);
		void _WindowCreated (const Message< ModuleMsg::WindowCreated > &);
		void _WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &);
		void _WindowRawMessage (const Message< ModuleMsg::WindowRawMessage > &);
		void _Link (const Message< ModuleMsg::Link > &);
		void _Update (const Message< ModuleMsg::Update > &);
	};


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
