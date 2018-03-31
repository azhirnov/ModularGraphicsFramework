// Generated header file

#pragma once

#ifdef VK_USE_PLATFORM_ANDROID_KHR
#define VK_KHR_android_surface 1
#define VK_KHR_ANDROID_SURFACE_SPEC_VERSION 6
#define VK_KHR_ANDROID_SURFACE_EXTENSION_NAME "VK_KHR_android_surface"
typedef VkFlags VkAndroidSurfaceCreateFlagsKHR;
struct VkAndroidSurfaceCreateInfoKHR { 
    VkStructureType                   sType;
    const void*                       pNext;
    VkAndroidSurfaceCreateFlagsKHR    flags;
    ANativeWindow*                    window;
};

typedef VkResult (VKAPI_PTR *PFN_vkCreateAndroidSurfaceKHR)(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

#endif /* VK_USE_PLATFORM_ANDROID_KHR */


#define VK1_OS_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( VkResult, vkCreateAndroidSurfaceKHR, ( VkInstance instance , const VkAndroidSurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \


