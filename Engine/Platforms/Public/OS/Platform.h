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
	struct GetDisplays : _MessageBase_
	{
	// types
		using Displays_t = FixedSizeArray< Platforms::Display, GlobalConst::OS_MaxDisplays >;

	// variables
		Out< Displays_t >	result;

	// methods
		GetDisplays () {}
	};


	//
	// Get OS Modules
	//
	struct GetOSModules : _MessageBase_
	{
	// variables
		Out< Platforms::OSModuleIDs >	result;

	// methods
		GetOSModules () {}
	};


	//
	// Get CPU Info
	//
	struct GetProccessorInfo : _MessageBase_
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

	// methods
		GetProccessorInfo () {}
	};


	//
	// Get (RAM) Memory Info
	//
	struct GetMemoryInfo : _MessageBase_
	{
	// types
		struct Info {
			// physical
			BytesUL			total;
			BytesUL			available;
			// virtual
			BytesUL			totalVirtual;
			BytesUL			availableVirtual;
		};

	// variables
		Out< Info >			result;

	// methods
		GetMemoryInfo () {}
	};


}	// OSMsg
}	// Engine
