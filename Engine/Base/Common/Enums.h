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


}	// Base
}	// Engine
