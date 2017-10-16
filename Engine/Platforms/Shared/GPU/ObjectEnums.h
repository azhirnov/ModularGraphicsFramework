// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	struct EGpuObject
	{
		enum type : uint
		{
			Instance,
			PhysicalDevice,
			Device,
			Queue,
			Semaphore,
			CommandBuffer,
			Fence,
			DeviceMemory,
			Buffer,
			Image,
			Event,
			QueryPool,
			BufferView,
			ImageView,
			ShaderModule,
			PipelineCache,
			PipelineLayout,
			RenderPass,
			Pipeline,
			Sampler,
			DescriptorSetLayout,
			DescriptorPool,
			DescriptorSet,
			Framebuffer,
			CommandPool,
			Surface,
			Swapchain,
			//DebugReport,
			Display,
			//DisplayMode,

			Unknown		= ~0u
		};
	};

}	// Platforms
}	// Engine
