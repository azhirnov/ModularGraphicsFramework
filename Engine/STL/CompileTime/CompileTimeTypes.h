// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/Common/UMax.h"

namespace GX_STL
{
namespace CompileTime
{
	using wchar		= GXTypes::wchar;
	using byte		= GXTypes::byte;
	using ubyte		= GXTypes::ubyte;
	using ushort	= GXTypes::ushort;
	using uint		= GXTypes::uint;
	using usize		= GXTypes::usize;
	using isize		= GXTypes::isize;
	using ilong		= GXTypes::ilong;
	using ulong		= GXTypes::ulong;

	static constexpr auto UMax = GXTypes::UMax;

}	// CompileTime
}	// GX_STL
