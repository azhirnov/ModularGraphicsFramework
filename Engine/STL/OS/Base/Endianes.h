// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace GX_STL
{
namespace OS
{

	inline bool IsBigEndian () noexcept
	{
		const union {
			uint i;
			char c[4];
		} bint = {0x01020304};
		return bint.c[0] == 1; 
	}

	inline bool IsLittleEndian () {
		return not IsBigEndian();
	}

}	// OS
}	// GX_STL
