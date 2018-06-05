// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Images/ImageConverter.h"

#ifdef GX_ENABLE_DEVIL
#	include "DevIL/include/IL/il.h"
#	include "DevIL/include/IL/ilu.h"
#endif

namespace ResPack
{

#ifdef GX_ENABLE_DEVIL

	using EPixelFormat	= Engine::Platforms::EPixelFormat;

	using namespace GX_STL::GXMath;

/*
=================================================
	InitDevlIL
=================================================
*/
	static bool InitDevlIL ()
	{
		static bool	isDevILInit = false;

		if ( isDevILInit )
			return true;

		isDevILInit = true;
	
		if (ilGetInteger( IL_VERSION_NUM ) < IL_VERSION)
		{
			RETURN_ERR( "Incorrect DevIL.dll version." );
		}
	
		if (iluGetInteger( ILU_VERSION_NUM ) < ILU_VERSION)
		{
			RETURN_ERR( "Incorrect ILU.dll version." );
		}

		ilInit();
		iluInit();

		ilEnable( IL_KEEP_DXTC_DATA );
		ilSetInteger( IL_KEEP_DXTC_DATA, IL_TRUE );
		return true;
	}
	
/*
=================================================
	ConvertDevILFormat
=================================================
*/
	static bool ConvertDevILFormat (ILenum fmt, ILenum type, ILenum dxtc, OUT EPixelFormat::type &outFmt, OUT bool &isCompressed)
	{
		isCompressed = false;
		switch (fmt)
		{
			case IL_RGB : {
				switch (type)
				{
					case IL_UNSIGNED_BYTE :		outFmt = EPixelFormat::RGB8_UNorm;		return true;
					case IL_BYTE :				outFmt = EPixelFormat::RGB8_SNorm;		return true;
					case IL_UNSIGNED_SHORT :	outFmt = EPixelFormat::RGB16_UNorm;		return true;
					case IL_SHORT :				outFmt = EPixelFormat::RGB16_SNorm;		return true;
					case IL_HALF :				outFmt = EPixelFormat::RGB16F;			return true;
					case IL_FLOAT :				outFmt = EPixelFormat::RGB32F;			return true;
					default :					RETURN_ERR( "unsupported format" );
				}
				break;
			}
			case IL_RGBA : {
				switch (type)
				{
					case IL_UNSIGNED_BYTE : 	outFmt = EPixelFormat::RGBA8_UNorm;		return true;
					case IL_BYTE :				outFmt = EPixelFormat::RGBA8_SNorm;		return true;
					case IL_UNSIGNED_SHORT :	outFmt = EPixelFormat::RGBA16_UNorm;	return true;
					case IL_SHORT :				outFmt = EPixelFormat::RGBA16_SNorm;	return true;
					case IL_HALF :				outFmt = EPixelFormat::RGBA16F;			return true;
					case IL_FLOAT :				outFmt = EPixelFormat::RGBA32F;			return true;
					default :					RETURN_ERR( "unsupported format" );
				}
				break;
			}
			case IL_ALPHA :
			case IL_LUMINANCE : {
				switch (type)
				{
					case IL_UNSIGNED_BYTE : 	outFmt = EPixelFormat::R8_UNorm;		return true;
					case IL_BYTE :				outFmt = EPixelFormat::R8_SNorm;		return true;
					case IL_UNSIGNED_SHORT :	outFmt = EPixelFormat::R16_UNorm;		return true;
					case IL_SHORT :				outFmt = EPixelFormat::R16_SNorm;		return true;
					case IL_HALF :				outFmt = EPixelFormat::R16F;			return true;
					case IL_FLOAT :				outFmt = EPixelFormat::R32F;			return true;
					default :					RETURN_ERR( "unsupported format" );
				}
				break;
			}
			case IL_LUMINANCE_ALPHA : {
				switch (type)
				{
					case IL_UNSIGNED_BYTE : 	outFmt = EPixelFormat::RG8_UNorm;		return true;
					case IL_BYTE :				outFmt = EPixelFormat::RG8_SNorm;		return true;
					case IL_UNSIGNED_SHORT :	outFmt = EPixelFormat::RG16_UNorm;		return true;
					case IL_SHORT :				outFmt = EPixelFormat::RG16_SNorm;		return true;
					case IL_HALF :				outFmt = EPixelFormat::RG16F;			return true;
					case IL_FLOAT :				outFmt = EPixelFormat::RG32F;			return true;
					default :					RETURN_ERR( "unsupported format" );
				}
				break;
			}
		}

		switch ( dxtc )
		{
			case IL_DXT_NO_COMP	:	break;
			case IL_DXT1		:	outFmt = EPixelFormat::BC1_RGB8_UNorm;		isCompressed = true;	return true;
			case IL_DXT1A		:	outFmt = EPixelFormat::BC1_RGB8_A1_UNorm;	isCompressed = true;	return true;
			case IL_DXT3		:	outFmt = EPixelFormat::BC2_RGBA8_UNorm;		isCompressed = true;	return true;
			case IL_DXT5		:	outFmt = EPixelFormat::BC3_RGBA8_UNorm;		isCompressed = true;	return true;
			default				:	RETURN_ERR( "unsupported DXT format" );
		}

		RETURN_ERR( "unknown format" )
	}
	
/*
=================================================
	LoadFromDevIL
=================================================
*/
	static bool LoadFromDevIL (OUT ImageConverter::ImageData::MipLevel &mipmap, OUT BinaryArray &pixels)
	{
		const ILint			fmt				= ilGetInteger( IL_IMAGE_FORMAT );
		const ILint			type			= ilGetInteger( IL_IMAGE_TYPE );
		const ILint			dxtc			= ilGetInteger( IL_DXTC_DATA_FORMAT );
		const ILint			width			= Max( 1, ilGetInteger(IL_IMAGE_WIDTH) );
		const ILint			height			= Max( 1, ilGetInteger(IL_IMAGE_HEIGHT) );
		const ILint			depth			= Max( 1, ilGetInteger(IL_IMAGE_DEPTH) );
		const BitsU			bpp				= BitsU(ilGetInteger( IL_IMAGE_BITS_PER_PIXEL ));
		const BytesU		calc_data_size	= BytesU(width * height * depth * bpp);

		EPixelFormat::type	format			= EPixelFormat::Unknown;
		bool				is_compressed	= false;

		CHECK_ERR( ConvertDevILFormat( fmt, type, dxtc, OUT format, OUT is_compressed ) );

		if ( dxtc != IL_DXT_NO_COMP )
		{
			ILuint	dxt_size = ilGetDXTCData( nullptr, 0, dxtc );
			pixels.Resize( dxt_size, false );
			ilGetDXTCData( pixels.ptr(), dxt_size, dxtc );
		}
		else
		{
			usize	data_size = Max( 0, ilGetInteger(IL_IMAGE_SIZE_OF_DATA) );
			CHECK_ERR( data_size == calc_data_size );

			pixels.Resize( data_size, false );
			UnsafeMem::MemCopy( pixels.ptr(), ilGetData(), BytesU(data_size) );
		}

		mipmap.dimension	= uint3(width, height, depth);
		mipmap.rowPitch		= Bytes<uint>(width * bpp);
		mipmap.slicePitch	= Bytes<uint>(width * height * bpp);
		mipmap.dataPtr		= pixels.RawPtr();
		mipmap.pixelFormat	= format;

		return true;
	}

/*
=================================================
	_DevIL_Load
=================================================
*/
	bool ImageConverter::_DevIL_Load (StringCRef filename, OUT ImageData &image)
	{
		using Level_t = ImageConverter::ImageData::MipLevel;

		InitDevlIL();
		CHECK_ERR( ilLoadImage( filename.cstr() ) == IL_TRUE );

		StringCRef	ext = FileAddress::GetExtension( filename );

		if ( ext.EqualsIC( "jpg" ) or ext.EqualsIC( "jpeg" ) or ext.EqualsIC( "png" ) )
		{
			iluFlipImage();
		}
		
		ilActiveImage( 0 );

		//const ILint	num_faces	= ilGetInteger( IL_NUM_FACES );
		
		//for (int layer = 0, num_layers = ilGetInteger(IL_NUM_LAYERS);
		//		layer < num_layers; ++layer)
		{
			//CHECK( ilActiveLayer( layer ) == IL_TRUE );
			int layer = 0;

			for (int mm = 0, num_mipmaps = ilGetInteger(IL_NUM_MIPMAPS)+1;
				 mm < num_mipmaps; ++mm)
			{
				CHECK( ilActiveMipmap( mm ) == IL_TRUE );

				Level_t		mipmap;
				BinaryArray	pixels;
				CHECK_ERR( LoadFromDevIL( OUT mipmap, OUT pixels ) );

				mipmap.level	= mm;
				mipmap.layer	= layer;

				if ( usize(mm) >= image.mipmaps.Count() )
					image.mipmaps.Resize( usize(mm)+1 );

				if ( usize(layer) >= image.mipmaps[mm].Count() )
					image.mipmaps[mm].Resize( usize(layer)+1 );

				auto&	curr_mm = image.mipmaps[mm][layer];

				CHECK( IsZero( curr_mm.dimension ) );

				curr_mm = mipmap;
				image.data.PushBack( RVREF(pixels) );
			}
		}
		return true;
	}
	
#else

	bool ImageConverter::_DevIL_Load (StringCRef filename, OUT ImageData &image)
	{
		return true;
	}

#endif	// GX_ENABLE_DEVIL

}	// ResPack
