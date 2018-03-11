// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/Impl/Vk1Enums.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/Public/GPU/PipelineLayout.h"

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
	// Get Framebuffer ID
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
	// Get Graphics Pipeline ID
	//
	struct GetVkGraphicsPipelineID
	{
		Out< vk::VkPipeline >		result;
	};


	//
	// Get Compute Pipeline ID
	//
	struct GetVkComputePipelineID
	{
		Out< vk::VkPipeline >		result;
	};
	

	//
	// Get Pipeline Layout
	//
	struct GetVkPipelineLayoutID
	{
		Out< vk::VkPipelineLayout >	result;
	};


	//
	// Get Pipeline Layout Push Constants Mapping
	//
	struct GetVkPipelineLayoutPushConstants
	{
	// types
		using EShader	= Platforms::EShader;

		struct PushConstant : CompileTime::FastCopyable
		{
			EShader::bits	stages;
			ushort			offset	= 0;
			ushort			size	= 0;

			PushConstant (EShader::bits stages, BytesU off, BytesU sz) : stages{stages}, offset{ushort(off)}, size{ushort(sz)} {}
		};

		using Name_t			= Platforms::PipelineLayoutDescriptor::Name_t;
		using PushConstants_t	= FixedSizeMultiHashMap< Name_t, PushConstant, 8 >;

	// variables
		Out< PushConstants_t >		result;
	};


	//
	// Get Descriptor Layout IDs
	//
	struct GetVkDescriptorLayouts
	{
		Out< vk::VkDescriptorSetLayout >	result;
	};


	//
	// Get Pipeline Resource Table ID (DescriptorSet)
	//
	struct GetVkPipelineResourceTableID
	{
		Out< vk::VkDescriptorSet >	result;
	};


	//
	// Get Render Pass ID
	//
	struct GetVkRenderPassID
	{
		Out< vk::VkRenderPass >		result;
	};
	

	//
	// Get Sampler ID
	//
	struct GetVkSamplerID
	{
		Out< vk::VkSampler >		result;
	};


	//
	// Get Image ID
	//
	struct GetVkImageID
	{
		Out< vk::VkImage >			result;
		Out< vk::VkImageView >		defaultView;
	};


	//
	// Create Image View
	//
	struct CreateVkImageView
	{
	// variables
		Platforms::ImageViewDescriptor	viewDescr;
		Out< vk::VkImageView >			result;

	// methods
		CreateVkImageView () {}
		explicit CreateVkImageView (const Platforms::ImageViewDescriptor &descr) : viewDescr{descr} {}
	};


	//
	// Get Shader Module IDs
	//
	struct GetVkShaderModuleIDs
	{
	// types
		struct ShaderModule : CompileTime::PODStruct
		{
			StaticString<64>			entry;
			vk::VkShaderModule			id		= 0;
			Platforms::EShader::type	type	= Platforms::EShader::Unknown;
		};
		using Shaders_t		= FixedSizeArray< ShaderModule, Platforms::EShader::_Count >;
		
	// variables
		Out< Shaders_t >	result;
	};

	
	//
	// Get Command Buffer ID
	//
	struct GetVkCommandBufferID
	{
		Out< vk::VkCommandBuffer >		result;
	};


	//
	// Get Command Pool ID
	//
	struct GetVkCommandPoolID
	{
		Out< vk::VkCommandPool >		result;
	};


	//
	// Memory Manger Allocate Block for Image / Buffer
	//
	struct VkAllocMemForImage
	{
	// types
		struct Data {
			vk::VkDeviceMemory	mem	= VK_NULL_HANDLE;
			BytesUL				offset;
			BytesUL				size;
		};
		using EGpuMemory	= Platforms::EGpuMemory;

	// variables
		ModuleWPtr			module;
		vk::VkImage			image	= VK_NULL_HANDLE;
		EGpuMemory::bits	flags;
		Out< Data >			result;

	// methods
		VkAllocMemForImage (GX_DEFCTOR) {}
		VkAllocMemForImage (const ModuleWPtr &mod, vk::VkImage img, EGpuMemory::bits flags) : module{mod}, image{img}, flags{flags} {}
	};


	struct VkAllocMemForBuffer
	{
	// types
		using Data			= VkAllocMemForImage::Data;
		using EGpuMemory	= Platforms::EGpuMemory;

	// variables
		ModuleWPtr			module;
		vk::VkBuffer		buffer	= VK_NULL_HANDLE;
		EGpuMemory::bits	flags;
		Out< Data >			result;

	// methods
		VkAllocMemForBuffer (GX_DEFCTOR) {}
		VkAllocMemForBuffer (const ModuleWPtr &mod, vk::VkBuffer buf, EGpuMemory::bits flags) : module{mod}, buffer{buf}, flags{flags} {}
	};


	//
	// Memory Manager Free Block
	//
	struct VkFreeMemory
	{
	// variables
		ModuleWPtr			module;
		vk::VkDeviceMemory	mem	= VK_NULL_HANDLE;
		BytesUL				offset;
		BytesUL				size;

	// methods
		VkFreeMemory (GX_DEFCTOR) {}
		VkFreeMemory (const ModuleWPtr &mod, vk::VkDeviceMemory mem, BytesUL off, BytesUL size) : module{mod}, mem{mem}, offset{off}, size{size} {}
	};


	//
	// Get Fence
	//
	struct GetVkFence
	{
	// types
		using Fence_t	= Platforms::GpuFenceId;

	// variables
		Fence_t				fenceId;
		Out< vk::VkFence >	result;

	// methods
		explicit GetVkFence (Fence_t id) : fenceId(id) {}
	};


	//
	// Get Event
	//
	struct GetVkEvent
	{
	// types
		using Event_t	= Platforms::GpuEventId;

	// variables
		Event_t				eventId;
		Out< vk::VkEvent >	result;

	// methods
		explicit GetVkEvent (Event_t id) : eventId(id) {}
	};


	//
	// Get Semaphore
	//
	struct GetVkSemaphore
	{
	// types
		using Semaphore_t	= Platforms::GpuSemaphoreId;

	// variables
		Semaphore_t				semId;
		Out< vk::VkSemaphore >	result;

	// methods
		explicit GetVkSemaphore (Semaphore_t id) : semId(id) {}
	};


}	// GpuMsg
}	// Engine

#endif	// GRAPHICS_API_VULKAN
