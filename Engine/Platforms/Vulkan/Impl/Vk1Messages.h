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
	struct GetVkBufferID
	{
		Out< vk::VkBuffer >		result;
	};
	

	//
	// Get GPU Framebuffer ID
	//
	struct GetVkFramebufferID
	{
		Out< vk::VkFramebuffer >	result;
	};


	//
	// Get GPU Graphics Pipeline ID
	//
	struct GetVkGraphicsPipelineID
	{
		Out< vk::VkPipeline >		result;
	};
	

	//
	// Get GPU Render Pass ID
	//
	struct GetVkRenderPassID
	{
		Out< vk::VkRenderPass >		result;
	};
	

	//
	// Get GPU Sampler ID
	//
	struct GetVkSamplerID
	{
		Out< vk::VkSampler >	result;
	};


	//
	// Get GPU Texture ID
	//
	struct GetVkTextureID
	{
		Out< vk::VkImage >		result;
		Out< vk::VkImageView >	defaultView;
	};


	//
	// Get GPU Texture View
	//
	struct GetVkTextureView
	{
		Out< vk::VkImageView >	result;
	};


	//
	// Get GPU Shader Module IDs
	//
	struct GetVkShaderModuleIDs
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
	struct GetVkCommandBufferID
	{
		Out< vk::VkCommandBuffer >	result;
	};


	struct GpuCmdBeginRenderPassID
	{
		// TODO
	};

	struct GpuCmdBindGraphicsPipelineID
	{
		// TODO
	};

	struct GpuCmdBindComputePipelineID
	{
		// TODO
	};

	struct GpuCmdBindVertexBufferIDs
	{
		// TODO
	};
	
	struct GpuCmdBindIndexBufferID
	{
		// TODO
	};

	struct GpuCmdDrawIndirectID
	{
		// TODO
	};

	struct GpuCmdDrawIndexedIndirectID
	{
		// TODO
	};

	struct GpuCmdExecuteID
	{
		// TODO
	};


}	// ModuleMsg
}	// Engine

#endif	// GRAPHICS_API_VULKAN
