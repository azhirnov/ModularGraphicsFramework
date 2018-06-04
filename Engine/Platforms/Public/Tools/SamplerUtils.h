// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Sampler.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Engine::Platforms;


	//
	// Sampler Utils
	//

	struct SamplerUtils final : public Noninstancable
	{
		static void ValidateDescriptor (INOUT SamplerDescriptor &descr, uint maxAnisotropyLevel);
	};

}	// PlatformTools
}	// Engine
