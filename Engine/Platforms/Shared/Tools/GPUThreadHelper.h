// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/VR.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Platforms;


	//
	// GPU Thread Helper
	//

	class GPUThreadHelper final : public Noninstancable
	{
	// methods
	public:
		static ModulePtr  FindGraphicsThread (GlobalSystemsRef gs);
		static ModulePtr  FindComputeThread (GlobalSystemsRef gs);
		static ModulePtr  FindVRThread (GlobalSystemsRef gs);		// graphics + VR
	};


}	// PlatformTools
}	// Engine
