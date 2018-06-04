// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/OpenCL/120/cl2.h"
#include "Engine/Platforms/Public/GPU/BufferEnums.h"
#include "Engine/Platforms/Public/GPU/CommandEnums.h"
#include "Engine/Platforms/Public/GPU/MemoryEnums.h"
#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Public/GPU/ShaderEnums.h"
#include "Engine/Platforms/Public/GPU/ImageEnums.h"
#include "Engine/Platforms/Public/GPU/ObjectEnums.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/SamplerEnums.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace Engine::Platforms;
	
	class CL1Device;
	

	struct ESharing
	{
		enum type : uint
		{
			None,
			OpenGL,
			DirectX10,
			DirectX11,
		};

		static constexpr bool IsDirectX (type value);
	};

/*
=================================================
	IsDirectX
=================================================
*/
	constexpr bool ESharing::IsDirectX (type value)
	{
		return value == DirectX10 or value == DirectX11;
	}
//-----------------------------------------------------------------------------


	
	enum CL1MemoryFlags {};
	enum CL1MemoryAccess {};
	enum CL1ImageType {};
	enum CL1ChannelOrder {};
	enum CL1ChannelDataType {};
	enum CL1BytePerPixel {};
	enum CL1AddressingMode {};
	enum CL1FilterMode {};

/*
=================================================
	MemoryFlags
=================================================
*/
	inline CL1MemoryFlags CL1Enum (EGpuMemory::bits value, EMemoryAccess::bits access)
	{
		using namespace cl;
		
		uint	flags = 0;

		if ( access[EMemoryAccess::GpuRead] and access[EMemoryAccess::GpuWrite] )
			flags = CL_MEM_READ_WRITE;
		else
		if ( access[EMemoryAccess::GpuRead] )
			flags = CL_MEM_READ_ONLY;
		else
		if ( access[EMemoryAccess::GpuWrite] )
			flags = CL_MEM_WRITE_ONLY;

		if ( value[EGpuMemory::LocalInGPU] )
			return CL1MemoryFlags( flags | CL_MEM_HOST_NO_ACCESS );
		

		if ( value[EGpuMemory::CoherentWithCPU] or value[EGpuMemory::CachedInCPU])
		{
			if ( access[EMemoryAccess::CpuRead] and access[EMemoryAccess::CpuWrite] )
				{}	// no optimizations
			else
			if ( access[EMemoryAccess::CpuRead] )
				flags |= CL_MEM_HOST_READ_ONLY;
			else
			if ( access[EMemoryAccess::CpuWrite] )
				flags |= CL_MEM_HOST_WRITE_ONLY;

			return CL1MemoryFlags( flags );
		}

		RETURN_ERR( "invalid memory flags", CL1MemoryFlags() );
	}
	
/*
=================================================
	MemoryAccess
=================================================
*/
	inline CL1MemoryAccess CL1Enum (EMemoryAccess::bits access, GpuMsg::EMappingFlags flags)
	{
		switch ( flags )
		{
			case GpuMsg::EMappingFlags::Read :
				CHECK_ERR( access[EMemoryAccess::GpuRead], CL1MemoryAccess() );
				return (CL1MemoryAccess) cl::CL_MAP_READ;

			case GpuMsg::EMappingFlags::Write :
				CHECK_ERR( access[EMemoryAccess::GpuWrite], CL1MemoryAccess() );
				return (CL1MemoryAccess) cl::CL_MAP_WRITE;

			case GpuMsg::EMappingFlags::ReadWrite :
				CHECK_ERR( access[EMemoryAccess::CpuRead] and access[EMemoryAccess::CpuWrite], CL1MemoryAccess() );
				return CL1MemoryAccess( cl::CL_MAP_READ | cl::CL_MAP_WRITE );

			case GpuMsg::EMappingFlags::WriteDiscard :
				CHECK_ERR( access[EMemoryAccess::CpuWrite], CL1MemoryAccess() );
				return (CL1MemoryAccess) cl::CL_MAP_WRITE_INVALIDATE_REGION;
		}

		RETURN_ERR( "invalid memory mapping flags", CL1MemoryAccess() );
	}
	
/*
=================================================
	ImageType
=================================================
*/
	inline CL1ImageType CL1Enum (EImage::type value)
	{
		switch ( value )
		{
			case EImage::Tex2D		:	return (CL1ImageType) cl::CL_MEM_OBJECT_IMAGE2D;
			case EImage::Tex3D		:	return (CL1ImageType) cl::CL_MEM_OBJECT_IMAGE3D;

#		 if COMPUTE_API_OPENCL >= 120
			case EImage::Buffer		:	return (CL1ImageType) cl::CL_MEM_OBJECT_IMAGE1D_BUFFER;
			case EImage::Tex1D		:	return (CL1ImageType) cl::CL_MEM_OBJECT_IMAGE1D;
			case EImage::Tex1DArray	:	return (CL1ImageType) cl::CL_MEM_OBJECT_IMAGE1D_ARRAY;
			case EImage::Tex2DArray	:	return (CL1ImageType) cl::CL_MEM_OBJECT_IMAGE2D_ARRAY;
#		 endif
		}
		RETURN_ERR( "invalid image type!", CL1ImageType() );
	}

/*
=================================================
	PixelFormat
=================================================
*/
	inline bool CL1Enum (EPixelFormat::type value, OUT CL1ChannelOrder &order, OUT CL1ChannelDataType &type, OUT BytesU &bpp)
	{
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

		const bool	norm = EnumEq( value, _vtypeinfo::_NORM );
		//const bool	srgb = EnumEq( value, _vtypeinfo::_SRGB );

		bpp = BytesU( EPixelFormat::BitPerPixel( value ) );

		switch ( value & (_vtypeinfo::_TYPE_MASK | _vtypeinfo::_UNSIGNED) )
		{
			case _vtypeinfo::_BYTE						:	type = (CL1ChannelDataType) (norm ? cl::CL_SNORM_INT8  : cl::CL_SIGNED_INT8);		break;
			case _vtypeinfo::_SHORT						:	type = (CL1ChannelDataType) (norm ? cl::CL_SNORM_INT16 : cl::CL_SIGNED_INT16);		break;
			case _vtypeinfo::_INT						:	type = (CL1ChannelDataType) cl::CL_SIGNED_INT32;									break;
			case _vtypeinfo::_UBYTE						:	type = (CL1ChannelDataType) (norm ? cl::CL_UNORM_INT8  : cl::CL_UNSIGNED_INT8);		break;
			case _vtypeinfo::_USHORT					:	type = (CL1ChannelDataType) (norm ? cl::CL_UNORM_INT16 : cl::CL_UNSIGNED_INT16);	break;
			case _vtypeinfo::_UINT						:	type = (CL1ChannelDataType) cl::CL_UNSIGNED_INT32;									break;
			case _vtypeinfo::_HALF						:	type = (CL1ChannelDataType) cl::CL_HALF_FLOAT;			break;
			case _vtypeinfo::_FLOAT						:	type = (CL1ChannelDataType) cl::CL_FLOAT;				break;
			case _vtypeinfo::_INT_10_10_10				:	type = (CL1ChannelDataType) cl::CL_UNORM_INT_101010;	break;
			case _vtypeinfo::_USHORT_5_5_5				:	type = (CL1ChannelDataType) cl::CL_UNORM_SHORT_555;		break;
			case _vtypeinfo::_USHORT_5_6_5				:	type = (CL1ChannelDataType) cl::CL_UNORM_SHORT_565;		break;
			//case _vtypeinfo::_FLOAT_DEPTH_32_STENCIL_8	:	type = (CL1ChannelDataType) cl::CL_FLOAT;				break;
				
#		 if COMPUTE_API_OPENCL >= 120
			case _vtypeinfo::_INT24						:	type = (CL1ChannelDataType) cl::CL_UNORM_INT24;			break;
			//case _vtypeinfo::_DEPTH_24_STENCIL_8		:	type = (CL1ChannelDataType) cl::CL_UNORM_INT24;			break;
#		 endif

			default											:	RETURN_ERR( "invalid pixel type" );
		}

		switch ( value & _vtypeinfo::_COL_MASK )
		{
			case _vtypeinfo::_R				:	order = (CL1ChannelOrder) cl::CL_R;								break;
			case _vtypeinfo::_RG			:	order = (CL1ChannelOrder) cl::CL_RG;							break;
				
#		 if COMPUTE_API_OPENCL >= 120
			case _vtypeinfo::_DEPTH			:	order = (CL1ChannelOrder) cl::CL_DEPTH;							break;
			case _vtypeinfo::_DEPTH_STENCIL	:	order = (CL1ChannelOrder) cl::CL_DEPTH_STENCIL;					break;
#		 endif

#		 if COMPUTE_API_OPENCL >= 200
			case _vtypeinfo::_RGB			:	order = (CL1ChannelOrder) (srgb ? cl::CL_sRGB : cl::CL_RGB);	break;
			case _vtypeinfo::_RGBA			:	order = (CL1ChannelOrder) (srgb ? cl::CL_sRGBA : cl::CL_RGBA);	break;
#		 else
			case _vtypeinfo::_RGB			:	order = (CL1ChannelOrder) cl::CL_RGB;							break;	// Warning: this format has some problems!
			case _vtypeinfo::_RGBA			:	order = (CL1ChannelOrder) cl::CL_RGBA;							break;
#		 endif

			default							:	RETURN_ERR( "invalid pixel format" );
		}
		return true;
	}
	
/*
=================================================
	EAddressMode
=================================================
*/
	inline CL1AddressingMode  CL1Enum (EAddressMode::type value)
	{
		switch ( value )
		{
			case EAddressMode::ClampToEdge :	return (CL1AddressingMode) cl::CL_ADDRESS_CLAMP_TO_EDGE;
			case EAddressMode::ClampToBorder :	return (CL1AddressingMode) cl::CL_ADDRESS_CLAMP;
			case EAddressMode::Repeat :			return (CL1AddressingMode) cl::CL_ADDRESS_REPEAT;
			case EAddressMode::MirroredRepeat :	return (CL1AddressingMode) cl::CL_ADDRESS_MIRRORED_REPEAT;
		}
		RETURN_ERR( "unsupported addressing mode", CL1AddressingMode() );
	}

/*
=================================================
	EFilter
=================================================
*/
	inline CL1FilterMode  CL1Enum (EFilter::type value)
	{
		static const uint	mag_mask = (EFilter::_MAG_NEAREST | EFilter::_MAG_LINEAR);
		
		switch ( value & mag_mask )
		{
			case EFilter::_MAG_NEAREST :	return (CL1FilterMode) cl::CL_FILTER_NEAREST;
			case EFilter::_MAG_LINEAR :		return (CL1FilterMode) cl::CL_FILTER_LINEAR;
		}
		RETURN_ERR( "invalid mag filtering flag", CL1FilterMode() );
	}

}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
