// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/MemoryEnums.h"
#include "Engine/Platforms/Public/GPU/IDs.h"
#include "Engine/Platforms/Public/GPU/MipmapLevel.h"
#include "Engine/Platforms/Public/GPU/ImageLayer.h"
#include "Core/STL/Math/Image/ImageUtils.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Memory Description
	//

	struct GpuMemoryDescription : CompileTime::PODStruct
	{
	// variables
		BytesUL					size;
		EGpuMemory::bits		flags;
		EMemoryAccess::bits		access;

	// methods
		GpuMemoryDescription (GX_DEFCTOR) {}

		GpuMemoryDescription (BytesUL size, EGpuMemory::bits flags, EMemoryAccess::bits access) :
			size(size), flags(flags), access(access) {}
	};

}	// Platforms


namespace CreateInfo
{

	//
	// GPU Memory Create Info
	//
	struct GpuMemory
	{
	// types
		using EGpuMemory	= Platforms::EGpuMemory;
		using EMemoryAccess	= Platforms::EMemoryAccess;

	// variables
		ModulePtr				gpuThread;		// can be null
		ModulePtr				memManager;
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		
	// methods
		GpuMemory (EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			memFlags(memFlags), access(access) {}

		GpuMemory (const ModulePtr &memMngr, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			memManager(memMngr), memFlags(memFlags), access(access) {}
	};



	//
	// GPU Memory Manager Create Info
	//
	struct GpuMemoryManager
	{
	// types
		using EGpuMemory	= Platforms::EGpuMemory;
		using EMemoryAccess	= Platforms::EMemoryAccess;

	// variables
		ModulePtr				gpuThread;		// can be null
		BytesUL					maxSize;		// max size of allocated memory
		BytesUL					blockSize;		// block size to avoid fragmentation
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		
	// methods
		GpuMemoryManager (Bytes<ulong> maxSize, Bytes<ulong> blockSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			maxSize(maxSize), blockSize(blockSize), memFlags(memFlags), access(access) {}

		GpuMemoryManager (Bytes<uint> maxSize, Bytes<uint> blockSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			maxSize(maxSize), blockSize(blockSize), memFlags(memFlags), access(access) {}
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get GPU Memory Description
	//
	struct GetGpuMemoryDescription : _MessageBase_
	{
		Out< Platforms::GpuMemoryDescription >	result;
	};


	enum class EMappingFlags
	{
		Read,
		Write,
		ReadWrite,
		WriteDiscard,
	};


	//
	// Make GPU Memory visible by CPU
	//
	struct MapMemoryToCpu : _MessageBase_
	{
	// variables
		BytesUL				position;
		BytesUL				size;
		EMappingFlags		flags	= EMappingFlags::ReadWrite;
		Out<BinArrayRef>	result;		// use 'ReadFromGpuMemory' or 'WriteToGpuMemory' instead of this

	// methods
		MapMemoryToCpu () {}
		explicit MapMemoryToCpu (EMappingFlags flags) : size{BytesUL(~0ull)}, flags{flags} {}
		MapMemoryToCpu (EMappingFlags flags, Bytes<uint> pos, Bytes<uint> size) : position(BytesUL(pos)), size(BytesUL(size)), flags(flags) {}
		MapMemoryToCpu (EMappingFlags flags, Bytes<ulong> pos, Bytes<ulong> size) : position(pos), size(size), flags(flags) {}
	};

	struct MapImageToCpu : _MessageBase_
	{
	// types
		struct MemoryRange {
			BytesUL		offset;
			BytesUL		size;
		};
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		BytesUL				memOffset;		// offset in global memory space, may be unsupported
		MipmapLevel			mipLevel;
		ImageLayer			layer;
		EMappingFlags		flags		= EMappingFlags::ReadWrite;

		Out<MemoryRange>	range;			// this values may be used in 'FlushMemoryRange' command
		Out<uint3>			dimension;		// dimension of mapped level (layer) in pixels

	// methods
		MapImageToCpu () {}
		
		explicit MapImageToCpu (EMappingFlags flags, MipmapLevel level = Uninitialized, ImageLayer layer = Uninitialized) :
			mipLevel(level), layer(layer), flags(flags) {}
	};


	// for mapped memory use commands:
	//	ReadRegion	where 'position' - offset in mapped memory space
	//	WriteRegion	where 'position' - offset in mapped memory space


	//
	// Hide GPU Memory
	//
	struct UnmapMemory : _MessageBase_
	{};


	//
	// Flush GPU Memory Range
	//
	struct FlushMemoryRange : _MessageBase_
	{
	// variables
		BytesUL			offset;		// offset in mapped memory space
		BytesUL			size;

	// methods
		FlushMemoryRange () {}
		FlushMemoryRange (Bytes<uint> offset, Bytes<uint> size) :  offset{BytesUL(offset)}, size{BytesUL(size)} {}
		FlushMemoryRange (Bytes<ulong> offset, Bytes<ulong> size) :  offset{offset}, size{size} {}
	};


	//
	// Read from memory without mapping
	//
	struct ReadFromGpuMemory : DSMsg::ReadRegion
	{
		// 'position' - position in global memory space

	// methods
		ReadFromGpuMemory () {}
		ReadFromGpuMemory (Bytes<uint> pos, BinArrayRef buf) : ReadRegion{pos, buf} {}
		ReadFromGpuMemory (Bytes<ulong> pos, BinArrayRef buf) : ReadRegion{pos, buf} {}
		explicit ReadFromGpuMemory (BinArrayRef buf) : ReadRegion(0_b, buf) {}
	};


	//
	// Write to memory without mapping
	//
	struct WriteToGpuMemory : DSMsg::WriteRegion
	{
		// 'position' - position in global memory space
		
	// methods
		WriteToGpuMemory () {}
		WriteToGpuMemory (Bytes<uint> pos, BinArrayCRef data) : WriteRegion(pos, data) {}
		WriteToGpuMemory (Bytes<ulong> pos, BinArrayCRef data) : WriteRegion(pos, data) {}
		explicit WriteToGpuMemory (BinArrayCRef data) : WriteRegion(0_b, data) {}
	};


	//
	// Read From Image memory without mapping
	//
	struct ReadFromImageMemory : _MessageBase_
	{
	// types
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		BytesUL					memOffset;		// offset in global memory space, may be unsupported
		uint3					offset;
		uint3					dimension;
		MipmapLevel				mipLevel;
		ImageLayer				layer;
		BytesU					rowPitch;		// memory alignment requirements for result
		BytesU					slicePitch;		// memory alignment requirements for result
		Editable< BinArrayRef >	writableBuffer;	// preallocated memory, 'result' may contains all or part of this buffer
		Out< BinArrayCRef >		result;
		
	// methods
		ReadFromImageMemory () {}

		ReadFromImageMemory (BinArrayRef buf, const uint3 &off, const uint3 &dim, Bytes<uint> rowPitch, Bytes<uint> slicePitch = Uninitialized) :
			offset{off}, dimension{Max(dim, 1u)}, rowPitch{BytesU(rowPitch)}, slicePitch{BytesU(slicePitch)}, writableBuffer{buf} {}

		ReadFromImageMemory (BinArrayRef buf, const uint3 &off, const uint3 &dim, Bytes<ulong> rowPitch, Bytes<ulong> slicePitch = Uninitialized) :
			offset{off}, dimension{Max(dim, 1u)}, rowPitch{BytesU(rowPitch)}, slicePitch{BytesU(slicePitch)}, writableBuffer{buf} {}
	};


	//
	// Write to Image memory without mapping
	//
	struct WriteToImageMemory : _MessageBase_
	{
	// types
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		BytesUL				memOffset;		// offset in global memory space, may be unsupported
		uint3				offset;
		uint3				dimension;
		MipmapLevel			mipLevel;
		ImageLayer			layer;
		BytesU				rowPitch;
		BytesU				slicePitch;		// size of array layer or 3D image slice
		BinArrayCRef		data;
		Out< BytesUL >		wasWritten;

	// methods
		WriteToImageMemory () {}

		WriteToImageMemory (BinArrayCRef data, const uint3 &off, const uint3 &dim, Bytes<uint> rowPitch, Bytes<uint> slicePitch = Uninitialized) :
			offset(off), dimension(Max(dim, 1u)), rowPitch(BytesU(rowPitch)), slicePitch(BytesU(slicePitch)), data(data) {}

		WriteToImageMemory (BinArrayCRef data, const uint3 &off, const uint3 &dim, Bytes<ulong> rowPitch, Bytes<ulong> slicePitch = Uninitialized) :
			offset(off), dimension(Max(dim, 1u)), rowPitch(BytesU(rowPitch)), slicePitch(BytesU(slicePitch)), data(data) {}
	};


	//
	// On GPU Memory Binding State Changed
	//
	struct OnMemoryBindingChanged : _MessageBase_
	{
	// types
		enum class EBindingTarget
		{
			Unbinded,
			Image,
			Buffer,
			Shared,
		};

	// variables
		ModulePtr			targetObject;	// image or buffer
		EBindingTarget		newState;
		bool				isDedicated;	// 'true' if memory can not be shared with other image/buffer
		
	// methods
		OnMemoryBindingChanged (const ModulePtr &obj, EBindingTarget newState) : targetObject{obj}, newState{newState}, isDedicated{true} {}
		OnMemoryBindingChanged (const ModulePtr &obj, bool isDedicated) : targetObject{obj}, newState{EBindingTarget::Shared}, isDedicated{isDedicated} {}
	};


	//
	// GPU Memory Region Changed Event	// TODO
	//
	struct GpuMemoryRegionChanged : _MessageBase_
	{
	// variables
		BytesUL		offset;
		BytesUL		size;

	// methods
		GpuMemoryRegionChanged (Bytes<uint> offset, Bytes<uint> size) : offset(BytesUL(offset)), size(BytesUL(size)) {}
		GpuMemoryRegionChanged (Bytes<ulong> offset, Bytes<ulong> size) : offset(offset), size(size) {}
	};

}	// GpuMsg
}	// Engine
