// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	namespace _platforms_hidden_
	{
		struct EValueTypeInfo
		{
			enum type : uint
			{
				// flags
				_FLAG_OFF		= 0,
				_FLAG_MASK		= (1 << (5 + _FLAG_OFF)) - 1,
				_NORM			= 1 << (0 + _FLAG_OFF),
				_UNSIGNED		= 1 << (1 + _FLAG_OFF),
				_COMPRESSED		= 1 << (2 + _FLAG_OFF),
				_REVERSE		= 1 << (3 + _FLAG_OFF),	// RGBA to BGRA
				_SRGB			= 1 << (4 + _FLAG_OFF),

				// types
				_TYPE_OFF		= CompileTime::IntLog2< uint, _FLAG_MASK > + 1,
				_TYPE_MASK		= 0xFF << _TYPE_OFF,
				_TYPE_FLAG_MASK	= _TYPE_MASK | _FLAG_MASK,
				_BOOL			= 1 << _TYPE_OFF,
				_BYTE			= 2 << _TYPE_OFF,
				_SHORT			= 3 << _TYPE_OFF,
				_INT			= 4 << _TYPE_OFF,
				_LONG			= 5 << _TYPE_OFF,
				_HALF			= 6 << _TYPE_OFF,
				_FLOAT			= 7 << _TYPE_OFF,
				_DOUBLE			= 8 << _TYPE_OFF,
				_UBYTE			= _BYTE  | _UNSIGNED,
				_USHORT			= _SHORT | _UNSIGNED,
				_UINT			= _INT   | _UNSIGNED,
				_ULONG			= _LONG  | _UNSIGNED,

				_INT_10_10_10_2			= 10 << _TYPE_OFF,
				_FLOAT_11_11_10			= 11 << _TYPE_OFF,
				_USHORT_4_4_4_4			= 12 << _TYPE_OFF,
				_USHORT_5_5_5_1			= 13 << _TYPE_OFF,
				_USHORT_5_6_5			= 14 << _TYPE_OFF,
				_INT_10_10_10			= 15 << _TYPE_OFF,
				_USHORT_5_5_5			= 16 << _TYPE_OFF,

				_INT24					= 20 << _TYPE_OFF,
				_DEPTH_16_STENCIL_8		= 21 << _TYPE_OFF,
				_DEPTH_24_STENCIL_8		= 22 << _TYPE_OFF,
				_FLOAT_DEPTH_32_STENCIL_8 = 23 << _TYPE_OFF,


				// columns
				_COL_OFF		= CompileTime::IntLog2< uint, _TYPE_MASK > + 1,
				_COL_MASK		= 0x7 << _COL_OFF,
				_COL1			= 1 << _COL_OFF,
				_COL2			= 2 << _COL_OFF,
				_COL3			= 3 << _COL_OFF,
				_COL4			= 4 << _COL_OFF,
				_R				= _COL1,
				_RG				= _COL2,
				_RGB			= _COL3,
				_RGBA			= _COL4,
				_DEPTH			= 5 << _COL_OFF,
				_DEPTH_STENCIL	= 6 << _COL_OFF,
				_STENCIL		= 7 << _COL_OFF,

				// rows
				_ROW_OFF		= CompileTime::IntLog2< uint, _COL_MASK > + 1,
				_ROW_MASK		= 0x7 << _ROW_OFF,
				_ROW1			= 1 << _ROW_OFF,
				_ROW2			= 2 << _ROW_OFF,
				_ROW3			= 3 << _ROW_OFF,
				_ROW4			= 4 << _ROW_OFF,

				// ext type
				_EXT_OFF		= CompileTime::IntLog2< uint, _ROW_MASK > + 1,
				_EXT_MASK		= 0x3 << _EXT_OFF,
				_SAMPLER		= 1 << _EXT_OFF,
				_IMAGE			= 2 << _EXT_OFF,
				_ATOMIC			= 3 << _EXT_OFF,

				// sampler
				_SAMP_OFF		= CompileTime::IntLog2< uint, _EXT_MASK > + 1,
				_SAMP_MASK		= 0xF << _SAMP_OFF,
				_SAMP_1D		=  1 << _SAMP_OFF,
				_SAMP_1DS		=  2 << _SAMP_OFF,
				_SAMP_1DA		=  3 << _SAMP_OFF,
				_SAMP_1DAS		=  4 << _SAMP_OFF,
				_SAMP_2D		=  5 << _SAMP_OFF,
				_SAMP_2DS		=  6 << _SAMP_OFF,
				_SAMP_2DMS		=  7 << _SAMP_OFF,
				_SAMP_2DA		=  8 << _SAMP_OFF,
				_SAMP_2DAS		=  9 << _SAMP_OFF,
				_SAMP_2DAMS		= 10 << _SAMP_OFF,
				_SAMP_3D		= 11 << _SAMP_OFF,
				_SAMP_CUBE		= 12 << _SAMP_OFF,
				_SAMP_CUBES		= 13 << _SAMP_OFF,
				_SAMP_CUBEA		= 14 << _SAMP_OFF,
				_SAMP_BUF		= 15 << _SAMP_OFF,

				_MAX			= CompileTime::IntLog2< uint, _SAMP_MASK > + 1,

				// other types
				Int_10_10_10_2			= _INT_10_10_10_2 | _RGBA,
				UInt_10_10_10_2			= Int_10_10_10_2  | _UNSIGNED,
				Float_11_11_10			= _FLOAT_11_11_10 | _NORM | _UNSIGNED | _RGB,
				UShort_4_4_4_4			= _USHORT_4_4_4_4 | _UNSIGNED | _RGBA,
				UShort_5_5_5_1			= _USHORT_5_5_5_1 | _UNSIGNED | _RGBA,
				UShort_5_6_5			= _USHORT_5_6_5   | _UNSIGNED | _RGB,

				Depth16					= _SHORT | _DEPTH | _NORM,
				Depth24					= _INT24 | _DEPTH | _NORM,
				Depth32					= _INT   | _DEPTH | _NORM,
				Depth32F				= _FLOAT | _DEPTH,
				Depth16_Stencil8		= _DEPTH_16_STENCIL_8 | _DEPTH_STENCIL | _NORM, 
				Depth24_Stencil8		= _DEPTH_24_STENCIL_8 | _DEPTH_STENCIL | _NORM,
				Depth32F_Stencil8		= _FLOAT_DEPTH_32_STENCIL_8 | _DEPTH_STENCIL,

				// compressed types
				BC1_RGB8_UNorm			= (100 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGB,	// DXT1
				BC1_RGB8_A1_UNorm		= (101 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,	// DXT1
				BC2_RGBA8_UNorm			= (102 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,	// DXT3
				BC3_RGBA8_UNorm			= (103 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,	// DXT5
				BC4_RED8_SNorm			= (104 << _TYPE_OFF) | _NORM | _COMPRESSED | _R,				// RGTC1
				BC4_RED8_UNorm			= BC4_RED8_SNorm  | _UNSIGNED,									// RGTC1
				BC5_RG8_SNorm			= (105 << _TYPE_OFF) | _NORM | _COMPRESSED | _RG,				// RGTC2
				BC5_RG8_UNorm			= BC5_RG8_SNorm   | _UNSIGNED,									// RGTC2
				BC7_RGBA8_UNorm			= (106 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,			// BPTC
				BC7_SRGB8_A8_UNorm		= (107 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA | _SRGB,	// BPTC
				BC6H_RGB16F				= (108 << _TYPE_OFF) | _COMPRESSED | _RGB,								// BPTC
				BC6H_RGB16F_Unsigned	= BC6H_RGB16F | _UNSIGNED,												// BPTC
				ETC2_RGB8_UNorm			= (109 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGB,
				ECT2_SRGB8_UNorm		= (110 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGB | _SRGB,
				ETC2_RGB8_A1_UNorm		= (111 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,
				ETC2_SRGB8_A1_UNorm		= (112 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA | _SRGB,
				ETC2_RGBA8_UNorm		= (113 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,
				ETC2_SRGB8_A8_UNorm		= (114 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA | _SRGB,
				EAC_R11_SNorm			= (115 << _TYPE_OFF) | _NORM | _COMPRESSED | _R,
				EAC_R11_UNorm			= EAC_R11_SNorm | _UNSIGNED,
				EAC_RG11_SNorm			= (116 << _TYPE_OFF) | _NORM | _COMPRESSED | _R,
				EAC_RG11_UNorm			= EAC_RG11_SNorm | _UNSIGNED,
				ASTC_RGBA_4x4			= (117 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_5x4			= (118 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_5x5			= (119 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_6x5			= (120 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_6x6			= (121 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_8x5			= (122 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_8x6			= (123 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_8x8			= (124 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_10x5			= (125 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_10x6			= (126 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_10x8			= (127 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_10x10			= (128 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_12x10			= (129 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_RGBA_12x12			= (130 << _TYPE_OFF) | _NORM | _UNSIGNED | _RGBA | _COMPRESSED,
				ASTC_SRGB8_A8_4x4		= ASTC_RGBA_4x4 | _SRGB,
				ASTC_SRGB8_A8_5x4		= ASTC_RGBA_5x4 | _SRGB,
				ASTC_SRGB8_A8_5x5		= ASTC_RGBA_5x5 | _SRGB,
				ASTC_SRGB8_A8_6x5		= ASTC_RGBA_6x5 | _SRGB,
				ASTC_SRGB8_A8_6x6		= ASTC_RGBA_6x6 | _SRGB,
				ASTC_SRGB8_A8_8x5		= ASTC_RGBA_8x5 | _SRGB,
				ASTC_SRGB8_A8_8x6		= ASTC_RGBA_8x6 | _SRGB,
				ASTC_SRGB8_A8_8x8		= ASTC_RGBA_8x8 | _SRGB,
				ASTC_SRGB8_A8_10x5		= ASTC_RGBA_10x5 | _SRGB,
				ASTC_SRGB8_A8_10x6		= ASTC_RGBA_10x6 | _SRGB,
				ASTC_SRGB8_A8_10x8		= ASTC_RGBA_10x8 | _SRGB,
				ASTC_SRGB8_A8_10x10		= ASTC_RGBA_10x10 | _SRGB,
				ASTC_SRGB8_A8_12x10		= ASTC_RGBA_12x10 | _SRGB,
				ASTC_SRGB8_A8_12x12		= ASTC_RGBA_12x12 | _SRGB,

				// compound
				Bool		= _BOOL | _COL1,
				Bool2		= _BOOL | _COL2,
				Bool3		= _BOOL | _COL3,
				Bool4		= _BOOL | _COL4,

				Byte		= _BYTE | _COL1,
				Byte2		= _BYTE | _COL2,
				Byte3		= _BYTE | _COL3,
				Byte4		= _BYTE | _COL4,
			
				UByte		= _UBYTE | _COL1,
				UByte2		= _UBYTE | _COL2,
				UByte3		= _UBYTE | _COL3,
				UByte4		= _UBYTE | _COL4,
			
				Short		= _SHORT | _COL1,
				Short2		= _SHORT | _COL2,
				Short3		= _SHORT | _COL3,
				Short4		= _SHORT | _COL4,
			
				UShort		= _USHORT | _COL1,
				UShort2		= _USHORT | _COL2,
				UShort3		= _USHORT | _COL3,
				UShort4		= _USHORT | _COL4,
			
				Int			= _INT | _COL1,
				Int2		= _INT | _COL2,
				Int3		= _INT | _COL3,
				Int4		= _INT | _COL4,
			
				UInt		= _UINT | _COL1,
				UInt2		= _UINT | _COL2,
				UInt3		= _UINT | _COL3,
				UInt4		= _UINT | _COL4,

				Long		= _LONG | _COL1,
				Long2		= _LONG | _COL2,
				Long3		= _LONG | _COL3,
				Long4		= _LONG | _COL4,

				ULong		= _ULONG | _COL1,
				ULong2		= _ULONG | _COL2,
				ULong3		= _ULONG | _COL3,
				ULong4		= _ULONG | _COL4,
			
				Half		= _HALF | _COL1,
				Half2		= _HALF | _COL2,
				Half3		= _HALF | _COL3,
				Half4		= _HALF | _COL4,

				Float		= _FLOAT | _COL1,
				Float2		= _FLOAT | _COL2,
				Float3		= _FLOAT | _COL3,
				Float4		= _FLOAT | _COL4,
				Float2x2	= _FLOAT | _COL2 | _ROW2,
				Float3x3	= _FLOAT | _COL3 | _ROW3,
				Float4x4	= _FLOAT | _COL4 | _ROW4,
			
				Double		= _DOUBLE | _COL1,
				Double2		= _DOUBLE | _COL2,
				Double3		= _DOUBLE | _COL3,
				Double4		= _DOUBLE | _COL4,
				Double2x2	= _DOUBLE | _COL2 | _ROW2,
				Double3x3	= _DOUBLE | _COL3 | _ROW3,
				Double4x4	= _DOUBLE | _COL4 | _ROW4,

				AtomicCounter	= _ATOMIC,

				// float sampler
				FloatSampler1D				= _FLOAT | _SAMPLER | _SAMP_1D,
				FloatSampler1DShadow		= _FLOAT | _SAMPLER | _SAMP_1DS,
				FloatSampler1DArray			= _FLOAT | _SAMPLER | _SAMP_1DA,
				FloatSampler1DArrayShadow	= _FLOAT | _SAMPLER | _SAMP_1DAS,
				FloatSampler2D				= _FLOAT | _SAMPLER | _SAMP_2D,
				FloatSampler2DShadow		= _FLOAT | _SAMPLER | _SAMP_2DS,
				FloatSampler2DMS			= _FLOAT | _SAMPLER | _SAMP_2DMS,
				FloatSampler2DArray			= _FLOAT | _SAMPLER | _SAMP_2DA,
				FloatSampler2DArrayShadow	= _FLOAT | _SAMPLER | _SAMP_2DAS,
				FloatSampler2DMSArray		= _FLOAT | _SAMPLER | _SAMP_2DAMS,
				FloatSamplerCube			= _FLOAT | _SAMPLER | _SAMP_CUBE,
				FloatSamplerCubeShadow		= _FLOAT | _SAMPLER | _SAMP_CUBES,
				FloatSamplerCubeArray		= _FLOAT | _SAMPLER | _SAMP_CUBEA,
				FloatSampler3D				= _FLOAT | _SAMPLER | _SAMP_3D,
				FloatSamplerBuffer			= _FLOAT | _SAMPLER | _SAMP_BUF,
			
				// int sampler
				IntSampler1D				= _INT | _SAMPLER | _SAMP_1D,
				IntSampler1DArray			= _INT | _SAMPLER | _SAMP_1DA,
				IntSampler2D				= _INT | _SAMPLER | _SAMP_2D,
				IntSampler2DMS				= _INT | _SAMPLER | _SAMP_2DMS,
				IntSampler2DArray			= _INT | _SAMPLER | _SAMP_2DA,
				IntSampler2DMSArray			= _INT | _SAMPLER | _SAMP_2DAMS,
				IntSamplerCube				= _INT | _SAMPLER | _SAMP_CUBE,
				IntSamplerCubeArray			= _INT | _SAMPLER | _SAMP_CUBEA,
				IntSampler3D				= _INT | _SAMPLER | _SAMP_3D,
				IntSamplerBuffer			= _INT | _SAMPLER | _SAMP_BUF,
			
				// uint sampler
				UIntSampler1D				= _UINT | _SAMPLER | _SAMP_1D,
				UIntSampler1DArray			= _UINT | _SAMPLER | _SAMP_1DA,
				UIntSampler2D				= _UINT | _SAMPLER | _SAMP_2D,
				UIntSampler2DMS				= _UINT | _SAMPLER | _SAMP_2DMS,
				UIntSampler2DArray			= _UINT | _SAMPLER | _SAMP_2DA,
				UIntSampler2DMSArray		= _UINT | _SAMPLER | _SAMP_2DAMS,
				UIntSamplerCube				= _UINT | _SAMPLER | _SAMP_CUBE,
				UIntSamplerCubeArray		= _UINT | _SAMPLER | _SAMP_CUBEA,
				UIntSampler3D				= _UINT | _SAMPLER | _SAMP_3D,
				UIntSamplerBuffer			= _UINT | _SAMPLER | _SAMP_BUF,

				// image
				Image1D						= _IMAGE | _SAMP_1D,
				Image1DArray				= _IMAGE | _SAMP_1DA,
				Image2D						= _IMAGE | _SAMP_2D,
				Image2DMS					= _IMAGE | _SAMP_2DMS,
				Image2DArray				= _IMAGE | _SAMP_2DA,
				Image2DMSArray				= _IMAGE | _SAMP_2DAMS,
				ImageCube					= _IMAGE | _SAMP_CUBE,
				ImageCubeArray				= _IMAGE | _SAMP_CUBEA,
				Image3D						= _IMAGE | _SAMP_3D,
				ImageBuffer					= _IMAGE | _SAMP_BUF,

				// colors
				RGB10_A2U		= UInt_10_10_10_2,
				RGB_11_11_10F	= Float_11_11_10,

				RGBA16_SNorm	= Short4 | _NORM,
				RGBA8_SNorm		= Byte4  | _NORM,
				RGB16_SNorm		= Short3 | _NORM,
				RGB8_SNorm		= Byte3  | _NORM,
				RG16_SNorm		= Short2 | _NORM,
				RG8_SNorm		= Byte2  | _NORM,
				R16_SNorm		= Short  | _NORM,
				R8_SNorm		= Byte   | _NORM,
			
				RGBA16_UNorm	= UShort4 | _NORM,
				RGBA8_UNorm		= UByte4  | _NORM,
				RGB16_UNorm		= UShort3 | _NORM,
				RGB8_UNorm		= UByte3  | _NORM,
				RG16_UNorm		= UShort2 | _NORM,
				RG8_UNorm		= UByte2  | _NORM,
				R16_UNorm		= UShort  | _NORM,
				R8_UNorm		= UByte   | _NORM,
				RGB10_A2_UNorm	= UInt_10_10_10_2 | _NORM,
				RGBA4_UNorm		= UShort_4_4_4_4 | _NORM,
				RGB5_A1_UNorm	= UShort_5_5_5_1 | _NORM,
				RGB_5_6_5_UNorm	= UShort_5_6_5   | _NORM,
			
				SRGB8			= RGB8_UNorm | _SRGB,
				SRGB8_A8		= RGBA8_UNorm | _SRGB,
			};
		};

	}	// _platforms_hidden_


	struct EPixelFormat
	{
	private:
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

	public:
		enum type : uint
		{
			// flags
			Rev_Flag				= _vtypeinfo::_REVERSE,
			sRGB_Flag				= _vtypeinfo::_SRGB,

			// signed normalized
			RGBA16_SNorm			= _vtypeinfo::RGBA16_SNorm,
			RGBA8_SNorm				= _vtypeinfo::RGBA8_SNorm,
			RGB16_SNorm				= _vtypeinfo::RGB16_SNorm,
			RGB8_SNorm				= _vtypeinfo::RGB8_SNorm,
			RG16_SNorm				= _vtypeinfo::RG16_SNorm,
			RG8_SNorm				= _vtypeinfo::RG8_SNorm,
			R16_SNorm				= _vtypeinfo::R16_SNorm,
			R8_SNorm				= _vtypeinfo::R8_SNorm,
			
			// unsigned normalized
			RGBA16_UNorm			= _vtypeinfo::RGBA16_UNorm,
			RGBA8_UNorm				= _vtypeinfo::RGBA8_UNorm,
			RGB16_UNorm				= _vtypeinfo::RGB16_UNorm,
			RGB8_UNorm				= _vtypeinfo::RGB8_UNorm,
			RG16_UNorm				= _vtypeinfo::RG16_UNorm,
			RG8_UNorm				= _vtypeinfo::RG8_UNorm,
			R16_UNorm				= _vtypeinfo::R16_UNorm,
			R8_UNorm				= _vtypeinfo::R8_UNorm,
			RGB10_A2_UNorm			= _vtypeinfo::RGB10_A2_UNorm,
			RGBA4_UNorm				= _vtypeinfo::RGBA4_UNorm,
			RGB5_A1_UNorm			= _vtypeinfo::RGB5_A1_UNorm,
			RGB_5_6_5_UNorm			= _vtypeinfo::RGB_5_6_5_UNorm,

			// BGRA
			BGR8_UNorm				= RGB8_SNorm | Rev_Flag,
			BGRA8_UNorm				= RGBA8_SNorm | Rev_Flag,
			
			// sRGB
			sRGB8					= _vtypeinfo::RGB8_UNorm | sRGB_Flag,
			sRGB8_A8				= _vtypeinfo::RGBA8_UNorm | sRGB_Flag,

			// signed integer
			R8I						= _vtypeinfo::Byte,
			RG8I					= _vtypeinfo::Byte2,
			RGB8I					= _vtypeinfo::Byte3,
			RGBA8I					= _vtypeinfo::Byte4,
			R16I					= _vtypeinfo::Short,
			RG16I					= _vtypeinfo::Short2,
			RGB16I					= _vtypeinfo::Short3,
			RGBA16I					= _vtypeinfo::Short4,
			R32I					= _vtypeinfo::Int,
			RG32I					= _vtypeinfo::Int2,
			RGB32I					= _vtypeinfo::Int3,
			RGBA32I					= _vtypeinfo::Int4,
			
			// unsigned integer
			R8U						= _vtypeinfo::UByte,
			RG8U					= _vtypeinfo::UByte2,
			RGB8U					= _vtypeinfo::UByte3,
			RGBA8U					= _vtypeinfo::UByte4,
			R16U					= _vtypeinfo::UShort,
			RG16U					= _vtypeinfo::UShort2,
			RGB16U					= _vtypeinfo::UShort3,
			RGBA16U					= _vtypeinfo::UShort4,
			R32U					= _vtypeinfo::UInt,
			RG32U					= _vtypeinfo::UInt2,
			RGB32U					= _vtypeinfo::UInt3,
			RGBA32U					= _vtypeinfo::UInt4,
			RGB10_A2U				= _vtypeinfo::RGB10_A2U,
			
			// float
			R16F					= _vtypeinfo::Half,
			RG16F					= _vtypeinfo::Half2,
			RGB16F					= _vtypeinfo::Half3,
			RGBA16F					= _vtypeinfo::Half4,
			R32F					= _vtypeinfo::Float,
			RG32F					= _vtypeinfo::Float2,
			RGB32F					= _vtypeinfo::Float3,
			RGBA32F					= _vtypeinfo::Float4,
			RGB_11_11_10F			= _vtypeinfo::RGB_11_11_10F,

			// depth stencil
			Depth16					= _vtypeinfo::Depth16,
			Depth24					= _vtypeinfo::Depth24,
			Depth32					= _vtypeinfo::Depth32,
			Depth32F				= _vtypeinfo::Depth32F,
			Depth16_Stencil8		= _vtypeinfo::Depth16_Stencil8,
			Depth24_Stencil8		= _vtypeinfo::Depth24_Stencil8,
			Depth32F_Stencil8		= _vtypeinfo::Depth32F_Stencil8,
			
			// compressed
			BC1_RGB8_UNorm			= _vtypeinfo::BC1_RGB8_UNorm,
			BC1_RGB8_A1_UNorm		= _vtypeinfo::BC1_RGB8_A1_UNorm,
			BC2_RGBA8_UNorm			= _vtypeinfo::BC2_RGBA8_UNorm,
			BC3_RGBA8_UNorm			= _vtypeinfo::BC3_RGBA8_UNorm,
			BC4_RED8_SNorm			= _vtypeinfo::BC4_RED8_SNorm,
			BC4_RED8_UNorm			= _vtypeinfo::BC4_RED8_UNorm,
			BC5_RG8_SNorm			= _vtypeinfo::BC5_RG8_SNorm,
			BC5_RG8_UNorm			= _vtypeinfo::BC5_RG8_UNorm,
			BC7_RGBA8_UNorm			= _vtypeinfo::BC7_RGBA8_UNorm,
			BC7_SRGB8_A8_UNorm		= _vtypeinfo::BC7_SRGB8_A8_UNorm,
			BC6H_RGB16F				= _vtypeinfo::BC6H_RGB16F,
			BC6H_RGB16F_Unsigned	= _vtypeinfo::BC6H_RGB16F_Unsigned,
			ETC2_RGB8_UNorm			= _vtypeinfo::ETC2_RGB8_UNorm,
			ECT2_SRGB8_UNorm		= _vtypeinfo::ECT2_SRGB8_UNorm,
			ETC2_RGB8_A1_UNorm		= _vtypeinfo::ETC2_RGB8_A1_UNorm,
			ETC2_SRGB8_A1_UNorm		= _vtypeinfo::ETC2_SRGB8_A1_UNorm,
			ETC2_RGBA8_UNorm		= _vtypeinfo::ETC2_RGBA8_UNorm,
			ETC2_SRGB8_A8_UNorm		= _vtypeinfo::ETC2_SRGB8_A8_UNorm,
			EAC_R11_SNorm			= _vtypeinfo::EAC_R11_SNorm,
			EAC_R11_UNorm			= _vtypeinfo::EAC_R11_UNorm,
			EAC_RG11_SNorm			= _vtypeinfo::EAC_RG11_SNorm,
			EAC_RG11_UNorm			= _vtypeinfo::EAC_RG11_UNorm,
			ASTC_RGBA_4x4			= _vtypeinfo::ASTC_RGBA_4x4,
			ASTC_RGBA_5x4			= _vtypeinfo::ASTC_RGBA_5x4,
			ASTC_RGBA_5x5			= _vtypeinfo::ASTC_RGBA_5x5,
			ASTC_RGBA_6x5			= _vtypeinfo::ASTC_RGBA_6x5,
			ASTC_RGBA_6x6			= _vtypeinfo::ASTC_RGBA_6x6,
			ASTC_RGBA_8x5			= _vtypeinfo::ASTC_RGBA_8x5,
			ASTC_RGBA_8x6			= _vtypeinfo::ASTC_RGBA_8x6,
			ASTC_RGBA_8x8			= _vtypeinfo::ASTC_RGBA_8x8,
			ASTC_RGBA_10x5			= _vtypeinfo::ASTC_RGBA_10x5,
			ASTC_RGBA_10x6			= _vtypeinfo::ASTC_RGBA_10x6,
			ASTC_RGBA_10x8			= _vtypeinfo::ASTC_RGBA_10x8,
			ASTC_RGBA_10x10			= _vtypeinfo::ASTC_RGBA_10x10,
			ASTC_RGBA_12x10			= _vtypeinfo::ASTC_RGBA_12x10,
			ASTC_RGBA_12x12			= _vtypeinfo::ASTC_RGBA_12x12,
			ASTC_SRGB8_A8_4x4		= _vtypeinfo::ASTC_SRGB8_A8_4x4,
			ASTC_SRGB8_A8_5x4		= _vtypeinfo::ASTC_SRGB8_A8_5x4,
			ASTC_SRGB8_A8_5x5		= _vtypeinfo::ASTC_SRGB8_A8_5x5,
			ASTC_SRGB8_A8_6x5		= _vtypeinfo::ASTC_SRGB8_A8_6x5,
			ASTC_SRGB8_A8_6x6		= _vtypeinfo::ASTC_SRGB8_A8_6x6,
			ASTC_SRGB8_A8_8x5		= _vtypeinfo::ASTC_SRGB8_A8_8x5,
			ASTC_SRGB8_A8_8x6		= _vtypeinfo::ASTC_SRGB8_A8_8x6,
			ASTC_SRGB8_A8_8x8		= _vtypeinfo::ASTC_SRGB8_A8_8x8,
			ASTC_SRGB8_A8_10x5		= _vtypeinfo::ASTC_SRGB8_A8_10x5,
			ASTC_SRGB8_A8_10x6		= _vtypeinfo::ASTC_SRGB8_A8_10x6,
			ASTC_SRGB8_A8_10x8		= _vtypeinfo::ASTC_SRGB8_A8_10x8,
			ASTC_SRGB8_A8_10x10		= _vtypeinfo::ASTC_SRGB8_A8_10x10,
			ASTC_SRGB8_A8_12x10		= _vtypeinfo::ASTC_SRGB8_A8_12x10,
			ASTC_SRGB8_A8_12x12		= _vtypeinfo::ASTC_SRGB8_A8_12x12,

			Unknown					= uint(-1),
		};
		
	// utils
		static constexpr bool IsCompressed		(type value);

		static constexpr bool IsLinear			(type value);
		static constexpr bool IsNonLinear		(type value);

		static constexpr bool IsColor			(type value);
		static constexpr bool IsDepth			(type value);
		static constexpr bool IsStencil			(type value);
		static constexpr bool IsDepthStencil	(type value);

		static constexpr bool HasDepth			(type value);
		static constexpr bool HasStencil		(type value);

		static constexpr uint ColorChannelsCount (type value);

		static StringCRef			ToString		(type value);
		static BitsVec<usize, 4>	BitsPerChannel	(type value);
		static BitsU				BitPerPixel		(type value);
	};
	


//-----------------------------------------------------------------------------//
// EPixelFormat

	inline constexpr bool EPixelFormat::IsCompressed (type value)
	{
		return EnumEq( value, _vtypeinfo::_COMPRESSED );
	}
	

	inline constexpr bool EPixelFormat::IsLinear (type value)
	{
		return not IsNonLinear( value );
	}


	inline constexpr bool EPixelFormat::IsNonLinear (type value)
	{
		return EnumEq( value, sRGB_Flag );
	}


	inline BitsVec<usize, 4>  EPixelFormat::BitsPerChannel (type value)
	{
		using vtype	= _platforms_hidden_::EValueTypeInfo;

		if ( IsCompressed( value ) ) {
			TODO( "" );
			return BitsVec<usize, 4>( 1_bit );
		}
		
		usize4	size;

		switch ( value & vtype::_TYPE_MASK )
		{
			case vtype::_BYTE :						size = usize4( 8 );					break;
			case vtype::_SHORT :					size = usize4( 16 );				break;
			case vtype::_INT :						size = usize4( 32 );				break;
			case vtype::_HALF :						size = usize4( 16 );				break;
			case vtype::_FLOAT :					size = usize4( 32 );				break;
			case vtype::_DOUBLE :					size = usize4( 64 );				break;
			case vtype::_INT_10_10_10_2 :			size = usize4( 10, 10, 10, 2 );		break;
			case vtype::_FLOAT_11_11_10 :			size = usize4( 11, 11, 10, 0 );		break;
			case vtype::_USHORT_4_4_4_4 :			size = usize4( 4, 4, 4, 4 );		break;
			case vtype::_USHORT_5_5_5_1 :			size = usize4( 5, 5, 5, 1 );		break;
			case vtype::_USHORT_5_6_5 :				size = usize4( 5, 6, 5, 0 );		break;
			case vtype::_INT_10_10_10 :				size = usize4( 10, 10, 10, 0 );		break;
			case vtype::_USHORT_5_5_5 :				size = usize4( 5, 5, 5, 0 );		break;
			case vtype::_INT24 :					size = usize4( 24, 0, 0, 0 );		break;
			case vtype::_DEPTH_24_STENCIL_8 :		size = usize4( 24, 8, 0, 0 );		break;
			case vtype::_FLOAT_DEPTH_32_STENCIL_8 :	size = usize4( 32, 8, 0, 0 );		break;
			default :								RETURN_ERR( "unknown type!" );
		}

		const uint	col_chans	= (value & vtype::_COL_MASK) >> vtype::_COL_OFF;

		if ( col_chans > 0 )
		{
			for (usize i = col_chans; i < size.Count(); ++i)
			{
				size[i] = 0;
			}
		}

		return BitsVec<usize, 4>( size );
	}
		

	inline BitsU  EPixelFormat::BitPerPixel (type value)
	{
		return BitsPerChannel( value ).Sum();
	}
		

	inline constexpr uint EPixelFormat::ColorChannelsCount (type value)
	{
		return	( value & _vtypeinfo::_COL_MASK ) >= _vtypeinfo::_R and
				( value & _vtypeinfo::_COL_MASK ) <= _vtypeinfo::_RGBA ?
					( ( value & _vtypeinfo::_COL_MASK ) >> _vtypeinfo::_COL_OFF ) :
					0;
	}

	
	inline constexpr bool EPixelFormat::IsColor (type value)
	{
		return ColorChannelsCount( value ) > 0;
	}


	inline constexpr bool EPixelFormat::IsDepth (type value)
	{
		return ( value & _vtypeinfo::_COL_MASK ) == _vtypeinfo::_DEPTH;
	}


	inline constexpr bool EPixelFormat::IsStencil (type value)
	{
		return ( value & _vtypeinfo::_COL_MASK ) == _vtypeinfo::_STENCIL;
	}


	inline constexpr bool EPixelFormat::IsDepthStencil (type value)
	{
		return ( value & _vtypeinfo::_COL_MASK ) == _vtypeinfo::_DEPTH_STENCIL;
	}
	

	inline constexpr bool EPixelFormat::HasDepth (type value)
	{
		return IsDepth( value ) or IsDepthStencil( value );
	}


	inline constexpr bool EPixelFormat::HasStencil (type value)
	{
		return IsStencil( value ) or IsDepthStencil( value );
	}
	

	inline StringCRef  EPixelFormat::ToString (type value)
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
			/*case BC2_RGBA8_UNorm
			case BC3_RGBA8_UNorm
			case BC4_RED8_SNorm
			case BC4_RED8_UNorm
			case BC5_RG8_SNorm
			case BC5_RG8_UNorm
			case BC7_RGBA8_UNorm
			case BC7_SRGB8_A8_UNorm
			case BC6H_RGB16F
			case BC6H_RGB16F_Unsigned
			case ETC2_RGB8_UNorm
			case ECT2_SRGB8_UNorm
			case ETC2_RGB8_A1_UNorm
			case ETC2_SRGB8_A1_UNorm
			case ETC2_RGBA8_UNorm
			case ETC2_SRGB8_A8_UNorm
			case EAC_R11_SNorm
			case EAC_R11_UNorm
			case EAC_RG11_SNorm
			case EAC_RG11_UNorm
			case ASTC_RGBA_4x4
			case ASTC_RGBA_5x4
			case ASTC_RGBA_5x5
			case ASTC_RGBA_6x5
			case ASTC_RGBA_6x6
			case ASTC_RGBA_8x5
			case ASTC_RGBA_8x6
			case ASTC_RGBA_8x8
			case ASTC_RGBA_10x5
			case ASTC_RGBA_10x6
			case ASTC_RGBA_10x8
			case ASTC_RGBA_10x10
			case ASTC_RGBA_12x10
			case ASTC_RGBA_12x12
			case ASTC_SRGB8_A8_4x4
			case ASTC_SRGB8_A8_5x4
			case ASTC_SRGB8_A8_5x5
			case ASTC_SRGB8_A8_6x5
			case ASTC_SRGB8_A8_6x6
			case ASTC_SRGB8_A8_8x5
			case ASTC_SRGB8_A8_8x6
			case ASTC_SRGB8_A8_8x8
			case ASTC_SRGB8_A8_10x5
			case ASTC_SRGB8_A8_10x6
			case ASTC_SRGB8_A8_10x8
			case ASTC_SRGB8_A8_10x10
			case ASTC_SRGB8_A8_12x10
			case ASTC_SRGB8_A8_12x12*/
		}
		RETURN_ERR( "unknown pixel format " << String().FormatI( value, 16 ) );
	}


}	// Platforms
}	// Engine
