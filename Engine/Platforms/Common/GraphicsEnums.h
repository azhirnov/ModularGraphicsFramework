// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EValueTypeInfo
	{
		enum type : uint
		{
			// flags
			_FLAG_OFF		= 0,
			_FLAG_MASK		= 0x1F << _FLAG_OFF,
			_NORM			= 1 << _FLAG_OFF,
			_UNSIGNED		= 2 << _FLAG_OFF,
			_COMPRESSED		= 4 << _FLAG_OFF,
			_REVERSE		= 8 << _FLAG_OFF,	// RGBA to BGRA
			_SRGB			= 16 << _FLAG_OFF,

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

			_INT_10_10_10_2			= 10 << _TYPE_OFF,
			_FLOAT_11_11_10			= 11 << _TYPE_OFF,
			_USHORT_4_4_4_4			= 12 << _TYPE_OFF,
			_USHORT_5_5_5_1			= 13 << _TYPE_OFF,
			_USHORT_5_6_5			= 14 << _TYPE_OFF,
			_INT_10_10_10			= 15 << _TYPE_OFF,
			_USHORT_5_5_5			= 16 << _TYPE_OFF,

			_INT24					= 20 << _TYPE_OFF,
			_DEPTH_24_STENCIL_8		= 21 << _TYPE_OFF,
			_FLOAT_DEPTH_32_STENCIL_8 = 22 << _TYPE_OFF,


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
			Depth24_Stencil8		= _DEPTH_24_STENCIL_8 | _DEPTH_STENCIL | _NORM,
			Depth32F_Stencil8		= _FLOAT_DEPTH_32_STENCIL_8 | _DEPTH_STENCIL,

			// compressed types
			DXT1_RGB8_UNorm			= (100 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGB,
			DXT1_RGB8_A1_UNorm		= (101 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,
			DXT3_RGBA8_UNorm		= (102 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,
			DXT5_RGBA8_UNorm		= (103 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,
			RGTC1_RED8_SNorm		= (104 << _TYPE_OFF) | _NORM | _COMPRESSED | _R,
			RGTC1_RED8_UNorm		= RGTC1_RED8_SNorm  | _UNSIGNED,
			RGTC2_RG8_SNorm			= (105 << _TYPE_OFF) | _NORM | _COMPRESSED | _RG,
			RGTC2_RG8_UNorm			= RGTC2_RG8_SNorm   | _UNSIGNED,
			BPTC_RGBA8_UNorm		= (106 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA,
			BPTC_SRGB8_A8_UNorm		= (107 << _TYPE_OFF) | _NORM | _UNSIGNED | _COMPRESSED | _RGBA | _SRGB,
			BPTC_RGB16F				= (108 << _TYPE_OFF) | _COMPRESSED | _RGB,
			BPTC_RGB16F_Unsigned	= BPTC_RGB16F | _UNSIGNED,
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


	struct EAttribute
	{
		enum type : uint
		{
			NormalizedFlag	= EValueTypeInfo::_NORM,

			Byte			= EValueTypeInfo::Byte,
			Byte2			= EValueTypeInfo::Byte2,
			Byte3			= EValueTypeInfo::Byte3,
			Byte4			= EValueTypeInfo::Byte4,
			
			Byte_Norm		= Byte  | NormalizedFlag,
			Byte2_Norm		= Byte2 | NormalizedFlag,
			Byte3_Norm		= Byte3 | NormalizedFlag,
			Byte4_Norm		= Byte4 | NormalizedFlag,

			UByte			= EValueTypeInfo::UByte,
			UByte2			= EValueTypeInfo::UByte2,
			UByte3			= EValueTypeInfo::UByte3,
			UByte4			= EValueTypeInfo::UByte4,
			
			UByte_Norm		= UByte  | NormalizedFlag,
			UByte2_Norm		= UByte2 | NormalizedFlag,
			UByte3_Norm		= UByte3 | NormalizedFlag,
			UByte4_Norm		= UByte4 | NormalizedFlag,
			
			Short			= EValueTypeInfo::Short,
			Short2			= EValueTypeInfo::Short2,
			Short3			= EValueTypeInfo::Short3,
			Short4			= EValueTypeInfo::Short4,
			
			Short_Norm		= Short  | NormalizedFlag,
			Short2_Norm		= Short2 | NormalizedFlag,
			Short3_Norm		= Short3 | NormalizedFlag,
			Short4_Norm		= Short4 | NormalizedFlag,
			
			UShort			= EValueTypeInfo::UShort,
			UShort2			= EValueTypeInfo::UShort2,
			UShort3			= EValueTypeInfo::UShort3,
			UShort4			= EValueTypeInfo::UShort4,
			
			UShort_Norm		= UShort   | NormalizedFlag,
			UShort2_Norm	= UShort2 | NormalizedFlag,
			UShort3_Norm	= UShort3 | NormalizedFlag,
			UShort4_Norm	= UShort4 | NormalizedFlag,
			
			Int				= EValueTypeInfo::Int,
			Int2			= EValueTypeInfo::Int2,
			Int3			= EValueTypeInfo::Int3,
			Int4			= EValueTypeInfo::Int4,
			
			Int_Norm		= Int  | NormalizedFlag,
			Int2_Norm		= Int2 | NormalizedFlag,
			Int3_Norm		= Int3 | NormalizedFlag,
			Int4_Norm		= Int4 | NormalizedFlag,
			
			UInt			= EValueTypeInfo::UInt,
			UInt2			= EValueTypeInfo::UInt2,
			UInt3			= EValueTypeInfo::UInt3,
			UInt4			= EValueTypeInfo::UInt4,
			
			UInt_Norm		= UInt  | NormalizedFlag,
			UInt2_Norm		= UInt2 | NormalizedFlag,
			UInt3_Norm		= UInt3 | NormalizedFlag,
			UInt4_Norm		= UInt4 | NormalizedFlag,
			
			Half			= EValueTypeInfo::Half,
			Half2			= EValueTypeInfo::Half2,
			Half3			= EValueTypeInfo::Half3,
			Half4			= EValueTypeInfo::Half4,

			Float			= EValueTypeInfo::Float,
			Float2			= EValueTypeInfo::Float2,
			Float3			= EValueTypeInfo::Float3,
			Float4			= EValueTypeInfo::Float4,
			
			Double			= EValueTypeInfo::Double,
			Double2			= EValueTypeInfo::Double2,
			Double3			= EValueTypeInfo::Double3,
			Double4			= EValueTypeInfo::Double4,

			Unknown			= uint(-1),
		};

		struct ValueType
		{
			inline static bool IsInteger	(type value);
			inline static bool IsFloat		(type value);
			inline static bool IsDouble		(type value);
			inline static type ToVSInputType (type value);	// vertex shader input type
		};

		inline static type Build (type scalarValue, uint count);

		inline static StringCRef ToString (type value);

		inline static type SetNormalized (type value, bool norm);
	};


	struct EIndex
	{
		enum type : uint
		{
			UByte		= 0,
			UShort,
			UInt,

			_Count,
			Unknown		= uint(-1),
		};

		inline static BytesU SizeOf (type value);
	};


	struct EPixelFormat
	{
		enum type : uint
		{
			// signed normalized
			RGBA16_SNorm			= EValueTypeInfo::RGBA16_SNorm,
			RGBA8_SNorm				= EValueTypeInfo::RGBA8_SNorm,
			RGB16_SNorm				= EValueTypeInfo::RGB16_SNorm,
			RGB8_SNorm				= EValueTypeInfo::RGB8_SNorm,
			RG16_SNorm				= EValueTypeInfo::RG16_SNorm,
			RG8_SNorm				= EValueTypeInfo::RG8_SNorm,
			R16_SNorm				= EValueTypeInfo::R16_SNorm,
			R8_SNorm				= EValueTypeInfo::R8_SNorm,
			
			// unsigned normalized
			RGBA16_UNorm			= EValueTypeInfo::RGBA16_UNorm,
			RGBA8_UNorm				= EValueTypeInfo::RGBA8_UNorm,
			RGB16_UNorm				= EValueTypeInfo::RGB16_UNorm,
			RGB8_UNorm				= EValueTypeInfo::RGB8_UNorm,
			RG16_UNorm				= EValueTypeInfo::RG16_UNorm,
			RG8_UNorm				= EValueTypeInfo::RG8_UNorm,
			R16_UNorm				= EValueTypeInfo::R16_UNorm,
			R8_UNorm				= EValueTypeInfo::R8_UNorm,
			RGB10_A2_UNorm			= EValueTypeInfo::RGB10_A2_UNorm,
			RGBA4_UNorm				= EValueTypeInfo::RGBA4_UNorm,
			RGB5_A1_UNorm			= EValueTypeInfo::RGB5_A1_UNorm,
			RGB_5_6_5_UNorm			= EValueTypeInfo::RGB_5_6_5_UNorm,
			
			// sRGB
			SRGB8					= EValueTypeInfo::SRGB8,
			SRGB8_A8				= EValueTypeInfo::SRGB8_A8,

			// signed integer
			R8I						= EValueTypeInfo::Byte,
			RG8I					= EValueTypeInfo::Byte2,
			RGB8I					= EValueTypeInfo::Byte3,
			RGBA8I					= EValueTypeInfo::Byte4,
			R16I					= EValueTypeInfo::Short,
			RG16I					= EValueTypeInfo::Short2,
			RGB16I					= EValueTypeInfo::Short3,
			RGBA16I					= EValueTypeInfo::Short4,
			R32I					= EValueTypeInfo::Int,
			RG32I					= EValueTypeInfo::Int2,
			RGB32I					= EValueTypeInfo::Int3,
			RGBA32I					= EValueTypeInfo::Int4,
			
			// unsigned integer
			R8U						= EValueTypeInfo::UByte,
			RG8U					= EValueTypeInfo::UByte2,
			RGB8U					= EValueTypeInfo::UByte3,
			RGBA8U					= EValueTypeInfo::UByte4,
			R16U					= EValueTypeInfo::UShort,
			RG16U					= EValueTypeInfo::UShort2,
			RGB16U					= EValueTypeInfo::UShort3,
			RGBA16U					= EValueTypeInfo::UShort4,
			R32U					= EValueTypeInfo::UInt,
			RG32U					= EValueTypeInfo::UInt2,
			RGB32U					= EValueTypeInfo::UInt3,
			RGBA32U					= EValueTypeInfo::UInt4,
			RGB10_A2U				= EValueTypeInfo::RGB10_A2U,
			
			// float
			R16F					= EValueTypeInfo::Half,
			RG16F					= EValueTypeInfo::Half2,
			RGB16F					= EValueTypeInfo::Half3,
			RGBA16F					= EValueTypeInfo::Half4,
			R32F					= EValueTypeInfo::Float,
			RG32F					= EValueTypeInfo::Float2,
			RGB32F					= EValueTypeInfo::Float3,
			RGBA32F					= EValueTypeInfo::Float4,
			RGB_11_11_10F			= EValueTypeInfo::RGB_11_11_10F,

			// depth stencil
			Depth16					= EValueTypeInfo::Depth16,
			Depth24					= EValueTypeInfo::Depth24,
			Depth32					= EValueTypeInfo::Depth32,
			Depth32F				= EValueTypeInfo::Depth32F,
			Depth24_Stencil8		= EValueTypeInfo::Depth24_Stencil8,
			Depth32F_Stencil8		= EValueTypeInfo::Depth32F_Stencil8,

			// compressed
			DXT1_RGB8_UNorm			= EValueTypeInfo::DXT1_RGB8_UNorm,
			DXT1_RGB8_A1_UNorm		= EValueTypeInfo::DXT1_RGB8_A1_UNorm,
			DXT3_RGBA8_UNorm		= EValueTypeInfo::DXT3_RGBA8_UNorm,
			DXT5_RGBA8_UNorm		= EValueTypeInfo::DXT5_RGBA8_UNorm,
			RGTC1_RED8_SNorm		= EValueTypeInfo::RGTC1_RED8_SNorm,
			RGTC1_RED8_UNorm		= EValueTypeInfo::RGTC1_RED8_UNorm,
			RGTC2_RG8_SNorm			= EValueTypeInfo::RGTC2_RG8_SNorm,
			RGTC2_RG8_UNorm			= EValueTypeInfo::RGTC2_RG8_UNorm,
			BPTC_RGBA8_UNorm		= EValueTypeInfo::BPTC_RGBA8_UNorm,
			BPTC_SRGB8_A8_UNorm		= EValueTypeInfo::BPTC_SRGB8_A8_UNorm,
			BPTC_RGB16F				= EValueTypeInfo::BPTC_RGB16F,
			BPTC_RGB16F_Unsigned	= EValueTypeInfo::BPTC_RGB16F_Unsigned,
			ETC2_RGB8_UNorm			= EValueTypeInfo::ETC2_RGB8_UNorm,
			ECT2_SRGB8_UNorm		= EValueTypeInfo::ECT2_SRGB8_UNorm,
			ETC2_RGB8_A1_UNorm		= EValueTypeInfo::ETC2_RGB8_A1_UNorm,
			ETC2_SRGB8_A1_UNorm		= EValueTypeInfo::ETC2_SRGB8_A1_UNorm,
			ETC2_RGBA8_UNorm		= EValueTypeInfo::ETC2_RGBA8_UNorm,
			ETC2_SRGB8_A8_UNorm		= EValueTypeInfo::ETC2_SRGB8_A8_UNorm,
			EAC_R11_SNorm			= EValueTypeInfo::EAC_R11_SNorm,
			EAC_R11_UNorm			= EValueTypeInfo::EAC_R11_UNorm,
			EAC_RG11_SNorm			= EValueTypeInfo::EAC_RG11_SNorm,
			EAC_RG11_UNorm			= EValueTypeInfo::EAC_RG11_UNorm,
			ASTC_RGBA_4x4			= EValueTypeInfo::ASTC_RGBA_4x4,
			ASTC_RGBA_5x4			= EValueTypeInfo::ASTC_RGBA_5x4,
			ASTC_RGBA_5x5			= EValueTypeInfo::ASTC_RGBA_5x5,
			ASTC_RGBA_6x5			= EValueTypeInfo::ASTC_RGBA_6x5,
			ASTC_RGBA_6x6			= EValueTypeInfo::ASTC_RGBA_6x6,
			ASTC_RGBA_8x5			= EValueTypeInfo::ASTC_RGBA_8x5,
			ASTC_RGBA_8x6			= EValueTypeInfo::ASTC_RGBA_8x6,
			ASTC_RGBA_8x8			= EValueTypeInfo::ASTC_RGBA_8x8,
			ASTC_RGBA_10x5			= EValueTypeInfo::ASTC_RGBA_10x5,
			ASTC_RGBA_10x6			= EValueTypeInfo::ASTC_RGBA_10x6,
			ASTC_RGBA_10x8			= EValueTypeInfo::ASTC_RGBA_10x8,
			ASTC_RGBA_10x10			= EValueTypeInfo::ASTC_RGBA_10x10,
			ASTC_RGBA_12x10			= EValueTypeInfo::ASTC_RGBA_12x10,
			ASTC_RGBA_12x12			= EValueTypeInfo::ASTC_RGBA_12x12,
			ASTC_SRGB8_A8_4x4		= EValueTypeInfo::ASTC_SRGB8_A8_4x4,
			ASTC_SRGB8_A8_5x4		= EValueTypeInfo::ASTC_SRGB8_A8_5x4,
			ASTC_SRGB8_A8_5x5		= EValueTypeInfo::ASTC_SRGB8_A8_5x5,
			ASTC_SRGB8_A8_6x5		= EValueTypeInfo::ASTC_SRGB8_A8_6x5,
			ASTC_SRGB8_A8_6x6		= EValueTypeInfo::ASTC_SRGB8_A8_6x6,
			ASTC_SRGB8_A8_8x5		= EValueTypeInfo::ASTC_SRGB8_A8_8x5,
			ASTC_SRGB8_A8_8x6		= EValueTypeInfo::ASTC_SRGB8_A8_8x6,
			ASTC_SRGB8_A8_8x8		= EValueTypeInfo::ASTC_SRGB8_A8_8x8,
			ASTC_SRGB8_A8_10x5		= EValueTypeInfo::ASTC_SRGB8_A8_10x5,
			ASTC_SRGB8_A8_10x6		= EValueTypeInfo::ASTC_SRGB8_A8_10x6,
			ASTC_SRGB8_A8_10x8		= EValueTypeInfo::ASTC_SRGB8_A8_10x8,
			ASTC_SRGB8_A8_10x10		= EValueTypeInfo::ASTC_SRGB8_A8_10x10,
			ASTC_SRGB8_A8_12x10		= EValueTypeInfo::ASTC_SRGB8_A8_12x10,
			ASTC_SRGB8_A8_12x12		= EValueTypeInfo::ASTC_SRGB8_A8_12x12,

			Unknown					= uint(-1),
		};

		inline static bool IsCompressed (type value);

		inline static bool IsSupportedForImage (type value);

		inline static bool IsSupportedForBuffer (type value);

		inline static uint NumColorChannels (type value);

		inline static BitsU BitPerPixel (type value);

		template <typename T>
		inline static type FromType (const T& = T(), bool normalized = false);
		
		template <typename T>
		inline static type FromNormType (const T& = T());

		struct ColorSpace
		{
			inline static bool IsNonLinear	(type value);
			inline static bool IsLinear		(type value);
		};

		struct Format
		{
			inline static bool IsColor			(type value);
			inline static bool IsDepth			(type value);
			inline static bool IsStencil		(type value);
			inline static bool IsDepthStencil	(type value);
			inline static bool HasDepth			(type value);
			inline static bool HasStencil		(type value);
		};

		struct DataType
		{
			inline static bool IsFloat		(type value);
			inline static bool IsInteger	(type value);
		};

		struct ValueType
		{
			inline static bool IsUnsignedInteger	(type value);
			inline static bool IsSignedInteger		(type value);
			inline static bool IsUnsignedNormalized	(type value);
			inline static bool IsSignedNormalized	(type value);
			inline static bool IsSignedFloat		(type value);
			inline static bool IsUnsignedFloat		(type value);

			inline static bool IsFloat				(type value);
			inline static bool IsInteger			(type value);
		};
	};


}	// Platforms
}	// Engine
