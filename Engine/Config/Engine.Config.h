// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once


// Options //

#ifdef __GX_DEBUG__
//#	define GX_ENABLE_PROFILING
//#	define GX_ENABLE_DEBUGGING
#endif

//#define GX_USE_LODEPNG


// Graphics
#define GRAPHICS_API_OPENGL			440		// 4.x		graphics & compute
#define GRAPHICS_API_VULKAN			100		// 1.x		graphics & compute
//#define GRAPHICS_API_OPENGLES		310		// 3.x		// not supported
//#define GRAPHICS_API_DIRECTX		110		// 11.x		// not supported
//#define COMPUTE_API_OPENCL		120		// 1.x or 2.x
//#define COMPUTE_API_CUDA			7000	// --		// not supported
//#define GRAPHICS_API_SOFT			100


// VR
//#define PLATFORM_OCULUS_VR		100
//#define GX_EMULATOR_VR			100

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
		// Engine.Base
		static const int	Base_MaxSubSystems					= 64;

		// Engine.Graphics
		static const int	Graphics_MaxColorBuffers			= 8;
		static const int	Graphics_MaxAttribs					= 16;
		static const int	Graphics_MaxViewports				= 16;
		static const int	Graphics_MaxTextureSize				= 1<<14;
		static const int	Graphics_MaxRenderPassSubpasses		= 8;

		// Engine.Compute
		//static const int	Compute_MaxTextureSize				= 1<<14;

		// Engine.Scene.Renderer
		//static const int	Graphics_MaxMaterialTextures		= 8;
		//static const int	Graphics_MaxMaterialUniforms		= 16;
		//static const int	Graphics_MaxRenderPassTextures		= 6;	// render pass input textures
		//static const int	Graphics_MaxModelDrawGroups			= 32;
		//static const int	Graphics_MaxRenderPassDrawGroups	= 32;
		//static const int	Graphics_MaxUniformBlocks			= 6;
		//static const int	Graphics_MaxUniformBlockSizeKb		= 128;
		//static const int	Graphics_MaxUniformNameLength		= 32;
		//static const int	Graphics_MaterialCompatibilityBits	= 32;
	};

}	// GX_STL
