// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/Common.h"

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

		static constexpr bits	All			= bits().SetInterval( Vertex, Fragment );
		static constexpr bits	ComputeBit	= bits().Set( Compute );

		static StringCRef ToString (type value);
	};


}	// Platforms
}	// Engine
