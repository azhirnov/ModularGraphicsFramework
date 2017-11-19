// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Shared/Common.h"

namespace Engine
{
namespace Graphics
{

	// Graphics
	static constexpr GModID::type  GraphicsContextModuleID	= "graph-ctx"_GModID;
	static constexpr TModID::type  GraphicsThreadModuleID	= "graph-thread"_TModID;
	
	// Font
	static constexpr OModID::type  FontModuleID				= "font"_OModID;
	static constexpr OModID::type  FontManagerModuleID		= "font-mngr"_OModID;

	// Batch
	static constexpr OModID::type  BatchRendererModuleID	= "batch-ren"_OModID;

}	// Graphics
}	// Engine
