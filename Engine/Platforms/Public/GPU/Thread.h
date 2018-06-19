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

		explicit GpuThread (const Platforms::ComputeSettings &settings, const ModulePtr &context = null, const ModulePtr &shared = null) :
			context{ context }, shared{ shared },
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
	struct DeviceCreated : _MessageBase_
	{
	};

	struct DeviceBeforeDestroy : _MessageBase_
	{
	};

	struct DeviceLost : _MessageBase_
	{
	};


	//
	// Get Device Info
	//
	struct GetDeviceInfo : _MessageBase_
	{
		struct Info {
			ModulePtr	gpuThread;			// current gpu thread		// TODO: remove?
			ModulePtr	sharedThread;		// can be null
			ModulePtr	memManager;			// can be null				// TODO: remove?
			ModulePtr	syncManager;		// can be null				// TODO: remove?
			ModulePtr	renderPass;			// this is default render pass used for on screen rendering
			uint		imageCount;			// count of images in swapchain
		};

		Out< Info >		result;
	};


	//
	// Submit Commands
	//
	template <typename MsgType>
	struct _SubmitCommands : _MessageBase_
	{
	// types
		using Base_t			= _SubmitCommands<MsgType>;
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
		_SubmitCommands () : fence{Uninitialized} {}
		explicit _SubmitCommands (const ModulePtr &cmd) : commands({ cmd }) {}
		explicit _SubmitCommands (ArrayCRef< ModulePtr > list) : commands( list ) {}

		template <typename Other> Other const&  _Cast () const					{ return reinterpret_cast<Other const&>( *this ); }

		MsgType&  SignalSemaphore (Semaphore_t sem)								{ signalSemaphores.PushBack( sem );			return Cast<MsgType &>(*this); }
		MsgType&  WaitSemaphore (Semaphore_t sem, EPipelineStage::bits stage)	{ waitSemaphores.PushBack({ sem, stage });	return Cast<MsgType &>(*this); }
		MsgType&  SetFence (Fence_t newFence)									{ fence = newFence;							return Cast<MsgType &>(*this); }
	};

	struct SubmitGraphicsQueueCommands : _SubmitCommands<SubmitGraphicsQueueCommands>
	{
		SubmitGraphicsQueueCommands () {}
		explicit SubmitGraphicsQueueCommands (const ModulePtr &cmd) : Base_t( cmd ) {}
		explicit SubmitGraphicsQueueCommands (ArrayCRef< ModulePtr > list) : Base_t( list ) {}
	};

	struct SubmitComputeQueueCommands : _SubmitCommands<SubmitComputeQueueCommands>
	{
		SubmitComputeQueueCommands () {}
		explicit SubmitComputeQueueCommands (const ModulePtr &cmd) : Base_t( cmd ) {}
		explicit SubmitComputeQueueCommands (ArrayCRef< ModulePtr > list) : Base_t( list ) {}
	};


	//
	// Begin / End Frame
	//
	struct ThreadBeginFrame : _MessageBase_
	{
		struct Data {
			ModulePtr	framebuffer;		// returns current framebuffer
			uint		frameIndex;			// index of image in swapchain, max value if 'GraphicsSettings::swapchainLength'-1
		};
		Out< Data >		result;
	};

	struct ThreadEndFrame : _SubmitCommands<ThreadEndFrame>
	{
	// variables
		ModulePtr		framebuffer;	// (optional) must be null or framebuffer returned by 'ThreadBeginFrame'

	// methods
		ThreadEndFrame () {}
		explicit ThreadEndFrame (const ModulePtr &command) : Base_t( command ), framebuffer( framebuffer ) {}
		explicit ThreadEndFrame (ArrayCRef<ModulePtr> commands) : Base_t( commands ), framebuffer( framebuffer ) {}

		ThreadEndFrame& SetFramebuffer (const ModulePtr &fb)	{ framebuffer = fb;  return *this; }
	};


	//
	// Get Compute / Graphics Module IDs
	//
	struct GetGraphicsModules : _MessageBase_
	{
		Out< Platforms::ComputeModuleIDs >		compute;
		Out< Platforms::GraphicsModuleIDs >		graphics;
	};


	//
	// Get Graphics Settings
	//
	struct GetGraphicsSettings : _MessageBase_
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
	struct GetComputeSettings : _MessageBase_
	{
		Out< Platforms::ComputeSettings >	result;
	};


	//
	// Get Device Properties
	//
	struct GetDeviceProperties : _MessageBase_
	{
	// types
		using uint3	= GXMath::uint3;

		struct Properties
		{
			// compute shader
			uint	maxComputeWorkGroupInvocations	= 0;
			uint3	maxComputeWorkGroupSize;		// local size
			uint3	maxComputeWorkGroupCount;

			// features
			bool	explicitMemoryObjects	= false;
		};

	// variables
		Out< Properties >	result;
	};

}	// GpuMsg
}	// Engine
