// Generated header file

#pragma once

#ifdef VK_USE_PLATFORM_WIN32_KHR
#define VK_KHR_win32_surface 1
#define VK_KHR_WIN32_SURFACE_SPEC_VERSION 5
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

#endif /* VK_USE_PLATFORM_WIN32_KHR */
#ifdef VK_USE_PLATFORM_WIN32_KHR
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

#endif /* VK_USE_PLATFORM_WIN32_KHR */
#ifdef VK_USE_PLATFORM_WIN32_KHR
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

#endif /* VK_USE_PLATFORM_WIN32_KHR */
#ifdef VK_USE_PLATFORM_WIN32_KHX
#define VK_KHX_external_memory_win32 1
#define VK_KHX_EXTERNAL_MEMORY_WIN32_SPEC_VERSION 1
#define VK_KHX_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME "VK_KHX_external_memory_win32"
struct VkImportMemoryWin32HandleInfoKHX { 
    VkStructureType                          sType;
    const void*                              pNext;
    VkExternalMemoryHandleTypeFlagBitsKHX    handleType;
    HANDLE                                   handle;
};

struct VkExportMemoryWin32HandleInfoKHX { 
    VkStructureType               sType;
    const void*                   pNext;
    const SECURITY_ATTRIBUTES*    pAttributes;
    DWORD                         dwAccess;
    LPCWSTR                       name;
};

struct VkMemoryWin32HandlePropertiesKHX { 
    VkStructureType    sType;
    void*              pNext;
    uint32_t           memoryTypeBits;
};

typedef VkResult (VKAPI_PTR *PFN_vkGetMemoryWin32HandleKHX)(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagBitsKHX handleType, HANDLE* pHandle);

typedef VkResult (VKAPI_PTR *PFN_vkGetMemoryWin32HandlePropertiesKHX)(VkDevice device, VkExternalMemoryHandleTypeFlagBitsKHX handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHX* pMemoryWin32HandleProperties);

#endif /* VK_USE_PLATFORM_WIN32_KHX */
#ifdef VK_USE_PLATFORM_WIN32_KHR
#define VK_KHX_win32_keyed_mutex 1
#define VK_KHX_WIN32_KEYED_MUTEX_SPEC_VERSION 1
#define VK_KHX_WIN32_KEYED_MUTEX_EXTENSION_NAME "VK_KHX_win32_keyed_mutex"
struct VkWin32KeyedMutexAcquireReleaseInfoKHX { 
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

#endif /* VK_USE_PLATFORM_WIN32_KHR */
#ifdef VK_USE_PLATFORM_WIN32_KHX
#define VK_KHX_external_semaphore_win32 1
#define VK_KHX_EXTERNAL_SEMAPHORE_WIN32_SPEC_VERSION 1
#define VK_KHX_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME "VK_KHX_external_semaphore_win32"
struct VkImportSemaphoreWin32HandleInfoKHX { 
    VkStructureType                          sType;
    const void*                              pNext;
    VkSemaphore                              semaphore;
    VkExternalSemaphoreHandleTypeFlagsKHX    handleType;
    HANDLE                                   handle;
};

struct VkExportSemaphoreWin32HandleInfoKHX { 
    VkStructureType               sType;
    const void*                   pNext;
    const SECURITY_ATTRIBUTES*    pAttributes;
    DWORD                         dwAccess;
    LPCWSTR                       name;
};

struct VkD3D12FenceSubmitInfoKHX { 
    VkStructureType    sType;
    const void*        pNext;
    uint32_t           waitSemaphoreValuesCount;
    const uint64_t*    pWaitSemaphoreValues;
    uint32_t           signalSemaphoreValuesCount;
    const uint64_t*    pSignalSemaphoreValues;
};

typedef VkResult (VKAPI_PTR *PFN_vkImportSemaphoreWin32HandleKHX)(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHX* pImportSemaphoreWin32HandleInfo);

typedef VkResult (VKAPI_PTR *PFN_vkGetSemaphoreWin32HandleKHX)(VkDevice device, VkSemaphore semaphore, VkExternalSemaphoreHandleTypeFlagBitsKHX handleType, HANDLE* pHandle);

#endif /* VK_USE_PLATFORM_WIN32_KHX */


#define VK1_OS_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( VkResult, vkCreateWin32SurfaceKHR, ( VkInstance instance , const VkWin32SurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkBool32, vkGetPhysicalDeviceWin32PresentationSupportKHR, ( VkPhysicalDevice physicalDevice , uint32_t queueFamilyIndex ) , (VkBool32)(0) ) \
	_buildFunc_( VkResult, vkGetMemoryWin32HandleNV, ( VkDevice device , VkDeviceMemory memory , VkExternalMemoryHandleTypeFlagsNV handleType , HANDLE * pHandle ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkResult, vkGetMemoryWin32HandleKHX, ( VkDevice device , VkDeviceMemory memory , VkExternalMemoryHandleTypeFlagBitsKHX handleType , HANDLE * pHandle ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkResult, vkGetMemoryWin32HandlePropertiesKHX, ( VkDevice device , VkExternalMemoryHandleTypeFlagBitsKHX handleType , HANDLE handle , VkMemoryWin32HandlePropertiesKHX * pMemoryWin32HandleProperties ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkResult, vkImportSemaphoreWin32HandleKHX, ( VkDevice device , const VkImportSemaphoreWin32HandleInfoKHX * pImportSemaphoreWin32HandleInfo ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkResult, vkGetSemaphoreWin32HandleKHX, ( VkDevice device , VkSemaphore semaphore , VkExternalSemaphoreHandleTypeFlagBitsKHX handleType , HANDLE * pHandle ) , VK_RESULT_MAX_ENUM ) \


