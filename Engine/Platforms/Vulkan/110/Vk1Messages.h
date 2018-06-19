// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1Enums.h"
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
	struct GetVkDeviceInfo : _MessageBase_
	{
		struct Info {
			vk::VkInstance			instance			= VK_NULL_HANDLE;
			vk::VkPhysicalDevice	physicalDevice		= VK_NULL_HANDLE;
			vk::VkDevice			logicalDevice		= VK_NULL_HANDLE;

			Info (vk::VkInstance inst, vk::VkPhysicalDevice pdev, vk::VkDevice ldev) :
				instance{inst}, physicalDevice{pdev}, logicalDevice{ldev} {}
		};

		Out< Info >			result;
	};


	//
	// Get GPU Buffer ID
	//
	struct GetVkBufferID : _MessageBase_
	{
		Out< vk::VkBuffer >			result;
	};
	

	//
	// Get Framebuffer ID
	//
	struct GetVkFramebufferID : _MessageBase_
	{
		Out< vk::VkFramebuffer >	result;
	};


	//
	// Get GPU Memory ID
	//
	struct GetVkMemoryID : _MessageBase_
	{
		Out< vk::VkDeviceMemory >	result;
	};


	//
	// Get Graphics Pipeline ID
	//
	struct GetVkGraphicsPipelineID : _MessageBase_
	{
		Out< vk::VkPipeline >		result;
	};


	//
	// Get Compute Pipeline ID
	//
	struct GetVkComputePipelineID : _MessageBase_
	{
		Out< vk::VkPipeline >		result;
	};
	

	//
	// Get Pipeline Layout
	//
	struct GetVkPipelineLayoutID : _MessageBase_
	{
		Out< vk::VkPipelineLayout >	result;
	};


	//
	// Get Pipeline Layout Push Constants Mapping
	//
	struct GetVkPipelineLayoutPushConstants : _MessageBase_
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

		using Name_t			= Platforms::PipelineLayoutDescription::Name_t;
		using PushConstants_t	= FixedSizeMultiHashMap< Name_t, PushConstant, GlobalConst::GAPI_MaxPushConstants/4 >;

	// variables
		Out< PushConstants_t >		result;
	};


	//
	// Get Description Layout IDs
	//
	struct GetVkDescriptionLayouts : _MessageBase_
	{
		Out< vk::VkDescriptorSetLayout >	result;
	};


	//
	// Get Pipeline Resource Table ID (DescriptorSet)
	//
	struct GetVkPipelineResourceTableID : _MessageBase_
	{
		Out< vk::VkDescriptorSet >		result;
	};


	//
	// Get Render Pass ID
	//
	struct GetVkRenderPassID : _MessageBase_
	{
		Out< vk::VkRenderPass >		result;
	};
	

	//
	// Get Sampler ID
	//
	struct GetVkSamplerID : _MessageBase_
	{
		Out< vk::VkSampler >		result;
	};


	//
	// Get Image ID
	//
	struct GetVkImageID : _MessageBase_
	{
	// types
		struct Data {
			vk::VkImage			id			= VK_NULL_HANDLE;
			vk::VkImageView		defaultView	= VK_NULL_HANDLE;

			Data () {}
			Data (vk::VkImage id, vk::VkImageView def) : id{id}, defaultView{def} {}
		};

	// variables
		Out< Data >			result;
	};


	//
	// Create Image View
	//
	struct CreateVkImageView : _MessageBase_
	{
	// variables
		Platforms::ImageViewDescription	viewDescr;
		Out< vk::VkImageView >			result;

	// methods
		CreateVkImageView () {}
		explicit CreateVkImageView (const Platforms::ImageViewDescription &descr) : viewDescr{descr} {}
	};


	//
	// Get Shader Module IDs
	//
	struct GetVkShaderModuleIDs : _MessageBase_
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
	struct GetVkCommandBufferID : _MessageBase_
	{
		Out< vk::VkCommandBuffer >		result;
	};


	//
	// Get Command Pool ID
	//
	struct GetVkCommandPoolID : _MessageBase_
	{
		Out< vk::VkCommandPool >		result;
	};


	//
	// Memory Manger Allocate Block for Image / Buffer
	//
	struct VkAllocMemForImage : _MessageBase_
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
		VkAllocMemForImage () {}
		VkAllocMemForImage (const ModuleWPtr &mod, vk::VkImage img, EGpuMemory::bits flags) : module{mod}, image{img}, flags{flags} {}
	};


	struct VkAllocMemForBuffer : _MessageBase_
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
		VkAllocMemForBuffer () {}
		VkAllocMemForBuffer (const ModuleWPtr &mod, vk::VkBuffer buf, EGpuMemory::bits flags) : module{mod}, buffer{buf}, flags{flags} {}
	};


	//
	// Memory Manager Free Block
	//
	struct VkFreeMemory : _MessageBase_
	{
	// variables
		ModuleWPtr			module;
		vk::VkDeviceMemory	mem	= VK_NULL_HANDLE;
		BytesUL				offset;
		BytesUL				size;

	// methods
		VkFreeMemory () {}
		VkFreeMemory (const ModuleWPtr &mod, vk::VkDeviceMemory mem, BytesUL off, BytesUL size) : module{mod}, mem{mem}, offset{off}, size{size} {}
	};


	//
	// Get Fence
	//
	struct GetVkFence : _MessageBase_
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
	struct GetVkEvent : _MessageBase_
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
	struct GetVkSemaphore : _MessageBase_
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
