// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Platforms/Common/Common.h"


#if defined( PLATFORM_WINDOWS )

#	include "Engine/Platforms/Windows/WinPlatform.h"
#	include "Engine/Platforms/Windows/WinWindow.h"
#	include "Engine/Platforms/Windows/WinKeyInput.h"
#	include "Engine/Platforms/Windows/WinMouseInput.h"

	using OSPlatform = Engine::Platforms::WinPlatform;

#endif	// PLATFORM_WINDOWS


#ifdef GRAPHICS_API_OPENGL

#endif	// GRAPHICS_API_OPENGL


#ifdef GRAPHICS_API_VULKAN

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

		OSPlatform::_RegisterAll( ms->GlobalSystems() );
		InputManager::_RegisterAll( ms->GlobalSystems() );
		
		#ifdef GRAPHICS_API_OPENGL
			OpenGLContext::_RegisterAll( ms->GlobalSystems() );
		#endif

		#ifdef GRAPHICS_API_VULKAN
			VulkanContext::_RegisterAll( ms->GlobalSystems() );
		#endif

		#ifdef COMPUTE_API_OPENCL
			OpenCLContext::_RegisterAll( ms->GlobalSystems() );
		#endif

	}

}	// Platforms
}	// Engine
