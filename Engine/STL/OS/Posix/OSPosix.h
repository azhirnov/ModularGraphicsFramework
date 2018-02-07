// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Base/Common.h"

#ifdef PLATFORM_BASE_POSIX

#if defined( PLATFORM_SDL )
#	include "Engine/STL/OS/SDL/OS_SDL.h"
#endif


namespace GX_STL
{
namespace OS
{
	using namespace GXTypes;

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX