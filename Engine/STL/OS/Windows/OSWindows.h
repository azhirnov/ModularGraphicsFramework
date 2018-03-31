// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_WINDOWS

#include "Engine/STL/OS/Base/Common.h"

#if defined( PLATFORM_SDL )
#	include "Engine/STL/OS/SDL/OS_SDL.h"
#endif


namespace GX_STL
{
namespace OS
{
	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
