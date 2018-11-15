// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/Public/GPU/CommandEnums.h"
#include "Engine/Platforms/Public/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Public/GPU/MipmapLevel.h"
#include "Engine/Platforms/Public/GPU/ImageLayer.h"

namespace Engine
{
namespace Platforms
{

	//
	// Command Queue Description
	//
	struct CommandQueueDescription
	{
	// variables
		EQueueFamily::bits	family;
		float				priority;

	// methods
		CommandQueueDescription (GX_DEFCTOR) : priority{1.0F}
		{}

		CommandQueueDescription (EQueueFamily::bits	family,
								 float				priority) :
			family{ family },	priority{ priority }
		{}
	};

}	// Platforms


namespace CreateInfo
{

	//
	// GPU Command Queue Create Info
	//
	struct GpuCommandQueue
	{
	// variables
		ModulePtr								gpuThread;
		Platforms::CommandQueueDescription		descr;
		uint									queueIndex = 0;

	// methods
		GpuCommandQueue () {}

		GpuCommandQueue (const Platforms::CommandQueueDescription &desc, uint index = 0) :
			descr{desc}, queueIndex{index} {}

		GpuCommandQueue (const ModulePtr &gpuThread, const Platforms::CommandQueueDescription &desc, uint index = 0) :
			gpuThread{gpuThread}, descr{desc}, queueIndex{index} {}
	};

}	// CreateInfo


namespace GpuMsg
{
	using GpuFenceId		= Platforms::GpuFenceId;
	using EImageAspect		= Platforms::EImageAspect;
	using MipmapLevel		= Platforms::MipmapLevel;
	using ImageLayer		= Platforms::ImageLayer;
	using GpuSemaphoreId	= Platforms::GpuSemaphoreId;
	using EPipelineStage	= Platforms::EPipelineStage;
	

	//
	// GPU Device Lost event
	//
	struct DeviceLost : _MsgBase_
	{
	};


	//
	// Get Command Queue Description
	//
	struct GetCommandQueueDescription : _MsgBase_
	{
		Out< Platforms::CommandQueueDescription >	result;
	};


	//
	// Submit Commands
	//
	struct SubmitCommands : _MsgBase_
	{
	// types
		using Commands_t		= FixedSizeArray< ModulePtr, 16 >;
		using Semaphores_t		= FixedSizeArray< GpuSemaphoreId, GlobalConst::GAPI_MaxWaitSemaphores >;
		using WaitSemaphores_t	= FixedSizeArray<Pair< GpuSemaphoreId, EPipelineStage::bits >, GlobalConst::GAPI_MaxWaitSemaphores >;

	// variables
		Commands_t			commands;							// command to submit.
		GpuFenceId			fence			= Uninitialized;	// fence that will be signaled when command buffer will complete executing.
		WaitSemaphores_t	waitSemaphores;						// wait before executing command buffers.
		Semaphores_t		signalSemaphores;					// will be signaleed when command buffers have completed execution.

	// methods
		SubmitCommands () {}
		explicit SubmitCommands (const ModulePtr &cmd) : commands({ cmd }) {}
		explicit SubmitCommands (ArrayCRef< ModulePtr > list) : commands( list ) {}

		SubmitCommands&  SignalSemaphore (GpuSemaphoreId sem)							{ signalSemaphores.PushBack( sem );			return *this; }
		SubmitCommands&  WaitSemaphore (GpuSemaphoreId sem, EPipelineStage::bits stage)	{ waitSemaphores.PushBack({ sem, stage });	return *this; }
		SubmitCommands&  SetFence (GpuFenceId newFence)									{ fence = newFence;							return *this; }
	};


	//
	// Bind Sparse
	//
	struct QueueBindSparse : _MsgBase_
	{
	// types
		struct ImageSubresource
		{
		// variables
			EImageAspect::bits	aspectMask;
			MipmapLevel			mipLevel;
			ImageLayer			arrayLayer;

		// methods
			ImageSubresource () {}

			ImageSubresource (EImageAspect::bits aspect, MipmapLevel mipLevel, ImageLayer layer) :
				aspectMask{aspect}, mipLevel{mipLevel}, arrayLayer{layer} {}
		};

		struct SparseMemoryBind : CompileTime::FastCopyable
		{
		// variables
			BytesU			resourceOffset;
			BytesU			size;
			ModulePtr		memory;
			BytesU			memoryOffset;
			// TODO: flags

		// methods
			SparseMemoryBind () {}
		};

		struct SparseImageBind : CompileTime::FastCopyable
		{
		// variables
			ImageSubresource	subresource;
			uint3				offset;
			uint3				extent;
			ModulePtr			memory;
			BytesU				memoryOffset;

		// methods
			SparseImageBind () {}
		};

		using Semaphores_t			= FixedSizeArray< GpuSemaphoreId, GlobalConst::GAPI_MaxWaitSemaphores >;
		using SparseMemoryBinds_t	= FixedSizeArray< SparseMemoryBind, GlobalConst::GAPI_MaxSparseMemoryParts >;
		using SparseImageBinds_t	= FixedSizeArray< SparseImageBind, GlobalConst::GAPI_MaxSparseMemoryParts >;
		using BufferBinds_t			= FixedSizeArray<Pair< ModulePtr, SparseMemoryBinds_t >, GlobalConst::GAPI_MaxSparseResources>;
		using ImageOpaqueBinds_t	= FixedSizeArray<Pair< ModulePtr, SparseMemoryBinds_t >, GlobalConst::GAPI_MaxSparseResources>;
		using ImageBinds_t			= FixedSizeArray<Pair< ModulePtr, SparseImageBinds_t >, GlobalConst::GAPI_MaxSparseResources>;

	// variables
		BufferBinds_t		bufferBinds;
		ImageOpaqueBinds_t	imageOpaqueBings;
		ImageBinds_t		imageBinds;
		GpuFenceId			fence;				// fence that will be signaled when sparse memory is binded.
		Semaphores_t		waitSemaphores;		// wait before bind sparse memory.
		Semaphores_t		signalSemaphores;	// will be signaleed when sparse memory binded.

	// methods
		QueueBindSparse () {}
	};


}	// GpuMsg
}	// Engine
