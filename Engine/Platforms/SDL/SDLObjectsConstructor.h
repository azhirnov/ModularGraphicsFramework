// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/OS/IDs.h"

#ifdef PLATFORM_SDL

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
	// SDL Objects Constructor
	//

	class SDLObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();
		
		static OSModuleIDs GetModuleIDs ();

		static ModulePtr  CreateSDLKeyInput (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::RawInputHandler &);
		static ModulePtr  CreateSDLMouseInput (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::RawInputHandler &);
		static ModulePtr  CreateSDLWindow (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Window &);
		static ModulePtr  CreateSDLPlatform (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Platform &);
	};


}	// Platforms
}	// Engine

#endif	// PLATFORM_SDL
