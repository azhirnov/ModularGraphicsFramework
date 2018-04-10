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
	struct ComputePipeline;
	struct GraphicsPipeline;
	struct PipelineResourceTable;

}	// CreateInfo


namespace Platforms
{

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
