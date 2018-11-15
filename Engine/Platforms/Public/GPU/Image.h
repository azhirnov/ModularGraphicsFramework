// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Texture sizes for any types:
		- Tex1D:			( width,	0,			0,		0		)
		- Tex1DArray :		( width,	0,			0,		layers	)
		- Tex2D :			( width,	height,		0,		0		)
		- Tex2DArray :		( width,	height,		0,		layers	)
		- Tex3D :			( width,	height,		depth,	0		)
		- TexCube :			( width,	height,		0,		6		)
		- TexCubeArray :	( width,	height,		0,		6*layers)
*/

#pragma once

#include "Engine/Platforms/Public/GPU/MultiSamples.h"
#include "Engine/Platforms/Public/GPU/MipmapLevel.h"
#include "Engine/Platforms/Public/GPU/ImageEnums.h"
#include "Engine/Platforms/Public/GPU/ImageSwizzle.h"
#include "Engine/Platforms/Public/GPU/ImageLayer.h"
#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Public/GPU/MemoryEnums.h"
#include "Engine/Platforms/Public/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Image Description
	//

	struct ImageDescription : CompileTime::PODStruct
	{
	// variables
		EImage::type		imageType;
		uint4				dimension;	// width, height, depth, layers
		EPixelFormat::type	format;
		EImageUsage::bits	usage;
		MipmapLevel			maxLevel;	// 0 - one level
		MultiSamples		samples;	// if > 1 then enabled multisampling

	// methods
		ImageDescription (GX_DEFCTOR) : imageType( EImage::Unknown ), format( EPixelFormat::Unknown )
		{}
		
		explicit
		ImageDescription (EImage::type			imageType,
						  const uint4	&		dimension,
						  EPixelFormat::type	format,
						  EImageUsage::bits		usage,
						  MipmapLevel			maxLevel	= Uninitialized,
						  MultiSamples			samples		= Uninitialized);
		
		DEBUG_ONLY( ND_ String ToString () const; )
	};


	
	//
	// GPU Image View Description
	//

	struct ImageViewDescription
	{
	// variables
		EImage::type			viewType	= EImage::Unknown;
		EPixelFormat::type		format		= EPixelFormat::Unknown;
		MipmapLevel				baseLevel;
		uint					levelCount	= 1;
		ImageLayer				baseLayer;
		uint					layerCount	= 1;
		ImageSwizzle			swizzle;

	// methods
		explicit
		ImageViewDescription (EImage::type			viewType	= EImage::Unknown,
							  EPixelFormat::type	format		= EPixelFormat::Unknown,
							  MipmapLevel			baseLevel	= Uninitialized,
							  uint					levelCount	= 1,
							  ImageLayer			baseLayer	= Uninitialized,
							  uint					layerCount	= 1,
							  Swizzle::type			swizzle		= "RGBA"_Swizzle);

		explicit
		ImageViewDescription (const ImageDescription &descr);

		ND_ bool operator == (const ImageViewDescription &right) const;
		ND_ bool operator >  (const ImageViewDescription &right) const;

		DEBUG_ONLY( ND_ String ToString () const; )
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Image Create Info
	//
	struct GpuImage
	{
	// types
		using ImageDescription	= Platforms::ImageDescription;
		using EGpuMemory		= Platforms::EGpuMemory;
		using EMemoryAccess		= Platforms::EMemoryAccess;

	// variables
		ModulePtr				gpuThread;			// can be null
		ModulePtr				memManager;			// can be null
		ImageDescription		descr;
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		bool					allocMem = true;	// if true then you don't need to attach memory module to image

	// methods
		GpuImage (GX_DEFCTOR) {}
		
		GpuImage (const ImageDescription &descr) : descr{descr}, allocMem{false} {}

		GpuImage (const ImageDescription &descr, EGpuMemory::bits memFlags, EMemoryAccess::bits access = EMemoryAccess::All) :
			descr{descr}, memFlags{memFlags}, access{access}, allocMem{true} {}

		GpuImage (const ImageDescription &descr, const ModulePtr &memMngr, EGpuMemory::bits memFlags, EMemoryAccess::bits access = EMemoryAccess::All) :
			memManager{memMngr}, descr{descr}, memFlags{memFlags}, access{access}, allocMem{true} {}
	};

}	// CreateInfo


namespace GpuMsg
{
	//
	// Set / Get Image Description
	//
	struct GetImageDescription : _MsgBase_
	{
		Out< Platforms::ImageDescription >		result;
	};

	struct SetImageDescription : _MsgBase_
	{
	// variables
		Platforms::ImageDescription				descr;

	// methods
		explicit SetImageDescription (const Platforms::ImageDescription &desc) : descr{desc} {}
	};
	

	//
	// Get Image Memory Layout
	//
	struct GetImageMemoryLayout : _MsgBase_
	{
	// types
		struct MemLayout
		{
			BytesU		offset;		// offset for mipmap level and array layer, may be zero for all levels and layers if not supported
			BytesU		rowPitch;
			BytesU		slicePitch;	// size of array layer or 3D image slice, may be zero for 1D and 2D images
			BytesU		size;
			uint3		dimension;	// dimension of current layer/level
		};
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		MipmapLevel			mipLevel;
		ImageLayer			layer;
		Out< MemLayout >	result;

	// methods
		GetImageMemoryLayout () {}
		explicit GetImageMemoryLayout (MipmapLevel mipLevel, ImageLayer layer) : mipLevel{mipLevel}, layer{layer} {}
	};


}	// GpuMsg
}	// Engine
