// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Enums.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace GpuMsg
{

	//
	// Get Device Info
	//
	struct GetVkDeviceInfo
	{
		struct Info {
			vk::VkInstance			instance			= VK_NULL_HANDLE;
			vk::VkPhysicalDevice	physicalDevice		= VK_NULL_HANDLE;
			vk::VkDevice			logicalDevice		= VK_NULL_HANDLE;
			vk::VkQueue				graphicsQueue		= VK_NULL_HANDLE;
			uint					graphicsQueueIndex	= 0;
			vk::VkQueue				computeQueue		= VK_NULL_HANDLE;
			uint					computeQueueIndex	= 0;
			vk::VkQueue				transferQueue		= VK_NULL_HANDLE;
			uint					transferQueueIndex	= 0;
		};

		Out< Info >			result;
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
	// Get GPU Memory ID
	//
	struct GetVkMemoryID
	{
		Out< vk::VkDeviceMemory >	result;
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
		struct ShaderModule : CompileTime::PODStruct
		{
			StaticString<64>			entry;
			vk::VkShaderModule			id		= 0;
			Platforms::EShader::type	type	= Platforms::EShader::Unknown;;
		};
		using Shaders_t		= FixedSizeArray< ShaderModule, Platforms::EShader::_Count >;

		Out< Shaders_t >	result;
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
