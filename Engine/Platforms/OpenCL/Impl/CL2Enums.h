// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenCL/Impl/cl2.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Shared/GPU/BufferEnums.h"
#include "Engine/Platforms/Shared/GPU/CommandEnums.h"
#include "Engine/Platforms/Shared/GPU/MemoryEnums.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Shared/GPU/ShaderEnums.h"
#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
#include "Engine/Platforms/Shared/GPU/ObjectEnums.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace Platforms;
	
	class CL2Device;
	
	enum CL2MemoryFlags {};
	enum CL2MemoryAccess {};
	enum CL2ImageType {};
	enum CL2ChannelOrder {};
	enum CL2ChannelDataType {};
	enum CL2BytePerPixel {};
	
/*
=================================================
	MemoryFlags
=================================================
*/
	inline CL2MemoryFlags CL2Enum (EGpuMemory::bits value)
	{
		if ( value[EGpuMemory::LocalInGPU] )
			return CL2MemoryFlags( cl::CL_MEM_READ_WRITE | cl::CL_MEM_HOST_NO_ACCESS );
		
		if ( value[EGpuMemory::CoherentWithCPU] or value[EGpuMemory::CachedInCPU])
			return CL2MemoryFlags( cl::CL_MEM_READ_WRITE );

		RETURN_ERR( "invalid memory flags", CL2MemoryFlags() );
	}
	
/*
=================================================
	MemoryAccess
=================================================
*/
	inline CL2MemoryAccess CL2Enum (EMemoryAccess::bits access, GpuMsg::MapMemoryToCpu::EMappingFlags flags)
	{
		using EMappingFlags = GpuMsg::MapMemoryToCpu::EMappingFlags;

		switch ( flags )
		{
			case EMappingFlags::Read :
				CHECK_ERR( access[EMemoryAccess::GpuRead], CL2MemoryAccess() );
				return (CL2MemoryAccess) cl::CL_MAP_READ;

			case EMappingFlags::Write :
				CHECK_ERR( access[EMemoryAccess::GpuWrite], CL2MemoryAccess() );
				return (CL2MemoryAccess) cl::CL_MAP_WRITE;

			case EMappingFlags::ReadWrite :
				CHECK_ERR( access[EMemoryAccess::CpuRead] and access[EMemoryAccess::CpuWrite], CL2MemoryAccess() );
				return CL2MemoryAccess( cl::CL_MAP_READ | cl::CL_MAP_WRITE );

			case EMappingFlags::WriteDiscard :
				CHECK_ERR( access[EMemoryAccess::CpuWrite], CL2MemoryAccess() );
				return (CL2MemoryAccess) cl::CL_MAP_WRITE_INVALIDATE_REGION;
		}

		RETURN_ERR( "invalid memory mapping flags", CL2MemoryAccess() );
	}
	
/*
=================================================
	ImageType
=================================================
*/
	inline CL2ImageType CL2Enum (EImage::type value)
	{
		switch ( value )
		{
			case EImage::Tex2D		:	return (CL2ImageType) cl::CL_MEM_OBJECT_IMAGE2D;
			case EImage::Tex3D		:	return (CL2ImageType) cl::CL_MEM_OBJECT_IMAGE3D;

#		 if COMPUTE_API_OPENCL >= 120
			case EImage::Buffer		:	return (CL2ImageType) cl::CL_MEM_OBJECT_IMAGE1D_BUFFER;
			case EImage::Tex1D		:	return (CL2ImageType) cl::CL_MEM_OBJECT_IMAGE1D;
			case EImage::Tex1DArray	:	return (CL2ImageType) cl::CL_MEM_OBJECT_IMAGE1D_ARRAY;
			case EImage::Tex2DArray	:	return (CL2ImageType) cl::CL_MEM_OBJECT_IMAGE2D_ARRAY;
#		 endif
		}
		RETURN_ERR( "invalid image type!", CL2ImageType() );
	}

/*
=================================================
	PixelFormat
=================================================
*/
	inline bool CL2Enum (EPixelFormat::type value, OUT CL2ChannelOrder &order, OUT CL2ChannelDataType &type, OUT BytesU &bpp)
	{
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

		const bool	norm = EnumEq( value, _vtypeinfo::_NORM );
		//const bool	srgb = EnumEq( value, _vtypeinfo::_SRGB );

		bpp = BytesU( EPixelFormat::BitPerPixel( value ) );

		switch ( value & (_vtypeinfo::_TYPE_MASK | _vtypeinfo::_UNSIGNED) )
		{
			case _vtypeinfo::_BYTE						:	type = (CL2ChannelDataType) (norm ? cl::CL_SNORM_INT8  : cl::CL_SIGNED_INT8);		break;
			case _vtypeinfo::_SHORT						:	type = (CL2ChannelDataType) (norm ? cl::CL_SNORM_INT16 : cl::CL_SIGNED_INT16);		break;
			case _vtypeinfo::_INT						:	type = (CL2ChannelDataType) cl::CL_SIGNED_INT32;									break;
			case _vtypeinfo::_UBYTE						:	type = (CL2ChannelDataType) (norm ? cl::CL_UNORM_INT8  : cl::CL_UNSIGNED_INT8);		break;
			case _vtypeinfo::_USHORT					:	type = (CL2ChannelDataType) (norm ? cl::CL_UNORM_INT16 : cl::CL_UNSIGNED_INT16);	break;
			case _vtypeinfo::_UINT						:	type = (CL2ChannelDataType) cl::CL_UNSIGNED_INT32;									break;
			case _vtypeinfo::_HALF						:	type = (CL2ChannelDataType) cl::CL_HALF_FLOAT;			break;
			case _vtypeinfo::_FLOAT						:	type = (CL2ChannelDataType) cl::CL_FLOAT;				break;
			case _vtypeinfo::_INT_10_10_10				:	type = (CL2ChannelDataType) cl::CL_UNORM_INT_101010;	break;
			case _vtypeinfo::_USHORT_5_5_5				:	type = (CL2ChannelDataType) cl::CL_UNORM_SHORT_555;		break;
			case _vtypeinfo::_USHORT_5_6_5				:	type = (CL2ChannelDataType) cl::CL_UNORM_SHORT_565;		break;
			//case _vtypeinfo::_FLOAT_DEPTH_32_STENCIL_8	:	type = (CL2ChannelDataType) cl::CL_FLOAT;				break;
				
#		 if COMPUTE_API_OPENCL >= 120
			case _vtypeinfo::_INT24						:	type = (CL2ChannelDataType) cl::CL_UNORM_INT24;			break;
			//case _vtypeinfo::_DEPTH_24_STENCIL_8		:	type = (CL2ChannelDataType) cl::CL_UNORM_INT24;			break;
#		 endif

			default											:	RETURN_ERR( "invalid pixel type" );
		}

		switch ( value & _vtypeinfo::_COL_MASK )
		{
			case _vtypeinfo::_R				:	order = (CL2ChannelOrder) cl::CL_R;								break;
			case _vtypeinfo::_RG			:	order = (CL2ChannelOrder) cl::CL_RG;							break;
				
#		 if COMPUTE_API_OPENCL >= 120
			case _vtypeinfo::_DEPTH			:	order = (CL2ChannelOrder) cl::CL_DEPTH;							break;
			case _vtypeinfo::_DEPTH_STENCIL	:	order = (CL2ChannelOrder) cl::CL_DEPTH_STENCIL;					break;
#		 endif

#		 if COMPUTE_API_OPENCL >= 200
			case _vtypeinfo::_RGB			:	order = (CL2ChannelOrder) (srgb ? cl::CL_sRGB : cl::CL_RGB);	break;
			case _vtypeinfo::_RGBA			:	order = (CL2ChannelOrder) (srgb ? cl::CL_sRGBA : cl::CL_RGBA);	break;
#		 else
			case _vtypeinfo::_RGB			:	order = (CL2ChannelOrder) cl::CL_RGB;							break;	// Warning: this format has some problems!
			case _vtypeinfo::_RGBA			:	order = (CL2ChannelOrder) cl::CL_RGBA;							break;
#		 endif

			default							:	RETURN_ERR( "invalid pixel format" );
		}
		return true;
	}


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
