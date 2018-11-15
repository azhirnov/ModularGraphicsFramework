// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/110/vulkan1.h"

#if defined( GRAPHICS_API_VULKAN ) and defined( PLATFORM_WINDOWS )

#include "Engine/Platforms/Public/Tools/WindowHelper.h"

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Windows Surface
	//

	class VkWinSurface
	{
	// types
	public:
		using HWND_t	= DeferredTypeFrom<void*>;

	// methods
	public:
		VkWinSurface ();
		~VkWinSurface ();

		bool Create (vk::VkInstance instance, const PlatformTools::WindowHelper::WinAPIWindow &, OUT vk::VkSurfaceKHR &surface);
		void Destroy ();
		void RequestExtensions (INOUT Array<const char*> &ext);
	};


	using Vk1Surface	= VkWinSurface;


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN and PLATFORM_WINDOWS
