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
		ModulePtr		context;
		ModulePtr		shared;
		GpuContext		settings;
	};

}	// CreateInfo


namespace ModuleMsg
{
	//
	// GPU Device Created / Destroyed
	//
	struct GpuDeviceCreated
	{
	};

	struct GpuDeviceBeforeDestory
	{
	};


	//
	// Begin / End Frame
	//
	struct GpuThreadBeginFrame
	{
		Out< ModulePtr >	framebuffer;	// returns current framebuffer
	};

	struct GpuThreadEndFrame
	{
	// types
		using Commands_t	= FixedSizeArray< ModulePtr, 32 >;
		
	// variables
		Commands_t			commands;		// (optional) commands to submit before present frame
		ModulePtr			framebuffer;	// (optional) must be null or framebuffer returned by GpuThreadBeginFrame

	// methods
		GpuThreadEndFrame ()
		{}

		explicit GpuThreadEndFrame (const ModulePtr &framebuffer) : framebuffer(framebuffer)
		{}

		GpuThreadEndFrame (const ModulePtr &framebuffer, std::initializer_list< ModulePtr > list) :
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

		SubmitGraphicsQueueCommands (std::initializer_list< ModulePtr > list) : commands( list )
		{}
	};


}	// ModuleMsg
}	// Engine
