// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"

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
	};

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
		struct PerEye {
			ModulePtr	framebuffer;		// current framebuffer for eye
			uint		layer	= 0;		// (optional) framebuffer layer, only for texture array
		};

		struct Data {
			PerEye		leftEye;
			PerEye		rightEye;
			uint		index	= 0;		// index of image in swapchain
		};

		Out< Data >		result;
	};

	struct ThreadEndVRFrame
	{
	// types
		using Commands_t	= ThreadEndFrame::Commands_t;

	// variables
		Commands_t		commands;		// (optional) commands to submit before present frame
	};


}	// GpuMsg
}	// Engine
