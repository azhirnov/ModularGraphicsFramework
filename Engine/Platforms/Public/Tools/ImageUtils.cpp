// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/Tools/ImageUtils.h"
#include "Core/STL/Math/Image/ImageUtils.h"

namespace Engine
{
namespace PlatformTools
{

/*
=================================================
	ConvertOffset
=================================================
*/
	template <typename T>
	inline Vec<T,3>  ImageUtils_ConvertOffset (EImage::type imgType, const Vec<T,4> &offset)
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
		return Uninitialized;
	}
	
	ulong3  ImageUtils::ConvertOffset (EImage::type imgType, const ulong4 &offset)	{ return ImageUtils_ConvertOffset( imgType, offset ); }
	uint3   ImageUtils::ConvertOffset (EImage::type imgType, const uint4 &offset)	{ return ImageUtils_ConvertOffset( imgType, offset ); }

/*
=================================================
	ConvertSize
=================================================
*/
	template <typename T>
	inline Vec<T,4>  ImageUtils_ConvertSize (EImage::type imgType, const Vec<T,3> &size)
	{
		Vec<T,4>	res;

		switch ( imgType )
		{
			case EImage::TexCube	:	res = size.xyoo();	break;
			case EImage::Tex2D		:	res = size.xyoo();	break;
			case EImage::Tex3D		:	res = size.xyzo();	break;
			case EImage::Buffer		:	res = size.xooo();	break;
			case EImage::Tex1D		:	res = size.xooo();	break;
			case EImage::Tex1DArray	:	res = size.xooy();	break;
			case EImage::Tex2DArray	:	res = size.xyoz();	break;
			default					:	res = size.xyzo();	WARNING( "unsupported image type!" );
		}
		
		return res;
	}
	
	ulong4  ImageUtils::ConvertSize (EImage::type imgType, const ulong3 &size)		{ return ImageUtils_ConvertSize( imgType, size ); }
	uint4   ImageUtils::ConvertSize (EImage::type imgType, const uint3 &size)		{ return ImageUtils_ConvertSize( imgType, size ); }

/*
=================================================
	ConvertSize
=================================================
*/
	template <typename T>
	inline Vec<T,3>  ImageUtils_ConvertSize (EImage::type imgType, const Vec<T,4> &size)
	{
		Vec<T,3>	res;

		switch( imgType )
		{
			case EImage::Tex2D		:	res = size.xyo();	break;
			case EImage::Tex3D		:	res = size.xyz();	break;
			case EImage::Buffer		:	res = size.xoo();	break;
			case EImage::Tex1D		:	res = size.xoo();	break;
			case EImage::Tex1DArray	:	res = size.xyo();	break;
			case EImage::Tex2DArray	:	res = size.xyz();	break;
			default					:	res = size.xyz();	WARNING( "unsupported image type!" );
		}

		return res;
	}
	
	ulong3  ImageUtils::ConvertSize (EImage::type imgType, const ulong4 &size)		{ return ImageUtils_ConvertSize( imgType, size ); }
	uint3   ImageUtils::ConvertSize (EImage::type imgType, const uint4 &size)		{ return ImageUtils_ConvertSize( imgType, size ); }

/*
=================================================
	ValidateDimension
=================================================
*/
	template <typename T>
	inline Vec<T,4>  ImageUtils_ValidateDimension (EImage::type imageType, const Vec<T,4> &dim)
	{
		switch ( imageType )
		{
			case EImage::Buffer :
			case EImage::Tex1D :
			{
				ASSERT( IsNotZero( dim.x ) );
				ASSERT( All( dim.yzw() <= 1 ) );
				return Vec<T,4>( dim.x, 0, 0, 0 );
			}	
			case EImage::Tex2DMSArray :
			case EImage::Tex2DMS :
			case EImage::Tex2D :
			{
				ASSERT( IsNotZero( dim.xy() ) );
				ASSERT( All( dim.zw() <= 1 ) );
				return Vec<T,4>( dim.xy(), 0, 0 );
			}
			case EImage::TexCube :
			{
				ASSERT( IsNotZero( dim.xy() ) );
				ASSERT( dim.z == 6 and dim.w <= 1 );
				return Vec<T,4>( dim.xy(), 6, 0 );
			}
			case EImage::Tex1DArray :
			{
				ASSERT( IsNotZero( dim.xw() ) );
			}
			case EImage::Tex2DArray :
			{
				ASSERT( IsNotZero( dim.xyw() ) );
				ASSERT( dim.z <= 1 );
				return Vec<T,4>( dim.xy(), 0, dim.w );
			}
			case EImage::Tex3D :
			{
				ASSERT( IsNotZero( dim.xyz() ) );
				ASSERT( dim.w <= 1 );
				return Vec<T,4>( dim.xyz(), 0 );
			}
			case EImage::TexCubeArray :
			{
				ASSERT( IsNotZero( dim.xyw() ) );
				ASSERT( dim.z == 6 );
				return Vec<T,4>( dim.xy(), 6, dim.w );
			}
		}

		RETURN_ERR( "invalid texture type" );
	}
	
	ulong4  ImageUtils::ValidateDimension (EImage::type imageType, const ulong4 &dim)	{ return ImageUtils_ValidateDimension( imageType, dim ); }
	uint4   ImageUtils::ValidateDimension (EImage::type imageType, const uint4 &dim)	{ return ImageUtils_ValidateDimension( imageType, dim ); }

/*
=================================================
	LevelDimension
=================================================
*/
	template <typename T>
	inline Vec<T,4> ImageUtils_LevelDimension (EImage::type imageType, const Vec<T,4> &dim, const uint level)
	{
		ASSERT( level == 0 or (imageType == EImage::Tex2DMS or imageType == EImage::Tex2DMSArray) );

		switch ( imageType )
		{
			case EImage::Buffer :
			case EImage::Tex1D :
				ASSERT( All( dim.yzw() <= 1 ) and dim.x > 0 );
				return Vec<T,4>( Max( dim.x >> level, 1u ), 0, 0, 0 );

			case EImage::Tex2D :
				ASSERT( All( dim.zw() <= 1 ) and All( dim.xy() > 0 ) );
				return Vec<T,4>( Max( dim.xy() >> level, 1u ), 0, 0 );

			case EImage::TexCube :
				ASSERT( All( dim.yz() > 0 ) and dim.z <= 1 and dim.w == 6 );
				return Vec<T,4>( Max( dim.xy() >> level, 1u ), 0, 6 );

			case EImage::Tex2DArray :
				ASSERT( All( dim.xyw() > 0 ) and dim.z <= 1 );
				return Vec<T,4>( Max( dim.xy() >> level, 1u ), 0, dim.w );

			case EImage::Tex3D :
				ASSERT( All( dim.xyz() > 0 ) and dim.w <= 1 );
				return Vec<T,4>( Max( dim.xyz() >> level, 1u ), 0 );

			case EImage::TexCubeArray :
				ASSERT( All( dim.xyw() > 0 ) and dim.z <= 1 and (dim.w % 6 == 0) );
				return Vec<T,4>( Max( dim.xy() >> level, 1u ), 0, dim.w );

			//case EImage::Tex2DMSArray :
			//case EImage::Tex2DMS :	// not supported for multisampled texture
		}

		RETURN_ERR( "invalid texture type" );
	}
		
	ulong4 ImageUtils::LevelDimension (EImage::type imageType, const ulong4 &dim, uint level)	{ return ImageUtils_LevelDimension( imageType, dim, level ); }
	uint4  ImageUtils::LevelDimension (EImage::type imageType, const uint4 &dim, uint level)	{ return ImageUtils_LevelDimension( imageType, dim, level ); }

	ulong3 ImageUtils::LevelDimension (EImage::type imageType, const ulong3 &dim, uint level)	{ return LevelDimension( imageType, ulong4(dim), level ).xyz(); }
	uint3  ImageUtils::LevelDimension (EImage::type imageType, const uint3 &dim, uint level)	{ return LevelDimension( imageType, uint4(dim), level ).xyz(); }

/*
=================================================
	NumberOfMipmaps
=================================================
*/	
	template <typename T>
	inline uint ImageUtils_NumberOfMipmaps (EImage::type imageType, const Vec<T,4> &dim)
	{
		switch ( imageType )
		{
			case EImage::Buffer :
			case EImage::Tex2DMS :
			case EImage::Tex2DMSArray :		return 1;

			case EImage::Tex2D :
			case EImage::Tex3D :			return GXImageUtils::GetNumberOfMipmaps( dim.Max() ) + 1;
				
			case EImage::Tex1D :
			case EImage::Tex1DArray :		return GXImageUtils::GetNumberOfMipmaps( dim.x ) + 1;

			case EImage::TexCube :
			case EImage::TexCubeArray :
			case EImage::Tex2DArray :		return GXImageUtils::GetNumberOfMipmaps( dim.xy().Max() ) + 1;
		}

		RETURN_ERR( "invalid texture type", 1u );
	}
	
	uint ImageUtils::NumberOfMipmaps (EImage::type imageType, const ulong4 &dim)	{ return ImageUtils_NumberOfMipmaps( imageType, dim ); }
	uint ImageUtils::NumberOfMipmaps (EImage::type imageType, const uint4 &dim)		{ return ImageUtils_NumberOfMipmaps( imageType, dim ); }

/*
=================================================
	GetImageSize
=================================================
*/
	BytesUL ImageUtils::GetImageSize (EPixelFormat::type format, EImage::type, const ulong3 &size, BytesU xAlign, BytesU xyAlign)
	{
		CHECK_ERR( not EPixelFormat::IsCompressed( format ) );

		const BytesUL	bpp = BytesUL( EPixelFormat::BitPerPixel( format ) );

		return GXImageUtils::AlignedDataSize( size, bpp, BytesUL(xAlign), BytesUL(xyAlign) );
	}

	BytesUL ImageUtils::GetImageSize (EPixelFormat::type format, EImage::type type, const ulong4 &dim, BytesU xAlign, BytesU xyAlign)
	{
		return GetImageSize( format, type, ConvertSize( type, dim ), xAlign, xyAlign );
	}
	
	BytesU ImageUtils::GetImageSize (EPixelFormat::type format, EImage::type type, const uint3 &size, BytesU xAlign, BytesU xyAlign)
	{
		return BytesU(GetImageSize( format, type, ulong3(size), xAlign, xyAlign ));
	}
	
	BytesU ImageUtils::GetImageSize (EPixelFormat::type format, EImage::type type, const uint4 &dim, BytesU xAlign, BytesU xyAlign)
	{
		return BytesU(GetImageSize( format, type, ulong4(dim), xAlign, xyAlign ));
	}
	
/*
=================================================
	IsValidDescription
=================================================
*/
	bool ImageUtils::IsValidDescription (const ImageDescription &descr)
	{
		return	not (descr.format    == EPixelFormat::Unknown and
					 descr.imageType == EImage::Unknown);
	}

/*
=================================================
	ValidateDescription
=================================================
*/
	void ImageUtils::ValidateDescription (INOUT ImageDescription &descr)
	{
		ASSERT( descr.format != EPixelFormat::Unknown );
		ASSERT( descr.imageType != EImage::Unknown );

		descr.dimension = ValidateDimension( descr.imageType, descr.dimension );
		descr.dimension = Max( descr.dimension, 1u );

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

}	// PlatformTools
}	// Engine
