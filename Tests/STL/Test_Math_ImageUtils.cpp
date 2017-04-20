// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;
using namespace GX_STL::GXMath::ColorFormat;


static void Test_AlignedImageSize ()
{
	BytesU	size;
	
	size = ImageUtils::AlignedDataSize( uint3( 2, 2, 1 ), BitsU(8).ToBytes(), BytesU(1) );
	ASSERT( size == 2*2 );

	size = ImageUtils::AlignedDataSize( uint3( 2, 2, 1 ), BitsU(8).ToBytes(), BytesU(4) );
	ASSERT( size == 4*2 );
	
	size = ImageUtils::AlignedDataSize( uint3( 4, 2, 1 ), BitsU(8).ToBytes(), BytesU(4) );
	ASSERT( size == 4*2 );
	
	size = ImageUtils::AlignedDataSize( uint3( 2, 4, 1 ), BitsU(8).ToBytes(), BytesU(4) );
	ASSERT( size == 4*4 );
	
	size = ImageUtils::AlignedDataSize( uint3( 3, 2, 1 ), BitsU(8*3).ToBytes(), BytesU(4) );
	ASSERT( size == 4*2*3 );

	// assert:
	//size = AlignedImageDataSize( uint3( 2, 2, 1 ), 8, 3 );
}


static void Test_ImageConverter ()
{
	R8u		src[4] = { R8u( 0xFF ), R8u( 0x1F ), R8u( 0x3F ), R8u( 0x07 ) };
	R32u	dst[4] = { R32u( 1 ), R32u( 2 ), R32u( 3 ), R32u( 4 ) };

	ImageUtils::Copy( uint3( 2, 2, 1 ), src, dst );

	ASSERT( dst[0].r == 0xFF );
	ASSERT( dst[1].r == 0x1F );
	ASSERT( dst[2].r == 0x3F );
	ASSERT( dst[3].r == 0x07 );


	//	0		0		0		0
	//	0	  0xFF	  0x1F		0
	//	0	  0x3F	  0x07		0
	//	0		0		0		0
	R32i	img[4*4] = {};

	ImageUtils::CopyPart( uint3( 2, 2, 0 ), uint3( 0 ), src,
						  uint3( 4, 4, 0 ), uint3( 1, 1, 0 ), img );
	
	ASSERT( img[5].r == 0xFF );
	ASSERT( img[6].r == 0x1F );
	ASSERT( img[9].r == 0x3F );
	ASSERT( img[10].r == 0x07 );

	
	//	0		0		0		0
	//	0	  0xFF	  0x1F		0
	//	0	  0x3F	  0x07		0
	//	0		0		0		0
	/*RGBA4_UNorm	pic[4*4] = {};

	ImageUtils::CopyPart( uint3( 2, 2, 0 ), uint3( 0 ), src,
						  uint3( 4, 4, 0 ), uint3( 2, 2, 0 ), pic );
	
	ASSERT( pic[5].bits.r == 0xFF );
	ASSERT( pic[6].bits.r == 0x1F );
	ASSERT( pic[9].bits.r == 0x3F );
	ASSERT( pic[10].bits.r == 0x07 );*/

	
	//	0		0		0		0
	//	0	  0xFF	  0xFF		0
	//	0	  0xFF	  0xFF		0
	//	0		0		0		0
	ZeroMem( img );
	ImageUtils::Fill( uint3( 2, 2, 0 ), RGBA8u( 0xFF, 0, 0, 0 ),
					  uint3( 1, 1, 0 ), uint3( 4, 4, 0 ), img );
	
	ASSERT( img[5].r == 0xFF );
	ASSERT( img[6].r == 0xFF );
	ASSERT( img[9].r == 0xFF );
	ASSERT( img[10].r == 0xFF );

}


extern void Test_Math_ImageUtils ()
{
	Test_AlignedImageSize();
	Test_ImageConverter();
}
