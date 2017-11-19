// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Messages.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace Platforms
{

	//
	// OpenGL Context
	//
	
	class OpenGLContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											GpuMsg::GetGraphicsModules
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using GLThreads_t			= Set< ModulePtr >;
		
		using GLThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetGLDeviceInfo >;
		using GLThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		GLThreads_t				_threads;
		

	// methods
	public:
		OpenGLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~OpenGLContext ();
		
		static _ENGINE_PLATFORMS_EXPORT_ void Register (GlobalSystemsRef);
		static _ENGINE_PLATFORMS_EXPORT_ void Unregister (GlobalSystemsRef);
		
		static GraphicsModuleIDs GetGraphicsModules ();
		static ComputeModuleIDs GetComputeModules ();

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &);
		
	private:
		static ModulePtr _CreateOpenGLThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr _CreateOpenGLContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr _CreateGL4Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr _CreateGL4Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr _CreateGL4Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr _CreateGL4Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr _CreateGL4RenderPass (GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr _CreateGL4Framebuffer (GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr _CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr _CreateGL4CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr _CreateGL4CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr _CreateGL4ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr _CreateGL4GraphicsPipeline (GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr _CreateGL4PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
