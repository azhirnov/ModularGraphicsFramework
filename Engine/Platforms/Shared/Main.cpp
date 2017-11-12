// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Common/Common.h"


#if defined( PLATFORM_WINDOWS )
#	include "Engine/Platforms/Windows/WinPlatform.h"
	using OSPlatform = Engine::Platforms::WinPlatform;
#endif


#ifdef GRAPHICS_API_OPENGL
#	include "Engine/Platforms/OpenGL/OpenGLContext.h"
#endif


#ifdef GRAPHICS_API_VULKAN
#	include "Engine/Platforms/Vulkan/VulkanContext.h"
#endif


#ifdef COMPUTE_API_OPENCL
#	include "Engine/Platforms/OpenCL/OpenCLContext.h"
#endif


#ifdef GRAPHICS_API_DIRECTX
#	include "Engine/Platforms/DirectX/DirectXContext.h"
#endif


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

		#ifdef GRAPHICS_API_DIRECTX
			DirectXContext::Register( ms->GlobalSystems() );
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

		#ifdef GRAPHICS_API_DIRECTX
			DirectXContext::Unregister( ms->GlobalSystems() );
		#endif
	}


}	// Platforms
}	// Engine
