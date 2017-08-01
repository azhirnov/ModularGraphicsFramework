// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/MemoryEnums.h"

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
		ModulePtr						gpuThread;
		BytesUL							maxSize;
		Platforms::EGpuMemory::bits		memFlags;
		Base::EMemoryAccess::bits		access;
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
	

	//
	// On GPU Memory Binding State Changed
	//
	struct OnGpuMemoryBindingChanged
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

}	// ModuleMsg
}	// Engine
