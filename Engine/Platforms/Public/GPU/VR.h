// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// GPU VR Thread Create Info
	//
	struct VRThread
	{
	// variables
		ModulePtr						gpuThread;					// can be null
		Platforms::GraphicsSettings		settings;
		GXMath::uint2					eyeTextureDimension;		// dimension of per eye textures
		bool							layered;					// create texture array or use multiview extension, depended of implementation

	// methods
		VRThread  (const Platforms::GraphicsSettings &settings,
				   const GXMath::uint2 &texDimension = Uninitialized,
				   bool layered = false) :
			settings{ settings },
			eyeTextureDimension{ texDimension },
			layered{ layered }
		{}

		VRThread  (const ModulePtr &gpuThread,
				   const GXMath::uint2 &texDimension,
				   bool layered = false) :
			gpuThread{ gpuThread },
			eyeTextureDimension{ texDimension },
			layered{ layered }
		{}
	};


	//
	// GPU VR Framebuffer Create Info
	//
	/*struct VRFramebuffer
	{
	// types
		enum class Eye {
			Left	= 0,
			Right	= 1,
		};

	// variables
		ModulePtr		gpuThread;
		GXMath::uint2	dimension;
		Eye				eye;

	// methods
		VRFramebuffer () : eye{Eye{-1}} {}
		VRFramebuffer (const GXMath::uint2 &dim, Eye eye) : dimension(dim), eye(eye) {}
		VRFramebuffer (const ModulePtr &gpuThread, const GXMath::uint2 &dim, Eye eye) : gpuThread(gpuThread), dimension(dim), eye(eye) {}
	};*/


}	// CreateInfo


namespace GpuMsg
{

	//
	// Get VR Device Info
	//
	struct GetVRDeviceInfo
	{
		struct Info {
			ModulePtr	renderPass;			// this is default render pass used for on screen rendering
			uint		imageCount;			// count of images in swapchain
		};

		Out< Info >		result;
	};


	//
	// Thread Begin / End VR Frame
	//
	struct ThreadBeginVRFrame
	{
		struct PerEye : CompileTime::FastCopyable
		{
		// variables
			ModulePtr	framebuffer;		// current framebuffer for eye
			uint		layer	= 0;		// (optional) framebuffer layer, only for texture array
			float4x4	viewMat;
			float4x4	projMat;

		// methods
			PerEye () {}

			PerEye (const ModulePtr &fb, const float4x4 &view, const float4x4 &proj, uint layer = 0) :
				framebuffer(fb), layer(layer), viewMat(view), projMat(proj)
			{}
		};

		struct Data
		{
			PerEye		leftEye;
			PerEye		rightEye;
			uint		frameIindex	= 0;	// index of image in swapchain
		};

		Out< Data >		result;
	};

	struct ThreadEndVRFrame : SubmitGraphicsQueueCommands
	{};

	
}	// GpuMsg
}	// Engine
