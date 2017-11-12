// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/MemoryEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"

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
		BytesUL					maxSize;		// max size of allocated memory (for memory managment)
		BytesUL					blockSize;		// block size to avoid fragmentation (for memory managment)
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		
	// methods
		GpuMemory (const ModulePtr &gpuThread, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), memFlags(memFlags), access(access)
		{}

		GpuMemory (const ModulePtr &gpuThread, Bytes<ulong> maxSize, Bytes<ulong> blockSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
			gpuThread(gpuThread), maxSize(maxSize), blockSize(blockSize), memFlags(memFlags), access(access)
		{}

		GpuMemory (const ModulePtr &gpuThread, Bytes<uint> maxSize, Bytes<uint> blockSize, EGpuMemory::bits memFlags, EMemoryAccess::bits access) :
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
		MapMemoryToCpu (EMappingFlags flags, BytesU size, BytesU offset = Uninitialized) : offset(offset), size(size), flags(flags) {}
		MapMemoryToCpu (EMappingFlags flags, BytesUL size, BytesUL offset = Uninitialized) : offset(offset), size(size), flags(flags) {}
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
		using uint4			= GXMath::uint4;
		using EMappingFlags	= MapMemoryToCpu::EMappingFlags;

	// variables
		uint4				offset;
		uint4				dimension;
		EMappingFlags		flags		= EMappingFlags::ReadWrite;

		Out<MemoryRange>	range;			// this values may be used in 'FlushMemoryRange' command
		Out<PixelAlign>		pixelAlign;		// this values may be used to calculate pixel offset

	// methods
		MapImageToCpu (EMappingFlags flags, const uint4 &dimension, const uint4 &offset = uint4()) : offset(offset), dimension(dimension), flags(flags) {}
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
		explicit FlushMemoryRange (BytesU size, BytesU offset = Uninitialized) :  offset(offset), size(size) {}
		explicit FlushMemoryRange (BytesUL size, BytesUL offset = Uninitialized) :  offset(offset), size(size) {}
	};


	//
	// Read from memory without mapping
	//
	struct ReadFromGpuMemory
	{
	// variables
		BytesUL						offset;			// offset in global memory space
		Optional< BytesUL >			size;			// must be undefined or equal to result.Size()
		Optional< BinArrayRef >		writableBuffer;	// will use exist buffer
		Out< BinArrayCRef >			result;

	// methods
		ReadFromGpuMemory () {}
		explicit ReadFromGpuMemory (BinArrayRef buf, BytesU off = 0_b) : offset(BytesUL(off)), writableBuffer(buf) {}
		explicit ReadFromGpuMemory (BytesU size, BytesU off = 0_b) : offset(BytesUL(off)), size(BytesUL(size)) {}
	};


	//
	// Write to memory without mapping
	//
	struct WriteToGpuMemory : WriteToStream
	{
		// 'offset' - offset in global memory space

		WriteToGpuMemory () {}
		explicit WriteToGpuMemory (BinArrayCRef data, BytesU offset = 0_b) : WriteToStream(data, offset) {}
		template <typename T>			explicit WriteToGpuMemory (ArrayCRef<T> arr, BytesU offset = 0_b) : WriteToStream(arr, offset) {}
		template <typename B, usize I>	explicit WriteToGpuMemory (const B (&arr)[I], BytesU offset = 0_b) : WriteToStream(arr, offset) {}
	};


	//
	// Read From Image memory without mapping
	//
	struct ReadFromImageMemory
	{
	// types
		using uint4			= GXMath::uint4;
		using MipmapLevel	= Platforms::MipmapLevel;

	// variables
		BytesUL					memOffset;		// offset in global memory space, may be unsupported
		uint4					offset;
		uint4					dimension;
		MipmapLevel				level;
		BytesU					rowPitch;
		BytesU					slicePitch;		// size of array layer or 3D image slice
		Optional< BinArrayRef >	writableBuffer;	// will use exist buffer
		Out< BinArrayCRef >		result;
		
	// methods
		ReadFromImageMemory () {}

		ReadFromImageMemory (const uint4 &off, const uint4 &dim, BytesU rowPitch, BytesU slicePitch) :
			offset(off), dimension(GXMath::Max(dim, uint4(1))), rowPitch(rowPitch), slicePitch(slicePitch)
		{}

		ReadFromImageMemory (const uint4 &dim, BytesU bpp, const uint4 &off = uint4(), BytesU rowAlign = 4_b, BytesU sliceAlign = 4_b) :
			offset(off), dimension(GXMath::Max(dim, uint4(1)))
		{
			rowPitch   = GXMath::ImageUtils::AlignedRowSize( dimension.x, bpp, rowAlign );
			slicePitch = GXMath::ImageUtils::AlignedSliceSize( dimension.xy(), bpp, rowAlign, sliceAlign );
		}

		ReadFromImageMemory (BinArrayRef buf, const uint4 &off, const uint4 &dim, BytesU rowPitch, BytesU slicePitch) :
			ReadFromImageMemory( off, dim, rowPitch, slicePitch )
		{
			writableBuffer = buf;
			ASSERT( writableBuffer->Size() == dimension.z * slicePitch );
		}

		ReadFromImageMemory (BinArrayRef buf, const uint4 &dim, BytesU bpp, const uint4 &off = uint4(), BytesU rowAlign = 4_b, BytesU sliceAlign = 4_b) :
			ReadFromImageMemory( dim, bpp, off, rowAlign, sliceAlign )
		{
			writableBuffer = buf;
			ASSERT( writableBuffer->Size() == dimension.z * slicePitch );
		}
	};


	//
	// Write to Image memory without mapping
	//
	struct WriteToImageMemory
	{
	// types
		using uint4			= GXMath::uint4;
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
			offset(off), dimension(GXMath::Max(dim, uint4(1))), rowPitch(rowPitch), slicePitch(slicePitch), data(data)
		{}

		WriteToImageMemory (BinArrayCRef data, const uint4 &dim, BytesU bpp, const uint4 &off = uint4(), BytesU rowAlign = 4_b, BytesU sliceAlign = 4_b) :
			offset(off), dimension(GXMath::Max(dim, uint4(1))), data(data)
		{
			rowPitch   = GXMath::ImageUtils::AlignedRowSize( dimension.x, bpp, rowAlign );
			slicePitch = GXMath::ImageUtils::AlignedSliceSize( dimension.xy(), bpp, rowAlign, sliceAlign );
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
