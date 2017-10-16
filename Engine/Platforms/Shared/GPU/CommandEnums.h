// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EImageAspect
	{
		enum type : uint
		{
			Color,
			Depth,
			Stencil,
			Metadata,
			
			_Count,
			Unknown	= ~0u,
		};

		GX_ENUM_BITFIELD( EImageAspect );
	};

}	// Platforms
}	// Engine
