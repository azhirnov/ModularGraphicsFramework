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
		static ModulePtr CreateCL2SyncManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateCL2Image (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateCL2Memory (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateCL2Buffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateCL2Sampler (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreatePipelineTemplate (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateCL2CommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateCL2CommandBuilder (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateCL2ComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateCL2PipelineResourceTable (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
