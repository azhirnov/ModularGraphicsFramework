// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Common/Common.h"


#if defined( PLATFORM_WINDOWS )

#	include "Engine/Platforms/Windows/WinPlatform.h"
#	include "Engine/Platforms/Windows/WinWindow.h"
#	include "Engine/Platforms/Windows/WinKeyInput.h"
#	include "Engine/Platforms/Windows/WinMouseInput.h"

	using OSPlatform = Engine::Platforms::WinPlatform;

#endif	// PLATFORM_WINDOWS


#ifdef GRAPHICS_API_OPENGL

#	include "Engine/Platforms/OpenGL/OpenGLContext.h"

#endif	// GRAPHICS_API_OPENGL


#ifdef GRAPHICS_API_VULKAN

#	include "Engine/Platforms/Vulkan/VulkanContext.h"

#endif	// GRAPHICS_API_VULKAN


#ifdef COMPUTE_API_OPENCL

#endif	// COMPUTE_API_OPENCL


#include "Engine/Platforms/Input/InputManager.h"


namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	RegisterPlatforms
=================================================
*/
	void RegisterPlatforms ()
	{
		auto	ms	= GetMainSystemInstace();

		OSPlatform::Register( ms->GlobalSystems() );
		InputManager::Register( ms->GlobalSystems() );
		
		#ifdef GRAPHICS_API_OPENGL
			OpenGLContext::Register( ms->GlobalSystems() );
		#endif

		#ifdef GRAPHICS_API_VULKAN
			VulkanContext::Register( ms->GlobalSystems() );
		#endif

		#ifdef COMPUTE_API_OPENCL
			OpenCLContext::Register( ms->GlobalSystems() );
		#endif
	}
	
/*
=================================================
	UnregisterPlatforms
=================================================
*/
	void UnregisterPlatforms ()
	{
		auto	ms	= GetMainSystemInstace();

		OSPlatform::Unregister( ms->GlobalSystems() );
		InputManager::Unregister( ms->GlobalSystems() );
		
		#ifdef GRAPHICS_API_OPENGL
			OpenGLContext::Unregister( ms->GlobalSystems() );
		#endif

		#ifdef GRAPHICS_API_VULKAN
			VulkanContext::Unregister( ms->GlobalSystems() );
		#endif

		#ifdef COMPUTE_API_OPENCL
			OpenCLContext::Unregister( ms->GlobalSystems() );
		#endif
	}


}	// Platforms
}	// Engine
