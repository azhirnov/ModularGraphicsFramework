// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/IDs.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace CreateInfo
{
	struct GpuThread;
	struct GpuContext;
	struct GpuImage;
	struct GpuMemory;
	struct GpuBuffer;
	struct GpuSampler;
	struct GpuRenderPass;
	struct GpuSyncManager;
	struct GpuCommandQueue;
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

	// Software
	static constexpr GModID::type  SWContextModuleID				= "sw.ctx"_GModID;
	static constexpr TModID::type  SWThreadModuleID					= "sw.thread"_TModID;
	static constexpr OModID::type  SWBufferModuleID					= "sw.buffer"_OModID;
	static constexpr OModID::type  SWCommandQueueModuleID			= "sw.queue"_OModID;
	static constexpr OModID::type  SWCommandBufferModuleID			= "sw.cmdbuf"_OModID;
	static constexpr OModID::type  SWCommandBuilderModuleID			= "sw.cmdbuild"_OModID;
	static constexpr OModID::type  SWImageModuleID					= "sw.image"_OModID;
	static constexpr OModID::type  SWSamplerModuleID				= "sw.sampler"_OModID;
	static constexpr OModID::type  SWMemoryModuleID					= "sw.memory"_OModID;
	static constexpr OModID::type  SWComputePipelineModuleID		= "sw.c-ppln"_OModID;
	static constexpr OModID::type  SWPipelineResourceTableModuleID	= "sw.restable"_OModID;
	static constexpr OModID::type  SWSyncManagerModuleID			= "sw.sync"_OModID;


	//
	// Soft Renderer Objects Constructor
	//

	class SoftRendererObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();
		
		static GraphicsModuleIDs GetGraphicsModules ();
		static ComputeModuleIDs GetComputeModules ();
		
		static ModulePtr CreateSoftRendererThread (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateSoftRendererContext (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateSWImage (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateSWMemory (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateSWBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateSWSampler (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreateSWRenderPass (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr CreateSWFramebuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr CreatePipelineTemplate (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateSWCommandQueue (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandQueue &);
		static ModulePtr CreateSWSyncManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateSWCommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateSWCommandBuilder (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateSWComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateSWGraphicsPipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr CreateSWPipelineResourceTable (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
