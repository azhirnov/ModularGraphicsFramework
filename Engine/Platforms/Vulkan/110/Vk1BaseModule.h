// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1Device.h"
#include "Engine/Platforms/Vulkan/110/Vk1Messages.h"
#include "Engine/Platforms/Public/GPU/Thread.h"

namespace Engine
{
namespace PlatformVK
{
	class Vk1SamplerCache;
	class Vk1PipelineCache;
	class Vk1PipelineLayoutCache;
	class Vk1RenderPassCache;
	class Vk1ResourceCache;

}	// PlatformVK

namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetVkPrivateClasses : _MessageBase_
	{
		struct Classes {
			PlatformVK::Vk1Device *					device			= null;
			PlatformVK::Vk1SamplerCache *			samplerCache	= null;
			PlatformVK::Vk1PipelineCache *			pipelineCache	= null;
			PlatformVK::Vk1PipelineLayoutCache *	layoutCache		= null;
			PlatformVK::Vk1RenderPassCache *		renderPassCache	= null;
			PlatformVK::Vk1ResourceCache *			resourceCache	= null;
			Base::Module *							memManager		= null;

			Classes (PlatformVK::Vk1Device *dev, PlatformVK::Vk1SamplerCache *sampCache,
					 PlatformVK::Vk1PipelineCache *pplnCache, PlatformVK::Vk1PipelineLayoutCache *pplnLayoutCache,
					 PlatformVK::Vk1RenderPassCache *rpCache, PlatformVK::Vk1ResourceCache *resCache,
					 Base::Module *memMngr) :
				device{dev}, samplerCache{sampCache}, pipelineCache{pplnCache}, layoutCache{pplnLayoutCache},
				renderPassCache{rpCache}, resourceCache{resCache}, memManager{memMngr}
			{}
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
											ModuleMsg::Link,
											ModuleMsg::AfterLink,
											ModuleMsg::Compose,
											ModuleMsg::AfterCompose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;


	// variables
	private:
		Ptr< Vk1Device >			_vkDevice;
		Ptr< Vk1ResourceCache >		_vkResourceCache;
		

	// methods
	protected:
		Vk1BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *msgTypes,
					   const TypeIdList *eventTypes);

		ND_ Ptr< Vk1Device >		GetDevice ()		const	{ return _vkDevice; }
		ND_ vk::VkDevice			GetVkDevice ()		const	{ return _vkDevice and _vkDevice->IsDeviceCreated() ? _vkDevice->GetLogicalDevice() : null; }
		ND_ Ptr< Vk1ResourceCache >	GetResourceCache ()	const	{ return _vkResourceCache; }

		ND_ ModulePtr _GetGPUThread (const ModulePtr &);


	// message handlers
	protected:
		bool _OnManagerChanged (const ModuleMsg::OnManagerChanged &);
		bool _GetDeviceInfo (const GpuMsg::GetDeviceInfo &);
		bool _GetVkDeviceInfo (const GpuMsg::GetVkDeviceInfo &);
		bool _GetVkPrivateClasses (const GpuMsg::GetVkPrivateClasses &);
		

	// event handlers
	private:
		bool _DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _DeviceDeleted (const ModuleMsg::Delete &);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
