// Generated header file

#pragma once

#ifndef VULKAN_WIN32_H_
#define VULKAN_WIN32_H_ 1
#define VK_KHR_win32_surface 1
#define VK_KHR_WIN32_SURFACE_SPEC_VERSION 6
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
typedef VkFlags VkWin32SurfaceCreateFlagsKHR;
struct VkWin32SurfaceCreateInfoKHR { 
    VkStructureType                 sType;
    const void*                     pNext;
    VkWin32SurfaceCreateFlagsKHR    flags;
    HINSTANCE                       hinstance;
    HWND                            hwnd;
};

typedef VkResult (VKAPI_PTR *PFN_vkCreateWin32SurfaceKHR)(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);

#define VK_KHR_external_memory_win32 1
#define VK_KHR_EXTERNAL_MEMORY_WIN32_SPEC_VERSION 1
#define VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME "VK_KHR_external_memory_win32"
struct VkImportMemoryWin32HandleInfoKHR { 
    VkStructureType                       sType;
    const void*                           pNext;
    VkExternalMemoryHandleTypeFlagBits    handleType;
    HANDLE                                handle;
    LPCWSTR                               name;
};

struct VkExportMemoryWin32HandleInfoKHR { 
    VkStructureType               sType;
    const void*                   pNext;
    const SECURITY_ATTRIBUTES*    pAttributes;
    DWORD                         dwAccess;
    LPCWSTR                       name;
};

struct VkMemoryWin32HandlePropertiesKHR { 
    VkStructureType    sType;
    void*              pNext;
    uint32_t           memoryTypeBits;
};

struct VkMemoryGetWin32HandleInfoKHR { 
    VkStructureType                       sType;
    const void*                           pNext;
    VkDeviceMemory                        memory;
    VkExternalMemoryHandleTypeFlagBits    handleType;
};

typedef VkResult (VKAPI_PTR *PFN_vkGetMemoryWin32HandleKHR)(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

typedef VkResult (VKAPI_PTR *PFN_vkGetMemoryWin32HandlePropertiesKHR)(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);

#define VK_KHR_win32_keyed_mutex 1
#define VK_KHR_WIN32_KEYED_MUTEX_SPEC_VERSION 1
#define VK_KHR_WIN32_KEYED_MUTEX_EXTENSION_NAME "VK_KHR_win32_keyed_mutex"
struct VkWin32KeyedMutexAcquireReleaseInfoKHR { 
    VkStructureType          sType;
    const void*              pNext;
    uint32_t                 acquireCount;
    const VkDeviceMemory*    pAcquireSyncs;
    const uint64_t*          pAcquireKeys;
    const uint32_t*          pAcquireTimeouts;
    uint32_t                 releaseCount;
    const VkDeviceMemory*    pReleaseSyncs;
    const uint64_t*          pReleaseKeys;
};

#define VK_KHR_external_semaphore_win32 1
#define VK_KHR_EXTERNAL_SEMAPHORE_WIN32_SPEC_VERSION 1
#define VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME "VK_KHR_external_semaphore_win32"
struct VkImportSemaphoreWin32HandleInfoKHR { 
    VkStructureType                          sType;
    const void*                              pNext;
    VkSemaphore                              semaphore;
    VkSemaphoreImportFlags                   flags;
    VkExternalSemaphoreHandleTypeFlagBits    handleType;
    HANDLE                                   handle;
    LPCWSTR                                  name;
};

struct VkExportSemaphoreWin32HandleInfoKHR { 
    VkStructureType               sType;
    const void*                   pNext;
    const SECURITY_ATTRIBUTES*    pAttributes;
    DWORD                         dwAccess;
    LPCWSTR                       name;
};

struct VkD3D12FenceSubmitInfoKHR { 
    VkStructureType    sType;
    const void*        pNext;
    uint32_t           waitSemaphoreValuesCount;
    const uint64_t*    pWaitSemaphoreValues;
    uint32_t           signalSemaphoreValuesCount;
    const uint64_t*    pSignalSemaphoreValues;
};

struct VkSemaphoreGetWin32HandleInfoKHR { 
    VkStructureType                          sType;
    const void*                              pNext;
    VkSemaphore                              semaphore;
    VkExternalSemaphoreHandleTypeFlagBits    handleType;
};

typedef VkResult (VKAPI_PTR *PFN_vkImportSemaphoreWin32HandleKHR)(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);

typedef VkResult (VKAPI_PTR *PFN_vkGetSemaphoreWin32HandleKHR)(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

#define VK_KHR_external_fence_win32 1
#define VK_KHR_EXTERNAL_FENCE_WIN32_SPEC_VERSION 1
#define VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME "VK_KHR_external_fence_win32"
struct VkImportFenceWin32HandleInfoKHR { 
    VkStructureType                      sType;
    const void*                          pNext;
    VkFence                              fence;
    VkFenceImportFlags                   flags;
    VkExternalFenceHandleTypeFlagBits    handleType;
    HANDLE                               handle;
    LPCWSTR                              name;
};

struct VkExportFenceWin32HandleInfoKHR { 
    VkStructureType               sType;
    const void*                   pNext;
    const SECURITY_ATTRIBUTES*    pAttributes;
    DWORD                         dwAccess;
    LPCWSTR                       name;
};

struct VkFenceGetWin32HandleInfoKHR { 
    VkStructureType                      sType;
    const void*                          pNext;
    VkFence                              fence;
    VkExternalFenceHandleTypeFlagBits    handleType;
};

typedef VkResult (VKAPI_PTR *PFN_vkImportFenceWin32HandleKHR)(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);

typedef VkResult (VKAPI_PTR *PFN_vkGetFenceWin32HandleKHR)(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

#define VK_NV_external_memory_win32 1
#define VK_NV_EXTERNAL_MEMORY_WIN32_SPEC_VERSION 1
#define VK_NV_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME "VK_NV_external_memory_win32"
struct VkImportMemoryWin32HandleInfoNV { 
    VkStructureType                      sType;
    const void*                          pNext;
    VkExternalMemoryHandleTypeFlagsNV    handleType;
    HANDLE                               handle;
};

struct VkExportMemoryWin32HandleInfoNV { 
    VkStructureType               sType;
    const void*                   pNext;
    const SECURITY_ATTRIBUTES*    pAttributes;
    DWORD                         dwAccess;
};

typedef VkResult (VKAPI_PTR *PFN_vkGetMemoryWin32HandleNV)(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);

#define VK_NV_win32_keyed_mutex 1
#define VK_NV_WIN32_KEYED_MUTEX_SPEC_VERSION 1
#define VK_NV_WIN32_KEYED_MUTEX_EXTENSION_NAME "VK_NV_win32_keyed_mutex"
struct VkWin32KeyedMutexAcquireReleaseInfoNV { 
    VkStructureType          sType;
    const void*              pNext;
    uint32_t                 acquireCount;
    const VkDeviceMemory*    pAcquireSyncs;
    const uint64_t*          pAcquireKeys;
    const uint32_t*          pAcquireTimeoutMilliseconds;
    uint32_t                 releaseCount;
    const VkDeviceMemory*    pReleaseSyncs;
    const uint64_t*          pReleaseKeys;
};

#endif


#define VK1_WIN32_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( [[nodiscard]] VkResult, vkCreateWin32SurfaceKHR, ( VkInstance instance , const VkWin32SurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkBool32, vkGetPhysicalDeviceWin32PresentationSupportKHR, ( VkPhysicalDevice physicalDevice , uint32_t queueFamilyIndex ) , (VkBool32)(0) ) \
	_buildFunc_( [[nodiscard]] VkResult, vkGetMemoryWin32HandleKHR, ( VkDevice device , const VkMemoryGetWin32HandleInfoKHR * pGetWin32HandleInfo , HANDLE * pHandle ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( [[nodiscard]] VkResult, vkGetMemoryWin32HandlePropertiesKHR, ( VkDevice device , VkExternalMemoryHandleTypeFlagBits handleType , HANDLE handle , VkMemoryWin32HandlePropertiesKHR * pMemoryWin32HandleProperties ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( [[nodiscard]] VkResult, vkImportSemaphoreWin32HandleKHR, ( VkDevice device , const VkImportSemaphoreWin32HandleInfoKHR * pImportSemaphoreWin32HandleInfo ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( [[nodiscard]] VkResult, vkGetSemaphoreWin32HandleKHR, ( VkDevice device , const VkSemaphoreGetWin32HandleInfoKHR * pGetWin32HandleInfo , HANDLE * pHandle ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( [[nodiscard]] VkResult, vkImportFenceWin32HandleKHR, ( VkDevice device , const VkImportFenceWin32HandleInfoKHR * pImportFenceWin32HandleInfo ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( [[nodiscard]] VkResult, vkGetFenceWin32HandleKHR, ( VkDevice device , const VkFenceGetWin32HandleInfoKHR * pGetWin32HandleInfo , HANDLE * pHandle ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( [[nodiscard]] VkResult, vkGetMemoryWin32HandleNV, ( VkDevice device , VkDeviceMemory memory , VkExternalMemoryHandleTypeFlagsNV handleType , HANDLE * pHandle ) , VK_RESULT_MAX_ENUM ) \


