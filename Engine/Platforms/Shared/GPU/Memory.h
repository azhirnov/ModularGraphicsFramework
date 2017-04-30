// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	//
	// GPU Memory Flags
	//

	struct EGpuMemory
	{
		enum type : uint
		{
			LocalInGPU,
			CoherentWithCPU,
			CachedInCPU,
			//LazilyAllocated,

			_Count,
			Unknown = uint(-1)
		};

		using bits = EnumBitfield< EGpuMemory >;
	};


	//
	// GPU Memory Descriptor
	//

	struct GpuMemoryDescriptor
	{
		BytesUL				size;
		EGpuMemory::bits	flags;
	};

}	// Platforms


namespace CreateInfo
{
	//
	// GPU Memory Create Info
	//

	struct GpuMemory
	{
		ModulePtr						gpuThread;
		BytesUL							size;
		Platforms::EGpuMemory::bits		memFlags;
		Base::EMemoryAccess::bits		access;
	};
	
	struct GpuMemoryForObject
	{
		ModulePtr						gpuThread;
		ModulePtr						inStream;		// to initialize gpu memory
		ModulePtr						object;			// only image or buffer are available
		Platforms::EGpuMemory::bits		memFlags;
	};

	struct GpuImageStream
	{
		ModulePtr						gpuThread;
		Platforms::EGpuMemory::bits		memFlags;
		// TODO
	};

}	// CreateInfo


namespace ModuleMsg
{

	//
	// Make GPU Memory visible by CPU
	//
	struct MapGpuMemoryToCpu
	{
		BytesUL			offset;
		BytesUL			size;
	};


	//
	// Hide GPU Memory
	//
	struct UnmapGpuMemory
	{};


	//
	// Flush GPU Memory Range
	//
	struct FlushGpuMemoryRange
	{
		BytesUL			offset;
		BytesUL			size;
	};


	//
	// Copy GPU Memory
	//
	struct CopyGpuMemory
	{
		ModulePtr		srcMemory;
		BytesUL			srcOffset;
		BytesUL			dstOffset;
	};

	
	//
	// Read CPU Visible Memory Data
	//
	/*struct ReadFromGpuToCpuMemory
	{
		BytesUL					offset;
		Optional< BytesUL >		size;
		Out< BinArrayCRef >		result;
	};


	//
	// Write Data to CPU Visible Memory
	//
	struct WriteFromCpuToGpuMemory
	{
		BytesUL				offset;
		BinArrayCRef		data;
		Out< BytesUL >		wasWritten;
	};*/

}	// ModuleMsg
}	// Engine
