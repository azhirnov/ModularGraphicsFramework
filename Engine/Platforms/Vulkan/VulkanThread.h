// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Windows/WinWindow.h"

#if defined( GRAPHICS_API_VULKAN )

#include "Engine/Platforms/Vulkan/Windows/VkWinSurface.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"

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
											ModuleMsg::GpuThreadBeginFrame,
											ModuleMsg::GpuThreadEndFrame,
											ModuleMsg::SubmitGraphicsQueueCommands
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::GpuDeviceCreated,
											ModuleMsg::GpuDeviceBeforeDestory
										> >;
		
		using VideoSettings_t		= CreateInfo::GpuContext;

		using Surface				= PlatformVK::Vk1Surface;
		using Device				= PlatformVK::Vk1Device;
		using SamplerCache			= PlatformVK::Vk1SamplerCache;
		using PipelineCache			= PlatformVK::Vk1PipelineCache;
		using RenderPassCache		= PlatformVK::Vk1RenderPassCache;
		using VkSubSystems			= PlatformVK::VkSubSystems;
		using VkSystemsRef			= PlatformVK::VkSystemsRef;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		VkSubSystems		_vkSystems;

		VideoSettings_t		_settings;

		ModulePtr			_window;

		Surface				_surface;
		Device				_device;

		SamplerCache		_samplerCache;
		PipelineCache		_pipelineCache;
		RenderPassCache		_renderPassCache;
		
		bool				_isWindowVisible;


	// methods
	public:
		VulkanThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~VulkanThread ();
		
		VkSystemsRef			VkSystems ()	const	{ return VkSystemsRef(&_vkSystems); }

		Ptr< Device >			GetDevice ()			{ return &_device; }

		Ptr< SamplerCache >		GetSamplerCache ()		{ return &_samplerCache; }
		Ptr< PipelineCache >	GetPipelineCache ()		{ return &_pipelineCache; }
		Ptr< RenderPassCache >	GetRenderPassCache ()	{ return &_renderPassCache; }

		static TModID::type		GetStaticID ()			{ return "vk1.thrd"_TModID; }
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

		bool _GpuThreadBeginFrame (const Message< ModuleMsg::GpuThreadBeginFrame > &);
		bool _GpuThreadEndFrame (const Message< ModuleMsg::GpuThreadEndFrame > &);
		bool _SubmitGraphicsQueueCommands (const Message< ModuleMsg::SubmitGraphicsQueueCommands > &);

		bool _WindowCreated (const Message< ModuleMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &);
		bool _WindowVisibilityChanged (const Message< ModuleMsg::WindowVisibilityChanged > &);
		bool _WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &);

	private:
		bool _CreateDevice (const ModuleMsg::WindowCreated &msg);
		void _DetachWindow ();
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
