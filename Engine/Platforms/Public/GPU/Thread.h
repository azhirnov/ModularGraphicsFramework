// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"
#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/Public/GPU/CommandEnums.h"
#include "Engine/Platforms/Public/GPU/RenderPassEnums.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// GPU Thread Create Info
	//
	struct GpuThread
	{
	// types
		using GAPI		= Platforms::GAPI;
		using EFlags	= Platforms::GraphicsSettings::EFlags;

	// variables
		ModulePtr						context;		// can be null
		ModulePtr						shared;			// can be null
		Platforms::GraphicsSettings		settings;

	// methods
		explicit GpuThread (GAPI::type version = GAPI::type(0)) : settings{ version } {}

		explicit GpuThread (const Platforms::GraphicsSettings &settings, const ModulePtr &context = null, const ModulePtr &shared = null) :
			context{ context }, shared{ shared }, settings{ settings }
		{}

		explicit GpuThread (const Platforms::ComputeSettings &settings) :
			settings{ settings.version, (settings.isDebug ? EFlags::DebugContext : EFlags::type(0)) | EFlags::NoSurface,
					  Platforms::EPixelFormat::Unknown, Platforms::EPixelFormat::Unknown,
					  Platforms::MultiSamples(), settings.device, 0 }
		{}
	};


	//
	// GPU Command Queue Create Info
	//
	struct GpuCommandQueue
	{
	// variables
		ModulePtr						gpuThread;
		Platforms::EQueueFamily::bits	family;

	// methods
		GpuCommandQueue () {}

		GpuCommandQueue (const ModulePtr &gpuThread, Platforms::EQueueFamily::bits family) : gpuThread{gpuThread}, family{family} {}
	};

}	// CreateInfo


namespace GpuMsg
{
	//
	// GPU Device Created / Destroyed
	//
	struct DeviceCreated
	{
	};

	struct DeviceBeforeDestroy
	{
	};

	struct DeviceLost
	{
	};


	//
	// Get Device Info
	//
	struct GetDeviceInfo
	{
		struct Info {
			ModulePtr	gpuThread;			// current gpu thread		// TODO: remove?
			ModulePtr	memManager;			// may be null				// TODO: remove?
			ModulePtr	syncManager;		// may be null				// TODO: remove?
			ModulePtr	renderPass;			// this is default render pass used for on screen rendering
			uint		imageCount;			// count of images in swapchain
		};

		Out< Info >		result;
	};


	//
	// Submit Commands
	//
	struct SubmitGraphicsQueueCommands
	{
	// types
		using Commands_t		= FixedSizeArray< ModulePtr, 16 >;
		using Fence_t			= Platforms::GpuFenceId;
		using Semaphore_t		= Platforms::GpuSemaphoreId;
		using EPipelineStage	= Platforms::EPipelineStage;
		using Semaphores_t		= FixedSizeArray< Semaphore_t, GlobalConst::GAPI_MaxWaitSemaphores >;
		using WaitSemaphores_t	= FixedSizeArray<Pair< Semaphore_t, EPipelineStage::bits >, GlobalConst::GAPI_MaxWaitSemaphores >;

	// variables
		Commands_t			commands;			// command to submit.
		Fence_t				fence;				// fence that will be signaled when command buffer will complete executing.
		WaitSemaphores_t	waitSemaphores;		// wait before executing command buffers.
		Semaphores_t		signalSemaphores;	// will be signaleed when command buffers have completed execution.

	// methods
		SubmitGraphicsQueueCommands () : fence{Uninitialized} {}
		explicit SubmitGraphicsQueueCommands (const ModulePtr &cmd, Fence_t fence = Uninitialized) : commands({ cmd }), fence( fence ) {}
		explicit SubmitGraphicsQueueCommands (ArrayCRef< ModulePtr > list, Fence_t fence = Uninitialized) : commands( list ), fence( fence ) {}
	};

	struct SubmitComputeQueueCommands : SubmitGraphicsQueueCommands
	{
		SubmitComputeQueueCommands () {}
		explicit SubmitComputeQueueCommands (const ModulePtr &cmd, Fence_t fence = Uninitialized) : SubmitGraphicsQueueCommands( cmd, fence ) {}
		explicit SubmitComputeQueueCommands (ArrayCRef< ModulePtr > list, Fence_t fence = Uninitialized) : SubmitGraphicsQueueCommands( list, fence ) {}
	};


	//
	// Begin / End Frame
	//
	struct ThreadBeginFrame
	{
		struct Data {
			ModulePtr	framebuffer;		// returns current framebuffer
			uint		frameIndex;			// index of image in swapchain, max value if 'GraphicsSettings::swapchainLength'-1
		};
		Out< Data >		result;
	};

	struct ThreadEndFrame : SubmitGraphicsQueueCommands
	{
	// variables
		ModulePtr		framebuffer;	// (optional) must be null or framebuffer returned by 'ThreadBeginFrame'

	// methods
		ThreadEndFrame () {}

		ThreadEndFrame (const ModulePtr &framebuffer, const ModulePtr &command, Fence_t fence = Uninitialized) :
			SubmitGraphicsQueueCommands( command, fence ), framebuffer( framebuffer )
		{}

		ThreadEndFrame (const ModulePtr &framebuffer, ArrayCRef<ModulePtr> commands, Fence_t fence = Uninitialized) :
			SubmitGraphicsQueueCommands( commands, fence ), framebuffer( framebuffer )
		{}
	};


	//
	// Get Compute / Graphics Module IDs
	//
	struct GetGraphicsModules
	{
		Out< Platforms::ComputeModuleIDs >		compute;
		Out< Platforms::GraphicsModuleIDs >		graphics;
	};


	//
	// Get Graphics Settings
	//
	struct GetGraphicsSettings
	{
	// types
		struct Settings : Platforms::GraphicsSettings
		{
		// variables
			uint2		surfaceSize;

		// methods
			Settings (const Platforms::GraphicsSettings &gs, const uint2 &size) :
				Platforms::GraphicsSettings(gs), surfaceSize(size)
			{}
		};

	// variables
		Out< Settings >		result;
	};


	//
	// Get Compute Settings
	//
	struct GetComputeSettings
	{
		Out< Platforms::ComputeSettings >	result;
	};


}	// GpuMsg
}	// Engine
