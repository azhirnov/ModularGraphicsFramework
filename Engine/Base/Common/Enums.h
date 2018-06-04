// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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

			SequentialOnly,

			_Count,
			Unknown		= ~0u
		};

		GX_ENUM_BITFIELD( EMemoryAccess );

		static constexpr bits	CpuReadWrite	= bits().Set( CpuRead ).Set( CpuWrite );
		static constexpr bits	GpuReadWrite	= bits().Set( GpuRead ).Set( GpuWrite );
		static constexpr bits	All				= CpuReadWrite | GpuReadWrite;

		static bool HasRandomAccess (bits flags);
		static bool HasSequentialAccess (bits flags);
	};


//-----------------------------------------------------------------------------
// EMemoryAccess
	
	inline bool  EMemoryAccess::HasRandomAccess (bits flags)
	{
		return not flags[SequentialOnly];
	}

	inline bool  EMemoryAccess::HasSequentialAccess (bits)
	{
		return true;
	}

}	// Base
}	// Engine
