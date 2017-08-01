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
			Unknown	= uint(-1),
		};

		using bits = EnumBitfield< EImageAspect >;
	};

}	// Platforms
}	// Engine
