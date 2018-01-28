// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Image input stream:
		- copy from client memory to staging buffer (cpu and gpu visible)
		- copy from staging buffer to image in gpu memory (gpu visible only)
		- send event

	Image output stream
		- copy from image in gpu memory to staging buffer
		- copy from staging buffer to memory
		- send event
*/

#pragma once

#include "Engine/Graphics/Shared/IDs.h"

namespace Engine
{
namespace CreateInfo
{
	//
	// GPU Memory Input Stream
	//
	struct GpuMemoryInStream
	{
		ModulePtr			gpuThread;
	};
	

	//
	// GPU Memory Output Stream
	//
	struct GpuMemoryOutStream
	{
		ModulePtr			gpuThread;
	};

}	// CreateInfo


namespace GraphicsMsg
{

	//
	// Reserve Client Memory
	//
	struct StreamReserveClientMem
	{
		BytesU		size;
	};


	//
	// Copy Image from CPU side to GPU side
	//
	struct StreamWriteImage
	{
	// types
		using RectU			= GXMath::RectU;
		using uint4			= GXMath::uint4;
		using EPixelFormat	= Platforms::EPixelFormat;
		using MipmapLevel	= Platforms::MipmapLevel;
		using Semaphore_t	= Platforms::GpuSemaphoreId;

	// variables
		ModulePtr				cmdBuffer;
		uint4					size;

		ReadOnce<BinaryArray>	srcImage;
		uint4					srcOffset;		// (optional)
		EPixelFormat::type		srcFormat	= EPixelFormat::Unknown;
		BytesU					rowPitch	= 4_b;
		BytesU					slicePitch	= 4_b;

		ModulePtr				dstImage;
		uint4					dstOffset;
		MipmapLevel				dstMipmap;

		//Out<Semaphore_t>		gpuSync;		// signaled when copying completed
	};


	//
	// Copy Image from GPU side to CPU side
	//
	struct StreamReadImage
	{
	// types
		using uint4			= GXMath::uint4;

	// variables
		ModulePtr				cmdBuffer;
		uint4					size;

		ModulePtr				srcImage;
		uint4					srcOffset;

		BinArrayRef				dstImage;
		BytesU					rowPitch	= 4_b;
		BytesU					slicePitch	= 4_b;
	};


	//
	// Copy Buffer from CPU side to GPU side
	//
	struct StreamWriteBuffer
	{
		ModulePtr				cmdBuffer;

		ReadOnce<BinaryArray>	srcBuffer;
		BytesU					srcOffset;		// (optional)
		BytesU					size;			// (optional)

		ModulePtr				dstBuffer;
		BytesUL					dstOffset;
	};


	//
	// Copy Buffer from GPU side to CPU side
	//
	struct StreamReadBuffer
	{
		ModulePtr				cmdBuffer;

		ModulePtr				srcBuffer;
		BytesUL					srcOffset;
		BytesUL					size;

		Out<BinArrayCRef>		dstBuffer;
	};


}	// GraphicsMsg
}	// Engine
