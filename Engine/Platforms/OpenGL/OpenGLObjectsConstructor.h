// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/IDs.h"

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
		
		static ModulePtr CreateOpenGLThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr CreateOpenGLContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr CreateGL4Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr CreateGL4Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr CreateGL4Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr CreateGL4Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr CreateGL4RenderPass (GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr CreateGL4Framebuffer (GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr CreateGL4SyncManager (GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr CreateGL4CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr CreateGL4CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr CreateGL4ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr CreateGL4GraphicsPipeline (GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr CreateGL4PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
