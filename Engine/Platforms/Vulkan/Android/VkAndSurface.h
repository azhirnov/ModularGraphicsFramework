// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/110/vulkan1.h"

#if defined( GRAPHICS_API_VULKAN ) and defined( PLATFORM_ANDROID )

namespace Engine
{
namespace PlatformVK
{
	using namespace Engine::Platforms;


	//
	// Vulkan Android Surface
	//

	class VkAndSurface
	{
	// types
	public:
		using AWindow_t = DeferredTypeFrom<void*>;


	// methods
	public:
		VkAndSurface ();
		~VkAndSurface ();

		bool Create (vk::VkInstance instance, const AWindow_t &windowHandle, OUT vk::VkSurfaceKHR &surface);
		void Destroy ();
		void RequestExtensions (INOUT Array<const char*> &ext);
	};


	using Vk1Surface	= VkAndSurface;


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN and PLATFORM_ANDROID
