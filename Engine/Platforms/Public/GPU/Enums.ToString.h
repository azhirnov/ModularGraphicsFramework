// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/BufferEnums.h"
#include "Engine/Platforms/Public/GPU/CommandEnums.h"
#include "Engine/Platforms/Public/GPU/MemoryEnums.h"
#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Public/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Public/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Public/GPU/SamplerEnums.h"
#include "Engine/Platforms/Public/GPU/ShaderEnums.h"
#include "Engine/Platforms/Public/GPU/ImageEnums.h"
#include "Engine/Platforms/Public/GPU/VertexEnums.h"

#include "Engine/STL/Log/ToString.h"

namespace Engine
{
namespace Platforms
{
	
//-----------------------------------------------------------------------------//
// EBlendFunc
	
	ND_ inline StringCRef EBlendFunc::ToString (type value)
	{
		switch ( value )
		{
			case EBlendFunc::Zero					:	return "Zero";
			case EBlendFunc::One					:	return "One";
			case EBlendFunc::SrcColor				:	return "SrcColor";
			case EBlendFunc::OneMinusSrcColor		:	return "OneMinusSrcColor";
			case EBlendFunc::DstColor				:	return "DstColor";
			case EBlendFunc::OneMinusDstColor		:	return "OneMinusDstColor";
			case EBlendFunc::SrcAlpha				:	return "SrcAlpha";
			case EBlendFunc::OneMinusSrcAlpha		:	return "OneMinusSrcAlpha";
			case EBlendFunc::DstAlpha				:	return "DstAlpha";
			case EBlendFunc::OneMinusDstAlpha		:	return "OneMinusDstAlpha";
			case EBlendFunc::ConstColor				:	return "ConstColor";
			case EBlendFunc::OneMinusConstColor		:	return "OneMinusConstColor";
			case EBlendFunc::ConstAlpha				:	return "ConstAlpha";
			case EBlendFunc::OneMinusConstAlpha		:	return "OneMinusConstAlpha";
			case EBlendFunc::SrcAlphaSaturate		:	return "SrcAlphaSaturate";
		}
		RETURN_ERR( "invalid blend function type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// EBlendEq
	
	ND_ inline StringCRef EBlendEq::ToString (type value)
	{
		switch( value )
		{
			case EBlendEq::Add		:	return "Add";
			case EBlendEq::Sub		:	return "Sub";
			case EBlendEq::RevSub	:	return "ReverseSub";
			case EBlendEq::Min		:	return "Min";
			case EBlendEq::Max		:	return "Max";
		}
		RETURN_ERR( "invalid blend equation type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// ELogicOp
	
	ND_ inline StringCRef ELogicOp::ToString (type value)
	{
		switch( value )
		{
			case ELogicOp::None			:	return "None";
			case ELogicOp::Clear		:	return "Clear";
			case ELogicOp::Set			:	return "Set";
			case ELogicOp::Copy			:	return "Copy";
			case ELogicOp::CopyInverted	:	return "CopyInversed";
			case ELogicOp::Noop			:	return "Noop";
			case ELogicOp::Invert		:	return "Invert";
			case ELogicOp::And			:	return "And";
			case ELogicOp::NotAnd		:	return "NotAnd";
			case ELogicOp::Or			:	return "Or";
			case ELogicOp::NotOr		:	return "NotOr";
			case ELogicOp::Xor			:	return "Xor";
			case ELogicOp::Equiv		:	return "Equiv";
			case ELogicOp::AndReverse	:	return "AndReverse";
			case ELogicOp::AndInverted	:	return "AndInverted";
			case ELogicOp::OrReverse	:	return "OrReverse";
			case ELogicOp::OrInverted	:	return "OrInverted";
		}
		RETURN_ERR( "invalid logical operation type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// ECompareFunc
	
	ND_ inline StringCRef ECompareFunc::ToString (type value)
	{
		switch ( value )
		{
			case ECompareFunc::None		:	return "None";
			case ECompareFunc::Never	:	return "Never";
			case ECompareFunc::Less		:	return "Less";
			case ECompareFunc::Equal	:	return "Equal";
			case ECompareFunc::LEqual	:	return "LessEqual";
			case ECompareFunc::Greater	:	return "Greater";
			case ECompareFunc::NotEqual	:	return "NotEqual";
			case ECompareFunc::GEqual	:	return "GreaterEqual";
			case ECompareFunc::Always	:	return "Always";
		}
		RETURN_ERR( "invalid compare function type", "unknown" );
	}
	

	
//-----------------------------------------------------------------------------//
// EStencilOp
	
	ND_ inline StringCRef EStencilOp::ToString (type value)
	{
		switch ( value )
		{
			case EStencilOp::Keep		:	return "Keep";
			case EStencilOp::Zero		:	return "Zero";
			case EStencilOp::Replace	:	return "Replace";
			case EStencilOp::Incr		:	return "Increment";
			case EStencilOp::IncrWrap	:	return "IncrementWrao";
			case EStencilOp::Decr		:	return "Decrement";
			case EStencilOp::DecrWrap	:	return "DecrementWrap";
			case EStencilOp::Invert		:	return "Invert";
		}
		RETURN_ERR( "invalid stencil operation type", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EPolygonMode
	
	ND_ inline StringCRef EPolygonMode::ToString (type value)
	{
		switch ( value )
		{
			case EPolygonMode::Point	:	return "Point";
			case EPolygonMode::Line		:	return "Line";
			case EPolygonMode::Fill		:	return "Fill";
		}
		RETURN_ERR( "invalid polygon mode", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EPrimitive
	
	ND_ inline StringCRef EPrimitive::ToString (type value)
	{
		switch ( value )
		{
			case EPrimitive::Point			:	return "Point";
			case EPrimitive::LineList		:	return "LineList";
			case EPrimitive::LineStrip		:	return "LineStrip";
			case EPrimitive::TriangleList	:	return "TriangleList";
			case EPrimitive::TriangleStrip	:	return "TriangleStrip";
			case EPrimitive::Patch			:	return "Patch";
		}
		RETURN_ERR( "invalid primitive type", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EPolygonFace
	
	ND_ inline StringCRef EPolygonFace::ToString (type value)
	{
		switch ( value )
		{
			case EPolygonFace::None			:	return "None";
			case EPolygonFace::Front		:	return "Front";
			case EPolygonFace::Back			:	return "Back";
			case EPolygonFace::FontAndBack	:	return "FontAndBack";
		}
		RETURN_ERR( "invalid polygon face", "unknown" );
	}
	


//-----------------------------------------------------------------------------//
// EFilter
	
	ND_ inline StringCRef EFilter::ToString (type value)
	{
		switch ( value )
		{
			case EFilter::MinMagMipNearest					:	return "MinMagMipNearest";
			case EFilter::MinMagNearest_MipLinear			:	return "MinMagNearest_MipLinear";
			case EFilter::MinNearest_MagLinear_MipNearest	:	return "MinNearest_MagLinear_MipNearest";
			case EFilter::MinNearest_MagMipLinear			:	return "MinNearest_MagMipLinear";
			case EFilter::MinLinear_MagMipNearest			:	return "MinLinear_MagMipNearest";
			case EFilter::MinLinear_MagNearest_MipLinear	:	return "MinLinear_MagNearest_MipLinear";
			case EFilter::MinMagLinear_MipNearest			:	return "MinMagLinear_MipNearest";
			case EFilter::MinMagMipLinear					:	return "MinMagMipLinear";
			case EFilter::Anisotropic_2						:	return "Anisotropic_2";
			case EFilter::Anisotropic_4						:	return "Anisotropic_4";
			case EFilter::Anisotropic_8						:	return "Anisotropic_8";
			case EFilter::Anisotropic_12					:	return "Anisotropic_12";
			case EFilter::Anisotropic_16					:	return "Anisotropic_16";
		}
		RETURN_ERR( "invalid filter type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// EAddressMode
	
	ND_ inline StringCRef EAddressMode::ToString (type value)
	{
		switch ( value )
		{
			case EAddressMode::ClampToEdge		:	return "ClampToEdge";
			case EAddressMode::ClampToBorder	:	return "ClampToBorder";
			case EAddressMode::Repeat			:	return "Repeat";
			case EAddressMode::MirroredRepeat	:	return "MirroredRepeat";
			case EAddressMode::MirroredClamp	:	return "MirroredClamp";
			case EAddressMode::ClampUnnorm		:	return "ClampUnnorm";
		}
		RETURN_ERR( "invalid wrap mode", "unknown" );
	}
	


//-----------------------------------------------------------------------------//
// ESamplerBorderColor
	
	ND_ inline String ESamplerBorderColor::ToString (bits value)
	{
		String	str;

		if ( value[ ESamplerBorderColor::Int ] )
			str << "Int";
		else
			str << "Float";

		if ( value[ ESamplerBorderColor::Black ] )
			str << "Black";
		else
		if ( value[ ESamplerBorderColor::White ] )
			str << "White";
		else
			str << "Transparent";

		return str;
	}



//-----------------------------------------------------------------------------//
// EShader
	
	ND_ inline StringCRef EShader::ToString (type value)
	{
		switch ( value )
		{
			case EShader::Vertex			:	return "Vertex";
			case EShader::TessControl		:	return "TessellationControl";
			case EShader::TessEvaluation	:	return "TessellationEvaluation";
			case EShader::Geometry			:	return "Geometry";
			case EShader::Fragment			:	return "Fragment";
			case EShader::Compute			:	return "Compute";
		}
		RETURN_ERR( "invalid shader type", "unknown" );
	}
	


//-----------------------------------------------------------------------------//
// EShaderMemoryModel
	
	ND_ inline StringCRef EShaderMemoryModel::ToString (type value)
	{
		switch ( value )
		{
			case EShaderMemoryModel::Default :
			case EShaderMemoryModel::Coherent :		return "Coherent";
			case EShaderMemoryModel::Volatile :		return "Volatile";
			case EShaderMemoryModel::Restrict :		return "Restrict";
			case EShaderMemoryModel::ReadOnly :		return "ReadOnly";
			case EShaderMemoryModel::WriteOnly :	return "WriteOnly";
		}
		RETURN_ERR( "invalid memory model!", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EImage
	
	ND_ inline StringCRef EImage::ToString (type value)
	{
		switch ( value )
		{
			case EImage::Tex1D			: return "Image1D";
			case EImage::Tex1DArray		: return "Image1DArray";
			case EImage::Tex2D			: return "Image2D";
			case EImage::Tex2DArray		: return "Image2DArray";
			case EImage::Tex2DMS		: return "Image2DMultisample";
			case EImage::Tex2DMSArray	: return "Image2DArrayMultisample";
			case EImage::TexCube		: return "ImageCubeMap";
			case EImage::TexCubeArray	: return "ImageCubeMapArray";
			case EImage::Tex3D			: return "Image3D";
			case EImage::Buffer			: return "ImageBuffer";
		}
		RETURN_ERR( "unknown texture type!" );
	}

	

//-----------------------------------------------------------------------------//
// EImageLayout
	
	ND_ inline StringCRef  EImageLayout::ToString (type value)
	{
		switch ( value )
		{
			case EImageLayout::Undefined :						return "Undefined";
			case EImageLayout::General :						return "General";
			case EImageLayout::ColorAttachmentOptimal :			return "ColorAttachmentOptimal";
			case EImageLayout::DepthStencilAttachmentOptimal :	return "DepthStencilAttachmentOptimal";
			case EImageLayout::DepthStencilReadOnlyOptimal :	return "DepthStencilReadOnlyOptimal";
			case EImageLayout::ShaderReadOnlyOptimal :			return "ShaderReadOnlyOptimal";
			case EImageLayout::TransferSrcOptimal :				return "TransferSrcOptimal";
			case EImageLayout::TransferDstOptimal :				return "TransferDstOptimal";
			case EImageLayout::Preinitialized :					return "Preinitialized";
			case EImageLayout::PresentSrc :						return "PresentSrc";
		}
		RETURN_ERR( "unknown image layout type!" );
	}
	


//-----------------------------------------------------------------------------//
// EPipelineStage
	
	ND_ inline StringCRef  EPipelineStage::ToString (type value)
	{
		switch ( value )
		{
			case EPipelineStage::TopOfPipe :				return "TopOfPipe";
			case EPipelineStage::DrawIndirect :				return "DrawIndirect";
			case EPipelineStage::VertexInput :				return "VertexInput";
			case EPipelineStage::VertexShader :				return "VertexShader";
			case EPipelineStage::TessControlShader :		return "TessControlShader";
			case EPipelineStage::TessEvaluationShader :		return "TessEvaluationShader";
			case EPipelineStage::GeometryShader :			return "GeometryShader";
			case EPipelineStage::FragmentShader :			return "FragmentShader";
			case EPipelineStage::EarlyFragmentTests :		return "EarlyFragmentTests";
			case EPipelineStage::LateFragmentTests :		return "LateFragmentTests";
			case EPipelineStage::ColorAttachmentOutput :	return "ColorAttachmentOutput";
			case EPipelineStage::ComputeShader :			return "ComputeShader";
			case EPipelineStage::Transfer :					return "Transfer";
			case EPipelineStage::BottomOfPipe :				return "BottomOfPipe";
			case EPipelineStage::Host :						return "Host";
		}
		RETURN_ERR( "unknown pipeline stage!" );
	}

	ND_ inline String  EPipelineStage::ToString (bits values)
	{
		String	str;

		FOR( i, values )
		{
			const auto t = EPipelineStage::type(i);

			if ( not values[t] )
				continue;

			str << (str.Empty() ? "" : " | ") << ToString( t );
		}
		return str;
	}
	


//-----------------------------------------------------------------------------//
// EPipelineAccess
	
	ND_ inline StringCRef  EPipelineAccess::ToString (type value)
	{
		switch ( value )
		{
			case EPipelineAccess::IndirectCommandRead :			return "IndirectCommandRead";
			case EPipelineAccess::IndexRead :					return "IndexRead";
			case EPipelineAccess::VertexAttributeRead :			return "VertexAttributeRead";
			case EPipelineAccess::UniformRead :					return "UniformRead";
			case EPipelineAccess::InputAttachmentRead :			return "InputAttachmentRead";
			case EPipelineAccess::ShaderRead :					return "ShaderRead";
			case EPipelineAccess::ShaderWrite :					return "ShaderWrite";
			case EPipelineAccess::ColorAttachmentRead :			return "ColorAttachmentRead";
			case EPipelineAccess::ColorAttachmentWrite :		return "ColorAttachmentWrite";
			case EPipelineAccess::DepthStencilAttachmentRead :	return "DepthStencilAttachmentRead";
			case EPipelineAccess::DepthStencilAttachmentWrite :	return "DepthStencilAttachmentWrite";
			case EPipelineAccess::TransferRead :				return "TransferRead";
			case EPipelineAccess::TransferWrite :				return "TransferWrite";
			case EPipelineAccess::HostRead :					return "HostRead";
			case EPipelineAccess::HostWrite :					return "HostWrite";
			case EPipelineAccess::MemoryRead :					return "MemoryRead";
			case EPipelineAccess::MemoryWrite :					return "MemoryWrite";
		}
		RETURN_ERR( "unknown access type!" );
	}

	ND_ inline String  EPipelineAccess::ToString (bits values)
	{
		String	str;

		FOR( i, values )
		{
			const auto t = EPipelineAccess::type(i);

			if ( not values[t] )
				continue;

			str << (str.Empty() ? "" : " | ") << ToString( t );
		}
		return str;
	}


//-----------------------------------------------------------------------------//
// EPixelFormat
	
	ND_ inline StringCRef  EPixelFormat::ToString (type value)
	{
		switch ( value )
		{
			case RGBA16_SNorm : return "RGBA16_SNorm";
			case RGBA8_SNorm : return "RGBA8_SNorm";
			case RGB16_SNorm : return "RGB16_SNorm";
			case RGB8_SNorm : return "RGB8_SNorm";
			case RG16_SNorm : return "RG16_SNorm";
			case RG8_SNorm : return "RG8_SNorm";
			case R16_SNorm : return "R16_SNorm";
			case R8_SNorm : return "R8_SNorm";
			case RGBA16_UNorm : return "RGBA16_UNorm";
			case RGBA8_UNorm : return "RGBA8_UNorm";
			case RGB16_UNorm : return "RGB16_UNorm";
			case RGB8_UNorm : return "RGB8_UNorm";
			case RG16_UNorm : return "RG16_UNorm";
			case RG8_UNorm : return "RG8_UNorm";
			case R16_UNorm : return "R16_UNorm";
			case R8_UNorm : return "R8_UNorm";
			case RGB10_A2_UNorm : return "RGB10_A2_UNorm";
			case RGBA4_UNorm : return "RGBA4_UNorm";
			case RGB5_A1_UNorm : return "RGB5_A1_UNorm";
			case RGB_5_6_5_UNorm : return "RGB_5_6_5_UNorm";
			case BGR8_UNorm : return "BGR8_UNorm";
			case BGRA8_UNorm : return "BGRA8_UNorm";
			case sRGB8 : return "sRGB8";
			case sRGB8_A8 : return "sRGB8_A8";
			case R8I : return "R8I";
			case RG8I : return "RG8I";
			case RGB8I : return "RGB8I";
			case RGBA8I : return "RGBA8I";
			case R16I : return "R16I";
			case RG16I : return "RG16I";
			case RGB16I : return "RGB16I";
			case RGBA16I : return "RGBA16I";
			case R32I : return "R32I";
			case RG32I : return "RG32I";
			case RGB32I : return "RGB32I";
			case RGBA32I : return "RGBA32I";
			case R8U : return "R8U";
			case RG8U : return "RG8U";
			case RGB8U : return "RGB8U";
			case RGBA8U : return "RGBA8U";
			case R16U : return "R16U";
			case RG16U : return "RG16U";
			case RGB16U : return "RGB16U";
			case RGBA16U : return "RGBA16U";
			case R32U : return "R32U";
			case RG32U : return "RG32U";
			case RGB32U : return "RGB32U";
			case RGBA32U : return "RGBA32U";
			case RGB10_A2U : return "RGB10_A2U";
			case R16F : return "R16F";
			case RG16F : return "RG16F";
			case RGB16F : return "RGB16F";
			case RGBA16F : return "RGBA16F";
			case R32F : return "R32F";
			case RG32F : return "RG32F";
			case RGB32F : return "RGB32F";
			case RGBA32F : return "RGBA32F";
			case RGB_11_11_10F : return "RGB_11_11_10F";
			case Depth16 : return "Depth16";
			case Depth24 : return "Depth24";
			case Depth32 : return "Depth32";
			case Depth32F : return "Depth32F";
			case Depth16_Stencil8 : return "Depth16_Stencil8";
			case Depth24_Stencil8 : return "Depth24_Stencil8";
			case Depth32F_Stencil8 : return "Depth32F_Stencil8";
			case BC1_RGB8_UNorm : return "BC1_RGB8_UNorm";
			case BC1_RGB8_A1_UNorm : return "BC1_RGB8_A1_UNorm";
			case BC2_RGBA8_UNorm : return "BC2_RGBA8_UNorm";
			case BC3_RGBA8_UNorm : return "BC3_RGBA8_UNorm";
			case BC4_RED8_SNorm : return "BC4_RED8_SNorm";
			case BC4_RED8_UNorm : return "BC4_RED8_UNorm";
			case BC5_RG8_SNorm : return "BC5_RG8_SNorm";
			case BC5_RG8_UNorm : return "BC5_RG8_UNorm";
			case BC7_RGBA8_UNorm : return "BC7_RGBA8_UNorm";
			case BC7_SRGB8_A8_UNorm : return "BC7_SRGB8_A8_UNorm";
			case BC6H_RGB16F : return "BC6H_RGB16F";
			case BC6H_RGB16F_Unsigned : return "BC6H_RGB16F_Unsigned";
			case ETC2_RGB8_UNorm : return "ETC2_RGB8_UNorm";
			case ECT2_SRGB8_UNorm : return "ECT2_SRGB8_UNorm";
			case ETC2_RGB8_A1_UNorm : return "ETC2_RGB8_A1_UNorm";
			case ETC2_SRGB8_A1_UNorm : return "ETC2_SRGB8_A1_UNorm";
			case ETC2_RGBA8_UNorm : return "ETC2_RGBA8_UNorm";
			case ETC2_SRGB8_A8_UNorm : return "ETC2_SRGB8_A8_UNorm";
			case EAC_R11_SNorm : return "EAC_R11_SNorm";
			case EAC_R11_UNorm : return "EAC_R11_UNorm";
			case EAC_RG11_SNorm : return "EAC_RG11_SNorm";
			case EAC_RG11_UNorm : return "EAC_RG11_UNorm";
			case ASTC_RGBA_4x4 : return "ASTC_RGBA_4x4";
			case ASTC_RGBA_5x4 : return "ASTC_RGBA_5x4";
			case ASTC_RGBA_5x5 : return "ASTC_RGBA_5x5";
			case ASTC_RGBA_6x5 : return "ASTC_RGBA_6x5";
			case ASTC_RGBA_6x6 : return "ASTC_RGBA_6x6";
			case ASTC_RGBA_8x5 : return "ASTC_RGBA_8x5";
			case ASTC_RGBA_8x6 : return "ASTC_RGBA_8x6";
			case ASTC_RGBA_8x8 : return "ASTC_RGBA_8x8";
			case ASTC_RGBA_10x5 : return "ASTC_RGBA_10x5";
			case ASTC_RGBA_10x6 : return "ASTC_RGBA_10x6";
			case ASTC_RGBA_10x8 : return "ASTC_RGBA_10x8";
			case ASTC_RGBA_10x10 : return "ASTC_RGBA_10x10";
			case ASTC_RGBA_12x10 : return "ASTC_RGBA_12x10";
			case ASTC_RGBA_12x12 : return "ASTC_RGBA_12x12";
			case ASTC_SRGB8_A8_4x4 : return "ASTC_SRGB8_A8_4x4";
			case ASTC_SRGB8_A8_5x4 : return "ASTC_SRGB8_A8_5x4";
			case ASTC_SRGB8_A8_5x5 : return "ASTC_SRGB8_A8_5x5";
			case ASTC_SRGB8_A8_6x5 : return "ASTC_SRGB8_A8_6x5";
			case ASTC_SRGB8_A8_6x6 : return "ASTC_SRGB8_A8_6x6";
			case ASTC_SRGB8_A8_8x5 : return "ASTC_SRGB8_A8_8x5";
			case ASTC_SRGB8_A8_8x6 : return "ASTC_SRGB8_A8_8x6";
			case ASTC_SRGB8_A8_8x8 : return "ASTC_SRGB8_A8_8x8";
			case ASTC_SRGB8_A8_10x5 : return "ASTC_SRGB8_A8_10x5";
			case ASTC_SRGB8_A8_10x6 : return "ASTC_SRGB8_A8_10x6";
			case ASTC_SRGB8_A8_10x8 : return "ASTC_SRGB8_A8_10x8";
			case ASTC_SRGB8_A8_10x10 : return "ASTC_SRGB8_A8_10x10";
			case ASTC_SRGB8_A8_12x10 : return "ASTC_SRGB8_A8_12x10";
			case ASTC_SRGB8_A8_12x12 : return "ASTC_SRGB8_A8_12x12";
		}
		RETURN_ERR( "unknown pixel format " << String().FormatI( value, 16 ) );
	}

}	// Platforms
}	// Engine
