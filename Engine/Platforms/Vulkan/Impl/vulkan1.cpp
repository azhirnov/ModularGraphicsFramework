// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "vulkan1.h"
#include "vulkan1_utils.h"

#if defined( GRAPHICS_API_VULKAN )
	
namespace vk
{

#	pragma warning (push)
#	pragma warning (disable : 4100)	// 'identifier' : unreferenced formal parameter

	VK1_CORE_FUNCTIONS( VULKAN1_DEFFUNC )
	VK1_CORE_FUNCTIONS( VULKAN1_INITFUNC )
	
#	pragma warning (pop)

/*
=================================================
	Vk1_Init
=================================================
*/
	bool Vk1_Init (VkInstance instance)
	{
		using namespace Engine;

		int	missing = 0;	// number of missing function addresses
		int	counter = 0;	// number of all functions

		CHECK_ERR( Vk1_LoadLibrary() );

		if ( instance == VK_NULL_HANDLE )
		{
			VULKAN1_GETFUNC( , vkEnumerateInstanceExtensionProperties, (), );
			VULKAN1_GETFUNC( , vkEnumerateInstanceLayerProperties, (), );
			VULKAN1_GETFUNC( , vkCreateInstance, (), );
			VULKAN1_GETFUNC( , vkGetInstanceProcAddr, (), );
			VULKAN1_GETFUNC( , vkGetDeviceProcAddr, (), );

			return true;
		}
		
		VK1_CORE_FUNCTIONS( VULKAN1_GETFUNC )

		if ( missing > 0 )
		{
			LOG( ("While get address of Vulkan functions: "_str <<
				 missing << " missing, " << (counter - missing) << " successfully.").cstr(),
				 ELog::Debug );
		}
		
		return true;
	}
	
/*
=================================================
	Vk1_Delete
=================================================
*/
	void Vk1_Delete ()
	{
		VK1_CORE_FUNCTIONS( VULKAN1_ZEROFUNC )
		
		Vk1_UnloadLibrary();
	}
	
/*
=================================================
	Vk1_CheckErrors
=================================================
*/
	bool Vk1_CheckErrors (VkResult errCode, const char *vkcall, const char *func, const char *file, int line)
	{
		using namespace Engine;

		if ( errCode == VK_SUCCESS )
			return true;

		#define VK1_CASE_ERR( _code_ ) \
			case _code_ :	msg << TOSTRING( _code_ ); break;
		
		String	msg( "Vulkan error: " );

		switch ( errCode )
		{
			VK1_CASE_ERR( VK_NOT_READY )
			VK1_CASE_ERR( VK_TIMEOUT )
			VK1_CASE_ERR( VK_EVENT_SET )
			VK1_CASE_ERR( VK_EVENT_RESET )
			VK1_CASE_ERR( VK_INCOMPLETE )
			VK1_CASE_ERR( VK_ERROR_OUT_OF_HOST_MEMORY )
			VK1_CASE_ERR( VK_ERROR_OUT_OF_DEVICE_MEMORY )
			VK1_CASE_ERR( VK_ERROR_INITIALIZATION_FAILED )
			VK1_CASE_ERR( VK_ERROR_DEVICE_LOST )
			VK1_CASE_ERR( VK_ERROR_MEMORY_MAP_FAILED )
			VK1_CASE_ERR( VK_ERROR_LAYER_NOT_PRESENT )
			VK1_CASE_ERR( VK_ERROR_EXTENSION_NOT_PRESENT )
			VK1_CASE_ERR( VK_ERROR_FEATURE_NOT_PRESENT )
			VK1_CASE_ERR( VK_ERROR_INCOMPATIBLE_DRIVER )
			VK1_CASE_ERR( VK_ERROR_TOO_MANY_OBJECTS )
			VK1_CASE_ERR( VK_ERROR_FORMAT_NOT_SUPPORTED )
			VK1_CASE_ERR( VK_ERROR_FRAGMENTED_POOL )
			VK1_CASE_ERR( VK_ERROR_SURFACE_LOST_KHR )
			VK1_CASE_ERR( VK_ERROR_NATIVE_WINDOW_IN_USE_KHR )
			VK1_CASE_ERR( VK_SUBOPTIMAL_KHR )
			VK1_CASE_ERR( VK_ERROR_OUT_OF_DATE_KHR )
			VK1_CASE_ERR( VK_ERROR_INCOMPATIBLE_DISPLAY_KHR )
			VK1_CASE_ERR( VK_ERROR_VALIDATION_FAILED_EXT )
			VK1_CASE_ERR( VK_ERROR_INVALID_SHADER_NV )
			VK1_CASE_ERR( VK_ERROR_OUT_OF_POOL_MEMORY_KHR )
			VK1_CASE_ERR( VK_ERROR_INVALID_EXTERNAL_HANDLE_KHX )
			VK1_CASE_ERR( VK_RESULT_MAX_ENUM )
			default :	msg << "unknown (" << int(errCode) << ')';
		}
		
		msg << ", in " << vkcall;
		msg << ", function: " << func;

		LOG( msg.cstr(), ELog::Error, file, line );
		return false;
	}

}	// vk

#endif	// GRAPHICS_API_VULKAN
