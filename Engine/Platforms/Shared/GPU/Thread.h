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
	// Begin / End Frame
	//
	struct ThreadBeginFrame
	{
		struct Data {
			ModulePtr	framebuffer;		// returns current framebuffer
			ModulePtr	commandBuilder;		// this builder destroys all command buffer before resize swapchain
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
		ModulePtr			framebuffer;	// (optional) must be null or framebuffer returned by GpuThreadBeginFrame

	// methods
		ThreadEndFrame ()
		{}

		explicit ThreadEndFrame (const ModulePtr &framebuffer) : framebuffer(framebuffer)
		{}

		ThreadEndFrame (const ModulePtr &framebuffer, InitializerList< ModulePtr > list) :
			framebuffer(framebuffer), commands( list )
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
		explicit SubmitGraphicsQueueCommands (const ModulePtr &cmd) : commands({ cmd })
		{}

		SubmitGraphicsQueueCommands (InitializerList< ModulePtr > list) : commands( list )
		{}
	};


}	// GpuMsg
}	// Engine
