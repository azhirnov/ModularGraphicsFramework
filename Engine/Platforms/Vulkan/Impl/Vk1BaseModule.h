// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Messages.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	class Vk1SamplerCache;
	class Vk1PipelineCache;
	class Vk1PipelineLayoutCache;
	class Vk1RenderPassCache;

}	// PlatformVK

namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetVkPrivateClasses
	{
		struct Classes {
			PlatformVK::Vk1Device *					device			= null;
			PlatformVK::Vk1SamplerCache *			samplerCache	= null;
			PlatformVK::Vk1PipelineCache *			pipelineCache	= null;
			PlatformVK::Vk1PipelineLayoutCache *	layoutCache		= null;
			PlatformVK::Vk1RenderPassCache *		renderPassCache	= null;
			Base::Module *							memManager		= null;
		};

		Out< Classes >		result;
	};
}	// GpuMsg

namespace PlatformVK
{

	//
	// Vulkan Base Module
	//

	class Vk1BaseModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetVkDeviceInfo,
											GpuMsg::GetVkPrivateClasses
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											GpuMsg::DeviceBeforeDestroy
										>;


	// variables
	private:
		Ptr< Vk1Device >	_vkDevice;
		

	// methods
	public:
		Vk1BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *msgTypes,
					   const TypeIdList *eventTypes);


	protected:
		Ptr< Vk1Device >	GetDevice ()	const	{ return _vkDevice; }
		vk::VkDevice		GetVkDevice ()	const	{ return _vkDevice ? _vkDevice->GetLogicalDevice() : null; }


	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetVkDeviceInfo (const Message< GpuMsg::GetVkDeviceInfo > &);
		bool _GetVkPrivateClasses (const Message< GpuMsg::GetVkPrivateClasses > &);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
