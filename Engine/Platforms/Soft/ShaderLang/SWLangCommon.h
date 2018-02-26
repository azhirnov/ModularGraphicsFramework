// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/Impl/SWEnums.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Soft/Impl/SWMessages.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangGLM.h"

#ifdef GRAPHICS_API_SOFT

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
