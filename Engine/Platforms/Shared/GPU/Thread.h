// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"
#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// GPU Thread Create Info
	//
	struct GpuThread
	{
		ModulePtr		context;
		ModulePtr		shared;
		GpuContext		settings;
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
			ModulePtr	commandBuilder;		// this builder destroys all command buffer before resize swapchain		// TODO: remove, create cmd manager
			uint		index;				// index of image in swapchain
		};
		Out< Data >		result;
	};

	struct ThreadEndFrame
	{
	// types
		using Commands_t	= FixedSizeArray< ModulePtr, 32 >;
		
	// variables
		Commands_t			commands;		// (optional) commands to submit before present frame
		ModulePtr			framebuffer;	// (optional) must be null or framebuffer returned by ThreadBeginFrame

	// methods
		ThreadEndFrame ()
		{}

		explicit ThreadEndFrame (const ModulePtr &framebuffer) : framebuffer(framebuffer)
		{}

		ThreadEndFrame (const ModulePtr &framebuffer, InitializerList< ModulePtr > list) :
			commands( list ), framebuffer( framebuffer )
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
		bool			sync	= false;

	// methods
		explicit SubmitGraphicsQueueCommands (const ModulePtr &cmd, bool sync = false) : commands({ cmd }), sync(sync) {}

		SubmitGraphicsQueueCommands (InitializerList< ModulePtr > list, bool sync = false) : commands( list ), sync(sync) {}
	};

	struct SubmitComputeQueueCommands
	{
	// types
		using Commands_t	= FixedSizeArray< ModulePtr, 32 >;

	// variables
		Commands_t		commands;
		bool			sync	= false;

	// methods
		explicit SubmitComputeQueueCommands (const ModulePtr &cmd, bool sync = false) : commands({ cmd }), sync(sync) {}

		SubmitComputeQueueCommands (InitializerList< ModulePtr > list, bool sync = false) : commands( list ), sync(sync) {}
	};


}	// GpuMsg
}	// Engine
