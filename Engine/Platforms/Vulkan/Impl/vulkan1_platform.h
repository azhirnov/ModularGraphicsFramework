// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "vulkan1.h"

#ifdef GRAPHICS_API_VULKAN

#ifdef PLATFORM_WINDOWS
#	include "Engine/STL/OS/Windows/WinHeader.h"
#endif

#ifdef PLATFORM_ANDROID
#	include <android/native_window.h>
#endif

namespace vk
{
	
# ifdef PLATFORM_WINDOWS
#	define VK_USE_PLATFORM_WIN32_KHR
#	define VK_USE_PLATFORM_WIN32_KHX
#	include "External/vulkan/gen_vulkan_1_win.h"
# endif

# ifdef PLATFORM_ANDROID
# 	define VK_USE_PLATFORM_ANDROID_KHR
# 	include "External/vulkan/gen_vulkan_1_and.h"
# endif
		

	// pointers to functions //
# define VULKAN1_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		extern PFN_##_funcName_		_funcName_;

	VK1_OS_FUNCTIONS( VULKAN1_BUILDFUNC )

# undef VULKAN1_BUILDFUNC

		
	// API
	bool Vk1_PlatformInit (VkInstance instance);
	void Vk1_PlatformDelete ();

}	// vk

#endif	// GRAPHICS_API_VULKAN
