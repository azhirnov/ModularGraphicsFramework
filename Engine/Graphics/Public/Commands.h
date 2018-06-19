// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Public/Common.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// Command Buffers Manager Create Info
	//
	struct CommandBufferManager
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
		ModulePtr	cmdBufMngr;			// module that supports 'CmdAddFrameDependency' message
	};


}	// CreateInfo


// for CommandBufferManager
namespace GraphicsMsg
{

	//
	// Begin / End Frame
	//
	struct CmdBeginFrame : _MessageBase_
	{
		struct Data {
			ModulePtr	framebuffer;		// returns current framebuffer, same as in 'ThreadBeginFrame'
			uint		frameIndex;			// index of image in swapchain, same as in 'ThreadBeginFrame'
			uint		cmdIndex;			// index of buffer sequence, max value is 'CommandBufferManager::bufferChainLength'-1
		};

		Out< Data >		result;
	};

	struct CmdEndFrame : _MessageBase_
	{
	// variables
		ModulePtr		framebuffer;		// (optional) must be null or framebuffer returned by 'CmdBeginFrame'

	// methods
		CmdEndFrame () {}
		explicit CmdEndFrame (const ModulePtr &fb) : framebuffer{fb} {}
	};
	

	//
	// Begin / End VR Frame
	//
	struct CmdBeginVRFrame : _MessageBase_
	{
	// types
		using PerEye_t	= GpuMsg::ThreadBeginVRFrame::PerEye;
		using _Data_t	= GpuMsg::ThreadBeginVRFrame::Data;

		struct Data : _Data_t
		{
			uint	cmdIndex;	// index of buffer sequence, max value is 'CommandBufferManager::bufferChainLength'-1

			Data (const _Data_t &d, uint idx) : _Data_t(d), cmdIndex(idx) {}
		};

	// variables
		Out< Data >		result;
	};

	struct CmdEndVRFrame : _MessageBase_
	{};


	//
	// Frame Begin / End Event
	//
	struct OnCmdBeginFrame : _MessageBase_
	{
	// variables
		uint	cmdIndex;

	// methods
		explicit OnCmdBeginFrame (uint idx) : cmdIndex{idx} {}
	};

	struct OnCmdEndFrame : _MessageBase_
	{
	// variables
		uint	cmdIndex;

	// methods
		explicit OnCmdEndFrame (uint idx) : cmdIndex{idx} {}
	};


	//
	// Add Frame Dependency
	//
	struct CmdAddFrameDependency : _MessageBase_
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
	// Append Command Buffers
	//
	struct CmdAppend : _MessageBase_
	{
	// types
		using Commands_t	= FixedSizeArray< ModulePtr, 16 >;

	// variables
		Commands_t		commands;		// this commands will be submited with other commands that created in current frame,
										// no synchronization needed, commands will be deleted when complete executing.

	// methods
		CmdAppend () {}
		explicit CmdAppend (const ModulePtr &cmd) : commands{ cmd } {}
		explicit CmdAppend (ArrayCRef<ModulePtr> cmds) : commands{ cmds } {}
	};


	//
	// Subscribe On Frame Completed
	//
	struct SubscribeOnFrameCompleted : _MessageBase_
	{
	// types
		using Callback_t	= Function< void (uint) >;

	// variables
		Callback_t		callback;		// callback that will be triggered when executing of all command buffers in current frame completed
		Out< uint >		index;			// (optional) returns implementation defined frame index

	// methods
		SubscribeOnFrameCompleted () {}

		template <typename F>
		explicit SubscribeOnFrameCompleted (F &&func) : callback(RVREF(func)) {}
	};


	//
	// Begin / End Commands Recording
	//
	struct CmdBegin : _MessageBase_
	{};

	struct CmdEnd : _MessageBase_
	{};


	//
	// Get Current State
	//
	struct CmdGetCurrentState : _MessageBase_
	{
		enum class EScope {
			None,
			Frame,
			Command,
			RenderPass
		};

		struct Data {
			ModulePtr		framebuffer	= null;			// returns current framebuffer, same as in 'ThreadBeginFrame'
			uint			frameIndex	= UMax;			// index of image in swapchain, same as in 'ThreadBeginFrame'
			uint			cmdIndex	= UMax;			// index of buffer sequence, max value is 'CommandBufferManager::bufferChainLength'-1
			EScope			scope		= EScope::None;	// current scope state

			// TODO: max frame index, max command index
		};

		Out< Data >		result;
	};
	
}	// GraphicsMsg

	
// for AsyncCommandBuffer
namespace GraphicsMsg
{

	//
	// Begin / End Async Commands Recording
	//
	struct CmdBeginAsync : _MessageBase_
	{
	// types
		enum class EMode {
			Async,			// command will be submited immediatly in 'CmdEndAsync' and synchronized with sync objects only.
			InFrame,		// command will be added as dependency to commands manager and will be submited on end of current frame rendering.
			Sync,			// command will be submited immediatly in 'CmdEndAsync', cliend will be waiting when commands complete executing.
			BetweenFrames,	// command executed after current frame and before next frame.
			BeforeFrame,	// command executed before current frame, but without synchronization with previous frames.
			//SyncInFrame,	// added to current frame and block client, this is wery bad and not supported yet.
		};

		using Semaphore_t		= Platforms::GpuSemaphoreId;
		using Fences_t			= FixedSizeArray< Platforms::GpuFenceId, 8 >;
		using WaitSemaphores_t	= FixedSizeArray<Pair< Semaphore_t, Platforms::EPipelineStage::bits >, 8 >;
		using Callback_t		= SubscribeOnFrameCompleted::Callback_t;

	// variables
		EMode				syncMode;
		Fences_t			waitFences;				// (optional) will wait before start recording
		WaitSemaphores_t	waitSemaphores;			// (optional) will wait beffore executing
		Semaphore_t			externalSemaphore	 = Uninitialized;	// (optional) signaled after executing in gpu side, for user purposes.
		Semaphore_t			beforeFrameExecuting = Uninitialized;	// (optional) signaled after executing, current frame will be waiting this semaphore until execute commands.
		Callback_t			onCompleted;			// (optional) will triggered when command buffer executed, before begining the new frame.

	// methods
		CmdBeginAsync () : syncMode{EMode::InFrame} {}
		explicit CmdBeginAsync (EMode syncMode) : syncMode{syncMode} {}
	};

	
	//
	// End Async Commands Recording
	//
	struct CmdEndAsync : _MessageBase_
	{};


}	// GraphicsMsg
}	// Engine
