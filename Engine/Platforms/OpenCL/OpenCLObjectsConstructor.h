// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/IDs.h"

#ifdef COMPUTE_API_OPENCL

namespace Engine
{
namespace CreateInfo
{
	struct GpuThread;
	struct GpuContext;
	struct GpuSyncManager;
	struct GpuCommandQueue;
	struct GpuImage;
	struct GpuMemory;
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

	// OpenCL
	static constexpr GModID::type  CLContextModuleID				= "cl.ctx"_GModID;
	static constexpr TModID::type  CLThreadModuleID					= "cl.thread"_TModID;
	static constexpr OModID::type  CLBufferModuleID					= "cl.buffer"_OModID;
	static constexpr OModID::type  CLCommandQueueModuleID			= "cl.queue"_OModID;
	static constexpr OModID::type  CLCommandBufferModuleID			= "cl.cmdbuf"_OModID;
	static constexpr OModID::type  CLCommandBuilderModuleID			= "cl.cmdbuild"_OModID;
	static constexpr OModID::type  CLImageModuleID					= "cl.image"_OModID;
	static constexpr OModID::type  CLComputePipelineModuleID		= "cl.c-ppln"_OModID;
	static constexpr OModID::type  CLPipelineResourceTableModuleID	= "cl.restable"_OModID;
	static constexpr OModID::type  CLSamplerModuleID				= "cl.sampler"_OModID;
	static constexpr OModID::type  CLSyncManagerModuleID			= "cl.sync"_OModID;


	//
	// OpenCL Objects Constructor
	//

	class OpenCLObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();
		
		static ComputeModuleIDs GetComputeModules ();
		
		static ModulePtr CreateOpenCLThread (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateOpenCLContext (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateCL1SyncManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateCL1Image (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateCL1Buffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateCL1Sampler (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreateCL1CommandQueue (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandQueue &);
		static ModulePtr CreatePipelineTemplate (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateCL1CommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateCL1CommandBuilder (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateCL1ComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateCL1PipelineResourceTable (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
