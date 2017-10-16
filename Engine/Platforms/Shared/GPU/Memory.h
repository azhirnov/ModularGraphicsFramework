// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/MemoryEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Memory Descriptor
	//

	struct GpuMemoryDescriptor : CompileTime::PODStruct
	{
	// variables
		BytesUL				size;
		EGpuMemory::bits	flags;

	// methods
		GpuMemoryDescriptor (GX_DEFCTOR) {}
		GpuMemoryDescriptor (BytesUL size, EGpuMemory::bits flags) : size(size), flags(flags) {}
	};

}	// Platforms


namespace CreateInfo
{
	//
	// GPU Memory Create Info
	//

	struct GpuMemory
	{
		using EGpuMemory	= Platforms::EGpuMemory;
		using EMemoryAccess	= Platforms::EMemoryAccess;

	// variables
		ModulePtr				gpuThread;
		BytesUL					maxSize;
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		
	// methods
		GpuMemory (const ModulePtr &gpuThread, Bytes<ulong> maxSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), maxSize(maxSize), memFlags(memFlags), access(access)
		{}

		GpuMemory (const ModulePtr &gpuThread, Bytes<uint> maxSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), maxSize(maxSize), memFlags(memFlags), access(access)
		{}
	};
	
	/*struct GpuMemoryForObject
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
	};*/

}	// CreateInfo


namespace GpuMsg
{

	//
	// Make GPU Memory visible by CPU
	//
	struct MapMemoryToCpu
	{
		BytesUL			offset;
		BytesUL			size;
	};


	//
	// Hide GPU Memory
	//
	struct UnmapMemory
	{};


	//
	// Flush GPU Memory Range
	//
	struct FlushMemoryRange
	{
		BytesUL			offset;
		BytesUL			size;
	};


	//
	// Copy GPU Memory
	//
	struct CopyMemory
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
	

	//
	// On GPU Memory Binding State Changed
	//
	struct OnMemoryBindingChanged
	{
	// types
		enum class EBindingState
		{
			Unbinded,
			BindedToImage,
			BindedToBuffer,
		};

	// variables
		ModulePtr			targetObject;	// image or buffer
		EBindingState		newState;
	};

}	// GpuMsg
}	// Engine
