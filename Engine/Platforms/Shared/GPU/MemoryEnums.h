// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EGpuMemory
	{
		enum type : uint
		{
			LocalInGPU,
			CoherentWithCPU,
			CachedInCPU,
			//LazilyAllocated,

			_Count,
			Unknown = uint(-1)
		};

		using bits = EnumBitfield< EGpuMemory >;
	};

}	// Platforms
}	// Engine
