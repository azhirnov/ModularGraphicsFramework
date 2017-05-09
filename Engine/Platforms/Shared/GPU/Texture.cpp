// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Texture.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	ConvertOffset
=================================================
*/
	uint3  TextureUtils::ConvertOffset (ETexture::type imgType, const uint4 &offset)
	{
		switch ( imgType )
		{
			case ETexture::Tex2D		:	return offset.xyo();
			case ETexture::Tex3D		:	return offset.xyz();
			case ETexture::Buffer		:	return offset.xoo();
			case ETexture::Tex1D		:	return offset.xoo();
			case ETexture::Tex1DArray	:	return offset.xwo();
			case ETexture::Tex2DArray	:	return offset.xyw();
		}
		WARNING( "unsupported image type!" );
		return uint3();
	}
	
/*
=================================================
	ConvertSize
=================================================
*/
	uint4  TextureUtils::ConvertSize (ETexture::type imgType, const uint3 &size)
	{
		uint4	res;

		switch ( imgType )
		{
			case ETexture::Tex2D		:	res = size.xyoo();	break;
			case ETexture::Tex3D		:	res = size.xyzo();	break;
			case ETexture::Buffer		:	res = size.xooo();	break;
			case ETexture::Tex1D		:	res = size.xooo();	break;
			case ETexture::Tex1DArray	:	res = size.xooy();	break;
			case ETexture::Tex2DArray	:	res = size.xyoz();	break;
			default						:	res = size.xyzo();	WARNING( "unsupported image type!" );
		}
		
		return res;
	}
	
/*
=================================================
	ConvertSize
=================================================
*/
	uint3  TextureUtils::ConvertSize (ETexture::type imgType, const uint4 &size)
	{
		uint3	res;

		switch( imgType )
		{
			case ETexture::Tex2D		:	res = size.xyo();	break;
			case ETexture::Tex3D		:	res = size.xyz();	break;
			case ETexture::Buffer		:	res = size.xoo();	break;
			case ETexture::Tex1D		:	res = size.xoo();	break;
			case ETexture::Tex1DArray	:	res = size.xyo();	break;
			case ETexture::Tex2DArray	:	res = size.xyz();	break;
			default						:	res = size.xyz();	WARNING( "unsupported image type!" );
		}

		return res;
	}
	
/*
=================================================
	ValidateDimension
=================================================
*/
	uint4  TextureUtils::ValidateDimension (ETexture::type target, const uint4 &dim)
	{
		switch ( target )
		{
			case ETexture::Buffer :
			case ETexture::Tex1D :			return uint4( dim.x, 0, 0, 0 );
				
			case ETexture::Tex2DMSArray :
			case ETexture::Tex2DMS :
			case ETexture::Tex2D :			return uint4( dim.xy(), 0, 0 );

			case ETexture::TexCube :		return uint4( dim.xy(), 6, 0 );

			case ETexture::Tex2DArray :		return uint4( dim.xy(), 0, dim.w );

			case ETexture::Tex3D :			return uint4( dim.xyz(), 0 );

			case ETexture::TexCubeArray :	return uint4( dim.xy(), 6, dim.w );
		}

		RETURN_ERR( "invalid texture type" );
	}
	
/*
=================================================
	LevelDimension
=================================================
*/
	uint4 TextureUtils::LevelDimension (ETexture::type target, const uint4 &dim, uint level)
	{
		ASSERT( level == 0 or (target == ETexture::Tex2DMS or target == ETexture::Tex2DMSArray) );

		switch ( target )
		{
			case ETexture::Buffer :
			case ETexture::Tex1D :			return uint4( Max( dim.x >> level, 1u ), 0, 0, 0 );

			case ETexture::Tex2D :			return uint4( Max( dim.xy() >> level, uint2(1) ), 0, 0 );

			case ETexture::TexCube :		return uint4( Max( dim.xy() >> level, uint2(1) ), 6, 0 );

			case ETexture::Tex2DArray :		return uint4( Max( dim.xy() >> level, uint2(1) ), 0, dim.w );

			case ETexture::Tex3D :			return uint4( Max( dim.xyz() >> level, uint3(1) ), 0 );

			case ETexture::TexCubeArray :	return uint4( Max( dim.xy() >> level, uint2(1) ), 6, dim.w );

			//case ETexture::Tex2DMSArray :
			//case ETexture::Tex2DMS :	// not supported for multisampled texture
		}

		RETURN_ERR( "invalid texture type" );
	}
		
/*
=================================================
	NumberOfMipmaps
=================================================
*/	
	uint TextureUtils::NumberOfMipmaps (ETexture::type target, const uint4 &dim)
	{
		switch ( target )
		{
			case ETexture::Buffer :
			case ETexture::Tex2DMS :
			case ETexture::Tex2DMSArray :	return 1;

			case ETexture::Tex1D :
			case ETexture::Tex2D :
			case ETexture::Tex3D :			return ImageUtils::GetNumberOfMipmaps( dim.Max() ) + 1;

			case ETexture::TexCube :
			case ETexture::TexCubeArray :
			case ETexture::Tex2DArray :		return ImageUtils::GetNumberOfMipmaps( dim.xy().Max() ) + 1;
		}

		RETURN_ERR( "invalid texture type", uint(1) );
	}

/*
=================================================
	GetImageSize
=================================================
*/
	BytesU TextureUtils::GetImageSize (EPixelFormat::type format, ETexture::type type, const uint3 &size, BytesU xAlign, BytesU xyAlign)
	{
		CHECK_ERR( not EPixelFormat::IsCompressed( format ), BytesU() );

		BytesU			bpp = BytesU( EPixelFormat::BitPerPixel( format ) );
		uint3 const		dim	= Max( size, uint3(1) );

		return ImageUtils::AlignedDataSize( dim, bpp, xAlign, xyAlign );
	}

	BytesU TextureUtils::GetImageSize (EPixelFormat::type format, ETexture::type type, const uint4 &dim, BytesU xAlign, BytesU xyAlign)
	{
		return GetImageSize( format, type, ConvertSize( type, dim ), xAlign, xyAlign );
	}


}	// Platforms
}	// Engine
