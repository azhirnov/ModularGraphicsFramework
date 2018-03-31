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
	// GPU Image Descriptor
	//

	struct ImageDescriptor : CompileTime::PODStruct
	{
	// variables
		EImage::type		imageType;
		uint4				dimension;	// width, height, depth, layers
		EPixelFormat::type	format;
		EImageUsage::bits	usage;
		MipmapLevel			maxLevel;	// 0 - one level
		MultiSamples		samples;	// if > 1 then enabled multisampling

	// methods
		ImageDescriptor (GX_DEFCTOR) : imageType( EImage::Unknown ), format( EPixelFormat::Unknown )
		{}
		
		explicit
		ImageDescriptor  (EImage::type			imageType,
						  const uint4	&		dimension,
						  EPixelFormat::type	format,
						  EImageUsage::bits		usage,
						  MipmapLevel			maxLevel	= Uninitialized,
						  MultiSamples			samples		= Uninitialized);
	};


	
	//
	// GPU Image View Descriptor
	//

	struct ImageViewDescriptor
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
		ImageViewDescriptor (EImage::type		viewType	= EImage::Unknown,
							 EPixelFormat::type	format		= EPixelFormat::Unknown,
							 MipmapLevel		baseLevel	= Uninitialized,
							 uint				levelCount	= 1,
							 ImageLayer			baseLayer	= Uninitialized,
							 uint				layerCount	= 1,
							 Swizzle::type		swizzle		= "RGBA"_Swizzle);

		explicit
		ImageViewDescriptor (const ImageDescriptor &descr);

		bool operator == (const ImageViewDescriptor &right) const;
		bool operator >  (const ImageViewDescriptor &right) const;
		bool operator <  (const ImageViewDescriptor &right) const;
	};



	//
	// Image Utils
	//

	class ImageUtils final
	{
	public:
		static ulong4	ValidateDimension (EImage::type imageType, const ulong4 &dim);
		static uint4	ValidateDimension (EImage::type imageType, const uint4 &dim);

		static ulong3	LevelDimension (EImage::type imageType, const ulong3 &dim, uint level);
		static uint3	LevelDimension (EImage::type imageType, const uint3 &dim, uint level);
		static ulong4	LevelDimension (EImage::type imageType, const ulong4 &dim, uint level);
		static uint4	LevelDimension (EImage::type imageType, const uint4 &dim, uint level);

		static uint		NumberOfMipmaps (EImage::type imageType, const ulong4 &dim);
		static uint		NumberOfMipmaps (EImage::type imageType, const uint4 &dim);
			
		static ulong3	ConvertOffset (EImage::type imgType, const ulong4 &offset);
		static uint3	ConvertOffset (EImage::type imgType, const uint4 &offset);

		static ulong4	ConvertSize (EImage::type imgType, const ulong3 &size);
		static uint4	ConvertSize (EImage::type imgType, const uint3 &size);

		static ulong3	ConvertSize (EImage::type imgType, const ulong4 &size);
		static uint3	ConvertSize (EImage::type imgType, const uint4 &size);
		
		static BytesUL	GetImageSize (EPixelFormat::type format, EImage::type type, const ulong4 &dim, BytesU xAlign, BytesU xyAlign);
		static BytesU	GetImageSize (EPixelFormat::type format, EImage::type type, const uint4 &dim, BytesU xAlign, BytesU xyAlign);

		static BytesUL	GetImageSize (EPixelFormat::type format, EImage::type type, const ulong3 &size, BytesU xAlign, BytesU xyAlign);
		static BytesU	GetImageSize (EPixelFormat::type format, EImage::type type, const uint3 &size, BytesU xAlign, BytesU xyAlign);
		
		static void		ValidateDescriptor (INOUT ImageDescriptor &descr);
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Image Create Info
	//
	struct GpuImage // TODO: rename
	{
	// types
		using ImageDescriptor	= Platforms::ImageDescriptor;
		using EGpuMemory		= Platforms::EGpuMemory;
		using EMemoryAccess		= Platforms::EMemoryAccess;

	// variables
		ModulePtr				gpuThread;			// can be null
		ModulePtr				memManager;			// can be null
		ImageDescriptor			descr;
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		bool					allocMem = true;	// if true then you don't need to attach memory module to image

	// methods
		GpuImage (GX_DEFCTOR) {}
		
		GpuImage (const ImageDescriptor &descr) : descr{descr}, allocMem{false} {}

		GpuImage (const ImageDescriptor &descr, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			descr{descr}, memFlags{memFlags}, access{access}, allocMem{true} {}

		GpuImage (const ImageDescriptor &descr, const ModulePtr &memMngr, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			memManager{memMngr}, descr{descr}, memFlags{memFlags}, access{access}, allocMem{true} {}
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

	struct SetImageDescriptor
	{
		Platforms::ImageDescriptor				descr;
	};
	

	//
	// Get Image Memory Layout
	//
	struct GetImageMemoryLayout
	{
	// types
		struct MemLayout
		{
			BytesUL		offset;		// offset for mipmap level and array layer, may be zero for all levels and layers if not supported
			BytesUL		rowPitch;
			BytesUL		slicePitch;	// size of array layer or 3D image slice
			BytesUL		size;
			uint3		dimension;	// dimension of current 
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


	//
	// Set/Get Image Layout
	//
	/*struct SetImageLayout
	{
		Platforms::EImageLayout::type			newLayout;		// TODO: layout can be changed with barrier operation
	};

	struct GetImageLayout
	{
		Out< Platforms::EImageLayout::type >	result;
	};*/


}	// GpuMsg
}	// Engine
