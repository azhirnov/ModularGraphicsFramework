// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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


	//
	// Compute Settings
	//
	struct ComputeSettings
	{
	// variables
		String			device;
		GAPI::type		version;
		bool			isDebug;

	// methods
		explicit
		ComputeSettings (GAPI::type	version		= GAPI::type(0),
						 StringCRef	deviceName	= StringCRef(),
						 bool		isDebug		= false) :
			device(deviceName), version(version), isDebug(isDebug)
		{}
	};


	//
	// Graphics Settings
	//
	struct GraphicsSettings
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


	// variables
		String				device;
		GAPI::type			version;
		EFlags::bits		flags;

		EPixelFormat::type	colorFmt;
		EPixelFormat::type	depthStencilFmt;

		MultiSamples		samples;
		uint				swapchainLength;


	// methods
		explicit
		GraphicsSettings (GAPI::type			version			= GAPI::type(0),
						  EFlags::bits			flags			= EFlags::bits(),
						  EPixelFormat::type	colorFmt		= EPixelFormat::RGBA8_UNorm,
						  EPixelFormat::type	depthStencilFmt	= EPixelFormat::Depth24,
						  MultiSamples			samples			= MultiSamples(),
						  StringCRef			deviceName		= StringCRef(),
						  uint					swapchainLength	= 0) :
			device{ deviceName },	version{ version },		flags{ flags },
			colorFmt{ colorFmt },	depthStencilFmt{ depthStencilFmt },
			samples{ samples },		swapchainLength{ swapchainLength }
		{}
	};


}	// Platforms

namespace CreateInfo
{

	//
	// GPU Context Create Info
	//
	struct GpuContext
	{
	// types
		using GAPI		= Platforms::GAPI;
		using EFlags	= Platforms::GraphicsSettings::EFlags;

	// variables
		Platforms::GraphicsSettings		settings;	// this is default settings for all threads

	// methods
		explicit GpuContext (GAPI::type version = GAPI::type(0)) : settings{ version } {}
		explicit GpuContext (const Platforms::GraphicsSettings &settings) : settings{ settings } {}

		explicit GpuContext (const Platforms::ComputeSettings &settings) :
			settings{ settings.version, EFlags::bits() | (settings.isDebug ? EFlags::DebugContext : EFlags::type(0)),
					  Platforms::EPixelFormat::Unknown, Platforms::EPixelFormat::Unknown,
					  Platforms::MultiSamples(), settings.device, 0 }
		{}
	};

}	// CreateInfo
}	// Engine
