// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/Common.h"

namespace Engine
{
namespace CreateInfo
{
	struct VRThread;
	struct VRFramebuffer;

}	// CreateInfo


namespace Platforms
{
	// Emulator
	static constexpr TModID::type  EmulatorVRThreadModuleID			= "evr.thread"_TModID;
	static constexpr OModID::type  EmulatorVRFramebufferModuleID	= "evr.fbo"_OModID;
	
	// Oculus VR
	static constexpr TModID::type  OculusVRThreadModuleID			= "oculus.thrd"_TModID;
	static constexpr OModID::type  GLOculusVRFramebufferModuleID	= "gl.ovr.fbo"_OModID;


	
	//
	// VR Objects Constructor
	//

	class VRObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();

		static ModulePtr CreateEmulatorVRThread (GlobalSystemsRef, const CreateInfo::VRThread &);

		static ModulePtr CreateOculusVRThread (GlobalSystemsRef, const CreateInfo::VRThread &);
	};


}	// Platforms
}	// Engine
