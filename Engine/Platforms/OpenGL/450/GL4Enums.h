// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/OpenGL/450/gl4.h"
#include "Engine/Platforms/Public/GPU/BufferEnums.h"
#include "Engine/Platforms/Public/GPU/CommandEnums.h"
#include "Engine/Platforms/Public/GPU/MemoryEnums.h"
#include "Engine/Platforms/Public/GPU/MultiSamples.h"
#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Public/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Public/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Public/GPU/SamplerEnums.h"
#include "Engine/Platforms/Public/GPU/ShaderEnums.h"
#include "Engine/Platforms/Public/GPU/ImageEnums.h"
#include "Engine/Platforms/Public/GPU/VertexEnums.h"
#include "Engine/Platforms/Public/GPU/ObjectEnums.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace Platforms;
	
	enum GL4InternalPixelFormat {};
	enum GL4PixelFormat {};
	enum GL4PixelType {};
	enum GL4TextureTarget {};
	enum GL4Shader {};
	enum GL4ShaderBits {};
	enum GL4Primitive {};
	enum GL4Index {};
	enum GL4VertexAttribType {};
	enum GL4BlendFunc {};
	enum GL4BlendEq {};
	enum GL4LogicOp {};
	enum GL4CompareFunc {};
	enum GL4StencilOp {};
	enum GL4MinFilter {};
	enum GL4MagFilter {};
	enum GL4AddressMode {};
	enum GL4Object {};
	enum GL4BufferUsage {};

	
/*
=================================================
	Shader
=================================================
*/
	inline GL4Shader GL4Enum (EShader::type value)
	{
		switch ( value )
		{
			case EShader::Vertex			:	return (GL4Shader) gl::GL_VERTEX_SHADER;
			case EShader::TessControl		:	return (GL4Shader) gl::GL_TESS_CONTROL_SHADER;
			case EShader::TessEvaluation	:	return (GL4Shader) gl::GL_TESS_EVALUATION_SHADER;
			case EShader::Geometry			:	return (GL4Shader) gl::GL_GEOMETRY_SHADER;
			case EShader::Fragment			:	return (GL4Shader) gl::GL_FRAGMENT_SHADER;
			case EShader::Compute			:	return (GL4Shader) gl::GL_COMPUTE_SHADER;
		}

		RETURN_ERR( "invalid shader type", GL4Shader() );
	}

/*
=================================================
	ShaderBits
=================================================
*/
	inline GL4ShaderBits GL4Enum (EShader::bits values)
	{
		gl::GLbitfield	flags = 0;

		FOR( i, values )
		{
			const auto	t = EShader::type(i);

			if ( not values[t] )
				continue;

			switch ( t )
			{
				case EShader::Vertex :			flags |= gl::GL_VERTEX_SHADER_BIT;				break;
				case EShader::TessControl :		flags |= gl::GL_TESS_CONTROL_SHADER_BIT;		break;
				case EShader::TessEvaluation :	flags |= gl::GL_TESS_EVALUATION_SHADER_BIT;		break;
				case EShader::Geometry :		flags |= gl::GL_GEOMETRY_SHADER_BIT;			break;
				case EShader::Fragment :		flags |= gl::GL_FRAGMENT_SHADER_BIT;			break;
				case EShader::Compute :			flags |= gl::GL_COMPUTE_SHADER_BIT;				break;
				default :						RETURN_ERR( "invalid shader type", GL4ShaderBits() );
			}
		}
		return GL4ShaderBits(flags);
	}

/*
=================================================
	TextureTarget
=================================================
*/
	inline GL4TextureTarget GL4Enum (EImage::type value)
	{
		switch ( value )
		{
			case EImage::Tex1D			:	return (GL4TextureTarget) gl::GL_TEXTURE_1D;
			case EImage::Tex1DArray		:	return (GL4TextureTarget) gl::GL_TEXTURE_1D_ARRAY;
			case EImage::Tex2D			:	return (GL4TextureTarget) gl::GL_TEXTURE_2D;
			case EImage::Tex2DArray		:	return (GL4TextureTarget) gl::GL_TEXTURE_2D_ARRAY;
			case EImage::Tex2DMS		:	return (GL4TextureTarget) gl::GL_TEXTURE_2D_MULTISAMPLE;
			case EImage::Tex2DMSArray	:	return (GL4TextureTarget) gl::GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
			case EImage::TexCube		:	return (GL4TextureTarget) gl::GL_TEXTURE_CUBE_MAP;
			case EImage::TexCubeArray	:	return (GL4TextureTarget) gl::GL_TEXTURE_CUBE_MAP_ARRAY;
			case EImage::Tex3D			:	return (GL4TextureTarget) gl::GL_TEXTURE_3D;
			case EImage::Buffer			:	return (GL4TextureTarget) gl::GL_TEXTURE_BUFFER;
		}

		RETURN_ERR( "invalid texture type", GL4TextureTarget() );
	}
	
/*
=================================================
	Primitive
=================================================
*/
	inline GL4Primitive GL4Enum (EPrimitive::type value)
	{
		switch ( value )
		{
			case EPrimitive::Point			:	return (GL4Primitive) gl::GL_POINTS;
			case EPrimitive::LineList		:	return (GL4Primitive) gl::GL_LINES;
			case EPrimitive::LineStrip		:	return (GL4Primitive) gl::GL_LINE_STRIP;
			case EPrimitive::TriangleList	:	return (GL4Primitive) gl::GL_TRIANGLES;
			case EPrimitive::TriangleStrip	:	return (GL4Primitive) gl::GL_TRIANGLE_STRIP;
			case EPrimitive::Patch			:	return (GL4Primitive) gl::GL_PATCHES;
		}

		RETURN_ERR( "invalid primitive type", GL4Primitive(0) );
	}

/*
=================================================
	IndexType
=================================================
*/
	inline GL4Index GL4Enum (EIndex::type value)
	{
		switch ( value )
		{
			case EIndex::UShort	:	return (GL4Index) gl::GL_UNSIGNED_SHORT;
			case EIndex::UInt	:	return (GL4Index) gl::GL_UNSIGNED_INT;
		}

		RETURN_ERR( "invalid index type", GL4Index() );
	}

/*
=================================================
	LogicOp
=================================================
*/
	inline GL4LogicOp GL4Enum (ELogicOp::type value)
	{
		switch ( value )
		{
			case ELogicOp::Clear		: return (GL4LogicOp) gl::GL_CLEAR;
			case ELogicOp::Set			: return (GL4LogicOp) gl::GL_SET;
			case ELogicOp::Copy			: return (GL4LogicOp) gl::GL_COPY;
			case ELogicOp::CopyInverted	: return (GL4LogicOp) gl::GL_COPY_INVERTED;
			case ELogicOp::Noop			: return (GL4LogicOp) gl::GL_NOOP;
			case ELogicOp::Invert		: return (GL4LogicOp) gl::GL_INVERT;
			case ELogicOp::And			: return (GL4LogicOp) gl::GL_AND;
			case ELogicOp::NotAnd		: return (GL4LogicOp) gl::GL_NAND;
			case ELogicOp::Or			: return (GL4LogicOp) gl::GL_OR;
			case ELogicOp::NotOr		: return (GL4LogicOp) gl::GL_NOR;
			case ELogicOp::Xor			: return (GL4LogicOp) gl::GL_XOR;
			case ELogicOp::Equiv		: return (GL4LogicOp) gl::GL_EQUIV;
			case ELogicOp::AndReverse	: return (GL4LogicOp) gl::GL_AND_REVERSE;
			case ELogicOp::AndInverted	: return (GL4LogicOp) gl::GL_AND_INVERTED;
			case ELogicOp::OrReverse	: return (GL4LogicOp) gl::GL_OR_REVERSE;
			case ELogicOp::OrInverted	: return (GL4LogicOp) gl::GL_OR_INVERTED;
		}

		RETURN_ERR( "invalid logical operation", GL4LogicOp() );
	}

/*
=================================================
	BlendEq
=================================================
*/
	inline GL4BlendEq GL4Enum (EBlendEq::type value)
	{
		switch ( value )
		{
			case EBlendEq::Add		:	return (GL4BlendEq) gl::GL_FUNC_ADD;
			case EBlendEq::Sub		:	return (GL4BlendEq) gl::GL_FUNC_SUBTRACT;
			case EBlendEq::RevSub	:	return (GL4BlendEq) gl::GL_FUNC_REVERSE_SUBTRACT;
			case EBlendEq::Min		:	return (GL4BlendEq) gl::GL_MIN;
			case EBlendEq::Max		:	return (GL4BlendEq) gl::GL_MAX;
		}

		RETURN_ERR( "invalid blend equation", GL4BlendEq() );
	}

/*
=================================================
	BlendFunc
=================================================
*/
	inline GL4BlendFunc GL4Enum (EBlendFunc::type value)
	{
		switch ( value )
		{
			case EBlendFunc::Zero					:	return (GL4BlendFunc) gl::GL_ZERO;
			case EBlendFunc::One					:	return (GL4BlendFunc) gl::GL_ONE;
			case EBlendFunc::SrcColor				:	return (GL4BlendFunc) gl::GL_SRC_COLOR;
			case EBlendFunc::OneMinusSrcColor		:	return (GL4BlendFunc) gl::GL_ONE_MINUS_SRC_COLOR;
			case EBlendFunc::DstColor				:	return (GL4BlendFunc) gl::GL_DST_COLOR;
			case EBlendFunc::OneMinusDstColor		:	return (GL4BlendFunc) gl::GL_ONE_MINUS_DST_COLOR;
			case EBlendFunc::SrcAlpha				:	return (GL4BlendFunc) gl::GL_SRC_ALPHA;
			case EBlendFunc::OneMinusSrcAlpha		:	return (GL4BlendFunc) gl::GL_ONE_MINUS_SRC_ALPHA;
			case EBlendFunc::DstAlpha				:	return (GL4BlendFunc) gl::GL_DST_ALPHA;
			case EBlendFunc::OneMinusDstAlpha		:	return (GL4BlendFunc) gl::GL_ONE_MINUS_DST_ALPHA;
			case EBlendFunc::ConstColor				:	return (GL4BlendFunc) gl::GL_CONSTANT_COLOR;
			case EBlendFunc::OneMinusConstColor		:	return (GL4BlendFunc) gl::GL_ONE_MINUS_CONSTANT_COLOR;
			case EBlendFunc::ConstAlpha				:	return (GL4BlendFunc) gl::GL_CONSTANT_ALPHA;
			case EBlendFunc::OneMinusConstAlpha		:	return (GL4BlendFunc) gl::GL_ONE_MINUS_CONSTANT_ALPHA;
			case EBlendFunc::SrcAlphaSaturate		:	return (GL4BlendFunc) gl::GL_SRC_ALPHA_SATURATE;
		}

		RETURN_ERR( "invalid blend func", GL4BlendFunc() );
	}
	
/*
=================================================
	CompareFunc
=================================================
*/
	inline GL4CompareFunc GL4Enum (ECompareFunc::type value)
	{
		switch ( value )
		{
			case ECompareFunc::None		:	return (GL4CompareFunc) gl::GL_NONE;
			case ECompareFunc::Never	:	return (GL4CompareFunc) gl::GL_NEVER;
			case ECompareFunc::Less		:	return (GL4CompareFunc) gl::GL_LESS;
			case ECompareFunc::Equal	:	return (GL4CompareFunc) gl::GL_EQUAL;
			case ECompareFunc::LEqual	:	return (GL4CompareFunc) gl::GL_LEQUAL;
			case ECompareFunc::Greater	:	return (GL4CompareFunc) gl::GL_GREATER;
			case ECompareFunc::NotEqual	:	return (GL4CompareFunc) gl::GL_NOTEQUAL;
			case ECompareFunc::GEqual	:	return (GL4CompareFunc) gl::GL_GEQUAL;
			case ECompareFunc::Always	:	return (GL4CompareFunc) gl::GL_ALWAYS;
		}

		RETURN_ERR( "invalid compare function", GL4CompareFunc() );
	}
	
/*
=================================================
	StencilOp
=================================================
*/
	inline GL4StencilOp GL4Enum (EStencilOp::type value)
	{
		switch ( value )
		{
			case EStencilOp::Keep		:	return (GL4StencilOp) gl::GL_KEEP;
			case EStencilOp::Zero		:	return (GL4StencilOp) gl::GL_ZERO;
			case EStencilOp::Replace	:	return (GL4StencilOp) gl::GL_REPLACE;
			case EStencilOp::Incr		:	return (GL4StencilOp) gl::GL_INCR;
			case EStencilOp::IncrWrap	:	return (GL4StencilOp) gl::GL_INCR_WRAP;
			case EStencilOp::Decr		:	return (GL4StencilOp) gl::GL_DECR;
			case EStencilOp::DecrWrap	:	return (GL4StencilOp) gl::GL_DECR_WRAP;
			case EStencilOp::Invert		:	return (GL4StencilOp) gl::GL_INVERT;
		}

		RETURN_ERR( "invalid stencil op", GL4StencilOp() );
	}

/*
=================================================
	WrapMode
=================================================
*/
	inline GL4AddressMode GL4Enum (EAddressMode::type value)
	{
		switch ( value )
		{
			case EAddressMode::Clamp			:	return (GL4AddressMode) gl::GL_CLAMP_TO_EDGE;
			case EAddressMode::ClampToBorder	:	return (GL4AddressMode) gl::GL_CLAMP_TO_BORDER;
			case EAddressMode::MirroredRepeat	:	return (GL4AddressMode) gl::GL_MIRRORED_REPEAT;
			case EAddressMode::Repeat			:	return (GL4AddressMode) gl::GL_REPEAT;
			case EAddressMode::MirroredClamp	:	return (GL4AddressMode) gl::GL_MIRROR_CLAMP_TO_EDGE;
		}

		RETURN_ERR( "invalid address mode", GL4AddressMode() );
	}
	
	
/*
=================================================
	Filter
=================================================
*/
	inline bool GL4Enum (EFilter::type value, OUT GL4MinFilter &minFilter, OUT GL4MagFilter &magFilter)
	{
		static const uint	min_mask = (EFilter::_MIN_NEAREST | EFilter::_MIN_LINEAR | EFilter::_MIP_NEAREST | EFilter::_MIP_LINEAR);
		static const uint	mag_mask = (EFilter::_MAG_NEAREST | EFilter::_MAG_LINEAR);

		minFilter	= GL4MinFilter();
		magFilter	= GL4MagFilter();

		switch ( value & min_mask )
		{
			case EFilter::_MIN_NEAREST							:	minFilter = (GL4MinFilter) gl::GL_NEAREST;					break;
			case EFilter::_MIN_LINEAR							:	minFilter = (GL4MinFilter) gl::GL_LINEAR;					break;
			case EFilter::_MIN_NEAREST | EFilter::_MIP_NEAREST	:	minFilter = (GL4MinFilter) gl::GL_NEAREST_MIPMAP_NEAREST;	break;
			case EFilter::_MIN_LINEAR  | EFilter::_MIP_NEAREST	:	minFilter = (GL4MinFilter) gl::GL_LINEAR_MIPMAP_NEAREST;	break;
			case EFilter::_MIN_NEAREST | EFilter::_MIP_LINEAR	:	minFilter = (GL4MinFilter) gl::GL_NEAREST_MIPMAP_LINEAR;	break;
			case EFilter::_MIN_LINEAR  | EFilter::_MIP_LINEAR	:	minFilter = (GL4MinFilter) gl::GL_LINEAR_MIPMAP_LINEAR;		break;
			default												:	RETURN_ERR( "invalid min and mip filtering flags" );
		}

		switch ( value & mag_mask )
		{
			case EFilter::_MAG_NEAREST	:	magFilter = (GL4MagFilter) gl::GL_NEAREST;	break;
			case EFilter::_MAG_LINEAR	:	magFilter = (GL4MagFilter) gl::GL_LINEAR;	break;
			default						:	RETURN_ERR( "invalid mag filtering flag" );
		}
		return true;
	}
	
/*
=================================================
	BorderColor
=================================================
*/
	inline bool  GL4Enum (ESamplerBorderColor::bits value, OUT uint4 &intColor)
	{
		// int
		CHECK_ERR( value[ ESamplerBorderColor::Int ] );
		
		if ( value[ ESamplerBorderColor::Black ] )
			intColor = uint4(0u, 0u, 0u, UMax);
		else
		if ( value[ ESamplerBorderColor::White ] )
			intColor = uint4(uint(UMax));
		else
		//if ( value[ ESamplerBorderColor::Transparent ] )
			intColor = uint4(0);

		return true;
	}

	inline bool  GL4Enum (ESamplerBorderColor::bits value, OUT float4 &color)
	{
		// float
		CHECK_ERR( not value[ ESamplerBorderColor::Int ] );
		
		if ( value[ ESamplerBorderColor::Black ] )
			color = float4(0.0f, 0.0f, 0.0f, 1.0f);
		else
		if ( value[ ESamplerBorderColor::White ] )
			color = float4(1.0f);
		else
		//if ( value[ ESamplerBorderColor::Transparent ] )
			color = float4(0.0f);

		return true;
	}

/*
=================================================
	VertexAttribute
=================================================
*/
	inline bool GL4Enum (EVertexAttribute::type value, OUT GL4VertexAttribType &type, OUT uint &count, OUT bool &norm)
	{
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

		type	= GL4VertexAttribType();
		count	= 0;

		const bool	is_unsigned = EnumEq( value, _vtypeinfo::_UNSIGNED );

		count = ( (value & _vtypeinfo::_COL_MASK) >> _vtypeinfo::_COL_OFF );
		CHECK_ERR( count >= 1 and count <= 4 );

		norm = EnumEq( value, _vtypeinfo::_NORM );

		switch ( value & _vtypeinfo::_TYPE_MASK )
		{
			case _vtypeinfo::_BYTE		:	type = (GL4VertexAttribType) (is_unsigned ? gl::GL_UNSIGNED_BYTE  : gl::GL_BYTE);	break;
			case _vtypeinfo::_SHORT		:	type = (GL4VertexAttribType) (is_unsigned ? gl::GL_UNSIGNED_SHORT : gl::GL_SHORT);	break;
			case _vtypeinfo::_INT		:	type = (GL4VertexAttribType) (is_unsigned ? gl::GL_UNSIGNED_INT   : gl::GL_INT);	break;
			case _vtypeinfo::_HALF		:	type = (GL4VertexAttribType) gl::GL_HALF_FLOAT;										break;
			case _vtypeinfo::_FLOAT		:	type = (GL4VertexAttribType) gl::GL_FLOAT;											break;
			case _vtypeinfo::_DOUBLE	:	type = (GL4VertexAttribType) gl::GL_DOUBLE;											break;
			default						:	RETURN_ERR( "invalid attrib type" );
		}
		return true;
	}

/*
=================================================
	Object
=================================================
*/
	inline GL4Object GL4Enum (EGpuObject::type value)
	{
		switch ( value )
		{
			case EGpuObject::Buffer :		return (GL4Object) gl::GL_BUFFER;
			case EGpuObject::Image :		return (GL4Object) gl::GL_TEXTURE;
			case EGpuObject::ShaderModule :	return (GL4Object) gl::GL_SHADER;
			case EGpuObject::Pipeline :		return (GL4Object) gl::GL_PROGRAM_PIPELINE;
			case EGpuObject::Sampler :		return (GL4Object) gl::GL_SAMPLER;
			case EGpuObject::Framebuffer :	return (GL4Object) gl::GL_FRAMEBUFFER;
			case EGpuObject::VertexArray :	return (GL4Object) gl::GL_VERTEX_ARRAY;
			case EGpuObject::Query :		return (GL4Object) gl::GL_QUERY;
		}
		RETURN_ERR( "invalid object type", GL4Object() );
	}
	
/*
=================================================
	BufferUsage
=================================================
*/
	inline GL4BufferUsage GL4Enum (EGpuMemory::bits flags, EMemoryAccess::bits access)
	{
		int result = gl::GL_DYNAMIC_STORAGE_BIT;

		if ( flags[EGpuMemory::CoherentWithCPU] )
			result |= gl::GL_MAP_PERSISTENT_BIT | gl::GL_MAP_COHERENT_BIT;

		if ( flags[EGpuMemory::CachedInCPU] )
			result |= gl::GL_CLIENT_STORAGE_BIT;

		if ( flags[EGpuMemory::LocalInGPU] )
			result |= 0;

		if ( access[EMemoryAccess::CpuRead] )
			result |= gl::GL_MAP_READ_BIT;

		if ( access[EMemoryAccess::CpuWrite] )
			result |= gl::GL_MAP_WRITE_BIT;

		return GL4BufferUsage(result);
	}

/*
=================================================
	InternalPixelFormat
=================================================
*/
	inline GL4InternalPixelFormat GL4Enum (EPixelFormat::type value)
	{
		switch ( value )
		{
			// signed normalized
			case EPixelFormat::RGBA16_SNorm :			return (GL4InternalPixelFormat)	gl::GL_RGBA16_SNORM;
			case EPixelFormat::RGBA8_SNorm :			return (GL4InternalPixelFormat)	gl::GL_RGBA8_SNORM;
			case EPixelFormat::RGB16_SNorm :			return (GL4InternalPixelFormat)	gl::GL_RGB16_SNORM;
			case EPixelFormat::RGB8_SNorm :				return (GL4InternalPixelFormat)	gl::GL_RGB8_SNORM;
			case EPixelFormat::RG16_SNorm :				return (GL4InternalPixelFormat)	gl::GL_RG16_SNORM;
			case EPixelFormat::RG8_SNorm :				return (GL4InternalPixelFormat)	gl::GL_RG8_SNORM;
			case EPixelFormat::R16_SNorm :				return (GL4InternalPixelFormat)	gl::GL_R16_SNORM;
			case EPixelFormat::R8_SNorm :				return (GL4InternalPixelFormat)	gl::GL_R8_SNORM;

			// unsigned normalized
			case EPixelFormat::RGBA16_UNorm :			return (GL4InternalPixelFormat)	gl::GL_RGBA16;
			case EPixelFormat::RGBA8_UNorm :			return (GL4InternalPixelFormat)	gl::GL_RGBA8;
			case EPixelFormat::RGB16_UNorm :			return (GL4InternalPixelFormat)	gl::GL_RGB16;
			case EPixelFormat::RGB8_UNorm :				return (GL4InternalPixelFormat)	gl::GL_RGB8;
			case EPixelFormat::RG16_UNorm :				return (GL4InternalPixelFormat)	gl::GL_RG16;
			case EPixelFormat::RG8_UNorm :				return (GL4InternalPixelFormat)	gl::GL_RG8;
			case EPixelFormat::R16_UNorm :				return (GL4InternalPixelFormat)	gl::GL_R16;
			case EPixelFormat::R8_UNorm :				return (GL4InternalPixelFormat)	gl::GL_R8;
			case EPixelFormat::RGB10_A2_UNorm :			return (GL4InternalPixelFormat)	gl::GL_RGB10_A2;
			case EPixelFormat::RGBA4_UNorm :			return (GL4InternalPixelFormat)	gl::GL_RGBA4;
			case EPixelFormat::RGB5_A1_UNorm :			return (GL4InternalPixelFormat)	gl::GL_RGB5_A1;
			case EPixelFormat::RGB_5_6_5_UNorm :		return (GL4InternalPixelFormat)	gl::GL_RGB565;
			
			// sRGB
			case EPixelFormat::sRGB8 :					return (GL4InternalPixelFormat)	gl::GL_SRGB8;
			case EPixelFormat::sRGB8_A8 :				return (GL4InternalPixelFormat)	gl::GL_SRGB8_ALPHA8;

			// signed integer
			case EPixelFormat::R8I :					return (GL4InternalPixelFormat)	gl::GL_R8I;
			case EPixelFormat::RG8I :					return (GL4InternalPixelFormat)	gl::GL_RG8I;
			case EPixelFormat::RGB8I :					return (GL4InternalPixelFormat)	gl::GL_RGB8I;
			case EPixelFormat::RGBA8I :					return (GL4InternalPixelFormat)	gl::GL_RGBA8I;
			case EPixelFormat::R16I :					return (GL4InternalPixelFormat)	gl::GL_R16I;
			case EPixelFormat::RG16I :					return (GL4InternalPixelFormat)	gl::GL_RG16I;
			case EPixelFormat::RGB16I :					return (GL4InternalPixelFormat)	gl::GL_RGB16I;
			case EPixelFormat::RGBA16I :				return (GL4InternalPixelFormat)	gl::GL_RGBA16I;
			case EPixelFormat::R32I :					return (GL4InternalPixelFormat)	gl::GL_R32I;
			case EPixelFormat::RG32I :					return (GL4InternalPixelFormat)	gl::GL_RG32I;
			case EPixelFormat::RGB32I :					return (GL4InternalPixelFormat)	gl::GL_RGB32I;
			case EPixelFormat::RGBA32I :				return (GL4InternalPixelFormat)	gl::GL_RGBA32I;
			
			// unsigned integer
			case EPixelFormat::R8U :					return (GL4InternalPixelFormat)	gl::GL_R8UI;
			case EPixelFormat::RG8U :					return (GL4InternalPixelFormat)	gl::GL_RG8UI;
			case EPixelFormat::RGB8U :					return (GL4InternalPixelFormat)	gl::GL_RGB8UI;
			case EPixelFormat::RGBA8U :					return (GL4InternalPixelFormat)	gl::GL_RGBA8UI;
			case EPixelFormat::R16U :					return (GL4InternalPixelFormat)	gl::GL_R16UI;
			case EPixelFormat::RG16U :					return (GL4InternalPixelFormat)	gl::GL_RG16UI;
			case EPixelFormat::RGB16U :					return (GL4InternalPixelFormat)	gl::GL_RGB16UI;
			case EPixelFormat::RGBA16U :				return (GL4InternalPixelFormat)	gl::GL_RGBA16UI;
			case EPixelFormat::R32U :					return (GL4InternalPixelFormat)	gl::GL_R32UI;
			case EPixelFormat::RG32U :					return (GL4InternalPixelFormat)	gl::GL_RG32UI;
			case EPixelFormat::RGB32U :					return (GL4InternalPixelFormat)	gl::GL_RGB32UI;
			case EPixelFormat::RGBA32U :				return (GL4InternalPixelFormat)	gl::GL_RGBA32UI;
			case EPixelFormat::RGB10_A2U :				return (GL4InternalPixelFormat)	gl::GL_RGB10_A2UI;
			
			// float
			case EPixelFormat::R16F :					return (GL4InternalPixelFormat)	gl::GL_R16F;
			case EPixelFormat::RG16F :					return (GL4InternalPixelFormat)	gl::GL_RG16F;
			case EPixelFormat::RGB16F :					return (GL4InternalPixelFormat)	gl::GL_RGB16F;
			case EPixelFormat::RGBA16F :				return (GL4InternalPixelFormat)	gl::GL_RGBA16F;
			case EPixelFormat::R32F :					return (GL4InternalPixelFormat)	gl::GL_R32F;
			case EPixelFormat::RG32F :					return (GL4InternalPixelFormat)	gl::GL_RG32F;
			case EPixelFormat::RGB32F :					return (GL4InternalPixelFormat)	gl::GL_RGB32F;
			case EPixelFormat::RGBA32F :				return (GL4InternalPixelFormat)	gl::GL_RGBA32F;
			case EPixelFormat::RGB_11_11_10F :			return (GL4InternalPixelFormat)	gl::GL_R11F_G11F_B10F;

			// depth stencil
			case EPixelFormat::Depth16 :				return (GL4InternalPixelFormat)	gl::GL_DEPTH_COMPONENT16;
			case EPixelFormat::Depth24 :				return (GL4InternalPixelFormat)	gl::GL_DEPTH_COMPONENT24;
			case EPixelFormat::Depth32 :				return (GL4InternalPixelFormat)	gl::GL_DEPTH_COMPONENT32;
			case EPixelFormat::Depth32F :				return (GL4InternalPixelFormat)	gl::GL_DEPTH_COMPONENT32F;
			case EPixelFormat::Depth24_Stencil8 :		return (GL4InternalPixelFormat)	gl::GL_DEPTH24_STENCIL8;
			case EPixelFormat::Depth32F_Stencil8 :		return (GL4InternalPixelFormat)	gl::GL_DEPTH32F_STENCIL8;

			// compressed
			case EPixelFormat::BC1_RGB8_UNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			case EPixelFormat::BC1_RGB8_A1_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			case EPixelFormat::BC2_RGBA8_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			case EPixelFormat::BC3_RGBA8_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			case EPixelFormat::BC4_RED8_SNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SIGNED_RED_RGTC1;
			case EPixelFormat::BC4_RED8_UNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RED_RGTC1;
			case EPixelFormat::BC5_RG8_SNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SIGNED_RG_RGTC2;
			case EPixelFormat::BC5_RG8_UNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RG_RGTC2;
			case EPixelFormat::BC7_RGBA8_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_BPTC_UNORM;
			case EPixelFormat::BC7_SRGB8_A8_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
			case EPixelFormat::BC6H_RGB16F :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
			case EPixelFormat::BC6H_RGB16F_Unsigned :	return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
			case EPixelFormat::ETC2_RGB8_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGB8_ETC2;
			case EPixelFormat::ECT2_SRGB8_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ETC2;
			case EPixelFormat::ETC2_RGB8_A1_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;
			case EPixelFormat::ETC2_SRGB8_A1_UNorm :	return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2;
			case EPixelFormat::ETC2_RGBA8_UNorm :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA8_ETC2_EAC;
			case EPixelFormat::ETC2_SRGB8_A8_UNorm :	return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
			case EPixelFormat::EAC_R11_SNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_R11_EAC;
			case EPixelFormat::EAC_R11_UNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SIGNED_R11_EAC;
			case EPixelFormat::EAC_RG11_SNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RG11_EAC;
			case EPixelFormat::EAC_RG11_UNorm :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SIGNED_RG11_EAC;
			case EPixelFormat::ASTC_RGBA_4x4 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
			case EPixelFormat::ASTC_RGBA_5x4 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_5x4_KHR;
			case EPixelFormat::ASTC_RGBA_5x5 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_5x5_KHR;
			case EPixelFormat::ASTC_RGBA_6x5 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_6x5_KHR;
			case EPixelFormat::ASTC_RGBA_6x6 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_6x6_KHR;
			case EPixelFormat::ASTC_RGBA_8x5 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_8x5_KHR;
			case EPixelFormat::ASTC_RGBA_8x6 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_8x6_KHR;
			case EPixelFormat::ASTC_RGBA_8x8 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
			case EPixelFormat::ASTC_RGBA_10x5 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_10x5_KHR;
			case EPixelFormat::ASTC_RGBA_10x6 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_10x6_KHR;
			case EPixelFormat::ASTC_RGBA_10x8 :			return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_10x8_KHR;
			case EPixelFormat::ASTC_RGBA_10x10 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_10x10_KHR;
			case EPixelFormat::ASTC_RGBA_12x10 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_12x10_KHR;
			case EPixelFormat::ASTC_RGBA_12x12 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_RGBA_ASTC_12x12_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_4x4 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_5x4 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_5x5 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_6x5 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_6x6 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_8x5 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_8x6 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_8x8 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_10x5 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_10x6 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_10x8 :		return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_10x10 :	return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_12x10 :	return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR;
			case EPixelFormat::ASTC_SRGB8_A8_12x12 :	return (GL4InternalPixelFormat)	gl::GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR;
		}

		RETURN_ERR( "invalid internal pixel format", GL4InternalPixelFormat() );
	}
	
/*
=================================================
	InternalPixelFormat
=================================================
*/
	inline bool GL4Enum (EPixelFormat::type value, OUT GL4InternalPixelFormat &ifmt)
	{
		ifmt = GL4Enum( value );
		return ifmt != GL4InternalPixelFormat();
	}
	
/*
=================================================
	PixelFormat
=================================================
*/
	inline bool GL4Enum (EPixelFormat::type value, OUT GL4InternalPixelFormat &ifmt, OUT GL4PixelFormat &fmt, OUT GL4PixelType &type)
	{
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

		const uint	val_type		= ( value & _vtypeinfo::_TYPE_MASK );
		const bool	is_compressed	= EnumEq( value, _vtypeinfo::_COMPRESSED );
		const bool	is_integer		= ( val_type >= _vtypeinfo::_BYTE and val_type <= _vtypeinfo::_INT ) and
										not EnumEq( value, _vtypeinfo::_NORM );
		const bool	is_unsigned		= EnumEq( value, _vtypeinfo::_UNSIGNED );
		
		// internal format
		CHECK_ERR( GL4Enum( value, ifmt ) );

		if ( is_compressed )
		{
			fmt		= (GL4PixelFormat) 0;
			type	= (GL4PixelType) 0;
			return true;
		}

		// pixel type
		switch ( value & _vtypeinfo::_TYPE_MASK )
		{
			case _vtypeinfo::_BYTE						:	type = (GL4PixelType) (is_unsigned ? gl::GL_UNSIGNED_BYTE : gl::GL_BYTE); break;
			case _vtypeinfo::_SHORT						:	type = (GL4PixelType) (is_unsigned ? gl::GL_UNSIGNED_SHORT : gl::GL_SHORT); break;
			case _vtypeinfo::_INT						:	type = (GL4PixelType) (is_unsigned ? gl::GL_UNSIGNED_INT : gl::GL_INT); break;
			case _vtypeinfo::_HALF						:	type = (GL4PixelType) gl::GL_HALF_FLOAT; break;
			case _vtypeinfo::_FLOAT						:	type = (GL4PixelType) gl::GL_FLOAT; break;
			case _vtypeinfo::_INT_10_10_10_2			:	type = (GL4PixelType) (is_unsigned ? gl::GL_UNSIGNED_INT_2_10_10_10_REV : gl::GL_INT_2_10_10_10_REV); break;
			case _vtypeinfo::_FLOAT_11_11_10			:	type = (GL4PixelType) gl::GL_UNSIGNED_INT_10F_11F_11F_REV; break;
			case _vtypeinfo::_USHORT_4_4_4_4			:	type = (GL4PixelType) gl::GL_UNSIGNED_SHORT_4_4_4_4_REV; break;
			case _vtypeinfo::_USHORT_5_5_5_1			:	type = (GL4PixelType) gl::GL_UNSIGNED_SHORT_1_5_5_5_REV; break;
			case _vtypeinfo::_USHORT_5_6_5				:	type = (GL4PixelType) gl::GL_UNSIGNED_SHORT_5_6_5_REV; break;
			case _vtypeinfo::_INT24						:	type = (GL4PixelType) gl::GL_UNSIGNED_BYTE; break;
			case _vtypeinfo::_DEPTH_24_STENCIL_8		:	type = (GL4PixelType) gl::GL_UNSIGNED_INT_24_8; break;
			case _vtypeinfo::_FLOAT_DEPTH_32_STENCIL_8	:	type = (GL4PixelType) gl::GL_FLOAT_32_UNSIGNED_INT_24_8_REV; break;
			default										:	RETURN_ERR( "invalid pixel type" );
		}

		// pixel format
		switch ( value & _vtypeinfo::_COL_MASK )
		{
			case _vtypeinfo::_R				:	fmt = (GL4PixelFormat) (is_integer ? gl::GL_RED_INTEGER : gl::GL_RED);		break;
			case _vtypeinfo::_RG			:	fmt = (GL4PixelFormat) (is_integer ? gl::GL_RG_INTEGER : gl::GL_RG);		break;
			case _vtypeinfo::_RGB			:	fmt = (GL4PixelFormat) (is_integer ? gl::GL_RGB_INTEGER : gl::GL_RGB);		break;
			case _vtypeinfo::_RGBA			:	fmt = (GL4PixelFormat) (is_integer ? gl::GL_RGBA_INTEGER : gl::GL_RGBA);	break;
			case _vtypeinfo::_DEPTH			:	fmt = (GL4PixelFormat) gl::GL_DEPTH_COMPONENT;								break;
			case _vtypeinfo::_DEPTH_STENCIL	:	fmt = (GL4PixelFormat) gl::GL_DEPTH_STENCIL;								break;
			default							:	RETURN_ERR( "invalid pixel format" );
		}
		return true;
	}
	
}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
