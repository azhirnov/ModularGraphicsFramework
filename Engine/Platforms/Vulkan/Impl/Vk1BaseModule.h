// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Messages.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
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
											GpuMsg::DeviceBeforeDestroy,
											GpuMsg::GetVkLogicDevice
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete
										>;


	// variables
	private:
		const VkSystemsRef		_vkSystems;
		

	// methods
	public:
		Vk1BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);
		
		Vk1BaseModule (const GlobalSystemsRef gs,
					   const ModulePtr &gpuThread,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		Vk1BaseModule (const GlobalSystemsRef gs,
					   const VkSystemsRef vkSys,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		VkSystemsRef	VkSystems ()	const	{ return _vkSystems; }


	protected:
		Ptr< VulkanThread >	GetGpuThread ()			{ return VkSystems()->Get< VulkanThread >(); }
		Ptr< Vk1Device >	GetDevice ()			{ return VkSystems()->Get< Vk1Device >(); }
		vk::VkDevice		GetLogicalDevice ()		{ return GetDevice()->GetLogicalDevice(); }
		
		virtual void _DestroyResources ();


	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &);
		//bool _LinkToMemory (const Message< ModuleMsg::Link > &);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
