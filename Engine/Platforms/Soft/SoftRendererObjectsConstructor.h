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
		
		static ModulePtr CreateSoftRendererThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateSoftRendererContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateSWImage (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateSWMemory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateSWBuffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateSWSampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreateSWRenderPass (GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr CreateSWFramebuffer (GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateSWCommandQueue (GlobalSystemsRef, const CreateInfo::GpuCommandQueue &);
		static ModulePtr CreateSWSyncManager (GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateSWCommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateSWCommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateSWComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateSWGraphicsPipeline (GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr CreateSWPipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
