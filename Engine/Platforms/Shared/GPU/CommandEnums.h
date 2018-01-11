// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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


	struct ECmdBufferCreate
	{
		enum type : uint
		{
			// set 'true' to create secondary command buffer
			Secondary,

			// if 'false' then you need return command buffer to initial state before begin recording
			// this value depends of command builder module
			ImplicitResetable,

			// will wait fence before restart or delete
			//UseFence,

			_Count,
			Unknown	= ~0u,
		};

		GX_ENUM_BITFIELD( ECmdBufferCreate );
	};


}	// Platforms
}	// Engine
