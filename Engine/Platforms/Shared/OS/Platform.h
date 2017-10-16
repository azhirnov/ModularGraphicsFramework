// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/IDs.h"

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
		uint					frequency;
		EPlatformFlags::bits	flags;


	// methods
		Platform ()
		{}
	};

}	// CreateInfo


namespace ModuleMsg
{
	
	// platform-dependent
	struct PlatformCreated;


	//
	// Request Display Params Message
	//
	struct RequestDisplayParams
	{
		Out< Platforms::Display const *>	result;
	};


}	// ModuleMsg
}	// Engine
