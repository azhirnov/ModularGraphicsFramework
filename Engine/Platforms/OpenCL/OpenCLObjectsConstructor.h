// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/IDs.h"

#if defined( COMPUTE_API_OPENCL )

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
		
		static ModulePtr CreateOpenCLThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateOpenCLContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateCL2SyncManager (GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateCL2Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateCL2Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateCL2Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateCL2Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateCL2CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateCL2CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateCL2ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateCL2PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
