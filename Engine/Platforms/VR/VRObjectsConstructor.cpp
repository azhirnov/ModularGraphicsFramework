// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/VR/VRObjectsConstructor.h"
#include "Engine/Platforms/Public/GPU/VR.h"

namespace Engine
{
namespace Platforms
{

/*
=================================================
	Register
=================================================
*/
	void VRObjectsConstructor::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		#ifdef GX_EMULATOR_VR
			mf->Register( EmulatorVRThreadModuleID, &CreateEmulatorVRThread );
		#endif

		#ifdef PLATFORM_OCULUS_VR
			mf->Register( GLEmulatorVRFramebufferModuleID, &CreateOculusVRThread );
		#endif
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void VRObjectsConstructor::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;
		
		#ifdef GX_EMULATOR_VR
			mf->UnregisterAll( EmulatorVRThreadModuleID );
		#endif

		#ifdef PLATFORM_OCULUS_VR
			mf->UnregisterAll( GLEmulatorVRFramebufferModuleID );
		#endif
	}

}	// Platforms
}	// Engine
