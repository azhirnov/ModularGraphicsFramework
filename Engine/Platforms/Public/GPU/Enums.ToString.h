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

#include "Core/STL/Log/ToString.h"

namespace Engine
{
namespace Platforms
{
	
//-----------------------------------------------------------------------------//
// EBlendFunc
	
	ND_ inline StringCRef  EBlendFunc::ToString (type value)
	{
		switch ( value )
		{
			case Zero					:	return "Zero";
			case One					:	return "One";
			case SrcColor				:	return "SrcColor";
			case OneMinusSrcColor		:	return "OneMinusSrcColor";
			case DstColor				:	return "DstColor";
			case OneMinusDstColor		:	return "OneMinusDstColor";
			case SrcAlpha				:	return "SrcAlpha";
			case OneMinusSrcAlpha		:	return "OneMinusSrcAlpha";
			case DstAlpha				:	return "DstAlpha";
			case OneMinusDstAlpha		:	return "OneMinusDstAlpha";
			case ConstColor				:	return "ConstColor";
			case OneMinusConstColor		:	return "OneMinusConstColor";
			case ConstAlpha				:	return "ConstAlpha";
			case OneMinusConstAlpha		:	return "OneMinusConstAlpha";
			case SrcAlphaSaturate		:	return "SrcAlphaSaturate";
		}
		RETURN_ERR( "invalid blend function type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// EBlendEq
	
	ND_ inline StringCRef  EBlendEq::ToString (type value)
	{
		switch( value )
		{
			case Add	:	return "Add";
			case Sub	:	return "Sub";
			case RevSub	:	return "ReverseSub";
			case Min	:	return "Min";
			case Max	:	return "Max";
		}
		RETURN_ERR( "invalid blend equation type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// ELogicOp
	
	ND_ inline StringCRef  ELogicOp::ToString (type value)
	{
		switch( value )
		{
			case None		:	return "None";
			case Clear		:	return "Clear";
			case Set		:	return "Set";
			case Copy		:	return "Copy";
			case CopyInverted:	return "CopyInversed";
			case Noop		:	return "Noop";
			case Invert		:	return "Invert";
			case And		:	return "And";
			case NotAnd		:	return "NotAnd";
			case Or			:	return "Or";
			case NotOr		:	return "NotOr";
			case Xor		:	return "Xor";
			case Equiv		:	return "Equiv";
			case AndReverse	:	return "AndReverse";
			case AndInverted:	return "AndInverted";
			case OrReverse	:	return "OrReverse";
			case OrInverted	:	return "OrInverted";
		}
		RETURN_ERR( "invalid logical operation type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// ECompareFunc
	
	ND_ inline StringCRef  ECompareFunc::ToString (type value)
	{
		switch ( value )
		{
			case None		:	return "None";
			case Never		:	return "Never";
			case Less		:	return "Less";
			case Equal		:	return "Equal";
			case LEqual		:	return "LessEqual";
			case Greater	:	return "Greater";
			case NotEqual	:	return "NotEqual";
			case GEqual		:	return "GreaterEqual";
			case Always		:	return "Always";
		}
		RETURN_ERR( "invalid compare function type", "unknown" );
	}
	

	
//-----------------------------------------------------------------------------//
// EStencilOp
	
	ND_ inline StringCRef  EStencilOp::ToString (type value)
	{
		switch ( value )
		{
			case Keep		:	return "Keep";
			case Zero		:	return "Zero";
			case Replace	:	return "Replace";
			case Incr		:	return "Increment";
			case IncrWrap	:	return "IncrementWrao";
			case Decr		:	return "Decrement";
			case DecrWrap	:	return "DecrementWrap";
			case Invert		:	return "Invert";
		}
		RETURN_ERR( "invalid stencil operation type", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EPolygonMode
	
	ND_ inline StringCRef  EPolygonMode::ToString (type value)
	{
		switch ( value )
		{
			case Point	:	return "Point";
			case Line	:	return "Line";
			case Fill	:	return "Fill";
		}
		RETURN_ERR( "invalid polygon mode", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EPrimitive
	
	ND_ inline StringCRef  EPrimitive::ToString (type value)
	{
		switch ( value )
		{
			case Point			:	return "Point";
			case LineList		:	return "LineList";
			case LineStrip		:	return "LineStrip";
			case TriangleList	:	return "TriangleList";
			case TriangleStrip	:	return "TriangleStrip";
			case Patch			:	return "Patch";
		}
		RETURN_ERR( "invalid primitive type", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EPolygonFace
	
	ND_ inline StringCRef  EPolygonFace::ToString (type value)
	{
		switch ( value )
		{
			case None		:	return "None";
			case Front		:	return "Front";
			case Back		:	return "Back";
			case FontAndBack:	return "FontAndBack";
		}
		RETURN_ERR( "invalid polygon face", "unknown" );
	}
	


//-----------------------------------------------------------------------------//
// EFilter
	
	ND_ inline StringCRef  EFilter::ToString (type value)
	{
		switch ( value )
		{
			case MinMagMipNearest				:	return "MinMagMipNearest";
			case MinMagNearest_MipLinear		:	return "MinMagNearest_MipLinear";
			case MinNearest_MagLinear_MipNearest:	return "MinNearest_MagLinear_MipNearest";
			case MinNearest_MagMipLinear		:	return "MinNearest_MagMipLinear";
			case MinLinear_MagMipNearest		:	return "MinLinear_MagMipNearest";
			case MinLinear_MagNearest_MipLinear	:	return "MinLinear_MagNearest_MipLinear";
			case MinMagLinear_MipNearest		:	return "MinMagLinear_MipNearest";
			case MinMagMipLinear				:	return "MinMagMipLinear";
			case Anisotropic_2					:	return "Anisotropic_2";
			case Anisotropic_4					:	return "Anisotropic_4";
			case Anisotropic_8					:	return "Anisotropic_8";
			case Anisotropic_12					:	return "Anisotropic_12";
			case Anisotropic_16					:	return "Anisotropic_16";
		}
		RETURN_ERR( "invalid filter type", "unknown" );
	}



//-----------------------------------------------------------------------------//
// EAddressMode
	
	ND_ inline StringCRef  EAddressMode::ToString (type value)
	{
		switch ( value )
		{
			case ClampToEdge	:	return "ClampToEdge";
			case ClampToBorder	:	return "ClampToBorder";
			case Repeat			:	return "Repeat";
			case MirroredRepeat	:	return "MirroredRepeat";
			case MirroredClamp	:	return "MirroredClamp";
			case ClampUnnorm	:	return "ClampUnnorm";
		}
		RETURN_ERR( "invalid wrap mode", "unknown" );
	}
	


//-----------------------------------------------------------------------------//
// ESamplerBorderColor
	
	ND_ inline String  ESamplerBorderColor::ToString (bits value)
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
// ESubpassDependency
	
	ND_ inline StringCRef  ESubpassDependency::ToString (type value)
	{
		switch ( value )
		{
			case ByRegion :		return "ByRegion";
			case ViewLocal :	return "ViewLocal";
			case DeviceGroup :	return "DeviceGroup";
		}
		RETURN_ERR( "unknown subpass dependency!" );
	}

	ND_ inline String  ESubpassDependency::ToString (bits values)
	{
		String	str;

		FOR( i, values )
		{
			const auto t = type(i);

			if ( not values[t] )
				continue;

			str << (str.Empty() ? "" : " | ") << ToString( t );
		}
		return str;
	}



//-----------------------------------------------------------------------------//
// EShader
	
	ND_ inline StringCRef  EShader::ToString (type value)
	{
		switch ( value )
		{
			case Vertex			:	return "Vertex";
			case TessControl	:	return "TessellationControl";
			case TessEvaluation	:	return "TessellationEvaluation";
			case Geometry		:	return "Geometry";
			case Fragment		:	return "Fragment";
			case Compute		:	return "Compute";
		}
		RETURN_ERR( "invalid shader type", "unknown" );
	}
	


//-----------------------------------------------------------------------------//
// EShaderMemoryModel
	
	ND_ inline StringCRef  EShaderMemoryModel::ToString (type value)
	{
		switch ( value )
		{
			case Default :
			case Coherent :		return "Coherent";
			case Volatile :		return "Volatile";
			case Restrict :		return "Restrict";
			case ReadOnly :		return "ReadOnly";
			case WriteOnly :	return "WriteOnly";
		}
		RETURN_ERR( "invalid memory model!", "unknown" );
	}


	
//-----------------------------------------------------------------------------//
// EImage
	
	ND_ inline StringCRef  EImage::ToString (type value)
	{
		switch ( value )
		{
			case Tex1D			: return "Image1D";
			case Tex1DArray		: return "Image1DArray";
			case Tex2D			: return "Image2D";
			case Tex2DArray		: return "Image2DArray";
			case Tex2DMS		: return "Image2DMultisample";
			case Tex2DMSArray	: return "Image2DArrayMultisample";
			case TexCube		: return "ImageCubeMap";
			case TexCubeArray	: return "ImageCubeMapArray";
			case Tex3D			: return "Image3D";
			case Buffer			: return "ImageBuffer";
		}
		RETURN_ERR( "unknown texture type!" );
	}

	

//-----------------------------------------------------------------------------//
// EImageLayout
	
	ND_ inline StringCRef  EImageLayout::ToString (type value)
	{
		switch ( value )
		{
			case Undefined :					return "Undefined";
			case General :						return "General";
			case ColorAttachmentOptimal :		return "ColorAttachmentOptimal";
			case DepthStencilAttachmentOptimal :return "DepthStencilAttachmentOptimal";
			case DepthStencilReadOnlyOptimal :	return "DepthStencilReadOnlyOptimal";
			case ShaderReadOnlyOptimal :		return "ShaderReadOnlyOptimal";
			case TransferSrcOptimal :			return "TransferSrcOptimal";
			case TransferDstOptimal :			return "TransferDstOptimal";
			case Preinitialized :				return "Preinitialized";
			case PresentSrc :					return "PresentSrc";
		}
		RETURN_ERR( "unknown image layout type!" );
	}
	
	

//-----------------------------------------------------------------------------//
// EAttachmentLoadOp

	ND_ inline StringCRef  EAttachmentLoadOp::ToString (type value)
	{
		switch ( value )
		{
			case Invalidate :	return "Invalidate";
			case Load :			return "Load";
			case Clear :		return "Clear";
		}
		RETURN_ERR( "unknown attachment load op!" );
	}
	
	

//-----------------------------------------------------------------------------//
// EAttachmentStoreOp

	ND_ inline StringCRef  EAttachmentStoreOp::ToString (type value)
	{
		switch ( value )
		{
			case Invalidate :	return "Invalidate";
			case Store :		return "Store";
		}
		RETURN_ERR( "unknown attachment store op!" );
	}



//-----------------------------------------------------------------------------//
// EPipelineStage
	
	ND_ inline StringCRef  EPipelineStage::ToString (type value)
	{
		switch ( value )
		{
			case TopOfPipe :				return "TopOfPipe";
			case DrawIndirect :				return "DrawIndirect";
			case VertexInput :				return "VertexInput";
			case VertexShader :				return "VertexShader";
			case TessControlShader :		return "TessControlShader";
			case TessEvaluationShader :		return "TessEvaluationShader";
			case GeometryShader :			return "GeometryShader";
			case FragmentShader :			return "FragmentShader";
			case EarlyFragmentTests :		return "EarlyFragmentTests";
			case LateFragmentTests :		return "LateFragmentTests";
			case ColorAttachmentOutput :	return "ColorAttachmentOutput";
			case ComputeShader :			return "ComputeShader";
			case Transfer :					return "Transfer";
			case BottomOfPipe :				return "BottomOfPipe";
			case Host :						return "Host";
		}
		RETURN_ERR( "unknown pipeline stage!" );
	}

	ND_ inline String  EPipelineStage::ToString (bits values)
	{
		String	str;

		FOR( i, values )
		{
			const auto t = type(i);

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
			case IndirectCommandRead :			return "IndirectCommandRead";
			case IndexRead :					return "IndexRead";
			case VertexAttributeRead :			return "VertexAttributeRead";
			case UniformRead :					return "UniformRead";
			case InputAttachmentRead :			return "InputAttachmentRead";
			case ShaderRead :					return "ShaderRead";
			case ShaderWrite :					return "ShaderWrite";
			case ColorAttachmentRead :			return "ColorAttachmentRead";
			case ColorAttachmentWrite :			return "ColorAttachmentWrite";
			case DepthStencilAttachmentRead :	return "DepthStencilAttachmentRead";
			case DepthStencilAttachmentWrite :	return "DepthStencilAttachmentWrite";
			case TransferRead :					return "TransferRead";
			case TransferWrite :				return "TransferWrite";
			case HostRead :						return "HostRead";
			case HostWrite :					return "HostWrite";
			case MemoryRead :					return "MemoryRead";
			case MemoryWrite :					return "MemoryWrite";
		}
		RETURN_ERR( "unknown access type!" );
	}

	ND_ inline String  EPipelineAccess::ToString (bits values)
	{
		String	str;

		FOR( i, values )
		{
			const auto t = type(i);

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
