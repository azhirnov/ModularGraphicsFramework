// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/MultiSamples.h"
#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Public/GPU/ObjectEnums.h"
#include "Engine/Platforms/Public/GPU/IDs.h"

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

		ND_ String GetAPIName () const;
		ND_ uint   GetAPIVersion () const;
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
				None = 0,
				DebugContext,	// create debug context
				NoErrorContext,	// OpenGL only
				VSync,			// enable vertical synchronization
				NoSurface,		// create context without surface
				_Count
			};

			GX_ENUM_BITFIELD( EFlags );
		};


	// variables
		String				device;
		GAPI::type			version;
		EFlags::bits		flags;

		EPixelFormat::type	colorFmt;			// render surface color format
		EPixelFormat::type	depthStencilFmt;	// render surface depth format

		MultiSamples		samples;			// render surface with multisampling
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

		explicit
		GraphicsSettings (const ComputeSettings &settings) :
			device{ settings.device },		version{ settings.version },
			flags{ (settings.isDebug ? EFlags::DebugContext : EFlags::None) | EFlags::NoSurface },
			colorFmt{ Uninitialized },		depthStencilFmt{ Uninitialized },
			samples{},						swapchainLength{ 0 }
		{}

		ND_ String GetAPIName () const;
		ND_ uint   GetAPIVersion () const;
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
		explicit GpuContext (const Platforms::GraphicsSettings &gsettings) : settings{ gsettings } {}
		explicit GpuContext (const Platforms::ComputeSettings &csettings) : settings{ csettings } {}
	};

}	// CreateInfo
}	// Engine
