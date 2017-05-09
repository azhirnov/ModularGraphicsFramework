// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Enums.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace ModuleMsg
{

	//
	// Get GPU Buffer ID
	//
	struct GetGpuBufferID
	{
		Out< vk::VkBuffer >		result;
	};
	

	//
	// Get GPU Framebuffer ID
	//
	struct GetGpuFramebufferID
	{
		Out< vk::VkFramebuffer >	result;
	};


	//
	// Get GPU Graphics Pipeline ID
	//
	struct GetGraphicsPipelineID
	{
		Out< vk::VkPipeline >		result;
	};
	

	//
	// Get GPU Render Pass ID
	//
	struct GetGpuRenderPassID
	{
		Out< vk::VkRenderPass >		result;
	};
	

	//
	// Get GPU Sampler ID
	//
	struct GetGpuSamplerID
	{
		Out< vk::VkSampler >	result;
	};


	//
	// Get GPU Texture ID
	//
	struct GetGpuTextureID
	{
		Out< vk::VkImage >		result;
		Out< vk::VkImageView >	defaultView;
	};


	//
	// Get GPU Texture View
	//
	struct GetGpuTextureView
	{
		Out< vk::VkImageView >	result;
	};


	//
	// Get GPU Shader Module IDs
	//
	struct GetGpuShaderModuleIDs
	{
		struct ShaderModule
		{
			StaticString<64>			entry;
			vk::VkShaderModule			id;
			Platforms::EShader::type	type;
		};

		Out< Array< ShaderModule > >	result;
	};

	
	//
	// Get GPU Command Buffer ID
	//
	struct GetGpuCommandBufferID
	{
		Out< vk::VkCommandBuffer >	result;
	};


	struct CommandBufferBeginRenderPassID
	{
	};

	struct CommandBufferBindGraphicsPipelineID
	{
	};

	struct CommandBufferBindComputePipelineID
	{
	};

	struct CommandBufferBindVertexBufferIDs
	{
	};
	
	struct CommandBufferBindIndexBufferID
	{
	};

	struct CommandBufferDrawIndirectID
	{
	};

	struct CommandBufferDrawIndexedIndirectID
	{
	};

	struct CommandBufferExecuteID
	{
	};


}	// ModuleMsg
}	// Engine

#endif	// GRAPHICS_API_VULKAN
