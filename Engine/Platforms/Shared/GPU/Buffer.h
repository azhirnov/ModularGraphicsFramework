// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/BufferEnums.h"

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
		BufferDescriptor (BytesUL size, EBufferUsage::bits usage) : size(size), usage(usage) {}
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
	};

}	// CreateInfo


namespace ModuleMsg
{

	//
	// Get GPU Buffer Descriptor
	//
	struct GetGpuBufferDescriptor
	{
		Out< Platforms::BufferDescriptor >	result;
	};

	
	// platform-dependent
	struct GetVkBufferID;
	struct GetGLBufferID;


}	// ModuleMsg
}	// Engine
