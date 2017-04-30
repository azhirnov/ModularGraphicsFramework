// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/vulkan1.h"

#if defined( GRAPHICS_API_VULKAN ) and defined( PLATFORM_WINDOWS )

namespace Engine
{
namespace Platforms
{

	//
	// Vulkan Windows Surface
	//

	class VkWinSurface
	{
	// types
	public:
		using Handle_t	= OS::HiddenOSTypeFrom<void*>;

	// methods
	public:
		VkWinSurface ();
		~VkWinSurface ();

		bool Create (vk::VkInstance instance, const Handle_t &windowHandle, OUT vk::VkSurfaceKHR &surface);
		void Destroy ();
		void RequestExtensions (INOUT Array<const char*> &ext);
	};


	using VkSurface		= VkWinSurface;


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN and PLATFORM_WINDOWS
