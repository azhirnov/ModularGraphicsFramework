// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "vulkan1.h"

#ifdef GRAPHICS_API_VULKAN

#ifdef PLATFORM_WINDOWS
#	include "Core/STL/OS/Windows/WinHeader.h"
#endif

#ifdef PLATFORM_ANDROID
#	include <android/native_window.h>
#endif

namespace vk
{
	
# ifdef PLATFORM_WINDOWS
#	define VK_USE_PLATFORM_WIN32_KHR
#	define VK_USE_PLATFORM_WIN32_KHX
#	define VK1_OS_FUNCTIONS		VK1_WIN32_FUNCTIONS
#	include "External/vulkan/gen_vulkan_1_win.h"
# endif

# ifdef PLATFORM_ANDROID
# 	define VK_USE_PLATFORM_ANDROID_KHR
#	define VK1_OS_FUNCTIONS		VK1_ANDROID_FUNCTIONS
# 	include "External/vulkan/gen_vulkan_1_and.h"
# endif

# ifdef PLATFORM_LINUX
#  ifdef VK_USE_PLATFORM_XLIB_KHR
#	define VK1_OS_FUNCTIONS		VK1_XLIB_FUNCTIONS
#	include "External/vulkan/gen_vulkan_1_xlib.h"

#  elif defined VK_USE_PLATFORM_XCB_KHR
#	define VK1_OS_FUNCTIONS		VK1_XCB_FUNCTIONS
#	include "External/vulkan/gen_vulkan_1_xcb.h"

#  elif defined VK_USE_PLATFORM_WAYLAND_KHR
#	define VK1_OS_FUNCTIONS		VK1_WAYLAND_FUNCTIONS
# 	include "External/vulkan/gen_vulkan_1_wayland.h"

#  elif defined VK_USE_PLATFORM_MIR_KHR
#	define VK1_OS_FUNCTIONS		VK1_MIR_FUNCTIONS
# 	include "External/vulkan/gen_vulkan_1_mir.h"
#  else
#	error unknown window manager
#  endif
# endif
		

	// pointers to functions //
# define VULKAN1_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		extern PFN_##_funcName_		_funcName_;

	VK1_OS_FUNCTIONS( VULKAN1_BUILDFUNC )

# undef VULKAN1_BUILDFUNC

		
	// API
	bool Vk1_PlatformInit (VkInstance instance, VkDevice device);
	void Vk1_PlatformDelete ();

}	// vk

#endif	// GRAPHICS_API_VULKAN
