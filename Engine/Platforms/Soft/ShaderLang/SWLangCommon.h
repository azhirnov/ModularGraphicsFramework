// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Public/GPU/ShaderEnums.h"

#include "Engine/Platforms/Soft/Impl/SWEnums.h"
#include "Engine/Platforms/Soft/Impl/SWMessages.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangGLM.h"

namespace SWShaderLang
{
namespace Impl
{

	using namespace GX_STL::GXMath;

	using EStorageAccess	= Engine::Platforms::EShaderMemoryModel;

	using EMemoryAccess		= Engine::Base::EMemoryAccess;

	//using EPixelFormat	= Engine::Platforms::EPixelFormat;


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
