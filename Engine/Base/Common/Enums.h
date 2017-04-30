// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/Common.h"

namespace Engine
{
namespace Base
{

	struct EMemoryAccess
	{
		enum type : uint
		{
			CpuRead,
			CpuWrite,

			GpuRead,
			GpuWrite,

			_Count,
			Unknown		= uint(-1)
		};

		using bits = EnumBitfield< EMemoryAccess >;
	};


	/*struct EStream
	{
		enum type : uint
		{
			// all data is contained in memory
			Memory,

			// data is fixed size in memory buffer in which data loaded from ram/vram/file/url
			MemoryStream,

			// all data is contained in video memory
			VideoMemory,

			// data is fixed size in video memory buffer in which data loaded from ram/vram/file/url
			VideoMemoryStream,

			_Count,
			Unknown = uint(-1)
		};
	};*/


}	// Base
}	// Engine
