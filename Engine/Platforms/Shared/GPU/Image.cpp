// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"

namespace Engine
{
namespace Platforms
{
	
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
	uint4  ImageUtils::ValidateDimension (EImage::type target, const uint4 &dim)
	{
		switch ( target )
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
	uint4 ImageUtils::LevelDimension (EImage::type target, const uint4 &dim, uint level)
	{
		ASSERT( level == 0 or (target == EImage::Tex2DMS or target == EImage::Tex2DMSArray) );

		switch ( target )
		{
			case EImage::Buffer :
			case EImage::Tex1D :			return uint4( Max( dim.x >> level, 1u ), 0, 0, 0 );

			case EImage::Tex2D :			return uint4( Max( dim.xy() >> level, uint2(1) ), 0, 0 );

			case EImage::TexCube :			return uint4( Max( dim.xy() >> level, uint2(1) ), 6, 0 );

			case EImage::Tex2DArray :		return uint4( Max( dim.xy() >> level, uint2(1) ), 0, dim.w );

			case EImage::Tex3D :			return uint4( Max( dim.xyz() >> level, uint3(1) ), 0 );

			case EImage::TexCubeArray :		return uint4( Max( dim.xy() >> level, uint2(1) ), 6, dim.w );

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
	uint ImageUtils::NumberOfMipmaps (EImage::type target, const uint4 &dim)
	{
		switch ( target )
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
	BytesU ImageUtils::GetImageSize (EPixelFormat::type format, EImage::type type, const uint3 &size, BytesU xAlign, BytesU xyAlign)
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
		descr.dimension = ValidateDimension( descr.target, descr.dimension );
		descr.dimension = Max( descr.dimension, uint4(1) );

		if ( EImage::IsMultisampled( descr.target ) )
		{
			ASSERT( descr.samples > MultiSamples(1) );
			ASSERT( descr.maxLevel == MipmapLevel(1) );
			descr.maxLevel = MipmapLevel(1);
		}
		else
		{
			ASSERT( descr.samples <= MultiSamples(1) );
			descr.samples = MultiSamples(1);
			descr.maxLevel = MipmapLevel( Max( descr.maxLevel.Get(), 1u ) );
		}
	}


}	// Platforms
}	// Engine
