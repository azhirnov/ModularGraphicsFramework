// Generated header file

#pragma once

#ifndef VULKAN_XLIB_H_
#define VULKAN_XLIB_H_ 1
#define VK_KHR_xlib_surface 1
#define VK_KHR_XLIB_SURFACE_SPEC_VERSION  6
#define VK_KHR_XLIB_SURFACE_EXTENSION_NAME "VK_KHR_xlib_surface"
typedef VkFlags VkXlibSurfaceCreateFlagsKHR;
struct VkXlibSurfaceCreateInfoKHR { 
    VkStructureType                sType;
    const void*                    pNext;
    VkXlibSurfaceCreateFlagsKHR    flags;
    Display*                       dpy;
    Window                         window;
};

typedef VkResult (VKAPI_PTR *PFN_vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);

#endif


#define VK1_XLIB_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( [[nodiscard]] VkResult, vkCreateXlibSurfaceKHR, ( VkInstance instance , const VkXlibSurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkBool32, vkGetPhysicalDeviceXlibPresentationSupportKHR, ( VkPhysicalDevice physicalDevice , uint32_t queueFamilyIndex , Display * dpy , VisualID visualID ) , (VkBool32)(0) ) \


