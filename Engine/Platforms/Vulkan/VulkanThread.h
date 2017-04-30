// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Windows/WinWindow.h"

#if defined( GRAPHICS_API_VULKAN )

#include "Engine/Platforms/Vulkan/Windows/VkWinSurface.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"

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
											ModuleMsg::OnManagerChanged,
											ModuleMsg::WindowCreated,
											ModuleMsg::WindowBeforeDestroy,
											ModuleMsg::WindowDescriptorChanged
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::GpuDeviceCreated,
											ModuleMsg::GpuDeviceBeforeDestory
										> >;
		
		using VideoSettings_t		= CreateInfo::GpuContext;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		VideoSettings_t		_settings;

		ModulePtr			_window;

		VkSurface			_surface;
		Vk1Device			_device;
		

	// methods
	public:
		VulkanThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~VulkanThread ();
		
		Ptr< Vk1Device >		GetDevice ()			{ return &_device; }

		static TModID::type		GetStaticID ()			{ return "vk1.thrd"_TModID; }
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);

		bool _WindowCreated (const Message< ModuleMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &);
		bool _WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &);

	private:
		bool _CreateDevice (const ModuleMsg::WindowCreated &msg);
		void _DetachWindow ();
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
