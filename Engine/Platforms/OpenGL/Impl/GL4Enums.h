// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/gl4.h"

#if defined( GRAPHICS_API_OPENGL )

#include "Engine/Platforms/Shared/GPU/BufferEnums.h"
#include "Engine/Platforms/Shared/GPU/CommandEnums.h"
#include "Engine/Platforms/Shared/GPU/MemoryEnums.h"
#include "Engine/Platforms/Shared/GPU/MultiSamples.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Shared/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Shared/GPU/SamplerEnums.h"
#include "Engine/Platforms/Shared/GPU/ShaderEnums.h"
#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
#include "Engine/Platforms/Shared/GPU/VertexEnums.h"

namespace Engine
{
namespace Platforms
{
	class OpenGLThread;

}	// Platforms

namespace PlatformGL
{
	using namespace Platforms;

	using GLSubSystems	= EngineSubSystems< "opengl 4"_StringToID >;
	using GLSystemsRef	= ConstReference< GLSubSystems >;

	class GL4Device;
	
	enum GL4InternalPixelFormat {};
	enum GL4PixelFormat {};
	enum GL4PixelType {};
	enum GL4TextureTarget {};
	enum GL4Shader {};

	
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
	TextureTarget
=================================================
*/
	inline GL4TextureTarget GL4Enum (EImage::type value)
	{
		switch ( value )
		{
			case EImage::Tex1D			:	return (GL4TextureTarget) gl::GL_TEXTURE_1D;
			case EImage::Tex2D			:	return (GL4TextureTarget) gl::GL_TEXTURE_2D;
			case EImage::Tex2DArray		:	return (GL4TextureTarget) gl::GL_TEXTURE_2D_ARRAY;
			case EImage::Tex2DMS		:	return (GL4TextureTarget) gl::GL_TEXTURE_2D_MULTISAMPLE;
			case EImage::Tex2DMSArray	:	return (GL4TextureTarget) gl::GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
			case EImage::TexCube		:	return (GL4TextureTarget) gl::GL_TEXTURE_CUBE_MAP;
			case EImage::TexCubeArray	:	return (GL4TextureTarget) gl::GL_TEXTURE_CUBE_MAP_ARRAY;
			case EImage::Tex3D			:	return (GL4TextureTarget) gl::GL_TEXTURE_3D;
			case EImage::Buffer			:	return (GL4TextureTarget) gl::GL_TEXTURE_BUFFER;
		}

		RETURN_ERR( "invalid texture target", GL4TextureTarget() );
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

	using GLSystemsTypeList_t	= SubSystemsTypeList< "opengl 4"_StringToID, CompileTime::TypeListFrom<
											Platforms::OpenGLThread,
											PlatformGL::GL4Device
										> >;
}	// Engine

GX_SUBSYSTEM_DECL( GLSystemsTypeList_t,		Platforms::OpenGLThread,		Platforms::OpenGLThread );
GX_SUBSYSTEM_DECL( GLSystemsTypeList_t,		PlatformGL::GL4Device,			PlatformGL::GL4Device );

#endif	// GRAPHICS_API_OPENGL
