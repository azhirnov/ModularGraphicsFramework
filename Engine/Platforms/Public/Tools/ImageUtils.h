// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Image.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Engine::Platforms;
	

	//
	// Image Utils
	//

	class ImageUtils final
	{
	public:
		ND_ static ulong4	ValidateDimension (EImage::type imageType, const ulong4 &dim);
		ND_ static uint4	ValidateDimension (EImage::type imageType, const uint4 &dim);

		ND_ static ulong3	LevelDimension (EImage::type imageType, const ulong3 &dim, uint level);
		ND_ static uint3	LevelDimension (EImage::type imageType, const uint3 &dim, uint level);
		ND_ static ulong4	LevelDimension (EImage::type imageType, const ulong4 &dim, uint level);
		ND_ static uint4	LevelDimension (EImage::type imageType, const uint4 &dim, uint level);

		ND_ static uint		NumberOfMipmaps (EImage::type imageType, const ulong4 &dim);
		ND_ static uint		NumberOfMipmaps (EImage::type imageType, const uint4 &dim);
			
		ND_ static ulong3	ConvertOffset (EImage::type imgType, const ulong4 &offset);
		ND_ static uint3	ConvertOffset (EImage::type imgType, const uint4 &offset);

		ND_ static ulong4	ConvertSize (EImage::type imgType, const ulong3 &size);
		ND_ static uint4	ConvertSize (EImage::type imgType, const uint3 &size);

		ND_ static ulong3	ConvertSize (EImage::type imgType, const ulong4 &size);
		ND_ static uint3	ConvertSize (EImage::type imgType, const uint4 &size);
		
		ND_ static BytesUL	GetImageSize (EPixelFormat::type format, EImage::type type, const ulong4 &dim, BytesU xAlign, BytesU xyAlign);
		ND_ static BytesU	GetImageSize (EPixelFormat::type format, EImage::type type, const uint4 &dim, BytesU xAlign, BytesU xyAlign);

		ND_ static BytesUL	GetImageSize (EPixelFormat::type format, EImage::type type, const ulong3 &size, BytesU xAlign, BytesU xyAlign);
		ND_ static BytesU	GetImageSize (EPixelFormat::type format, EImage::type type, const uint3 &size, BytesU xAlign, BytesU xyAlign);
		
		ND_ static bool		IsValidDescriptor (const ImageDescriptor &descr);
			static void		ValidateDescriptor (INOUT ImageDescriptor &descr);
	};


}	// PlatformTools
}	// Engine
