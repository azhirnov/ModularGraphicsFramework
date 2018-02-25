// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Soft/ShaderLang/SWLangImage.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

/*
=================================================
	constructor
=================================================
*/
	BaseImage::BaseImage (MemLayout_t &&memLayout, EOutputFormat outFmt) :
		_memLayout{ RVREF(memLayout) }
	{
		CHECK( _Init( outFmt ) );
	}
	
/*
=================================================
	_Init
=================================================
*/
	bool BaseImage::_Init (EOutputFormat outFmt)
	{
		using namespace GX_STL::GXMath::ColorFormat;

		CHECK_ERR( not _memLayout.layers.Empty() and
					not _memLayout.layers.Front().mipmaps.Count() == 1 );

		const bool	is_float = (outFmt == EOutputFormat::Float4);
		const bool	is_int	 = (outFmt == EOutputFormat::Int4);
		const bool	is_uint	 = (outFmt == EOutputFormat::UInt4);

		switch ( _memLayout.layers.Front().mipmaps.Front().format )
		{
			// signed normalized
			case EPixelFormat::RGBA16_SNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGBA16_SNorm, float4 >;	_store = &_StoreTexel< RGBA16_SNorm, float4 >;	break;
			case EPixelFormat::RGBA8_SNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGBA8_SNorm, float4 >;		_store = &_StoreTexel< RGBA8_SNorm, float4 >;	break;
			case EPixelFormat::RGB16_SNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGB16_SNorm, float4 >;		_store = &_StoreTexel< RGB16_SNorm, float4 >;	break;
			case EPixelFormat::RGB8_SNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< RGB8_SNorm, float4 >;		_store = &_StoreTexel< RGB8_SNorm, float4 >;	break;
			case EPixelFormat::RG16_SNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< RG16_SNorm, float4 >;		_store = &_StoreTexel< RG16_SNorm, float4 >;	break;
			case EPixelFormat::RG8_SNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< RG8_SNorm, float4 >;		_store = &_StoreTexel< RG8_SNorm, float4 >;		break;
			case EPixelFormat::R16_SNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< R16_SNorm, float4 >;		_store = &_StoreTexel< R16_SNorm, float4 >;		break;
			case EPixelFormat::R8_SNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< R8_SNorm, float4 >;		_store = &_StoreTexel< R8_SNorm, float4 >;		break;
			
			// unsigned normalized
			case EPixelFormat::RGBA16_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGBA16_UNorm, float4 >;	_store = &_StoreTexel< RGBA16_UNorm, float4 >;	break;
			case EPixelFormat::RGBA8_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGBA8_UNorm, float4 >;		_store = &_StoreTexel< RGBA8_UNorm, float4 >;	break;
			case EPixelFormat::RGB16_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGB16_UNorm, float4 >;		_store = &_StoreTexel< RGB16_UNorm, float4 >;	break;
			case EPixelFormat::RGB8_UNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< RGB8_UNorm, float4 >;		_store = &_StoreTexel< RGB8_UNorm, float4 >;	break;
			case EPixelFormat::RG16_UNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< RG16_SNorm, float4 >;		_store = &_StoreTexel< RG16_SNorm, float4 >;	break;
			case EPixelFormat::RG8_UNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< RG8_UNorm, float4 >;		_store = &_StoreTexel< RG8_UNorm, float4 >;		break;
			case EPixelFormat::R16_UNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< R16_UNorm, float4 >;		_store = &_StoreTexel< R16_UNorm, float4 >;		break;
			case EPixelFormat::R8_UNorm :			CHECK_ERR( is_float );	_load = &_LoadTexel< R8_UNorm, float4 >;		_store = &_StoreTexel< R8_UNorm, float4 >;		break;
			case EPixelFormat::RGB10_A2_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGB10_A2_UNorm, float4 >;	_store = &_StoreTexel< RGB10_A2_UNorm, float4 >;break;
			case EPixelFormat::RGBA4_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGBA4_UNorm, float4 >;		_store = &_StoreTexel< RGBA4_UNorm, float4 >;	break;
			case EPixelFormat::RGB5_A1_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< RGB5_A1_UNorm, float4 >;	_store = &_StoreTexel< RGB5_A1_UNorm, float4 >;	break;
			case EPixelFormat::RGB_5_6_5_UNorm :	CHECK_ERR( is_float );	_load = &_LoadTexel< R5_G6_B5_UNorm, float4 >;	_store = &_StoreTexel< R5_G6_B5_UNorm, float4 >;break;

			// BGRA
			//case EPixelFormat::BGR8_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< BGR8_UNorm, float4 >;		_store = &_StoreTexel< BGR8_UNorm, float4 >;	break;
			//case EPixelFormat::BGRA8_UNorm :		CHECK_ERR( is_float );	_load = &_LoadTexel< BGRA8_UNorm, float4 >;		_store = &_StoreTexel< BGRA8_UNorm, float4 >;	break;
			
			// sRGB
			//case EPixelFormat::sRGB8
			//case EPixelFormat::sRGB8_A8
			//case EPixelFormat::sBGR8
			//case EPixelFormat::sBGR8_A8

			// signed integer
			case EPixelFormat::R8I :				CHECK_ERR( is_int );	_load = &_LoadTexel< R8i, int4 >;			_store = &_StoreTexel< R8i, int4 >;			break;
			case EPixelFormat::RG8I :				CHECK_ERR( is_int );	_load = &_LoadTexel< RG8i, int4 >;			_store = &_StoreTexel< RG8i, int4 >;		break;
			case EPixelFormat::RGB8I :				CHECK_ERR( is_int );	_load = &_LoadTexel< RGB8i, int4 >;			_store = &_StoreTexel< RGB8i, int4 >;		break;
			case EPixelFormat::RGBA8I :				CHECK_ERR( is_int );	_load = &_LoadTexel< RGBA8i, int4 >;		_store = &_StoreTexel< RGBA8i, int4 >;		break;
			case EPixelFormat::R16I :				CHECK_ERR( is_int );	_load = &_LoadTexel< R16i, int4 >;			_store = &_StoreTexel< R16i, int4 >;		break;
			case EPixelFormat::RG16I :				CHECK_ERR( is_int );	_load = &_LoadTexel< RG16i, int4 >;			_store = &_StoreTexel< RG16i, int4 >;		break;
			case EPixelFormat::RGB16I :				CHECK_ERR( is_int );	_load = &_LoadTexel< RGB16i, int4 >;		_store = &_StoreTexel< RGB16i, int4 >;		break;
			case EPixelFormat::RGBA16I :			CHECK_ERR( is_int );	_load = &_LoadTexel< RGBA16i, int4 >;		_store = &_StoreTexel< RGBA16i, int4 >;		break;
			case EPixelFormat::R32I :				CHECK_ERR( is_int );	_load = &_LoadTexel< R32i, int4 >;			_store = &_StoreTexel< R32i, int4 >;		break;
			case EPixelFormat::RG32I :				CHECK_ERR( is_int );	_load = &_LoadTexel< RG32i, int4 >;			_store = &_StoreTexel< RG32i, int4 >;		break;
			case EPixelFormat::RGB32I :				CHECK_ERR( is_int );	_load = &_LoadTexel< RGB32i, int4 >;		_store = &_StoreTexel< RGB32i, int4 >;		break;
			case EPixelFormat::RGBA32I :			CHECK_ERR( is_int );	_load = &_LoadTexel< RGBA32i, int4 >;		_store = &_StoreTexel< RGBA32i, int4 >;		break;
			
			// unsigned integer
			case EPixelFormat::R8U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< R8u, uint4 >;			_store = &_StoreTexel< R8u, uint4 >;		break;
			case EPixelFormat::RG8U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< RG8u, uint4 >;			_store = &_StoreTexel< RG8u, uint4 >;		break;
			case EPixelFormat::RGB8U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< RGB8u, uint4 >;		_store = &_StoreTexel< RGB8u, uint4 >;		break;
			case EPixelFormat::RGBA8U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< RGBA8u, uint4 >;		_store = &_StoreTexel< RGBA8u, uint4 >;		break;
			case EPixelFormat::R16U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< R16u, uint4 >;			_store = &_StoreTexel< R16u, uint4 >;		break;
			case EPixelFormat::RG16U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< RG16u, uint4 >;		_store = &_StoreTexel< RG16u, uint4 >;		break;
			case EPixelFormat::RGB16U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< RGB16u, uint4 >;		_store = &_StoreTexel< RGB16u, uint4 >;		break;
			case EPixelFormat::RGBA16U :			CHECK_ERR( is_uint );	_load = &_LoadTexel< RGBA16u, uint4 >;		_store = &_StoreTexel< RGBA16u, uint4 >;	break;
			case EPixelFormat::R32U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< R32u, uint4 >;			_store = &_StoreTexel< R32u, uint4 >;		break;
			case EPixelFormat::RG32U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< RG32u, uint4 >;		_store = &_StoreTexel< RG32u, uint4 >;		break;
			case EPixelFormat::RGB32U :				CHECK_ERR( is_uint );	_load = &_LoadTexel< RGB32u, uint4 >;		_store = &_StoreTexel< RGB32u, uint4 >;		break;
			case EPixelFormat::RGBA32U :			CHECK_ERR( is_uint );	_load = &_LoadTexel< RGBA32u, uint4 >;		_store = &_StoreTexel< RGBA32u, uint4 >;	break;
			case EPixelFormat::RGB10_A2U :			CHECK_ERR( is_uint );	_load = &_LoadTexel< RGB10_A2u, uint4 >;	_store = &_StoreTexel< RGB10_A2u, uint4 >;	break;
			
			// float
			case EPixelFormat::R16F :				CHECK_ERR( is_float );	_load = &_LoadTexel< R16f, float4 >;		_store = &_StoreTexel< R16f, float4 >;			break;
			case EPixelFormat::RG16F :				CHECK_ERR( is_float );	_load = &_LoadTexel< RG16f, float4 >;		_store = &_StoreTexel< RG16f, float4 >;			break;
			case EPixelFormat::RGB16F :				CHECK_ERR( is_float );	_load = &_LoadTexel< RGB16f, float4 >;		_store = &_StoreTexel< RGB16f, float4 >;		break;
			case EPixelFormat::RGBA16F :			CHECK_ERR( is_float );	_load = &_LoadTexel< RGBA16f, float4 >;		_store = &_StoreTexel< RGBA16f, float4 >;		break;
			case EPixelFormat::R32F :				CHECK_ERR( is_float );	_load = &_LoadTexel< R32f, float4 >;		_store = &_StoreTexel< R32f, float4 >;			break;
			case EPixelFormat::RG32F :				CHECK_ERR( is_float );	_load = &_LoadTexel< RG32f, float4 >;		_store = &_StoreTexel< RG32f, float4 >;			break;
			case EPixelFormat::RGB32F :				CHECK_ERR( is_float );	_load = &_LoadTexel< RGB32f, float4 >;		_store = &_StoreTexel< RGB32f, float4 >;		break;
			case EPixelFormat::RGBA32F :			CHECK_ERR( is_float );	_load = &_LoadTexel< RGBA32f, float4 >;		_store = &_StoreTexel< RGBA32f, float4 >;		break;
			case EPixelFormat::RGB_11_11_10F :		CHECK_ERR( is_float );	_load = &_LoadTexel< R11_G11_B10f, float4 >;_store = &_StoreTexel< R11_G11_B10f, float4 >;	break;

			// depth stencil
			//case EPixelFormat::Depth16
			//case EPixelFormat::Depth24
			//case EPixelFormat::Depth32
			//case EPixelFormat::Depth32F
			//case EPixelFormat::Depth16_Stencil8
			//case EPixelFormat::Depth24_Stencil8
			//case EPixelFormat::Depth32F_Stencil8

			default :								RETURN_ERR( "unsupported format" );
		}

		return true;
	}
	
/*
=================================================
	_LoadTexel
=================================================
*/
	template <typename SrcColor, typename DstColor>
	void BaseImage::_LoadTexel (const MemLayout_t &memLayout, const int3 &coord, OUT void *texel)
	{
		auto&		layer	= memLayout.layers[ Clamp(coord.z, 0, int(memLayout.layers.Count())-1) ];
		BytesU		bpp		= SizeOf<SrcColor>();
		BytesU		off		= GXImageUtils::GetPixelOffset( coord.xyo(), int2(layer.dimension).xyo(), bpp, memLayout.align );
		const void*	ptr		= layer.mipmaps.Front().memory + off;
		UnsafeMem::MemCopy( OUT texel, ptr, bpp );
	}
		
/*
=================================================
	_StoreTexel
=================================================
*/
	template <typename DstColor, typename SrcColor>
	void BaseImage::_StoreTexel (MemLayout_t &memLayout, const int3 &coord, const void* texel)
	{
		auto&		layer	= memLayout.layers[ Clamp(coord.z, 0, int(memLayout.layers.Count())-1) ];
		BytesU		bpp		= SizeOf<SrcColor>();
		BytesU		off		= GXImageUtils::GetPixelOffset( coord.xyo(), int2(layer.dimension).xyo(), bpp, memLayout.align );
		void*		ptr		= layer.mipmaps.Front().memory + off;
		UnsafeMem::MemCopy( OUT ptr, texel, bpp );
	}
	
/*
=================================================
	Size
=================================================
*/
	int3 BaseImage::Size () const
	{
		return int3(_memLayout.dimension);
	}
	
/*
=================================================
	Samples
=================================================
*/
	int BaseImage::Samples () const
	{
		return 0;	// TODO
	}


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
