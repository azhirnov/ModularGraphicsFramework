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

	struct TextureDescriptor : CompileTime::PODStruct
	{
	// variables
		ETexture::type		target;
		uint4				dimension;
		EPixelFormat::type	format;
		EImageUsage::bits	usage;
		MipmapLevel			maxLevel;
		MultiSamples		samples;

	// methods
		TextureDescriptor (GX_DEFCTOR) : target( ETexture::Unknown ), format( EPixelFormat::Unknown )
		{}

		TextureDescriptor  (ETexture::type		target,
							const uint4	&		dimension,
							EPixelFormat::type	format,
							EImageUsage::bits	usage,
							MipmapLevel			maxLevel = Uninitialized,
							MultiSamples		samples = Uninitialized) :
			target(target), dimension(dimension),
			format(format), usage(usage),
			maxLevel(maxLevel), samples(samples)
		{}
	};



	//
	// Texture Utils
	//

	class TextureUtils final
	{
	public:
		static uint4	ValidateDimension (ETexture::type target, const uint4 &dim);
		static uint4	LevelDimension (ETexture::type target, const uint4 &dim, uint level);
		static uint		NumberOfMipmaps (ETexture::type target, const uint4 &dim);
			
		static uint3	ConvertOffset (ETexture::type imgType, const uint4 &offset);
		static uint4	ConvertSize (ETexture::type imgType, const uint3 &size);
		static uint3	ConvertSize (ETexture::type imgType, const uint4 &size);
		
		static BytesU	GetImageSize (EPixelFormat::type format, ETexture::type type, const uint4 &dim, BytesU xAlign, BytesU xyAlign);
		static BytesU	GetImageSize (EPixelFormat::type format, ETexture::type type, const uint3 &size, BytesU xAlign, BytesU xyAlign);
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
		Platforms::TextureDescriptor	descr;
	};


}	// CreateInfo


namespace ModuleMsg
{
	//
	// Texture Descriptor Request
	//
	struct GetGpuTextureDescriptor
	{
		Out< Platforms::TextureDescriptor >		result;
	};

	
	// platform-dependent
	struct GetGpuTextureID;
	struct GetGpuTextureView;


}	// ModuleMsg
}	// Engine
