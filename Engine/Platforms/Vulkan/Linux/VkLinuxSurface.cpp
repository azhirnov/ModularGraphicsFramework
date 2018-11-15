// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Linux/VkLinuxSurface.h"

#if defined( GRAPHICS_API_VULKAN ) and defined( PLATFORM_LINUX )

#include "Engine/Platforms/Vulkan/110/vulkan1_platform.h"

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;
	
	
# ifdef VK_USE_PLATFORM_XLIB_KHR
/*
=================================================
	Create
=================================================
*/
	bool VkXlibSurface::Create (VkInstance instance, const PlatformTools::WindowHelper::XlibWindow &wnd, OUT VkSurfaceKHR &surface)
	{
		CHECK_ERR( instance != VK_NULL_HANDLE );
		
		CHECK_ERR( Vk1_PlatformInit( instance, null ) );

		VkXlibSurfaceCreateInfoKHR		surface_info = {};

		surface_info.sType		= VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		surface_info.dpy		= wnd.display.Get< Display *>();
		surface_info.window		= wnd.window.Get< Window >();
		
		VK_CHECK( vkCreateXlibSurfaceKHR( instance, &surface_info, null, OUT &surface ) );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void VkXlibSurface::Destroy ()
	{
		Vk1_PlatformDelete();
	}
	
/*
=================================================
	RequestExtensions
=================================================
*/
	void VkXlibSurface::RequestExtensions (INOUT Array<const char*> &ext)
	{
		ext << VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
	}
//-----------------------------------------------------------------------------
	


# elif defined VK_USE_PLATFORM_XCB_KHR
/*
=================================================
	Create
=================================================
*/
	bool VkXcbSurface::Create (VkInstance instance, const PlatformTools::WindowHelper::XcbWindow &wnd, OUT VkSurfaceKHR &surface)
	{
		CHECK_ERR( instance != VK_NULL_HANDLE );
		
		CHECK_ERR( Vk1_PlatformInit( instance, null ) );

		VkXcbSurfaceCreateInfoKHR		surface_info = {};

		surface_info.sType		= VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surface_info.connection	= wnd.connection.Get< xcb_connection_t *>();
		surface_info.window		= wnd.window.Get< xcb_window_t >();
		
		VK_CHECK( vkCreateXcbSurfaceKHR( instance, &surface_info, null, OUT &surface ) );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void VkXcbSurface::Destroy ()
	{
		Vk1_PlatformDelete();
	}
	
/*
=================================================
	RequestExtensions
=================================================
*/
	void VkXcbSurface::RequestExtensions (INOUT Array<const char*> &ext)
	{
		ext << VK_KHR_XCB_SURFACE_EXTENSION_NAME;
	}
//-----------------------------------------------------------------------------
	


# elif defined VK_USE_PLATFORM_WAYLAND_KHR
/*
=================================================
	Create
=================================================
*/
	bool VkWaylandSurface::Create (VkInstance instance, const PlatformTools::WindowHelper::WaylandWindow &wnd, OUT VkSurfaceKHR &surface)
	{
		CHECK_ERR( instance != VK_NULL_HANDLE );
		
		CHECK_ERR( Vk1_PlatformInit( instance, null ) );

		VkWaylandSurfaceCreateInfoKHR	surface_info = {};

		surface_info.sType		= VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
		surface_info.display	= wnd.display.Get< struct wl_display *>();
		surface_info.surface	= wnd.surface.Get< struct wl_surface *>();
		
		VK_CHECK( vkCreateWaylandSurfaceKHR( instance, &surface_info, null, OUT &surface ) );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void VkWaylandSurface::Destroy ()
	{
		Vk1_PlatformDelete();
	}
	
/*
=================================================
	RequestExtensions
=================================================
*/
	void VkWaylandSurface::RequestExtensions (INOUT Array<const char*> &ext)
	{
		ext << VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
	}
//-----------------------------------------------------------------------------


	
# elif defined VK_USE_PLATFORM_MIR_KHR
/*
=================================================
	Create
=================================================
*/
	bool VkMirSurface::Create (VkInstance instance, const PlatformTools::WindowHelper::MirWindow &wnd, OUT VkSurfaceKHR &surface)
	{
		CHECK_ERR( instance != VK_NULL_HANDLE );
		
		CHECK_ERR( Vk1_PlatformInit( instance, null ) );

		VkMirSurfaceCreateInfoKHR	surface_info = {};

		surface_info.sType			= VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
		surface_info.connection		= wnd.connection.Get< MirConnection *>();
		surface_info.mirSurface		= wnd.surface.Get< MirSurface* >();
		
		VK_CHECK( vkCreateWin32SurfaceKHR( instance, &surface_info, null, OUT &surface ) );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void VkMirSurface::Destroy ()
	{
		Vk1_PlatformDelete();
	}
	
/*
=================================================
	RequestExtensions
=================================================
*/
	void VkMirSurface::RequestExtensions (INOUT Array<const char*> &ext)
	{
		ext << VK_KHR_MIR_SURFACE_EXTENSION_NAME;
	}
//-----------------------------------------------------------------------------
# endif


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN and PLATFORM_WINDOWS
