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
		GXMath::uint2			resolution;			// TODO: multidisplay support
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
	struct GetDisplays : _MsgBase_
	{
	// types
		using Displays_t = FixedSizeArray< Platforms::Display, GlobalConst::OS_MaxDisplays >;

	// variables
		Out< Displays_t >	result;
	};


	//
	// Get OS Modules
	//
	struct GetOSModules : _MsgBase_
	{
		Out< Platforms::OSModuleIDs >	result;
	};


	//
	// Get CPU Info
	//
	struct GetProccessorInfo : _MsgBase_
	{
	// types
		enum class EProcessorArch {
			Unknown,
			ARM,
			ARM64,
			X86,
			X64,
		};

		struct Info {
			uint			coresCount	= 0;
			uint			baseClock	= 0;
			EProcessorArch	arch		= EProcessorArch::Unknown;
		};

	// variables
		Out< Info >			result;
	};


	//
	// Get (RAM) Memory Info
	//
	struct GetMemoryInfo : _MsgBase_
	{
	// types
		struct Info {
			// physical
			BytesU			total;
			BytesU			available;
			// virtual
			BytesU			totalVirtual;
			BytesU			availableVirtual;
		};

	// variables
		Out< Info >			result;
	};


}	// OSMsg
}	// Engine
