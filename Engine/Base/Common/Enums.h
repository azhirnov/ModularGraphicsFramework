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
			Unknown		= ~0u
		};

		GX_ENUM_BITFIELD( EMemoryAccess );

		static constexpr bits	CpuReadWrite	= bits().Set( CpuRead ).Set( CpuWrite );
		static constexpr bits	GpuReadWrite	= bits().Set( GpuRead ).Set( GpuWrite );
		static constexpr bits	All				= CpuReadWrite | GpuReadWrite;
	};


}	// Base
}	// Engine
