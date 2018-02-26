// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/MemoryEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"
#include "Engine/Platforms/Shared/GPU/ImageLayer.h"

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
		BytesUL					size;
		EGpuMemory::bits		flags;
		EMemoryAccess::bits		access;

	// methods
		GpuMemoryDescriptor (GX_DEFCTOR) {}
		GpuMemoryDescriptor (BytesUL size, EGpuMemory::bits flags, EMemoryAccess::bits access) : size(size), flags(flags), access(access) {}
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
		GpuMemory (const ModulePtr &gpuThread, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), memFlags(memFlags), access(access)
		{}

		GpuMemory (const ModulePtr &gpuThread, const ModulePtr &memMngr, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), memManager(memMngr), memFlags(memFlags), access(access)
		{}
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
		GpuMemoryManager (const ModulePtr &gpuThread, Bytes<ulong> maxSize, Bytes<ulong> blockSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), maxSize(maxSize), blockSize(blockSize), memFlags(memFlags), access(access)
		{}

		GpuMemoryManager (const ModulePtr &gpuThread, Bytes<uint> maxSize, Bytes<uint> blockSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), maxSize(maxSize), blockSize(blockSize), memFlags(memFlags), access(access)
		{}
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get GPU Memory Descriptor
	//
	struct GetGpuMemoryDescriptor
	{
		Out< Platforms::GpuMemoryDescriptor >	result;
	};


	//
	// Make GPU Memory visible by CPU
	//
	struct MapMemoryToCpu
	{
	// types
		enum class EMappingFlags {
			Read,
			Write,
			ReadWrite,
			WriteDiscard,
		};

	// variables
		BytesUL			offset;
		BytesUL			size;
		EMappingFlags	flags	= EMappingFlags::ReadWrite;

	// methods
		MapMemoryToCpu () {}
		MapMemoryToCpu (EMappingFlags flags, Bytes<uint> size, Bytes<uint> offset = Uninitialized) : offset(offset), size(size), flags(flags) {}
		MapMemoryToCpu (EMappingFlags flags, Bytes<ulong> size, Bytes<ulong> offset = Uninitialized) : offset(offset), size(size), flags(flags) {}
	};

	struct MapImageToCpu
	{
	// types
		struct MemoryRange {
			BytesUL		offset;
			BytesUL		size;
		};
		struct PixelAlign {
			BytesUL		rowPitch;
			BytesUL		slicePitch;	// size of array layer or 3D image slice
		};
		using EMappingFlags	= MapMemoryToCpu::EMappingFlags;
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		BytesUL				memOffset;		// offset in global memory space, may be unsupported
		MipmapLevel			level;
		ImageLayer			layer;
		EMappingFlags		flags		= EMappingFlags::ReadWrite;

		Out<MemoryRange>	range;			// this values may be used in 'FlushMemoryRange' command
		Out<PixelAlign>		pixelAlign;		// this values may be used to calculate pixel offset
		Out<uint4>			dimension;		// dimension of mapped level (layer) in pixels

	// methods
		MapImageToCpu () {}
		MapImageToCpu (EMappingFlags flags, MipmapLevel level = Uninitialized, ImageLayer layer = Uninitialized) : level(level), layer(layer), flags(flags) {}
	};


	// for mapped memory use commands:
	//	ReadFromStream	where 'offset' - offset in mapped memory space
	//	WriteToStream	where 'offset' - offset in mapped memory space


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
	// variables
		BytesUL			offset;		// offset in mapped memory space
		BytesUL			size;

	// methods
		FlushMemoryRange () {}
		explicit FlushMemoryRange (BytesU size, Bytes<uint> offset = Uninitialized) :  offset(offset), size(size) {}
		explicit FlushMemoryRange (BytesUL size, Bytes<ulong> offset = Uninitialized) :  offset(offset), size(size) {}
	};


	//
	// Read from memory without mapping
	//
	struct ReadFromGpuMemory
	{
	// variables
		BytesUL					offset;			// offset in global memory space
		mutable BinArrayRef		writableBuffer;	// preallocated memory, 'result' may contains all or part of this buffer
		Out< BinArrayCRef >		result;

	// methods
		ReadFromGpuMemory () {}
		explicit ReadFromGpuMemory (BinArrayRef buf, Bytes<uint> off = Uninitialized) : offset(BytesUL(off)), writableBuffer(buf) {}
		explicit ReadFromGpuMemory (BinArrayRef buf, Bytes<ulong> off) : offset(off), writableBuffer(buf) {}
	};


	//
	// Write to memory without mapping
	//
	struct WriteToGpuMemory : WriteToStream
	{
	// types
		template <typename T>
		using IsPOD_t = CompileTime::EnableIf< not CompileTime::IsCtorAvailable<T>, int >;

		// 'offset' - offset in global memory space
		
	// methods
		WriteToGpuMemory () {}
		explicit WriteToGpuMemory (BinArrayCRef data, Bytes<uint> offset = Uninitialized) : WriteToStream(data, offset) {}
		explicit WriteToGpuMemory (BinArrayCRef data, Bytes<ulong> offset) : WriteToStream(data, offset) {}

		//template <typename T>			explicit WriteToGpuMemory (ArrayCRef<T> arr, BytesU offset = Uninitialized, IsPOD_t<T> = 0) : WriteToStream(arr, offset) {}
		//template <typename T, usize I>	explicit WriteToGpuMemory (const T (&arr)[I], BytesU offset = Uninitialized, IsPOD_t<T> = 0) : WriteToStream(arr, offset) {}
		//template <typename T>			explicit WriteToGpuMemory (const T &value, BytesU offset = Uninitialized, IsPOD_t<T> = 0) : WriteToStream(BinArrayCRef::FromValue(value), offset) {}
	};


	//
	// Read From Image memory without mapping
	//
	struct ReadFromImageMemory
	{
	// types
		using MipmapLevel	= Platforms::MipmapLevel;

	// variables
		BytesUL					memOffset;		// offset in global memory space, may be unsupported
		uint4					offset;
		uint4					dimension;
		MipmapLevel				level;
		BytesU					rowPitch;		// memory alignment requirements for result
		BytesU					slicePitch;		// memory alignment requirements for result
		mutable BinArrayRef		writableBuffer;	// preallocated memory, 'result' may contains all or part of this buffer
		Out< BinArrayCRef >		result;
		
	// methods
		ReadFromImageMemory () {}

		ReadFromImageMemory (BinArrayRef buf, const uint4 &off, const uint4 &dim, BytesU rowPitch, BytesU slicePitch) :
			offset(off), dimension(Max(dim, uint4(1))), rowPitch(rowPitch), slicePitch(slicePitch)
		{
			writableBuffer = buf;
			ASSERT( writableBuffer.Size() == dimension.z * slicePitch );
		}

		ReadFromImageMemory (BinArrayRef buf, const uint4 &dim, BytesU bpp, const uint4 &off = uint4(), BytesU rowAlign = 4_b, BytesU sliceAlign = 4_b) :
			offset(off), dimension(Max(dim, uint4(1)))
		{
			writableBuffer	= buf;
			rowPitch		= GXImageUtils::AlignedRowSize( dimension.x, bpp, rowAlign );
			slicePitch		= GXImageUtils::AlignedSliceSize( dimension.xy(), bpp, rowAlign, sliceAlign );
			ASSERT( writableBuffer.Size() == dimension.z * slicePitch );
		}
	};


	//
	// Write to Image memory without mapping
	//
	struct WriteToImageMemory
	{
	// types
		using MipmapLevel	= Platforms::MipmapLevel;

	// variables
		BytesUL				memOffset;		// offset in global memory space, may be unsupported
		uint4				offset;
		uint4				dimension;
		MipmapLevel			level;
		BytesU				rowPitch;
		BytesU				slicePitch;		// size of array layer or 3D image slice
		BinArrayCRef		data;
		Out< BytesUL >		wasWritten;

	// methods
		WriteToImageMemory () {}

		WriteToImageMemory (BinArrayCRef data, const uint4 &off, const uint4 &dim, BytesU rowPitch, BytesU slicePitch) :
			offset(off), dimension(Max(dim, uint4(1))), rowPitch(rowPitch), slicePitch(slicePitch), data(data)
		{}

		WriteToImageMemory (BinArrayCRef data, const uint4 &dim, BytesU bpp, const uint4 &off = uint4(), BytesU rowAlign = 4_b, BytesU sliceAlign = 4_b) :
			offset(off), dimension(Max(dim, uint4(1))), data(data)
		{
			rowPitch   = GXImageUtils::AlignedRowSize( dimension.x, bpp, rowAlign );
			slicePitch = GXImageUtils::AlignedSliceSize( dimension.xy(), bpp, rowAlign, sliceAlign );
			ASSERT( data.Size() == dimension.z * slicePitch );
		}
	};


	// TODO
	struct AsyncReadFromGpuMemory;
	struct AsyncWriteToGpuMemory;
	struct AsyncReadFromImageMemory;
	struct AsyncWriteToImageMemory;
	

	//
	// On GPU Memory Binding State Changed
	//
	struct OnMemoryBindingChanged
	{
	// types
		enum class EBindingTarget
		{
			Unbinded,
			Image,
			Buffer,
		};

	// variables
		ModulePtr			targetObject;	// image or buffer
		EBindingTarget		newState;
	};


	//
	// GPU Memory Region Changed Event	// TODO
	//
	struct GpuMemoryRegionChanged
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
