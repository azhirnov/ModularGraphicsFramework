// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{

	// Shared
	static constexpr OModID::type  PipelineTemplateModuleID			= "pp-templ"_OModID;

	// TODO: remove platform-dependent ids



	struct ComputeModuleIDs
	{
		GModID::type	context			= GModID::type(0);
		TModID::type	thread			= TModID::type(0);
		OModID::type	buffer			= OModID::type(0);
		OModID::type	image			= OModID::type(0);
		OModID::type	sampler			= OModID::type(0);
		OModID::type	memory			= OModID::type(0);
		OModID::type	memoryManager	= OModID::type(0);
		OModID::type	pipeline		= OModID::type(0);
		OModID::type	resourceTable	= OModID::type(0);
		OModID::type	commandBuffer	= OModID::type(0);
		OModID::type	commandBuilder	= OModID::type(0);
		OModID::type	queryPool		= OModID::type(0);

		ComputeModuleIDs (GX_DEFCTOR) {}
	};


	struct GraphicsModuleIDs
	{
		GModID::type	context			= GModID::type(0);
		TModID::type	thread			= TModID::type(0);
		OModID::type	buffer			= OModID::type(0);
		OModID::type	image			= OModID::type(0);
		OModID::type	sampler			= OModID::type(0);
		OModID::type	memory			= OModID::type(0);
		OModID::type	memoryManager	= OModID::type(0);
		OModID::type	pipeline		= OModID::type(0);
		OModID::type	resourceTable	= OModID::type(0);
		OModID::type	commandBuffer	= OModID::type(0);
		OModID::type	commandBuilder	= OModID::type(0);
		OModID::type	framebuffer		= OModID::type(0);
		OModID::type	renderPass		= OModID::type(0);
		OModID::type	queryPool		= OModID::type(0);
		
		GraphicsModuleIDs (GX_DEFCTOR) {}
	};


}	// Platforms
}	// Engine
