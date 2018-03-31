// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Base/Common/Common.h"

namespace vk
{
	
#	include "External/vulkan/vulkan_api_1.h"

	
	// pointers to functions //
#	define VULKAN1_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		extern PFN_##_funcName_		_funcName_;
	
	VK1_CORE_FUNCTIONS( VULKAN1_BUILDFUNC )

#	undef VULKAN1_BUILDFUNC

		
	// API
	bool Vk1_Init (VkInstance instance);
	void Vk1_Delete ();
	
	bool Vk1_CheckErrors (VkResult errCode, const char *vkcall, const char *func, const char *file, int line);
	

	// macro
# if !defined( __GX_GRAPHICS_API_ERROR_CHECKS__ )
	
#	define VK_CALL( ... )		{ __VA_ARGS__; }
#	define VK_CHECK( ... )		{ __VA_ARGS__; }

# else

#	define VK_CALL( ... ) \
	{ \
		const ::vk::VkResult __vk_err__ = (__VA_ARGS__); \
		::vk::Vk1_CheckErrors( __vk_err__, TOSTRING( __VA_ARGS__ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ); \
	}

#	define __VK_CALL_R( _func_, _ret_, ... ) \
	{ \
		const ::vk::VkResult __vk_err__ = (_func_); \
		if ( not ::vk::Vk1_CheckErrors( __vk_err__, TOSTRING( _func_ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ) ) \
			return _ret_; \
	}

	// Warning: different behavior on Debug and Release!
#	define VK_CHECK( ... ) \
		__VK_CALL_R( AUXDEF_GETARG_0( __VA_ARGS__ ), AUXDEF_GETARG_1( __VA_ARGS__, Uninitialized ) )

# endif	// __GX_GRAPHICS_API_ERROR_CHECKS__

}	// vk

#endif	// GRAPHICS_API_VULKAN
