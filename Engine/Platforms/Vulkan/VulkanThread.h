// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Windows/WinMessages.h"

#if defined( GRAPHICS_API_VULKAN )

#include "Engine/Platforms/Vulkan/Windows/VkWinSurface.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Fence.h"

namespace Engine
{
namespace Platforms
{

	//
	// Vulkan Thread
	//
	
	class VulkanThread final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											ModuleMsg::WindowCreated,
											ModuleMsg::WindowBeforeDestroy,
											ModuleMsg::WindowDescriptorChanged,
											ModuleMsg::WindowVisibilityChanged,
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::GetVkLogicDevice
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
											// TODO: device lost event
										> >;
		
		using VideoSettings_t		= CreateInfo::GpuContext;

		using Surface				= PlatformVK::Vk1Surface;
		using Device				= PlatformVK::Vk1Device;
		using SamplerCache			= PlatformVK::Vk1SamplerCache;
		using PipelineCache			= PlatformVK::Vk1PipelineCache;
		using LayoutCache			= PlatformVK::Vk1PipelineLayoutCache;
		using RenderPassCache		= PlatformVK::Vk1RenderPassCache;
		using VkSubSystems			= PlatformVK::VkSubSystems;
		using VkSystemsRef			= PlatformVK::VkSystemsRef;

		//
		// Command Buffers Life Control
		//
		struct CmdBufferLifeControl
		{
		// types
			struct PerFrame
			{
				Array< ModulePtr >			cmdBuffers;
				PlatformVK::Vk1FencePtr		fence;
				uint						imageIndex = 0;		// index of image in swapchain
			};
			using PerFrameData_t		= StaticArray< PerFrame, 8 >;
			using FrameIndex_t			= Limit< uint, LimitStrategy::Wrap >;
			using CommandBuffers_t		= Array< vk::VkCommandBuffer >;

		// variables
			CommandBuffers_t	tempCmdBuffers;
			PerFrameData_t		perFrameData;
			FrameIndex_t		frameIndex		{ 0, 0, 3 };

		// methods
			CmdBufferLifeControl (const VkSystemsRef vkSys);
			~CmdBufferLifeControl ();

			bool SubmitQueue (Ptr<Device> dev, ArrayCRef<ModulePtr> cmdBuffers);
			void FreeBuffers (uint len);

			bool Resize (uint size);
			void FreeAll ();

			void Destroy ();
		};


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		VkSubSystems			_vkSystems;

		VideoSettings_t			_settings;

		ModulePtr				_window;

		Surface					_surface;
		Device					_device;

		SamplerCache			_samplerCache;
		PipelineCache			_pipelineCache;
		LayoutCache				_layoutCache;
		RenderPassCache			_renderPassCache;
		
		CmdBufferLifeControl	_cmdControl;

		bool					_isWindowVisible;


	// methods
	public:
		VulkanThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~VulkanThread ();
		
		VkSystemsRef			VkSystems ()	const		{ return VkSystemsRef(&_vkSystems); }

		Ptr< Device >			GetDevice ()				{ return &_device; }

		Ptr< SamplerCache >		GetSamplerCache ()			{ return &_samplerCache; }
		Ptr< PipelineCache >	GetPipelineCache ()			{ return &_pipelineCache; }
		Ptr< LayoutCache >		GetLayoutCache ()			{ return &_layoutCache; }
		Ptr< RenderPassCache >	GetRenderPassCache ()		{ return &_renderPassCache; }
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

		bool _ThreadBeginFrame (const Message< GpuMsg::ThreadBeginFrame > &);
		bool _ThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &);
		bool _SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &);

		bool _WindowCreated (const Message< ModuleMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &);
		bool _WindowVisibilityChanged (const Message< ModuleMsg::WindowVisibilityChanged > &);
		bool _WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &);
		
		bool _GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &);

	private:
		bool _CreateDevice (const ModuleMsg::WindowCreated &msg);
		void _DetachWindow ();
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
