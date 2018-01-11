// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
			Unknown = ~0u
		};

		GX_ENUM_BITFIELD( EGpuMemory );
	};

}	// Platforms
}	// Engine
