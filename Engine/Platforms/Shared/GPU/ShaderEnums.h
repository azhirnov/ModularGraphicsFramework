// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EShader
	{
		enum type : uint
		{
			Vertex		= 0,
			TessControl,
			TessEvaluation,
			Geometry,
			Fragment,
			Compute,

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( EShader );

		static StringCRef ToString (type value);
	};


}	// Platforms
}	// Engine
