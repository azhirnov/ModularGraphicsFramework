// Generated header file

#pragma once

#ifndef VULKAN_ANDROID_H_
#define VULKAN_ANDROID_H_ 1
#define VK_KHR_android_surface 1
#define VK_KHR_ANDROID_SURFACE_SPEC_VERSION 6
#define VK_KHR_ANDROID_SURFACE_EXTENSION_NAME "VK_KHR_android_surface"
typedef VkFlags VkAndroidSurfaceCreateFlagsKHR;
struct VkAndroidSurfaceCreateInfoKHR { 
    VkStructureType                   sType;
    const void*                       pNext;
    VkAndroidSurfaceCreateFlagsKHR    flags;
    struct ANativeWindow*             window;
};

typedef VkResult (VKAPI_PTR *PFN_vkCreateAndroidSurfaceKHR)(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

#define VK_ANDROID_external_memory_android_hardware_buffer 1
#define VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION 3
#define VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME "VK_ANDROID_external_memory_android_hardware_buffer"
struct VkAndroidHardwareBufferUsageANDROID { 
    VkStructureType    sType;
    void*              pNext;
    uint64_t           androidHardwareBufferUsage;
};

struct VkAndroidHardwareBufferPropertiesANDROID { 
    VkStructureType    sType;
    void*              pNext;
    VkDeviceSize       allocationSize;
    uint32_t           memoryTypeBits;
};

struct VkAndroidHardwareBufferFormatPropertiesANDROID { 
    VkStructureType                  sType;
    void*                            pNext;
    VkFormat                         format;
    uint64_t                         externalFormat;
    VkFormatFeatureFlags             formatFeatures;
    VkComponentMapping               samplerYcbcrConversionComponents;
    VkSamplerYcbcrModelConversion    suggestedYcbcrModel;
    VkSamplerYcbcrRange              suggestedYcbcrRange;
    VkChromaLocation                 suggestedXChromaOffset;
    VkChromaLocation                 suggestedYChromaOffset;
};

struct VkImportAndroidHardwareBufferInfoANDROID { 
    VkStructureType            sType;
    const void*                pNext;
    struct AHardwareBuffer*    buffer;
};

struct VkMemoryGetAndroidHardwareBufferInfoANDROID { 
    VkStructureType    sType;
    const void*        pNext;
    VkDeviceMemory     memory;
};

struct VkExternalFormatANDROID { 
    VkStructureType    sType;
    void*              pNext;
    uint64_t           externalFormat;
};

typedef VkResult (VKAPI_PTR *PFN_vkGetAndroidHardwareBufferPropertiesANDROID)(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties);

typedef VkResult (VKAPI_PTR *PFN_vkGetMemoryAndroidHardwareBufferANDROID)(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer);

#endif


#define VK1_OS_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( VkResult, vkCreateAndroidSurfaceKHR, ( VkInstance instance , const VkAndroidSurfaceCreateInfoKHR * pCreateInfo , const VkAllocationCallbacks * pAllocator , VkSurfaceKHR * pSurface ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkResult, vkGetAndroidHardwareBufferPropertiesANDROID, ( VkDevice device , const struct AHardwareBuffer * buffer , VkAndroidHardwareBufferPropertiesANDROID * pProperties ) , VK_RESULT_MAX_ENUM ) \
	_buildFunc_( VkResult, vkGetMemoryAndroidHardwareBufferANDROID, ( VkDevice device , const VkMemoryGetAndroidHardwareBufferInfoANDROID * pInfo , struct AHardwareBuffer * * pBuffer ) , VK_RESULT_MAX_ENUM ) \


