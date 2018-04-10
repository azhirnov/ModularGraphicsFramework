// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/IDs.h"

#ifdef GRAPHICS_API_OPENGL

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
	struct GpuFramebuffer;
	struct GpuSyncManager;
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
	// OpenGL Objects Constructor
	//

	class OpenGLObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();
		
		static GraphicsModuleIDs GetGraphicsModules ();
		static ComputeModuleIDs GetComputeModules ();
		
		static ModulePtr CreateOpenGLThread (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateOpenGLContext (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateGL4Image (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateGL4Memory (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateGL4Buffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateGL4Sampler (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreateGL4RenderPass (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr CreateGL4Framebuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr CreateGL4SyncManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreatePipelineTemplate (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateGL4CommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateGL4CommandBuilder (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateGL4ComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateGL4GraphicsPipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr CreateGL4PipelineResourceTable (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
