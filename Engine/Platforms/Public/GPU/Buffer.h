// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/BufferEnums.h"
#include "Engine/Platforms/Public/GPU/MemoryEnums.h"
#include "Engine/Platforms/Public/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// Buffer Description
	//

	struct BufferDescription : CompileTime::PODStruct
	{
	// variables
		BytesU				size;
		EBufferUsage::bits	usage;

	// methods
		BufferDescription (GX_DEFCTOR) {}
		BufferDescription (BytesU size, EBufferUsage::bits usage) : size(size), usage(usage) {}
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Buffer Create Info
	//
	struct GpuBuffer
	{
	// types
		using BufferDescription	= Platforms::BufferDescription;
		using EGpuMemory		= Platforms::EGpuMemory;
		using EMemoryAccess		= Platforms::EMemoryAccess;

	// variables
		ModulePtr				gpuThread;			// can be null
		ModulePtr				memManager;			// can be null
		BufferDescription		descr;
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		bool					allocMem = true;	// if true then you don't need to attach memory module to buffer

	// methods
		GpuBuffer () {}

		explicit GpuBuffer (const BufferDescription &descr) : descr{descr}, allocMem{false} {}

		GpuBuffer (const BufferDescription &descr, EGpuMemory::bits memFlags, EMemoryAccess::bits access = EMemoryAccess::All) :
			descr{descr}, memFlags{memFlags}, access{access}, allocMem{true} {}

		GpuBuffer (const BufferDescription &descr, const ModulePtr &memMngr, EGpuMemory::bits memFlags, EMemoryAccess::bits access = EMemoryAccess::All) :
			memManager{memMngr}, descr{descr}, memFlags{memFlags}, access{access}, allocMem{true} {}
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get Buffer Description
	//
	struct GetBufferDescription : _MsgBase_
	{
		Out< Platforms::BufferDescription >	result;
	};

	struct SetBufferDescription : _MsgBase_
	{
	// variables
		Platforms::BufferDescription			descr;

	// methods
		explicit SetBufferDescription (const Platforms::BufferDescription &descr) : descr{descr} {}
	};


}	// GpuMsg
}	// Engine
