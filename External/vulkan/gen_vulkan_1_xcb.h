// Generated header file

#pragma once

#ifndef VULKAN_XCB_H_
#define VULKAN_XCB_H_ 1
#define VK_KHR_xcb_surface 1
#define VK_KHR_XCB_SURFACE_SPEC_VERSION   6
#define VK_KHR_XCB_SURFACE_EXTENSION_NAME "VK_KHR_xcb_surface"
typedef VkFlags VkXcbSurfaceCreateFlagsKHR;
struct VkXcbSurfaceCreateInfoKHR { 
    VkStructureType               sType;
    const void*                   pNext;
    VkXcbSurfaceCreateFlagsKHR    flags;
    xcb_connection_t*             connection;
    xcb_window_t                  window;
};

typedef VkResult (VKAPI_PTR *PFN_vkCreateXcbSurfaceKHR)(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);

#endif


#define VK1_XCB_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( [[nodiscard]] VkResult, vkCreateXcbSurfaceKHR, ( VkInstance instance , const VkXcbSurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkBool32, vkGetPhysicalDeviceXcbPresentationSupportKHR, ( VkPhysicalDevice physicalDevice , uint32_t queueFamilyIndex , xcb_connection_t * connection , xcb_visualid_t visual_id ) , (VkBool32)(0) ) \


