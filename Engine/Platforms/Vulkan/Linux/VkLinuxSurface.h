// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/110/vulkan1.h"

#if defined( GRAPHICS_API_VULKAN ) and defined( PLATFORM_LINUX )

//#define VK_USE_PLATFORM_XLIB_KHR
//#define VK_USE_PLATFORM_XCB_KHR
//#define VK_USE_PLATFORM_WAYLAND_KHR
//#define VK_USE_PLATFORM_MIR_KHR
//#define VK_USE_PLATFORM_DISPLAY_KHR

#include "Engine/Platforms/Public/Tools/WindowHelper.h"

namespace Engine
{
namespace PlatformVK
{

# ifdef VK_USE_PLATFORM_XLIB_KHR

	//
	// Vulkan Xlib Surface
	//
	class VkXlibSurface
	{
	// methods
	public:
		VkXlibSurface ()	{}
		~VkXlibSurface ()	{ Destroy(); }

		bool Create (vk::VkInstance instance, const PlatformTools::WindowHelper::XlibWindow &, OUT vk::VkSurfaceKHR &surface);
		void Destroy ();
		void RequestExtensions (INOUT Array<const char*> &ext);
	};
	using Vk1Surface = VkXlibSurface;


# elif defined VK_USE_PLATFORM_XCB_KHR
	
	//
	// Vulkan XCB Surface
	//
	class VkXcbSurface
	{
	// methods
	public:
		VkXcbSurface ()		{}
		~VkXcbSurface ()	{ Destroy(); }

		bool Create (vk::VkInstance instance, const PlatformTools::WindowHelper::XcbWindow &, OUT vk::VkSurfaceKHR &surface);
		void Destroy ();
		void RequestExtensions (INOUT Array<const char*> &ext);
	};
	using Vk1Surface = VkXcbSurface;


# elif defined VK_USE_PLATFORM_WAYLAND_KHR
	
	//
	// Vulkan Wayland Surface
	//
	class VkWaylandSurface
	{
	// methods
	public:
		VkWaylandSurface ()		{}
		~VkWaylandSurface ()	{ Destroy(); }

		bool Create (vk::VkInstance instance, const PlatformTools::WindowHelper::WaylandWindow &, OUT vk::VkSurfaceKHR &surface);
		void Destroy ();
		void RequestExtensions (INOUT Array<const char*> &ext);
	};
	using Vk1Surface = VkWaylandSurface;


# elif defined VK_USE_PLATFORM_MIR_KHR
	
	//
	// Vulkan Mir Surface
	//
	class VkMirSurface
	{
	// methods
	public:
		VkMirSurface ()		{}
		~VkMirSurface ()	{ Destroy(); }

		bool Create (vk::VkInstance instance, const PlatformTools::WindowHelper::MirWindow &, OUT vk::VkSurfaceKHR &surface);
		void Destroy ();
		void RequestExtensions (INOUT Array<const char*> &ext);
	};
	using Vk1Surface = VkMirSurface;


# else
#	error unknown window manager
# endif


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN and PLATFORM_LINUX
