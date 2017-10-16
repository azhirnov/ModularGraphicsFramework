// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Enums.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace GpuMsg
{

	//
	// Get Logic Device
	//
	struct GetVkLogicDevice
	{
		Out< vk::VkDevice >			result;
	};


	//
	// Get GPU Buffer ID
	//
	struct GetVkBufferID
	{
		Out< vk::VkBuffer >			result;
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
	// Get GPU Compute Pipeline ID
	//
	struct GetVkComputePipelineID
	{
		Out< vk::VkPipeline >		result;
	};
	

	//
	// Get GPU Pipeline Layout
	//
	struct GetVkPipelineLayoutID
	{
		Out< vk::VkPipelineLayout >	result;
	};


	//
	// Get Descriptor Layout IDs
	//
	struct GetVkDescriptorLayouts
	{
		using IDs	= FixedSizeArray< vk::VkDescriptorSetLayout, 8 >;

		Out< IDs >					result;
	};


	//
	// Get GPU Pipeline Resource Table ID (DescriptorSet)
	//
	struct GetVkPipelineResourceTableID
	{
		Out< vk::VkDescriptorSet >	result;
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
		Out< vk::VkSampler >		result;
	};


	//
	// Get GPU Image ID
	//
	struct GetVkImageID
	{
		Out< vk::VkImage >			result;
		Out< vk::VkImageView >		defaultView;
	};


	//
	// Get GPU Image View
	//
	struct GetVkImageView
	{
		Out< vk::VkImageView >		result;
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
		Out< vk::VkCommandBuffer >		result;
	};


	//
	// Get GPU Command Pool ID
	//
	struct GetVkCommandPoolID
	{
		Out< vk::VkCommandPool >		result;
	};


}	// GpuMsg
}	// Engine

#endif	// GRAPHICS_API_VULKAN
