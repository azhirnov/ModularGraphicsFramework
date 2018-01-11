// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	constructor
=================================================
*/
	ImageDescriptor::ImageDescriptor (EImage::type			imageType,
									  const uint4	&		dimension,
									  EPixelFormat::type	format,
									  EImageUsage::bits		usage,
									  MipmapLevel			maxLevel,
									  MultiSamples			samples) :
		imageType(imageType),	dimension(dimension),
		format(format),			usage(usage),
		maxLevel(maxLevel),		samples(samples)
	{}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	ImageViewDescriptor::ImageViewDescriptor (EImage::type			viewType,
											  EPixelFormat::type	format,
											  MipmapLevel			baseLevel,
											  uint					levelCount,
											  ImageLayer			baseLayer,
											  uint					layerCount,
											  Swizzle::type			swizzle) :
		viewType{viewType},		format{format},
		baseLevel{baseLevel},	levelCount{levelCount},
		baseLayer{baseLayer},	layerCount{layerCount},
		swizzle{swizzle}
	{}

/*
=================================================
	compare operators
=================================================
*/
	bool ImageViewDescriptor::operator == (const ImageViewDescriptor &right) const
	{
		return	this->viewType		== right.viewType	and
				this->format		== right.format		and
				this->baseLevel		== right.baseLevel	and
				this->levelCount	== right.levelCount	and
				this->baseLayer		== right.baseLayer	and
				this->layerCount	== right.layerCount	and
				this->swizzle		== right.swizzle;
	}
	
	bool ImageViewDescriptor::operator >  (const ImageViewDescriptor &right) const
	{
		return	viewType	!= right.viewType	?	viewType	> right.viewType	:
				format		!= right.format		?	format		> right.format		:
				baseLevel	!= right.baseLevel	?	baseLevel	> right.baseLevel	:
				levelCount	!= right.levelCount	?	levelCount	> right.levelCount	:
				baseLayer	!= right.baseLayer	?	baseLayer	> right.baseLayer	:
				layerCount	!= right.layerCount	?	layerCount	> right.layerCount	:
													swizzle		> right.swizzle;
	}
	
	bool ImageViewDescriptor::operator <  (const ImageViewDescriptor &right) const
	{
		return	viewType	!= right.viewType	?	viewType	< right.viewType	:
				format		!= right.format		?	format		< right.format		:
				baseLevel	!= right.baseLevel	?	baseLevel	< right.baseLevel	:
				levelCount	!= right.levelCount	?	levelCount	< right.levelCount	:
				baseLayer	!= right.baseLayer	?	baseLayer	< right.baseLayer	:
				layerCount	!= right.layerCount	?	layerCount	< right.layerCount	:
													swizzle		< right.swizzle;
	}
//-----------------------------------------------------------------------------


/*
=================================================
	ConvertOffset
=================================================
*/
	uint3  ImageUtils::ConvertOffset (EImage::type imgType, const uint4 &offset)
	{
		switch ( imgType )
		{
			case EImage::Tex2D		:	return offset.xyo();
			case EImage::Tex3D		:	return offset.xyz();
			case EImage::Buffer		:	return offset.xoo();
			case EImage::Tex1D		:	return offset.xoo();
			case EImage::Tex1DArray	:	return offset.xwo();
			case EImage::Tex2DArray	:	return offset.xyw();
		}
		WARNING( "unsupported image type!" );
		return uint3();
	}
	
/*
=================================================
	ConvertSize
=================================================
*/
	uint4  ImageUtils::ConvertSize (EImage::type imgType, const uint3 &size)
	{
		uint4	res;

		switch ( imgType )
		{
			case EImage::Tex2D		:	res = size.xyoo();	break;
			case EImage::Tex3D		:	res = size.xyzo();	break;
			case EImage::Buffer		:	res = size.xooo();	break;
			case EImage::Tex1D		:	res = size.xooo();	break;
			case EImage::Tex1DArray	:	res = size.xooy();	break;
			case EImage::Tex2DArray	:	res = size.xyoz();	break;
			default						:	res = size.xyzo();	WARNING( "unsupported image type!" );
		}
		
		return res;
	}
	
/*
=================================================
	ConvertSize
=================================================
*/
	uint3  ImageUtils::ConvertSize (EImage::type imgType, const uint4 &size)
	{
		uint3	res;

		switch( imgType )
		{
			case EImage::Tex2D		:	res = size.xyo();	break;
			case EImage::Tex3D		:	res = size.xyz();	break;
			case EImage::Buffer		:	res = size.xoo();	break;
			case EImage::Tex1D		:	res = size.xoo();	break;
			case EImage::Tex1DArray	:	res = size.xyo();	break;
			case EImage::Tex2DArray	:	res = size.xyz();	break;
			default						:	res = size.xyz();	WARNING( "unsupported image type!" );
		}

		return res;
	}
	
/*
=================================================
	ValidateDimension
=================================================
*/
	uint4  ImageUtils::ValidateDimension (EImage::type imageType, const uint4 &dim)
	{
		switch ( imageType )
		{
			case EImage::Buffer :
			case EImage::Tex1D :
			{
				ASSERT( IsNotZero( dim.x ) );
				ASSERT( All( dim.yzw() <= 1 ) );
				return uint4( dim.x, 0, 0, 0 );
			}	
			case EImage::Tex2DMSArray :
			case EImage::Tex2DMS :
			case EImage::Tex2D :
			{
				ASSERT( IsNotZero( dim.xy() ) );
				ASSERT( All( dim.zw() <= 1 ) );
				return uint4( dim.xy(), 0, 0 );
			}
			case EImage::TexCube :
			{
				ASSERT( IsNotZero( dim.xy() ) );
				ASSERT( dim.z == 6 and dim.w <= 1 );
				return uint4( dim.xy(), 6, 0 );
			}
			case EImage::Tex2DArray :
			{
				ASSERT( IsNotZero( dim.xyw() ) );
				ASSERT( dim.z <= 1 );
				return uint4( dim.xy(), 0, dim.w );
			}
			case EImage::Tex3D :
			{
				ASSERT( IsNotZero( dim.xyz() ) );
				ASSERT( dim.w <= 1 );
				return uint4( dim.xyz(), 0 );
			}
			case EImage::TexCubeArray :
			{
				ASSERT( IsNotZero( dim.xyw() ) );
				ASSERT( dim.z == 6 );
				return uint4( dim.xy(), 6, dim.w );
			}
		}

		RETURN_ERR( "invalid texture type" );
	}
	
/*
=================================================
	LevelDimension
=================================================
*/
	uint4 ImageUtils::LevelDimension (EImage::type imageType, const uint4 &dim, uint level)
	{
		ASSERT( level == 0 or (imageType == EImage::Tex2DMS or imageType == EImage::Tex2DMSArray) );

		switch ( imageType )
		{
			case EImage::Buffer :
			case EImage::Tex1D :			return Max( uint4( dim.x >> level, 0, 0, 0 ), uint4(1) );

			case EImage::Tex2D :			return Max( uint4( dim.xy() >> level, 0, 0 ), uint4(1) );

			case EImage::TexCube :			return Max( uint4( dim.xy() >> level, 6, 0 ), uint4(1) );

			case EImage::Tex2DArray :		return Max( uint4( dim.xy() >> level, 0, dim.w ), uint4(1) );

			case EImage::Tex3D :			return Max( uint4( dim.xyz() >> level, 0 ), uint4(1) );

			case EImage::TexCubeArray :		return Max( uint4( dim.xy() >> level, 6, dim.w ), uint4(1) );

			//case EImage::Tex2DMSArray :
			//case EImage::Tex2DMS :	// not supported for multisampled texture
		}

		RETURN_ERR( "invalid texture type" );
	}
		
/*
=================================================
	NumberOfMipmaps
=================================================
*/	
	uint ImageUtils::NumberOfMipmaps (EImage::type imageType, const uint4 &dim)
	{
		switch ( imageType )
		{
			case EImage::Buffer :
			case EImage::Tex2DMS :
			case EImage::Tex2DMSArray :		return 1;

			case EImage::Tex1D :
			case EImage::Tex2D :
			case EImage::Tex3D :			return GXMath::ImageUtils::GetNumberOfMipmaps( dim.Max() ) + 1;

			case EImage::TexCube :
			case EImage::TexCubeArray :
			case EImage::Tex2DArray :		return GXMath::ImageUtils::GetNumberOfMipmaps( dim.xy().Max() ) + 1;
		}

		RETURN_ERR( "invalid texture type", uint(1) );
	}

/*
=================================================
	GetImageSize
=================================================
*/
	BytesU ImageUtils::GetImageSize (EPixelFormat::type format, EImage::type, const uint3 &size, BytesU xAlign, BytesU xyAlign)
	{
		CHECK_ERR( not EPixelFormat::IsCompressed( format ), BytesU() );

		BytesU			bpp = BytesU( EPixelFormat::BitPerPixel( format ) );
		uint3 const		dim	= Max( size, uint3(1) );

		return GXMath::ImageUtils::AlignedDataSize( dim, bpp, xAlign, xyAlign );
	}

	BytesU ImageUtils::GetImageSize (EPixelFormat::type format, EImage::type type, const uint4 &dim, BytesU xAlign, BytesU xyAlign)
	{
		return GetImageSize( format, type, ConvertSize( type, dim ), xAlign, xyAlign );
	}
	
/*
=================================================
	ValidateDescriptor
=================================================
*/
	void ImageUtils::ValidateDescriptor (INOUT ImageDescriptor &descr)
	{
		descr.dimension = ValidateDimension( descr.imageType, descr.dimension );
		descr.dimension = Max( descr.dimension, uint4(1) );

		if ( EImage::IsMultisampled( descr.imageType ) )
		{
			ASSERT( descr.samples > MultiSamples(1) );
			ASSERT( descr.maxLevel == MipmapLevel(1) );
			descr.maxLevel = MipmapLevel(1);
		}
		else
		{
			ASSERT( descr.samples <= MultiSamples(1) );
			descr.samples = MultiSamples(1);
			descr.maxLevel = MipmapLevel( Clamp( descr.maxLevel.Get(), 1u, NumberOfMipmaps( descr.imageType, descr.dimension ) ));
		}
	}


}	// Platforms
}	// Engine
