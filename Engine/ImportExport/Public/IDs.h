// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ImportExport/Public/Common.h"

namespace Engine
{
namespace ImportExport
{

	// Image Loaders & Savers
	static constexpr OModID::type  GXImageLoaderModuleID			= "gx.iload"_OModID;
	static constexpr OModID::type  GXImageSaverModuleID				= "gx.isave"_OModID;

	// Mesh Loaders & Savers
	static constexpr OModID::type  GXMeshLoaderModuleID				= "gx.mload"_OModID;
	static constexpr OModID::type  GXMeshSaverModuleID				= "gx.msave"_OModID;


}	// ImportExport
}	// Engine
