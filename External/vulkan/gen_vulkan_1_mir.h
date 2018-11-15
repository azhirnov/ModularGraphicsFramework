// Generated header file

#pragma once

#ifndef VULKAN_MIR_H_
#define VULKAN_MIR_H_ 1
#define VK_KHR_mir_surface 1
#define VK_KHR_MIR_SURFACE_SPEC_VERSION   4
#define VK_KHR_MIR_SURFACE_EXTENSION_NAME "VK_KHR_mir_surface"
typedef VkFlags VkMirSurfaceCreateFlagsKHR;
struct VkMirSurfaceCreateInfoKHR { 
    VkStructureType               sType;
    const void*                   pNext;
    VkMirSurfaceCreateFlagsKHR    flags;
    MirConnection*                connection;
    MirSurface*                   mirSurface;
};

typedef VkResult (VKAPI_PTR *PFN_vkCreateMirSurfaceKHR)(VkInstance instance, const VkMirSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceMirPresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection);

#endif


#define VK1_MIR_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( [[nodiscard]] VkResult, vkCreateMirSurfaceKHR, ( VkInstance instance , const VkMirSurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkBool32, vkGetPhysicalDeviceMirPresentationSupportKHR, ( VkPhysicalDevice physicalDevice , uint32_t queueFamilyIndex , MirConnection * connection ) , (VkBool32)(0) ) \


