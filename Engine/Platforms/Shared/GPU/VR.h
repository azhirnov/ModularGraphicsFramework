// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Thread.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// GPU VR Context Create Info
	//
	struct VRContext : GpuContext
	{
	// variables
		GXMath::uint2	eyeTextureDimension;		// dimension of per eye textures
		bool			layered;					// create texture array or use multiview extension, depended of implementation

	// methods
		VRContext  (GXMath::uint2		texDimension,
				    bool				layered			= false,
					GAPI::type			version			= GAPI::type(0),
					EFlags::bits		flags			= EFlags::bits(),
					EPixelFormat::type	colorFmt		= EPixelFormat::RGBA8_UNorm,
					EPixelFormat::type	depthStencilFmt	= EPixelFormat::Depth24,
					MultiSamples		samples			= MultiSamples(),
					StringCRef			deviceName		= StringCRef(),
					uint				swapchainLength	= 0) :
			GpuContext{ version, flags, colorFmt, depthStencilFmt,
						samples, deviceName, swapchainLength },
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
			ModulePtr	commandBuilder;		// this builder destroys all command buffer before resize swapchain		// TODO: remove
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
