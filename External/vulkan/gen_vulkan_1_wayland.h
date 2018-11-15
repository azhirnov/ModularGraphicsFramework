// Generated header file

#pragma once

#ifndef VULKAN_WAYLAND_H_
#define VULKAN_WAYLAND_H_ 1
#define VK_KHR_wayland_surface 1
#define VK_KHR_WAYLAND_SURFACE_SPEC_VERSION 6
#define VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME "VK_KHR_wayland_surface"
typedef VkFlags VkWaylandSurfaceCreateFlagsKHR;
struct VkWaylandSurfaceCreateInfoKHR { 
    VkStructureType                   sType;
    const void*                       pNext;
    VkWaylandSurfaceCreateFlagsKHR    flags;
    struct wl_display*                display;
    struct wl_surface*                surface;
};

typedef VkResult (VKAPI_PTR *PFN_vkCreateWaylandSurfaceKHR)(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);

#endif


#define VK1_WAYLAND_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( [[nodiscard]] VkResult, vkCreateWaylandSurfaceKHR, ( VkInstance instance , const VkWaylandSurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkBool32, vkGetPhysicalDeviceWaylandPresentationSupportKHR, ( VkPhysicalDevice physicalDevice , uint32_t queueFamilyIndex , struct wl_display * display ) , (VkBool32)(0) ) \


