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
		BytesU					size;
		EGpuMemory::bits		flags;
		EMemoryAccess::bits		access;

	// methods
		GpuMemoryDescription (GX_DEFCTOR) {}

		GpuMemoryDescription (BytesU size, EGpuMemory::bits flags, EMemoryAccess::bits access) :
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
		EGpuMemory::bits		memFlags;
		EMemoryAccess::bits		access;
		
	// methods
		explicit GpuMemoryManager (EGpuMemory::bits memFlags, EMemoryAccess::bits access = EMemoryAccess::All) :
			memFlags(memFlags), access(access) {}
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get GPU Memory Description
	//
	struct GetGpuMemoryDescription : _MsgBase_
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
	struct MapMemoryToCpu : _MsgBase_
	{
	// variables
		BytesU					position;
		BytesU					size;
		EMappingFlags			flags	= EMappingFlags::ReadWrite;
		Out_opt<BinArrayRef>	result;		// use 'ReadFromGpuMemory' or 'WriteToGpuMemory' instead of this

	// methods
		MapMemoryToCpu () {}
		explicit MapMemoryToCpu (EMappingFlags flags) : size{~0_b}, flags{flags} {}
		MapMemoryToCpu (EMappingFlags flags, BytesU pos, BytesU size) : position(pos), size(size), flags(flags) {}
	};

	struct MapImageToCpu : _MsgBase_
	{
	// types
		struct MemoryRange {
			BytesU		offset;
			BytesU		size;
		};
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		BytesU					memOffset;		// offset in global memory space, may be unsupported
		MipmapLevel				mipLevel;
		ImageLayer				layer;
		EMappingFlags			flags		= EMappingFlags::ReadWrite;

		Out_opt<MemoryRange>	range;			// this values may be used in 'FlushMemoryRange' command
		Out_opt<uint3>			dimension;		// dimension of mapped level (layer) in pixels

	// methods
		MapImageToCpu () {}
		
		explicit MapImageToCpu (EMappingFlags flags, MipmapLevel level = Uninitialized, ImageLayer layer = Uninitialized) :
			mipLevel(level), layer(layer), flags(flags) {}
	};


	// for mapped memory use commands:
	//	ReadMemRange	where 'position' - offset in mapped memory space
	//	WriteMemRange	where 'position' - offset in mapped memory space


	//
	// Hide GPU Memory
	//
	struct UnmapMemory : _MsgBase_
	{};


	//
	// Flush GPU Memory Range
	//
	struct FlushMemoryRange : _MsgBase_
	{
	// variables
		BytesU			offset;		// offset in mapped memory space
		BytesU			size;

	// methods
		FlushMemoryRange () {}
		FlushMemoryRange (BytesU offset, BytesU size) :  offset{offset}, size{size} {}
	};


	//
	// Read from memory without mapping
	//
	struct ReadFromGpuMemory : DSMsg::ReadMemRange
	{
		// 'position' - position in global memory space

	// methods
		ReadFromGpuMemory () {}
		ReadFromGpuMemory (BytesU pos, BinArrayRef buf) : ReadMemRange{pos, buf} {}
		explicit ReadFromGpuMemory (BinArrayRef buf) : ReadMemRange(0_b, buf) {}
	};


	//
	// Write to memory without mapping
	//
	struct WriteToGpuMemory : DSMsg::WriteMemRange
	{
		// 'position' - position in global memory space
		
	// methods
		WriteToGpuMemory () {}
		WriteToGpuMemory (BytesU pos, BinArrayCRef data) : WriteMemRange(pos, data) {}
		explicit WriteToGpuMemory (BinArrayCRef data) : WriteMemRange(0_b, data) {}
	};


	//
	// Read From Image memory without mapping
	//
	struct ReadFromImageMemory : _MsgBase_
	{
	// types
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		BytesU						memOffset;		// offset in global memory space, may be unsupported
		uint3						offset;
		uint3						dimension;
		MipmapLevel					mipLevel;
		ImageLayer					layer;
		BytesU						rowPitch;		// memory alignment requirements for result
		BytesU						slicePitch;		// memory alignment requirements for result
		Editable< BinArrayRef >		writableBuffer;	// preallocated memory, 'result' may contains all or part of this buffer
		Out_opt< BinArrayCRef >		result;
		
	// methods
		ReadFromImageMemory () {}

		ReadFromImageMemory (BinArrayRef buf, const uint3 &off, const uint3 &dim, BytesU rowPitch, BytesU slicePitch = Uninitialized) :
			offset{off}, dimension{Max(dim, 1u)}, rowPitch{rowPitch}, slicePitch{slicePitch}, writableBuffer{buf} {}
	};


	//
	// Write to Image memory without mapping
	//
	struct WriteToImageMemory : _MsgBase_
	{
	// types
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImageLayer	= Platforms::ImageLayer;

	// variables
		BytesU				memOffset;		// offset in global memory space, may be unsupported
		uint3				offset;
		uint3				dimension;
		MipmapLevel			mipLevel;
		ImageLayer			layer;
		BytesU				rowPitch;
		BytesU				slicePitch;		// size of array layer or 3D image slice
		BinArrayCRef		data;
		Out_opt< BytesU >	wasWritten;

	// methods
		WriteToImageMemory () {}

		WriteToImageMemory (BinArrayCRef data, const uint3 &off, const uint3 &dim, BytesU rowPitch, BytesU slicePitch = Uninitialized) :
			offset(off), dimension(Max(dim, 1u)), rowPitch(rowPitch), slicePitch(slicePitch), data(data) {}
	};


	//
	// On GPU Memory Binding State Changed
	//
	struct OnMemoryBindingChanged : _MsgBase_
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


}	// GpuMsg
}	// Engine
