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
	struct GpuQueryPool;
	struct GpuRenderPass;
	struct GpuFramebuffer;
	struct GpuSyncManager;
	struct GpuCommandQueue;
	struct PipelineTemplate;
	struct GpuCommandBuffer;
	struct GpuCommandBuilder;
	struct ComputePipeline;
	struct GraphicsPipeline;
	struct PipelineResourceTable;

}	// CreateInfo


namespace Platforms
{
	
	// OpenGL
	static constexpr GModID::type  GLContextModuleID				= "gl.ctx"_GModID;
	static constexpr TModID::type  GLThreadModuleID					= "gl.thread"_TModID;
	static constexpr OModID::type  GLBufferModuleID					= "gl.buffer"_OModID;
	static constexpr OModID::type  GLCommandQueueModuleID			= "gl.queue"_OModID;
	static constexpr OModID::type  GLCommandBufferModuleID			= "gl.cmdbuf"_OModID;
	static constexpr OModID::type  GLCommandBuilderModuleID			= "gl.cmdbuild"_OModID;
	static constexpr OModID::type  GLFramebufferModuleID			= "gl.fbuffer"_OModID;
	static constexpr OModID::type  GLImageModuleID					= "gl.image"_OModID;
	static constexpr OModID::type  GLMemoryModuleID					= "gl.memory"_OModID;
	static constexpr OModID::type  GLGraphicsPipelineModuleID		= "gl.g-ppln"_OModID;
	static constexpr OModID::type  GLComputePipelineModuleID		= "gl.c-ppln"_OModID;
	static constexpr OModID::type  GLPipelineResourceTableModuleID	= "gl.restable"_OModID;
	static constexpr OModID::type  GLRenderPassModuleID				= "gl.renpass"_OModID;
	static constexpr OModID::type  GLSamplerModuleID				= "gl.sampler"_OModID;
	static constexpr OModID::type  GLSyncManagerModuleID			= "gl.sync"_OModID;
	static constexpr OModID::type  GLQueryPoolModuleID				= "gl.query"_OModID;


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
		static ModulePtr CreateGL4QueryPool (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuQueryPool &);
		static ModulePtr CreateGL4RenderPass (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr CreateGL4Framebuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr CreateGL4SyncManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSyncManager &);
		static ModulePtr CreateGL4CommandQueue (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandQueue &);
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
