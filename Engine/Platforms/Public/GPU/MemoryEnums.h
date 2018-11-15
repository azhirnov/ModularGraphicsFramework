// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

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
			Dedicated,			// indicates that buffer/image have exlusive memory object
			SupportAliasing,	// memory object may be binded to many buffers/images

			_Count,
			Unknown = ~0u
		};

		GX_ENUM_BITFIELD( EGpuMemory );
	};

}	// Platforms
}	// Engine
