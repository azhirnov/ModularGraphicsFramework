// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1Device.h"
#include "Engine/Platforms/Vulkan/110/Vk1Messages.h"

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Base Object
	//

	class Vk1BaseObject : public StaticRefCountedObject
	{
	// variables
	private:
		Ptr< Vk1Device >	_vkDevice;


	// emthods
	public:
		explicit Vk1BaseObject (Ptr<Vk1Device> dev) : _vkDevice(dev) {}

		ND_ Ptr< Vk1Device >	GetDevice ()	const	{ return _vkDevice; }
		ND_ vk::VkDevice		GetVkDevice ()	const	{ return _vkDevice and _vkDevice->IsDeviceCreated() ? _vkDevice->GetLogicalDevice() : null; }
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
