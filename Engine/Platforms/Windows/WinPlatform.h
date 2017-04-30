// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/Platform.h"
#include "Engine/Platforms/Shared/OS/OSEnums.h"
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
											ModuleMsg::RequestDisplayParams,
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Delete,
											ModuleMsg::PlatformCreated
										>;

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
		WinPlatform (const GlobalSystemsRef gs, const CreateInfo::Platform &ci);
		~WinPlatform ();
		
		static GModID::type		GetStaticID ()			{ return "win.platform"_GModID; }
		
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		//bool _Update (const Message< ModuleMsg::Update > &);
		bool _RequestDisplayParams (const Message< ModuleMsg::RequestDisplayParams > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

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
		static ModulePtr _CreateWinPlatform (GlobalSystemsRef, const CreateInfo::Platform &);
		static ModulePtr _CreateWinWindow (GlobalSystemsRef, const CreateInfo::Window &);
		static ModulePtr _CreateWinKeyInput (GlobalSystemsRef, const CreateInfo::RawInputHandler &);
		static ModulePtr _CreateWinMouseInput (GlobalSystemsRef, const CreateInfo::RawInputHandler &);
	};


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
