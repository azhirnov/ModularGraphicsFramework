// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/OS/Display.h"
#include "Engine/Platforms/Public/OS/IDs.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// Platform Create Info
	//

	struct Platform
	{
	// types
		struct EPlatformFlags
		{
			enum type : uint
			{
				FullscreenOnly	= 0,		// all windows creates as fullscreen
				ChangeDisplayMode,
				_Count,
			};

			GX_ENUM_BITFIELD( EPlatformFlags );
		};


	// variables
		GXMath::uint2			resolution;
		uint					frequency	= 0;
		EPlatformFlags::bits	flags;


	// methods
		Platform ()
		{}
	};

}	// CreateInfo


namespace OSMsg
{

	//
	// Get Displays
	//
	struct GetDisplays
	{
		using Displays_t = FixedSizeArray< Platforms::Display, GlobalConst::OS_MaxDisplays >;

		Out< Displays_t >	result;
	};


	//
	// Get OS Modules
	//
	struct GetOSModules
	{
		Out< Platforms::OSModuleIDs >	result;
	};


}	// OSMsg
}	// Engine
