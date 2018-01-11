// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/IDs.h"

#if defined( GRAPHICS_API_VULKAN )

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

		static ModulePtr CreateVulkanThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateVulkanContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateVk1Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		//static ModulePtr CreateVk1Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateVk1Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateVk1Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreateVk1ManagedMemory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateVk1RenderPass (GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr CreateVk1SyncManager (GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateVk1Framebuffer (GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateVk1CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateVk1MemoryManager (GlobalSystemsRef, const CreateInfo::GpuMemoryManager &);
		static ModulePtr CreateVk1CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateCachedVk1ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateCachedVk1GraphicsPipeline (GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr CreateVk1PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
		
		// for internal usage
		static ModulePtr CreateVk1ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateVk1GraphicsPipeline (GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
