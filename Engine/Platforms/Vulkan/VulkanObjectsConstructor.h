// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/IDs.h"

#ifdef GRAPHICS_API_VULKAN

namespace Engine
{
namespace CreateInfo
{
	struct GpuThread;
	struct GpuContext;
	struct GpuImage;
	struct GpuMemory;
	struct GpuMemoryManager;
	struct GpuBuffer;
	struct GpuSampler;
	struct GpuRenderPass;
	struct GpuFramebuffer;
	struct PipelineTemplate;
	struct GpuCommandBuffer;
	struct GpuCommandBuilder;
	struct GpuCommandQueue;
	struct ComputePipeline;
	struct GraphicsPipeline;
	struct PipelineResourceTable;

}	// CreateInfo


namespace Platforms
{

	// Vulkan
	static constexpr GModID::type  VkContextModuleID				= "vk.ctx"_GModID;
	static constexpr TModID::type  VkThreadModuleID					= "vk.thread"_TModID;
	static constexpr OModID::type  VkBufferModuleID					= "vk.buffer"_OModID;
	static constexpr OModID::type  VkCommandQueueModuleID			= "vk.queue"_OModID;
	static constexpr OModID::type  VkCommandBufferModuleID			= "vk.cmdbuf"_OModID;
	static constexpr OModID::type  VkCommandBuilderModuleID			= "vk.cmdbuild"_OModID;
	static constexpr OModID::type  VkFramebufferModuleID			= "vk.fbuffer"_OModID;
	static constexpr OModID::type  VkImageModuleID					= "vk.image"_OModID;
	static constexpr OModID::type  VkManagedMemoryModuleID			= "vk.mngd-mem"_OModID;	// part of memory manager
	static constexpr OModID::type  VkMemoryManagerModuleID			= "vk.mem-mngr"_OModID;
	static constexpr OModID::type  VkGraphicsPipelineModuleID		= "vk.g-ppln"_OModID;
	static constexpr OModID::type  VkComputePipelineModuleID		= "vk.c-ppln"_OModID;
	static constexpr OModID::type  VkPipelineResourceTableModuleID	= "vk.restable"_OModID;
	static constexpr OModID::type  VkRenderPassModuleID				= "vk.renpass"_OModID;
	static constexpr OModID::type  VkSamplerModuleID				= "vk.sampler"_OModID;
	static constexpr OModID::type  VkSyncManagerModuleID			= "vk.sync"_OModID;


	//
	// Vulkan Objects Constructor
	//

	class VulkanObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();
		
		static GraphicsModuleIDs GetGraphicsModules ();
		static ComputeModuleIDs GetComputeModules ();

		static ModulePtr CreateVulkanThread (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateVulkanContext (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateVk1Image (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateVk1Buffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateVk1Sampler (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreateVk1ManagedMemory (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateVk1RenderPass (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr CreateVk1SyncManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateVk1Framebuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr CreateVk1CommandQueue (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandQueue &);
		static ModulePtr CreatePipelineTemplate (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateVk1CommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateVk1MemoryManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuMemoryManager &);
		static ModulePtr CreateVk1CommandBuilder (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateCachedVk1ComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateCachedVk1GraphicsPipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr CreateVk1PipelineResourceTable (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
		
		// for internal usage
		static ModulePtr CreateVk1ComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateVk1GraphicsPipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
