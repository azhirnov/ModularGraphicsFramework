// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/MultiSamples.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"
#include "Engine/Platforms/Shared/GPU/TextureEnums.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"

namespace Engine
{
namespace Platforms
{
	//
	// GPU Texture Descriptor
	//

	struct TextureDescriptor
	{
		uint4				dimension;
		MipmapLevel			maxLevel;
		MultiSamples		samples;
		EPixelFormat::type	format;
		EImageUsage::bits	usage;
		ETexture::type		target;
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Texture Create Info
	//

	struct GpuTexture
	{
		ModulePtr						gpuThread;
		ModulePtr						dataSource;
		Platforms::TextureDescriptor	descr;
	};


}	// CreateInfo


namespace ModuleMsg
{


}	// ModuleMsg
}	// Engine
