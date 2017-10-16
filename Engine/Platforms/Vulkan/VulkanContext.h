// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace Platforms
{

	//
	// Vulkan Context
	//
	
	class VulkanContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using VkThreads_t			= Set< ModulePtr >;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		VkThreads_t				_threads;
		

	// methods
	public:
		VulkanContext (const GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~VulkanContext ();
		
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	private:
		friend void RegisterPlatforms ();
		static void _RegisterAll (GlobalSystemsRef);

		static ModulePtr _CreateVulkanThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr _CreateVulkanContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr _CreateVk1Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr _CreateVk1Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr _CreateVk1Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr _CreateVk1Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr _CreateVk1RenderPass (GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
		static ModulePtr _CreateVk1Framebuffer (GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
		static ModulePtr _CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr _CreateVk1CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr _CreateVk1CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr _CreateVk1ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr _CreateVk1GraphicsPipeline (GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
		static ModulePtr _CreateVk1PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
