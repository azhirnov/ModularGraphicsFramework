// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Windows/VkWinSurface.h"

#if defined( GRAPHICS_API_VULKAN ) and defined( PLATFORM_WINDOWS )

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
	VkWinSurface::VkWinSurface ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	VkWinSurface::~VkWinSurface ()
	{
		Destroy();
	}
	
/*
=================================================
	Create
=================================================
*/
	bool VkWinSurface::Create (VkInstance instance, const HWND_t &windowHandle, OUT VkSurfaceKHR &surface)
	{
		CHECK_ERR( instance != VK_NULL_HANDLE );
		CHECK_ERR( windowHandle.IsNotNull<HWND>() );
		
		CHECK_ERR( Vk1_PlatformInit( instance ) );

		VkWin32SurfaceCreateInfoKHR		surface_info = {};

		surface_info.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surface_info.hinstance	= ::GetModuleHandle( (LPCSTR) null );
		surface_info.hwnd		= windowHandle.Get<HWND>();
		
		VK_CHECK( vkCreateWin32SurfaceKHR( instance, &surface_info, null, OUT &surface ) );
		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void VkWinSurface::Destroy ()
	{
		Vk1_PlatformDelete();
	}
	
/*
=================================================
	RequestExtensions
=================================================
*/
	void VkWinSurface::RequestExtensions (INOUT Array<const char*> &ext)
	{
		ext << VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN and PLATFORM_WINDOWS
