// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Public/GPU/CommandQueue.h"
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

		explicit GpuThread (const Platforms::GraphicsSettings &gsettings, const ModulePtr &context = null, const ModulePtr &shared = null) :
			context{ context }, shared{ shared }, settings{ gsettings } {}

		explicit GpuThread (const Platforms::ComputeSettings &csettings, const ModulePtr &context = null, const ModulePtr &shared = null) :
			context{ context }, shared{ shared }, settings{ csettings } {}
	};

}	// CreateInfo


namespace GpuMsg
{
	//
	// GPU Device Created / Destroyed
	//
	struct DeviceCreated : _MsgBase_
	{
	};

	struct DeviceBeforeDestroy : _MsgBase_
	{
	};


	//
	// Get Device Info
	//
	struct GetDeviceInfo : _MsgBase_
	{
	// types
		struct Info {
			ModulePtr	gpuThread;			// current gpu thread		// TODO: remove?
			ModulePtr	sharedThread;		// can be null
			ModulePtr	memManager;			// can be null				// TODO: remove?
			ModulePtr	syncManager;		// can be null				// TODO: remove?
			ModulePtr	renderPass;			// this is default render pass used for on screen rendering
			uint		imageCount;			// count of images in swapchain
		};
		
	// variables
		Out< Info >		result;
	};


	//
	// Begin / End Frame
	//
	struct ThreadBeginFrame : _MsgBase_
	{
	// types
		struct Data {
			ModulePtr	framebuffer;		// returns current framebuffer
			ModulePtr	image;				// returns current image (color render target in framebuffer), may be null
			uint		frameIndex;			// index of image in swapchain, max value if 'GraphicsSettings::swapchainLength'-1
		};

	// variables
		Out_opt< Data >		result;
	};


	struct ThreadEndFrame : SubmitCommands
	{
	// variables
		ModulePtr		framebuffer;	// (optional) must be null or framebuffer returned by 'ThreadBeginFrame'

	// methods
		ThreadEndFrame () {}
		explicit ThreadEndFrame (const ModulePtr &command) : SubmitCommands( command ) {}
		explicit ThreadEndFrame (ArrayCRef<ModulePtr> commands) : SubmitCommands( commands ) {}

		ThreadEndFrame&  SetFramebuffer (const ModulePtr &fb)							{ framebuffer = fb;							return *this; }
		ThreadEndFrame&  SignalSemaphore (GpuSemaphoreId sem)							{ signalSemaphores.PushBack( sem );			return *this; }
		ThreadEndFrame&  WaitSemaphore (GpuSemaphoreId sem, EPipelineStage::bits stage)	{ waitSemaphores.PushBack({ sem, stage });	return *this; }
		ThreadEndFrame&  SetFence (GpuFenceId newFence)									{ fence = newFence;							return *this; }
	};


	//
	// Get Compute / Graphics Module IDs
	//
	struct GetGraphicsModules : _MsgBase_
	{
	// types
		struct Data {
			Platforms::ComputeModuleIDs		compute;
			Platforms::GraphicsModuleIDs	graphics;
		};
		
	// variables
		Out< Data >		result;
	};


	//
	// Get Graphics Settings
	//
	struct GetGraphicsSettings : _MsgBase_
	{
	// types
		struct Settings : Platforms::GraphicsSettings
		{
		// variables
			uint2		surfaceSize;

		// methods
			Settings (const Platforms::GraphicsSettings &gs, const uint2 &size) :
				Platforms::GraphicsSettings(gs), surfaceSize(size) {}
		};

	// variables
		Out< Settings >		result;
	};


	//
	// Get Compute Settings
	//
	struct GetComputeSettings : _MsgBase_
	{
		Out< Platforms::ComputeSettings >	result;
	};


	//
	// Get Device Properties
	//
	struct GetDeviceProperties : _MsgBase_
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
