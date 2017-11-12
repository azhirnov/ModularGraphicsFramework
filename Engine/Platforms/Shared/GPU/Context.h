// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/MultiSamples.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Shared/GPU/ObjectEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{
	DECL_STRING_TO_ID( GAPI, 10 );

}	// Platforms

namespace CreateInfo
{

	//
	// GPU Context Create Info
	//

	struct GpuContext
	{
	// types
		struct EFlags
		{
			enum type : uint
			{
				DebugContext,
				NoErrorContext,
				VSync,
				_Count
			};

			GX_ENUM_BITFIELD( EFlags );
		};

		using EPixelFormat	= Platforms::EPixelFormat;
		using MultiSamples	= Platforms::MultiSamples;
		using GAPI			= Platforms::GAPI;


	// variables
		String				device;
		GAPI::type			version;
		EFlags::bits		flags;

		EPixelFormat::type	colorFmt;
		EPixelFormat::type	depthStencilFmt;

		MultiSamples		samples;
		uint				swapchainLength;


	// methods
		GpuContext (GAPI::type			version			= GAPI::type(0),
					EFlags::bits		flags			= EFlags::bits(),
					EPixelFormat::type	colorFmt		= EPixelFormat::RGBA8_UNorm,
					EPixelFormat::type	depthStencilFmt	= EPixelFormat::Depth24,
					MultiSamples		samples			= MultiSamples(),
					StringCRef			deviceName		= StringCRef(),
					uint				swapchainLength	= 0) :
			device{ deviceName },	version{ version },		flags{ flags },
			colorFmt{ colorFmt },	depthStencilFmt{ depthStencilFmt },
			samples{ samples },		swapchainLength{ swapchainLength }
		{}
	};

}	// CreateInfo
}	// Engine
