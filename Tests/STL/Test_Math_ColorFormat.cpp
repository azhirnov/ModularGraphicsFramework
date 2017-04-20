// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;
using namespace GX_STL::GXMath::ColorFormat;

typedef IntColorFormatInfo		IFormatInfo;
typedef FloatColorFormatInfo	FFormatInfo;


static void ColorFormat_FormatInfoTest ()
{
	IFormatInfo const	u8fmt = IFormatInfo::SimpleType<ubyte, 4>( RGBA8u() );
	IFormatInfo const	i8fmt = IFormatInfo::SimpleType<byte, 4>( RGBA8i() );
	IFormatInfo const	u32fmt = IFormatInfo::SimpleType<uint, 4>( RGBA32u() );
	IFormatInfo const	i32fmt = IFormatInfo::SimpleType<int, 4>( RGBA32i() );
	FFormatInfo const	f32fmt = FFormatInfo::SimpleType<float>();

	ASSERT( MaxValue<ubyte>() == u8fmt.MaxValue().x );
	ASSERT( MinValue<ubyte>() == u8fmt.MinValue().x );

	ASSERT( MaxValue<byte>() == i8fmt.MaxValue().x );
	ASSERT( MinValue<byte>() == i8fmt.MinValue().x );

	ASSERT( MaxValue<uint>() == u32fmt.MaxValue().x );
	ASSERT( MinValue<uint>() == u32fmt.MinValue().x );
	
	ASSERT( MaxValue<int>() == i32fmt.MaxValue().x );
	ASSERT( MinValue<int>() == i32fmt.MinValue().x );
}


static void ColorFormat_ConversionIntToInt ()
{
	RGBA4_UNorm const	src( 0xF, 0x7, 0x3, 0xB );
	RGBA8_UNorm			dst8;
	RGBA4_UNorm			dst4;
		
	ColorFormatConverter::NormIntToNormInt( dst8, src );
	ColorFormatConverter::NormIntToNormInt( dst4, dst8 );
	ASSERT( RGBA8_UNorm( 0xF7, 0x77, 0x37, 0xB7 ) == dst8 );
	ASSERT( src == dst4 );

	RGBA8u		src8( 0xE8, 0xA2, 0x09, 0 );

	ColorFormatConverter::NormIntToNormInt( dst8, src8 );
	ColorFormatConverter::NormIntToNormInt( dst4, src8 );
	ASSERT( RGBA4_UNorm( 0xE, 0xA, 0x1, 0x0 ) == dst4 );
	ASSERT( src8 == dst8 );
}


static void ColorFormat_ConversionIntToInt2 ()
{
	RGBA8u const	src( 0xFF, 0x6F, 0x35, 0x0B );
	RGBA4_UNorm		dst4;
		
	ColorFormatConverter::NormIntToNormInt( dst4, src );
	ASSERT( RGBA4_UNorm( 0xF, 0x7, 0x3, 0x1 ) == dst4 );
}
	

static void ColorFormat_ConversionNormFloatToInt ()
{
	RGBA32f const	src( 1.0f, 0.5f, 0.25f, 0.0f );
	RGBA8u			dst8;
	R5_G6_B5_UNorm	dst5;

	ColorFormatConverter::NormFloatToNormInt( dst8, src );
	ColorFormatConverter::NormFloatToNormInt( dst5, src );
	ASSERT( RGBA8u( 0xFF, 0x80, 0x40, 0 ) == dst8 );
	ASSERT( R5_G6_B5_UNorm( 0x1F, 0x20, 0x8 ) == dst5 );
}


template <typename DstColorType, typename SrcColorType>
static void UniConversion (DstColorType &dst, const SrcColorType &src, const DstColorType &cmp)
{
	ColorFormatConverter::Convert( dst, src );
	ASSERT( dst == cmp );
}


static void ColorFormat_TestUniConversion ()
{
	RGBA32f	rgba32f;
	UniConversion( rgba32f, RG32u( 0xFF, 0xFFFF ), RGBA32f( 255.0f, 65535.0f, 0.0f, 0.0f ) );
	UniConversion( rgba32f, RG8_UNorm( 127, 255 ), RGBA32f( 0.6f, 1.0f, 0.0f, 0.0f ) );

	// TODO: more tests
}


static void ColorFormat_TestSwizzle ()
{								
	typedef ColorSwizzle< RGBA32f_Norm, 3, 2, 1, 0 >		ABGR32f_Norm;
	
	RGBA32f_Norm	src( 1.0f, 0.5f, 0.333f, 0.0f );
	ABGR32f_Norm	dst;

	ColorFormatConverter::Convert( dst, src );

	ASSERT( All( FloatFormat(src) == FloatFormat(dst) ) );
}


extern void Test_Math_ColorFormat ()
{
	ColorFormat_FormatInfoTest();
	ColorFormat_ConversionIntToInt();
	ColorFormat_ConversionIntToInt2();
	ColorFormat_ConversionNormFloatToInt();
	//ColorFormat_TestUniConversion();
	ColorFormat_TestSwizzle();
}
