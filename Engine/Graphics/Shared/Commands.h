// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Shared/Common.h"

namespace Engine
{
namespace Graphics
{

}	// Graphics


namespace CreateInfo
{

	//
	// Per Frame Command Buffers Create Info
	//
	struct PerFrameCommandBuffers
	{
		ModulePtr	gpuThread;					// can be null
		uint		bufferChainLength	= 2;	// indicates frame latency
	};


	//
	// Async Command Buffer
	//
	struct AsyncCommandBuffer
	{
		ModulePtr	gpuThread;			// can be null
		ModulePtr	perFrameCmd;		// module that supports 'CmdAddFrameDependency' message
	};


}	// CreateInfo


// for PerFrameCommandBuffers
namespace GraphicsMsg
{

	//
	// Begin / End Frame
	//
	struct CmdBeginFrame
	{
		struct Data {
			ModulePtr	framebuffer;		// returns current framebuffer, same as in 'ThreadBeginFrame'
			uint		frameIndex;			// index of image in swapchain, same as in 'ThreadBeginFrame'
			uint		cmdIndex;			// index of buffer sequence, max value is 'PerFrameCommandBuffers::bufferChainLength'-1
		};

		Out< Data >		result;
	};

	struct CmdEndFrame
	{
		ModulePtr		framebuffer;		// (optional) must be null or framebuffer returned by 'CmdBeginFrame'
	};
	

	//
	// Frame Begin / End Event
	//
	struct OnCmdBeginFrame
	{
		uint	cmdIndex;
	};

	struct OnCmdEndFrame
	{
		uint	cmdIndex;
	};


	//
	// Add Frame Dependency
	//
	struct CmdAddFrameDependency
	{
	// types
		using Fence_t			= Platforms::GpuFenceId;
		using Semaphore_t		= Platforms::GpuSemaphoreId;
		using EPipelineStage	= Platforms::EPipelineStage;
		using Fences_t			= FixedSizeArray< Fence_t, 8 >;
		using Semaphores_t		= FixedSizeArray< Semaphore_t, 8 >;
		using WaitSemaphores_t	= FixedSizeArray<Pair< Semaphore_t, EPipelineStage::bits >, 8 >;

	// variables
		Fences_t			waitFences;			// on 'CmdBeginFrame' will wait all fences (block client side)
		WaitSemaphores_t	waitSemaphores;		// on 'CmdEndFrame' add semaphores to queue submission (block in gpu side before execution)
		Semaphores_t		signalSemaphores;	// on 'CmdEndFrame' add semaphores to queue submission (signal in gpu side after execution)

	// methods
		CmdAddFrameDependency () {}

		explicit CmdAddFrameDependency (Fence_t fence) : waitFences({ fence }) {}

		CmdAddFrameDependency (ArrayCRef<Fence_t> waitFences, ArrayCRef<Pair<Semaphore_t, EPipelineStage::bits>> waitSemaphores, ArrayCRef<Semaphore_t> signalSemaphores) :
			waitFences(waitFences), waitSemaphores(waitSemaphores), signalSemaphores(signalSemaphores)
		{}
	};


	//
	// Begin / End Commands Recording
	//
	struct CmdBegin
	{};

	struct CmdEnd
	{};


	//
	// Get Current Framebuffer
	//
	struct CmdGetCurrentFramebuffer
	{
		Out< ModulePtr >	result;
	};
	
}	// GraphicsMsg

	
// for AsyncCommandBuffer
namespace GraphicsMsg
{

	//
	// Begin / End Async Commands Recording
	//
	struct CmdBeginAsync
	{
	// types
		using Fences_t			= FixedSizeArray< Platforms::GpuFenceId, 8 >;
		using WaitSemaphores_t	= FixedSizeArray<Pair< Platforms::GpuSemaphoreId, Platforms::EPipelineStage::bits >, 8 >;

	// variables
		Fences_t			waitFences;			// will wait before start recording
		WaitSemaphores_t	waitSemaphores;		// will wait beffore executing

	// methods
		CmdBeginAsync () {}
	};

	
	//
	// End Async Commands Recording
	//
	struct CmdEndAsync
	{
	// types
		using Fence_t		= Platforms::GpuFenceId;
		using Semaphore_t	= Platforms::GpuSemaphoreId;

	// variables
		Fence_t			fence;			// signaled after executing in client side
		Semaphore_t		semaphore;		// signaled after executing in gpu side

	// methods
		CmdEndAsync () : fence(Fence_t::Unknown), semaphore(Semaphore_t::Unknown) {}
		CmdEndAsync (Fence_t fence, Semaphore_t sem) : fence(fence), semaphore(sem) {}
		explicit CmdEndAsync (Fence_t fence) : fence(fence), semaphore(Semaphore_t::Unknown) {}
		explicit CmdEndAsync (Semaphore_t sem) : fence(Fence_t::Unknown), semaphore(sem) {}
	};

	
	//
	// End Sync Commands Recording
	//
	struct CmdEndAndSync
	{};


}	// GraphicsMsg
}	// Engine
