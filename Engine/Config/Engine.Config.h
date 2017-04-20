// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once


// Emulators //

//#define GX_ANDROID_EMULATOR	// not supported

//-------------------------------------------------------------------



// Options //

#define GX_ENABLE_PROFILING

//#define GX_AUDIO_IN_SEPARATE_THREAD	// not supported

#define GX_USE_LODEPNG

// Graphics
//#define GRAPHICS_API_OPENGL		450		// 4.x
//#define GRAPHICS_API_VULKAN		100		// 1.x
//#define GRAPHICS_API_OPENGLES		200		// 2.x		// not supported
//#define GRAPHICS_API_DIRECTX		110		// 11.x		// not supported
//#define COMPUTE_API_OPENCL		120		// 1.x or 2.x
//#define COMPUTE_API_CUDA			7000	// --		// not supported

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
