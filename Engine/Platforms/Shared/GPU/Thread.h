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


}	// ModuleMsg
}	// Engine
