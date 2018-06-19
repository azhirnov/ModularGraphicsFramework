// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/ShaderLang/SWLangTexture.h"
#include "Engine/Platforms/Soft/Impl/SWDeviceProperties.h"

#include "Core/STL/Math/Color/ColorFormats.h"
#include "Core/STL/Math/Image/ImageUtils.h"

namespace SWShaderLang
{
namespace Impl
{
	using EPipelineAccess	= Engine::Platforms::EPipelineAccess;
	using EImageLayout		= Engine::Platforms::EImageLayout;

	
/*
=================================================
	constructor
=================================================
*/
	BaseTexture::BaseTexture (MemLayout_t &&memLayout, const Sampler_t &samplerDescr, EOutputFormat outFmt) :
		_memLayout{ RVREF(memLayout) },
		_sampler{ _ChooseSampler( samplerDescr ), 0.0f, float2() }
	{
		CHECK( _Init( outFmt ) );
	}
	
/*
=================================================
	_Init
=================================================
*/
	bool BaseTexture::_Init (EOutputFormat outFmt)
	{
		using namespace GX_STL::GXMath::ColorFormat;

		STATIC_ASSERT( sizeof(RGBA32f) == sizeof(glm::vec4) );
		STATIC_ASSERT( sizeof(RGBA32i) == sizeof(glm::ivec4) );
		STATIC_ASSERT( sizeof(RGBA32u) == sizeof(glm::uvec4) );

		CHECK_ERR( not _memLayout.layers.Empty() and not _memLayout.layers.Front().mipmaps.Empty() );

		const bool	is_float = (outFmt == EOutputFormat::Float4);
		const bool	is_int	 = (outFmt == EOutputFormat::Int4);
		const bool	is_uint	 = (outFmt == EOutputFormat::UInt4);

		switch ( _memLayout.layers.Front().mipmaps.Front().format )
		{
			// signed normalized
			case EPixelFormat::RGBA16_SNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGBA16_SNorm, RGBA32f >() ));	break;
			case EPixelFormat::RGBA8_SNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGBA8_SNorm, RGBA32f >() ));		break;
			case EPixelFormat::RGB16_SNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB16_SNorm, RGBA32f >() ));		break;
			case EPixelFormat::RGB8_SNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB8_SNorm, RGBA32f >() ));		break;
			case EPixelFormat::RG16_SNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RG16_SNorm, RGBA32f >() ));		break;
			case EPixelFormat::RG8_SNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RG8_SNorm, RGBA32f >() ));		break;
			case EPixelFormat::R16_SNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R16_SNorm, RGBA32f >() ));		break;
			case EPixelFormat::R8_SNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R8_SNorm, RGBA32f >() ));		break;
			
			// unsigned normalized
			case EPixelFormat::RGBA16_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGBA16_UNorm, RGBA32f >() ));	break;
			case EPixelFormat::RGBA8_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGBA8_UNorm, RGBA32f >() ));		break;
			case EPixelFormat::RGB16_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB16_UNorm, RGBA32f >() ));		break;
			case EPixelFormat::RGB8_UNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB8_UNorm, RGBA32f >() ));		break;
			case EPixelFormat::RG16_UNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RG16_SNorm, RGBA32f >() ));		break;
			case EPixelFormat::RG8_UNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RG8_UNorm, RGBA32f >() ));		break;
			case EPixelFormat::R16_UNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R16_UNorm, RGBA32f >() ));		break;
			case EPixelFormat::R8_UNorm :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R8_UNorm, RGBA32f >() ));		break;
			case EPixelFormat::RGB10_A2_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB10_A2_UNorm, RGBA32f >() ));	break;
			case EPixelFormat::RGBA4_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGBA4_UNorm, RGBA32f >() ));		break;
			case EPixelFormat::RGB5_A1_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB5_A1_UNorm, RGBA32f >() ));	break;
			case EPixelFormat::RGB_5_6_5_UNorm :	CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R5_G6_B5_UNorm, RGBA32f >() ));	break;

			// BGRA
			//case EPixelFormat::BGR8_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< BGR8_UNorm, RGBA32f >() ));		break;
			//case EPixelFormat::BGRA8_UNorm :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< BGRA8_UNorm, RGBA32f >() ));		break;
			
			// sRGB
			//case EPixelFormat::sRGB8
			//case EPixelFormat::sRGB8_A8
			//case EPixelFormat::sBGR8
			//case EPixelFormat::sBGR8_A8

			// signed integer
			case EPixelFormat::R8I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< R8i, RGBA32i >() ));				break;
			case EPixelFormat::RG8I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RG8i, RGBA32i >() ));			break;
			case EPixelFormat::RGB8I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RGB8i, RGBA32i >() ));			break;
			case EPixelFormat::RGBA8I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RGBA8i, RGBA32i >() ));			break;
			case EPixelFormat::R16I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< R16i, RGBA32i >() ));			break;
			case EPixelFormat::RG16I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RG16i, RGBA32i >() ));			break;
			case EPixelFormat::RGB16I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RGB16i, RGBA32i >() ));			break;
			case EPixelFormat::RGBA16I :			CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RGBA16i, RGBA32i >() ));			break;
			case EPixelFormat::R32I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< R32i, RGBA32i >() ));			break;
			case EPixelFormat::RG32I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RG32i, RGBA32i >() ));			break;
			case EPixelFormat::RGB32I :				CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RGB32i, RGBA32i >() ));			break;
			case EPixelFormat::RGBA32I :			CHECK_ERR( is_int );	CHECK_ERR(( _InitByFormat< RGBA32i, RGBA32i >() ));			break;
			
			// unsigned integer
			case EPixelFormat::R8U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< R8u, RGBA32u >() ));				break;
			case EPixelFormat::RG8U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RG8u, RGBA32u >() ));			break;
			case EPixelFormat::RGB8U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RGB8u, RGBA32u >() ));			break;
			case EPixelFormat::RGBA8U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RGBA8u, RGBA32u >() ));			break;
			case EPixelFormat::R16U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< R16u, RGBA32u >() ));			break;
			case EPixelFormat::RG16U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RG16u, RGBA32u >() ));			break;
			case EPixelFormat::RGB16U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RGB16u, RGBA32u >() ));			break;
			case EPixelFormat::RGBA16U :			CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RGBA16u, RGBA32u >() ));			break;
			case EPixelFormat::R32U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< R32u, RGBA32u >() ));			break;
			case EPixelFormat::RG32U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RG32u, RGBA32u >() ));			break;
			case EPixelFormat::RGB32U :				CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RGB32u, RGBA32u >() ));			break;
			case EPixelFormat::RGBA32U :			CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RGBA32u, RGBA32u >() ));			break;
			case EPixelFormat::RGB10_A2U :			CHECK_ERR( is_uint );	CHECK_ERR(( _InitByFormat< RGB10_A2u, RGBA32u >() ));		break;
			
			// float
			case EPixelFormat::R16F :				CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R16f, RGBA32f >() ));			break;
			case EPixelFormat::RG16F :				CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RG16f, RGBA32f >() ));			break;
			case EPixelFormat::RGB16F :				CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB16f, RGBA32f >() ));			break;
			case EPixelFormat::RGBA16F :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGBA16f, RGBA32f >() ));			break;
			case EPixelFormat::R32F :				CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R32f, RGBA32f >() ));			break;
			case EPixelFormat::RG32F :				CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RG32f, RGBA32f >() ));			break;
			case EPixelFormat::RGB32F :				CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGB32f, RGBA32f >() ));			break;
			case EPixelFormat::RGBA32F :			CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< RGBA32f, RGBA32f >() ));			break;
			case EPixelFormat::RGB_11_11_10F :		CHECK_ERR( is_float );	CHECK_ERR(( _InitByFormat< R11_G11_B10f, RGBA32f >() ));	break;

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
	_ChooseSampler
=================================================
*/
	BaseTexture::ESamplerMode::type  BaseTexture::_ChooseSampler (const Sampler_t &samp)
	{
		using S						= ESamplerMode;
		using EFilter				= Engine::Platforms::EFilter;
		using EAddressMode			= Engine::Platforms::EAddressMode;
		using ECompareFunc			= Engine::Platforms::ECompareFunc;
		using ESamplerBorderColor	= Engine::Platforms::ESamplerBorderColor;

		S::type	result = S::type(0);

		switch ( samp.Filter() )
		{
			case EFilter::MinMagMipNearest :				result |= S::MinMagMipNearest;					break;
			case EFilter::MinMagNearest_MipLinear :			result |= S::MinMagNearest_MipLinear;			break;
			case EFilter::MinNearest_MagLinear_MipNearest :	result |= S::MinNearest_MagLinear_MipNearest;	break;
			case EFilter::MinNearest_MagMipLinear :			result |= S::MinNearest_MagMipLinear;			break;
			case EFilter::MinLinear_MagMipNearest :			result |= S::MinLinear_MagMipNearest;			break;
			case EFilter::MinLinear_MagNearest_MipLinear :	result |= S::MinLinear_MagNearest_MipLinear;	break;
			case EFilter::MinMagLinear_MipNearest :			result |= S::MinMagLinear_MipNearest;			break;
			case EFilter::MinMagMipLinear :					result |= S::MinMagMipLinear;					break;
			case EFilter::Anisotropic_2 :					result |= S::Anisotropic_2;						break;
			case EFilter::Anisotropic_4 :					result |= S::Anisotropic_4;						break;
			case EFilter::Anisotropic_8 :					result |= S::Anisotropic_8;						break;
			case EFilter::Anisotropic_12 :					result |= S::Anisotropic_12;					break;
			case EFilter::Anisotropic_16 :					result |= S::Anisotropic_16;					break;
			default :										RETURN_ERR( "unsupported filtering mode!" );
		}

		FOR( i, samp.AddressMode() )
		{
			const uint offsets[] = { 0, S::_V_ADDRESS_OFF - S::_U_ADDRESS_OFF, S::_W_ADDRESS_OFF - S::_U_ADDRESS_OFF };

			switch ( samp.AddressMode()[i] )
			{
				case EAddressMode::ClampToEdge :		result |= (S::U_ClampToEdge << offsets[i]);		break;
				case EAddressMode::ClampToBorder :		result |= (S::U_ClampToBorder << offsets[i]);	break;
				case EAddressMode::Repeat :				result |= (S::U_Repeat << offsets[i]);			break;
				case EAddressMode::MirroredRepeat :		result |= (S::U_MirroredRepeat << offsets[i]);	break;
				//case EAddressMode::MirroredClamp
				//case EAddressMode::ClampUnnorm
				default :								RETURN_ERR( "unsupported addressing mode!" );
			}
		}

		FOR( i, samp.BorderColor() )
		{
			const auto	t = ESamplerBorderColor::type(i);

			if ( not samp.BorderColor()[t] )
				continue;

			switch ( t )
			{
				case ESamplerBorderColor::Float :		result |= S::Border_Float;			break;
				case ESamplerBorderColor::Int :			result |= S::Border_Int;			break;
				case ESamplerBorderColor::Transparent :	result |= S::Border_Transparent;	break;
				case ESamplerBorderColor::Black :		result |= S::Border_Black;			break;
				case ESamplerBorderColor::White :		result |= S::Border_White;			break;
				default :								RETURN_ERR( "unsupported border color!" );
			}
		}

		switch ( samp.CompareOp() )
		{
			case ECompareFunc::None :		break;
			case ECompareFunc::Never :		result |= S::CompareOp_Never;		break;
			case ECompareFunc::Less :		result |= S::CompareOp_Less;		break;
			case ECompareFunc::Equal :		result |= S::CompareOp_Equal;		break;
			case ECompareFunc::LEqual :		result |= S::CompareOp_LEqual;		break;
			case ECompareFunc::Greater :	result |= S::CompareOp_Greater;		break;
			case ECompareFunc::NotEqual :	result |= S::CompareOp_NotEqual;	break;
			case ECompareFunc::GEqual :		result |= S::CompareOp_GEqual;		break;
			case ECompareFunc::Always :		result |= S::CompareOp_Always;		break;
			default :						RETURN_ERR( "unsupported compare function!" );
		}

		return result;
	}

/*
=================================================
	_InitByFormat
=================================================
*/
	template <typename SrcColor, typename DstColor>
	bool BaseTexture::_InitByFormat ()
	{
		using Converter = ColorFormatUtils::ColorFormatConverter;

		const bool	is_anisotropic		= _GetAnisotropyLevel( _sampler.mode ) > 0;
		const bool	has_linear_filter	= _IsMipmapLinearFilter( _sampler.mode ) or _IsMinLinearFilter( _sampler.mode ) or _IsMagLinearFilter( _sampler.mode );
		const bool	is_float			= Converter::IsFloat<DstColor> or Converter::IsNormalized<DstColor>;

		if_constexpr( not is_float ) {
			if ( is_anisotropic or has_linear_filter ) {
				RETURN_ERR( "filtering not supported for integer format!" );
			}
		}

		_fetch		= &_FetchImpl< SrcColor, DstColor >;
		_sample		= is_anisotropic ? &_SampleAnisotropic< SrcColor, DstColor > : &_SampleTrilinear< SrcColor, DstColor >;
		_sampleLod	= is_anisotropic ? &_SampleLodAnisotropic< SrcColor, DstColor > : &_SampleLodTrilinear< SrcColor, DstColor >;

		return true;
	}
	
/*
=================================================
	_TransformCoords
=================================================
*/
	inline void BaseTexture::_TransformCoords (OUT float3 &outFrac, OUT float3 &outICoord, OUT bool &isBorder,
											   const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, ESamplerMode::type samplerMode)
	{
		const auto	WrapCoord = LAMBDA( &isBorder ) (OUT float &icoord, OUT float &frac,
													 float unorm, float dim, float off, ESamplerMode::type addressMode)
		{{
			using namespace GX_STL::GXMath;

			unorm = unorm * dim + off;

			switch ( addressMode )
			{
				case ESamplerMode::U_ClampToEdge : {
					icoord	= Clamp( unorm, 0.0f, dim );
					break;
				}
				case ESamplerMode::U_Repeat : {
					icoord	= Wrap( unorm, 0.0f, dim );
					break;
				}
				case ESamplerMode::U_MirroredRepeat : {
					icoord	= MirroredWrap( unorm, 0.0f, dim );
					break;
				}
				case ESamplerMode::U_ClampToBorder : {
					icoord	 = Clamp( unorm, 0.0f, dim );
					isBorder |= (icoord != unorm);
					break;
				}
				default : {
					frac	= 0.0f;
					icoord	= 0.0f;
				}
			}
		}};
		
		isBorder = false;

		if ( _IsTexture3D( memLayout ) )
		{
			WrapCoord( OUT outICoord.z, OUT outFrac.z, coord.z, float(memLayout.dimension.z - 1), float(offset.z),
						(samplerMode & ESamplerMode::_W_ADDRESS_MASK) >> (ESamplerMode::_W_ADDRESS_OFF - ESamplerMode::_U_ADDRESS_OFF) );
		}
		else
		if ( _IsTextureArray2D( memLayout ) )
		{
			// set 2d array layer
			outFrac.z   = 0.0f;
			outICoord.z = Clamp( coord.z, 0.0f, float(memLayout.layers.Count()) - 1.0f );
		}

		if ( _IsTextureArray1D( memLayout ) )
		{
			// set 1d array layer
			outFrac.y   = 0.0f;
			outICoord.y = Clamp( coord.y, 0.0f, float(memLayout.layers.Count()) - 1.0f );
		}
		else
		{
			WrapCoord( OUT outICoord.y, OUT outFrac.y, coord.y, float(memLayout.dimension.y - 1), float(offset.y),
						(samplerMode & ESamplerMode::_V_ADDRESS_MASK) >> (ESamplerMode::_V_ADDRESS_OFF - ESamplerMode::_U_ADDRESS_OFF) );
		}

		WrapCoord( OUT outICoord.x, OUT outFrac.x, coord.x, float(memLayout.dimension.x - 1), float(offset.x),
					(samplerMode & ESamplerMode::_U_ADDRESS_MASK) );

	}
	
/*
=================================================
	_CheckOffset
=================================================
*/
	void BaseTexture::_CheckOffset (const MemLayout_t &memLayout, const int3 &offset)
	{
		const int	max_offset = Engine::PlatformSW::SWDeviceProperties.limits.maxProgramTexelOffset;
		const int	min_offset = Engine::PlatformSW::SWDeviceProperties.limits.minProgramTexelOffset;

		CHECK( offset.x >= min_offset and offset.x <= max_offset );
		
		if ( _IsTexture2D( memLayout ) or _IsTextureArray2D( memLayout ) ) {
			CHECK( offset.y >= min_offset and offset.y <= max_offset );
		} else {
			CHECK( offset.y == 0 );
		}

		if ( _IsTexture3D( memLayout ) ) {
			CHECK( offset.z >= min_offset and offset.z <= max_offset );
		} else {
			CHECK( offset.z == 0 );
		}
	}
	
/*
=================================================
	_IsTexture***
=================================================
*/
	inline bool BaseTexture::_IsTextureArray1D (const MemLayout_t &memLayout)
	{
		return All( memLayout.dimension.yz() == 1 ) and memLayout.layers.Count() > 1;
	}
	
	inline bool BaseTexture::_IsTextureArray2D (const MemLayout_t &memLayout)
	{
		return memLayout.dimension.z == 1 and memLayout.layers.Count() > 1;
	}
	
	inline bool BaseTexture::_IsTexture1D (const MemLayout_t &memLayout)
	{
		return memLayout.dimension.x > 1 and All( memLayout.dimension.yz() == 1 ) and memLayout.layers.Count() == 1;
	}

	inline bool BaseTexture::_IsTexture2D (const MemLayout_t &memLayout)
	{
		return All( memLayout.dimension.xy() > 1 ) and memLayout.dimension.z == 1 and memLayout.layers.Count() == 1; 
	}

	inline bool BaseTexture::_IsTexture3D (const MemLayout_t &memLayout)
	{
		return memLayout.dimension.z > 1;
	}

/*
=================================================
	_GetBorderColor
=================================================
*/
	template <typename DstColor>
	DstColor  BaseTexture::_GetBorderColor (ESamplerMode::type value)
	{
		using Converter = ColorFormatUtils::ColorFormatConverter;

		constexpr bool is_float = Converter::IsFloat<DstColor> or Converter::IsNormalized<DstColor>;

		if_constexpr( is_float )
		{
			if ( not EnumEq( value, ESamplerMode::Border_Int ) )
			{
				using Fmt  = ColorFormatUtils::FloatFormat;
				using Info = ColorFormatUtils::FloatColorFormatInfo;

				const Info	dst_info = Info(DstColor());

				if ( EnumEq( value, ESamplerMode::Border_Black ) )
					return dst_info.Clamp( Fmt(0.0f, 0.0f, 0.0f, 1.0f) );
			
				if ( EnumEq( value, ESamplerMode::Border_White ) )
					return dst_info.Clamp( Fmt(1.0f, 1.0f, 1.0f, 1.0f) );

				//if ( EnumEq( value, ESamplerMode::Border_Transparent ) )
					return dst_info.Clamp( Fmt(0.0f, 0.0f, 0.0f, 0.0f) );
			}
			else
			{
				RETURN_ERR( "integer border color not supported for float point format!", DstColor() );
			}
		}
		else
		{
			if ( EnumEq( value, ESamplerMode::Border_Int ) )
			{
				using Fmt  = ColorFormatUtils::IntFormat;
				using Info = ColorFormatUtils::IntColorFormatInfo;
			
				const Info	dst_info = Info(DstColor());
				const auto	imax	 = dst_info.MaxValue();

				if ( EnumEq( value, ESamplerMode::Border_Black ) )
					return dst_info.Clamp( Fmt(0, 0, 0, imax[3]) );
			
				if ( EnumEq( value, ESamplerMode::Border_White ) )
					return dst_info.Clamp( Fmt(imax[0], imax[1], imax[2], imax[3]) );

				//if ( EnumEq( value, ESamplerMode::Border_Transparent ) )
					return dst_info.Clamp( Fmt(0, 0, 0, 0) );
			}
			else
			{
				RETURN_ERR( "float point border color not supported for integer format!", DstColor() );
			}
		}
	}
	
/*
=================================================
	_IsMipmapLinearFilter
=================================================
*/
	inline bool BaseTexture::_IsMipmapLinearFilter (ESamplerMode::type sampler)
	{
		switch ( sampler )
		{
			case ESamplerMode::MinMagNearest_MipLinear :
			case ESamplerMode::MinNearest_MagMipLinear :
			case ESamplerMode::MinLinear_MagNearest_MipLinear :
			case ESamplerMode::MinMagMipLinear :
				return true;
		}
		return false;
	}
	
/*
=================================================
	_IsMinLinearFilter
=================================================
*/
	inline bool BaseTexture::_IsMinLinearFilter (ESamplerMode::type sampler)
	{
		switch ( sampler )
		{
			case ESamplerMode::MinLinear_MagMipNearest :
			case ESamplerMode::MinLinear_MagNearest_MipLinear :
			case ESamplerMode::MinMagLinear_MipNearest :
			case ESamplerMode::MinMagMipLinear :
				return true;
		}
		return false;
	}
	
/*
=================================================
	_IsMagLinearFilter
=================================================
*/
	inline bool BaseTexture::_IsMagLinearFilter (ESamplerMode::type sampler)
	{
		switch ( sampler )
		{
			case ESamplerMode::MinNearest_MagLinear_MipNearest :
			case ESamplerMode::MinNearest_MagMipLinear :
			case ESamplerMode::MinMagLinear_MipNearest :
			case ESamplerMode::MinMagMipLinear :
				return true;
		}
		return false;
	}
	
/*
=================================================
	_GetAnisotropyLevel
=================================================
*/
	inline uint BaseTexture::_GetAnisotropyLevel (ESamplerMode::type sampler)
	{
		switch ( sampler )
		{
			case ESamplerMode::Anisotropic_2 :	return 2;
			case ESamplerMode::Anisotropic_4 :	return 4;
			case ESamplerMode::Anisotropic_8 :	return 8;
			case ESamplerMode::Anisotropic_12 :	return 12;
			case ESamplerMode::Anisotropic_16 :	return 16;
		}
		return 0;
	}

/*
=================================================
	_CheckMipmap
=================================================
*/
	bool BaseTexture::_CheckMipmap (const Engine::GpuMsg::GetSWImageMemoryLayout::ImgLayer &mipmap)
	{
		if ( Engine::PlatformSW::SWDeviceProperties.vulkanCompatibility )
		{
			CHECK_ERR( mipmap.layout == EImageLayout::General or mipmap.layout == EImageLayout::ShaderReadOnlyOptimal );
			CHECK_ERR( mipmap.access[ EPipelineAccess::ShaderRead ] );
		}
		return true;
	}

/*
=================================================
	_FetchImpl
=================================================
*/
	template <typename SrcColor, typename DstColor>
	void BaseTexture::_FetchImpl (const MemLayout_t &memLayout, const int3 &coord, const int3 &offset, int lod, OUT void *texel)
	{
		auto&		layer	= memLayout.layers[ Clamp( coord.z + offset.z, 0, int(memLayout.layers.Count())-1 ) ];
		BytesU		bpp		= SizeOf<SrcColor>;
		BytesU		off		= GXImageUtils::GetPixelOffset( coord.xyo() + offset.xyo(), int2(layer.dimension).xyo(), bpp, memLayout.align );
		auto&		mipmap	= layer.mipmaps[ Clamp( lod, 0, int(layer.mipmaps.Count())-1 ) ];
		const void*	ptr		= mipmap.memory + off;
		
		CHECK( off + bpp <= mipmap.size );

		_CheckMipmap( mipmap );
		_CheckOffset( memLayout, offset );

		SrcColor	src;
		UnsafeMem::MemCopy( OUT &src, ptr, BytesU::SizeOf( src ) );

		DstColor	dst;
		ColorFormatUtils::ColorFormatConverter::Convert( OUT dst, src );

		UnsafeMem::MemCopy( OUT texel, &dst, BytesU::SizeOf( dst ) );
	}

/*
=================================================
	_SampleNearest
=================================================
*
	template <typename SrcColor, typename DstColor>
	void BaseTexture::_SampleNearest (const MemLayout_t &memLayout, const float3 &point, const int3 &offset, float bias, const Sampler &samp, OUT void *texel)
	{
		float3	icoord;
		float3	unorm;
		int		lod			= 0;
		bool	is_border	= false;

		_TransformCoords( OUT unorm, OUT icoord, OUT lod, OUT is_border, memLayout, point, offset, bias, samp );

		if ( is_border )
		{
			DstColor	dst = _GetBorderColor<DstColor>( samp.mode );

			UnsafeMem::MemCopy( OUT texel, &dst, BytesU::SizeOf( dst ) );
			return;
		}

		auto&		layer	= memLayout.layers[ Clamp( coord.z + offset.z, 0, int(memLayout.layers.Count())-1 ) ];
		BytesU		bpp		= SizeOf<SrcColor>;
		BytesU		off		= GXImageUtils::GetPixelOffset( coord.xyo() + offset.xyo(), int2(layer.dimension).xyo(), bpp, memLayout.align );
		auto&		mipmap	= layer.mipmaps[ Clamp( lod, 0, int(layer.mipmaps.Count())-1 ) ];
		const void*	ptr		= mipmap.memory + off;
		
		CHECK( off + bpp <= mipmap.size );

		_CheckMipmap( mipmap );
		_CheckOffset( memLayout, offset );

		SrcColor	src;
		UnsafeMem::MemCopy( OUT &src, ptr, BytesU::SizeOf( src ) );

		DstColor	dst;
		ColorFormatUtils::ColorFormatConverter::Convert( OUT dst, src );

		UnsafeMem::MemCopy( OUT texel, &dst, BytesU::SizeOf( dst ) );
	}

/*
=================================================
	_SampleBilinear
=================================================
*
	template <typename SrcColor, typename DstColor>
	void BaseTexture::_SampleBilinear (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float bias, const Sampler &samp, OUT void *texel)
	{
		TODO( "" );
	}

/*
=================================================
	_SampleTrilinear
=================================================
*/
	template <typename SrcColor, typename DstColor>
	void BaseTexture::_SampleTrilinear (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float bias, const Sampler &samp, OUT void *texel)
	{
		// get lod
		float	lod = 0.0f;

		_SampleLodTrilinear< SrcColor, DstColor >( memLayout, coord, offset, lod, samp, OUT texel );
	}

/*
=================================================
	_SampleAnisotropic
=================================================
*/
	template <typename SrcColor, typename DstColor>
	void BaseTexture::_SampleAnisotropic (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float bias, const Sampler &samp, OUT void *texel)
	{
		// get lod
		float	lod = 0.0f;

		_SampleLodAnisotropic< SrcColor, DstColor >( memLayout, coord, offset, lod, samp, OUT texel );
	}

/*
=================================================
	_SampleLodTrilinear
=================================================
*/
	template <typename SrcColor, typename DstColor>
	void BaseTexture::_SampleLodTrilinear (const MemLayout_t &memLayout, const float3 &point, const int3 &offset, float lod, const Sampler &samp, OUT void *texel)
	{
		float3	icoord;
		float3	frac;
		bool	is_border	= false;

		_TransformCoords( OUT frac, OUT icoord, OUT is_border, memLayout, point, offset, samp.mode );
		
		if ( is_border )
		{
			DstColor	dst = _GetBorderColor<DstColor>( samp.mode );

			UnsafeMem::MemCopy( OUT texel, &dst, BytesU::SizeOf( dst ) );
			return;
		}
		
		int3		coord	= RoundToInt(icoord);
		int			ilod	= RoundToInt(lod);
		auto&		layer	= memLayout.layers[ Clamp( coord.z, 0, int(memLayout.layers.Count())-1 ) ];
		BytesU		bpp		= SizeOf<SrcColor>;
		BytesU		off		= GXImageUtils::GetPixelOffset( coord.xyo(), int2(layer.dimension).xyo(), bpp, memLayout.align );
		auto&		mipmap	= layer.mipmaps[ Clamp( ilod, 0, int(layer.mipmaps.Count())-1 ) ];
		const void*	ptr		= mipmap.memory + off;
		
		CHECK( off + bpp <= mipmap.size );

		_CheckMipmap( mipmap );
		_CheckOffset( memLayout, offset );

		SrcColor	src;
		UnsafeMem::MemCopy( OUT &src, ptr, BytesU::SizeOf( src ) );

		DstColor	dst;
		ColorFormatUtils::ColorFormatConverter::Convert( OUT dst, src );

		UnsafeMem::MemCopy( OUT texel, &dst, BytesU::SizeOf( dst ) );
	}
	
/*
=================================================
	_SampleLodAnisotropic
=================================================
*/
	template <typename SrcColor, typename DstColor>
	void BaseTexture::_SampleLodAnisotropic (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float lod, const Sampler &samp, OUT void *texel)
	{
		TODO( "" );
	}

/*
=================================================
	Size
=================================================
*/
	int3  BaseTexture::Size (int lod) const
	{
		if ( lod >= 0 and lod < int(GX_STL::CompileTime::SizeOf<int>::bits)-1 )
			return int3( Max( _memLayout.dimension >> uint(lod), 1u ) );
		else
			return int3(0);
	}


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
