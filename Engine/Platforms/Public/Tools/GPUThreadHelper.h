// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/VR.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Engine::Platforms;


	//
	// GPU Thread Helper
	//

	class GPUThreadHelper final : public Noninstancable
	{
	// methods
	public:
		ND_ static ModulePtr  FindGraphicsThread (GlobalSystemsRef gs);
		ND_ static ModulePtr  FindComputeThread (GlobalSystemsRef gs);
		ND_ static ModulePtr  FindVRThread (GlobalSystemsRef gs);		// graphics + VR
	};


}	// PlatformTools
}	// Engine
