// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/OS/IDs.h"

#ifdef PLATFORM_WINDOWS

namespace Engine
{
namespace CreateInfo
{
	struct RawInputHandler;
	struct Window;
	struct Platform;

}	// CreateInfo


namespace Platforms
{

	//
	// Windows Objects Constructor
	//

	class WinObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();
		
		static OSModuleIDs GetModuleIDs ();

		static ModulePtr  CreateWinKeyInput (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::RawInputHandler &);
		static ModulePtr  CreateWinMouseInput (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::RawInputHandler &);
		static ModulePtr  CreateWinWindow (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Window &);
		static ModulePtr  CreateWinPlatform (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Platform &);
	};


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
