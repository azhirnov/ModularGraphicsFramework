// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Platforms/Shared/Display.h"
#include "Engine/Platforms/Shared/Platform.h"
#include "Engine/Platforms/Common/OSEnums.h"
#include "Engine/Platforms/Windows/WinWindow.h"
#include "Engine/Platforms/Windows/WinKeyInput.h"
#include "Engine/Platforms/Windows/WinMouseInput.h"

#if defined( PLATFORM_WINDOWS )

namespace Engine
{
namespace ModuleMsg
{
	//
	// Platform Created Message
	//
	struct PlatformCreated
	{
		OS::HiddenOSTypeFrom<void*>		inst;
		String							className;
	};

}	// ModuleMsg

namespace Platforms
{

	//
	// Windows Platform
	//

	class WinPlatform final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											//ModuleMsg::OnRegistered,
											//ModuleMsg::OnUnregistered,
											ModuleMsg::RequestDisplayParams,
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::PlatformCreated
										> >;

		using ModArray_t			= Set< ModulePtr >;
		using Directories_t			= StaticArray< String, EDirectory::_Count >;
		using Handle_t				= OS::HiddenOSTypeFrom<void*>;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		ModArray_t				_windows;
		
		CreateInfo::Platform	_createInfo;

		String					_className;
		Handle_t				_instance;		// HMODULE

		Directories_t			_directories;
		Display					_display;


	// methods
	public:
		WinPlatform (const SubSystemsRef gs, const CreateInfo::Platform &ci);
		~WinPlatform ();
		
		static GModID::type		GetStaticID ()			{ return "win.platform"_GModID; }


	// message handlers
	private:
		void _Delete (const Message< ModuleMsg::Delete > &);
		void _Compose (const Message< ModuleMsg::Compose > &);
		//void _Update (const Message< ModuleMsg::Update > &);
		//void _OnRegistered (const Message< ModuleMsg::OnRegistered > &);
		//void _OnUnregistered (const Message< ModuleMsg::OnUnregistered > &);
		void _RequestDisplayParams (const Message< ModuleMsg::RequestDisplayParams > &);
		void _AddToManager (const Message< ModuleMsg::AddToManager > &);
		void _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

	private:
		bool _IsCreated () const;
		bool _Create ();
		void _Destroy ();

		bool _GetDirectories ();
		bool _RegisterClass ();
		void _UpdateDisplayParams ();

		uint2  _GetScreenResolution () const;
		float2 _ScreenPhysicalSize () const;
		uint   _GetDisplayFrequency () const;

		bool   _SetScreenResolution (const uint2 &size, uint freq);
		bool   _ReturnToDefaultResolution ();

	private:
		friend void RegisterPlatforms ();
		static void _RegisterAll (SubSystemsRef);

		static ModulePtr _CreateWinPlatform (SubSystemsRef, const CreateInfo::Platform &);
		static ModulePtr _CreateWinWindow (SubSystemsRef, const CreateInfo::Window &);
		static ModulePtr _CreateWinKeyInput (SubSystemsRef, const CreateInfo::RawInputHandler &);
		static ModulePtr _CreateWinMouseInput (SubSystemsRef, const CreateInfo::RawInputHandler &);
	};


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
