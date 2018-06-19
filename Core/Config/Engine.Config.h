// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once


// Options //

#ifdef __GX_DEBUG__
//#	define GX_ENABLE_PROFILING
//#	define GX_ENABLE_DEBUGGING
#endif

/*
// Graphics
#define GRAPHICS_API_OPENGL			450		// 4.x		graphics & compute
#define GRAPHICS_API_VULKAN			100		// 1.x		graphics & compute
//#define GRAPHICS_API_OPENGLES		310		// 3.x		// not supported
//#define GRAPHICS_API_DIRECTX		110		// 11.x		// not supported
#define COMPUTE_API_OPENCL			120		// 1.x or 2.x
//#define COMPUTE_API_CUDA			7000	// --		// not supported
#define GRAPHICS_API_SOFT			100		//			compute
*/

// VR
//#define PLATFORM_OCULUS_VR		100
#define GX_EMULATOR_VR				100

//-------------------------------------------------------------------



// Error Checks //

// Graphics API error checks
#if !defined( __GX_FAST__ )
#	define __GX_GRAPHICS_API_ERROR_CHECKS__
#endif

// Compute API error checks, only for OpenCL, CUDA
#if !defined( __GX_FAST__ )
#	define __GX_COMPUTE_API_ERROR_CHECKS__
#endif

//-------------------------------------------------------------------



// Constants //

namespace GX_STL
{
	//
	// Global Constants
	//

	namespace GlobalConst
	{
		// Engine.Platforms
		static constexpr int	GAPI_MaxColorBuffers			= 8;
		static constexpr int	GAPI_MaxAttribs					= 16;
		static constexpr int	GAPI_MaxViewports				= 16;
		//static constexpr int	GAPI_MaxTextureSize				= 1<<14;
		static constexpr int	GAPI_MaxRenderPassSubpasses		= 8;
		static constexpr int	GAPI_MaxSpecializationConstants	= 16;
		static constexpr int	GAPI_MaxPushConstants			= 32;
		static constexpr int	GAPI_MaxCopyRegions				= 8;
		static constexpr int	GAPI_MaxBlitRegions				= 8;
		static constexpr int	GAPI_MaxClearRects				= 16;
		static constexpr int	GAPI_MaxWaitEvents				= 8;
		static constexpr int	GAPI_MaxWaitFence				= 8;
		static constexpr int	GAPI_MaxWaitSemaphores			= 8;

		static constexpr int	OS_MaxDisplays					= 8;

		// Engine.FrameGraph
		static constexpr int	FG_MaxTaskDependencies			= 8;
	};

}	// GX_STL
