// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/MultiSamples.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"
#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{
	//
	// GPU Image Descriptor
	//

	struct ImageDescriptor : CompileTime::PODStruct
	{
	// variables
		EImage::type		target;
		uint4				dimension;
		EPixelFormat::type	format;
		EImageUsage::bits	usage;
		MipmapLevel			maxLevel;
		MultiSamples		samples;

	// methods
		ImageDescriptor (GX_DEFCTOR) : target( EImage::Unknown ), format( EPixelFormat::Unknown )
		{}

		ImageDescriptor  (EImage::type			target,
						  const uint4	&		dimension,
						  EPixelFormat::type	format,
						  EImageUsage::bits		usage,
						  MipmapLevel			maxLevel	= Uninitialized,
						  MultiSamples			samples		= Uninitialized) :
			target(target),		dimension(dimension),
			format(format),		usage(usage),
			maxLevel(maxLevel),	samples(samples)
		{}
	};



	//
	// Image Utils
	//

	class ImageUtils final
	{
	public:
		static uint4	ValidateDimension (EImage::type target, const uint4 &dim);
		static uint4	LevelDimension (EImage::type target, const uint4 &dim, uint level);
		static uint		NumberOfMipmaps (EImage::type target, const uint4 &dim);
			
		static uint3	ConvertOffset (EImage::type imgType, const uint4 &offset);
		static uint4	ConvertSize (EImage::type imgType, const uint3 &size);
		static uint3	ConvertSize (EImage::type imgType, const uint4 &size);
		
		static BytesU	GetImageSize (EPixelFormat::type format, EImage::type type, const uint4 &dim, BytesU xAlign, BytesU xyAlign);
		static BytesU	GetImageSize (EPixelFormat::type format, EImage::type type, const uint3 &size, BytesU xAlign, BytesU xyAlign);
		
		static void		ValidateDescriptor (INOUT ImageDescriptor &descr);
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Image Create Info
	//

	struct GpuImage
	{
		ModulePtr						gpuThread;
		Platforms::ImageDescriptor		descr;
		bool							allocMemory		= false;	// set 'true' to automaticaly create memory object
	};


}	// CreateInfo


namespace GpuMsg
{
	//
	// Image Descriptor Request
	//
	struct GetImageDescriptor
	{
		Out< Platforms::ImageDescriptor >		result;
	};


	//
	// Set/Get Image Layout
	//
	struct SetImageLayout
	{
		Platforms::EImageLayout::type			newLayout;		// TODO: layout can be changed with barrier operation
	};

	struct GetImageLayout
	{
		Out< Platforms::EImageLayout::type >	result;
	};

	
	// platform-dependent
	struct GetVkImageID;
	struct GetGLImageID;
	struct GetVkImageView;


}	// GpuMsg
}	// Engine
