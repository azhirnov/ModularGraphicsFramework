// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Android/VkAndSurface.h"

#if defined( GRAPHICS_API_VULKAN ) and defined( PLATFORM_ANDROID )

#include <android/native_window.h>
#include "Engine/Platforms/Vulkan/Impl/vulkan1_platform.h"

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;
	
/*
=================================================
	constructor
=================================================
*/
	VkAndSurface::VkAndSurface ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	VkAndSurface::~VkAndSurface ()
	{
		Destroy();
	}
	
/*
=================================================
	Create
=================================================
*/
	bool VkAndSurface::Create (VkInstance instance, const AWindow_t &windowHandle, OUT VkSurfaceKHR &surface)
	{
		CHECK_ERR( instance != VK_NULL_HANDLE );
		CHECK_ERR( windowHandle.IsNotNull<ANativeWindow*>() );
		
		CHECK_ERR( Vk1_PlatformInit( instance ) );

		VkAndroidSurfaceCreateInfoKHR		surface_info = {};

		surface_info.sType		= VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
		surface_info.flags		= 0;
		surface_info.window		= windowHandle.Get<ANativeWindow*>();
		
		VK_CHECK( vkCreateAndroidSurfaceKHR( instance, &surface_info, null, OUT &surface ) );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void VkAndSurface::Destroy ()
	{
		Vk1_PlatformDelete();
	}
	
/*
=================================================
	RequestExtensions
=================================================
*/
	void VkAndSurface::RequestExtensions (INOUT Array<const char*> &ext)
	{
		ext << VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN and PLATFORM_ANDROID
