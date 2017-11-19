// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/Platform.h"
#include "Engine/Platforms/Shared/OS/OSEnums.h"
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/Windows/WinDisplay.h"

#if defined( PLATFORM_WINDOWS )

namespace Engine
{
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
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											OSMsg::GetDisplays,
											OSMsg::GetOSModules
										> >;
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Delete,
											OSMsg::OnWinPlatformCreated
										>;

		using ModArray_t			= Set< ModulePtr >;
		using Directories_t			= StaticArray< String, EDirectory::_Count >;

		using HMODULE_t				= OS::HiddenOSTypeFrom<void*>;
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModArray_t				_windows;
		WinDisplay				_display;
		
		CreateInfo::Platform	_createInfo;

		String					_className;
		HMODULE_t				_instance;

		Directories_t			_directories;


	// methods
	public:
		WinPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci);
		~WinPlatform ();
		
		static _ENGINE_PLATFORMS_EXPORT_ void Register (GlobalSystemsRef);
		static _ENGINE_PLATFORMS_EXPORT_ void Unregister (GlobalSystemsRef);

		static OSModuleIDs GetModuleIDs ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		//bool _Update (const Message< ModuleMsg::Update > &);
		bool _GetDisplays (const Message< OSMsg::GetDisplays > &);
		bool _GetOSModules (const Message< OSMsg::GetOSModules > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

	private:
		bool _IsCreated () const;
		bool _Create ();
		void _Destroy ();

		bool _GetDirectories ();
		bool _RegisterClass ();

		bool _SetScreenResolution (const uint2 &size, uint freq);
		bool _ReturnToDefaultResolution ();

	private:
		static ModulePtr _CreateWinPlatform (GlobalSystemsRef, const CreateInfo::Platform &);
		static ModulePtr _CreateWinWindow (GlobalSystemsRef, const CreateInfo::Window &);
		static ModulePtr _CreateWinKeyInput (GlobalSystemsRef, const CreateInfo::RawInputHandler &);
		static ModulePtr _CreateWinMouseInput (GlobalSystemsRef, const CreateInfo::RawInputHandler &);
	};


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
