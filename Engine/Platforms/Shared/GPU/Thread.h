// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"
#include "Engine/Platforms/Shared/GPU/Context.h"

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
			settings{ settings.version, EFlags::bits() | (settings.isDebug ? EFlags::DebugContext : EFlags::type(0)),
					  Platforms::EPixelFormat::Unknown, Platforms::EPixelFormat::Unknown,
					  Platforms::MultiSamples(), settings.device, 0 }
		{}
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


	//
	// Get Device Info
	//
	struct GetDeviceInfo
	{
		struct Info {
			ModulePtr	renderPass;			// this is default render pass used for on screen rendering
			uint		imageCount;			// count of images in swapchain
		};

		Out< Info >		result;
	};


	//
	// Begin / End Frame
	//
	struct ThreadBeginFrame
	{
		struct Data {
			ModulePtr	framebuffer;		// returns current framebuffer
			uint		index;				// index of image in swapchain
		};
		Out< Data >		result;
	};

	struct ThreadEndFrame
	{
	// types
		using Commands_t	= FixedSizeArray< ModulePtr, 32 >;
		
	// variables
		ModulePtr		framebuffer;	// (optional) must be null or framebuffer returned by ThreadBeginFrame
		ModulePtr		commands;		// (optional) commands to submit before present frame

	// methods
		ThreadEndFrame ()
		{}

		ThreadEndFrame (const ModulePtr &framebuffer, const ModulePtr &commands) :
			framebuffer( framebuffer ), commands( commands )
		{}
	};


	//
	// Submit Commands
	//
	struct SubmitGraphicsQueueCommands
	{
	// types
		using Commands_t	= FixedSizeArray< ModulePtr, 32 >;

	// variables
		Commands_t		commands;

	// methods
		explicit SubmitGraphicsQueueCommands (const ModulePtr &cmd) : commands({ cmd }) {}
		explicit SubmitGraphicsQueueCommands (ArrayCRef< ModulePtr > list) : commands( list ) {}
	};

	struct SubmitComputeQueueCommands
	{
	// types
		using Commands_t	= FixedSizeArray< ModulePtr, 32 >;

	// variables
		Commands_t		commands;

	// methods
		explicit SubmitComputeQueueCommands (const ModulePtr &cmd) : commands({ cmd }) {}
		explicit SubmitComputeQueueCommands (ArrayCRef< ModulePtr > list) : commands( list ) {}
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
		Out< Platforms::GraphicsSettings >		result;
	};


}	// GpuMsg
}	// Engine
