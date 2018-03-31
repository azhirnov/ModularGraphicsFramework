// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "vulkan1_utils.h"
#include "vulkan1_platform.h"

#ifdef GRAPHICS_API_VULKAN
	
namespace vk
{

# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable : 4100)	// 'identifier' : unreferenced formal parameter
# endif

	VK1_OS_FUNCTIONS( VULKAN1_DEFFUNC )
	VK1_OS_FUNCTIONS( VULKAN1_INITFUNC )
	
# ifdef COMPILER_MSVC
#	pragma warning (pop)
# endif

/*
=================================================
	Vk1_PlatformInit
=================================================
*/
	bool Vk1_PlatformInit (VkInstance instance)
	{
		using namespace Engine;

		int	missing = 0;	// number of missing function addresses
		int	counter = 0;	// number of all functions
		
		CHECK_ERR( instance != null );
		CHECK_ERR( Vk1_LoadLibrary() );
		
		VK1_OS_FUNCTIONS( VULKAN1_GETFUNC )

		/*if ( missing > 0 )
		{
			LOG( ("While get address of Vulkan functions: "_str <<
				 missing << " missing, " << (counter - missing) << " successfully.").cstr(),
				 ELog::Debug );
		}*/
		
		return true;
	}
	
/*
=================================================
	Vk1_PlatformDelete
=================================================
*/
	void Vk1_PlatformDelete ()
	{
		VK1_OS_FUNCTIONS( VULKAN1_ZEROFUNC )
		
		//Vk1_UnloadLibrary();
	}

}	// vk

#endif	// GRAPHICS_API_VULKAN
