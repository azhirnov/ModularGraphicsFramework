// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Shared/Common.h"

namespace Engine
{
namespace Graphics
{

	// Font
	static constexpr OModID::type  FontModuleID					= "font"_OModID;
	static constexpr OModID::type  FontManagerModuleID			= "font-mngr"_OModID;

	// Batch
	static constexpr OModID::type  BatchRendererModuleID		= "batch-ren"_OModID;
	
	// Command buffer
	static constexpr OModID::type  CommandBufferManagerModuleID	= "pf-cmdbuf"_OModID;
	static constexpr OModID::type  AsyncCommandBufferModuleID	= "asc-cmdbuf"_OModID;

	// Memory Stream
	static constexpr OModID::type  MemoryInputStreamModuleID	= "mem.istream"_OModID;
	static constexpr OModID::type  MemoryOutputStreamModuleID	= "mem.ostream"_OModID;

}	// Graphics
}	// Engine
