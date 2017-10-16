// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/BufferEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// Buffer Descriptor
	//

	struct BufferDescriptor : CompileTime::PODStruct
	{
	// variables
		BytesUL				size;
		EBufferUsage::bits	usage;

	// methods
		BufferDescriptor (GX_DEFCTOR) {}
		BufferDescriptor (Bytes<ulong> size, EBufferUsage::bits usage) : size(size), usage(usage) {}
		BufferDescriptor (Bytes<uint> size, EBufferUsage::bits usage) : size(size), usage(usage) {}
	};

}	// Platforms


namespace CreateInfo
{
	//
	// GPU Buffer Create Info
	//

	struct GpuBuffer
	{
		ModulePtr						gpuThread;
		Platforms::BufferDescriptor		descr;
		bool							allocMemory		= false;	// set 'true' to automaticaly create memory object
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get Buffer Descriptor
	//
	struct GetBufferDescriptor
	{
		Out< Platforms::BufferDescriptor >	result;
	};

	
	// platform-dependent
	struct GetVkBufferID;
	struct GetGLBufferID;


}	// GpuMsg
}	// Engine
