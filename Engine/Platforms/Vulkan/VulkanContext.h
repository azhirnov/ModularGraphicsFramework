// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Messages.h"

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
											ModuleMsg::RemoveFromManager,
											GpuMsg::GetGraphicsModules
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using VkThreads_t			= Set< ModulePtr >;

		using VkThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetVkDeviceInfo >;
		using VkThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		VkThreads_t				_threads;
		

	// methods
	public:
		VulkanContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~VulkanContext ();
		
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
		static ModulePtr _CreateVulkanThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr _CreateVulkanContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr _CreateVk1Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr _CreateVk1Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr _CreateVk1Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr _CreateVk1Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr _CreateVk1MemoryManager (GlobalSystemsRef, const CreateInfo::GpuMemory &);
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
