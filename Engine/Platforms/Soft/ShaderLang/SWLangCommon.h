// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/Impl/SWEnums.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Soft/Impl/SWMessages.h"

#ifdef GRAPHICS_API_SOFT

#ifdef COMPILER_MSVC
# pragma warning (push, 1)
# pragma warning (disable: 4201)
#endif

#define GLM_FORCE_SWIZZLE
#include "External/glm/glm/glm.hpp"

#ifdef COMPILER_MSVC
# pragma warning (pop)
#endif

namespace SWShaderLang
{
namespace Impl
{

	using namespace GX_STL::GXMath;

	using namespace Engine;
	using namespace Engine::Base;

	
	struct EStorageAccess
	{
		enum type
		{
			ReadOnly,
			WriteOnly,
			Coherent,
			Restrict,
			Volatile,
		};

		static constexpr bool CanRead (type value)
		{
			return value != WriteOnly;
		}

		static constexpr bool CanWrite (type value)
		{
			return value != ReadOnly;
		}

		// TODO: barriers?
	};


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
