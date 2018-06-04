// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_BASE_POSIX

#include "Engine/STL/OS/Base/Common.h"

#if defined( PLATFORM_SDL )
#	include "Engine/STL/OS/SDL/OS_SDL.h"
#endif


namespace GX_STL
{
namespace OS
{
	using namespace GX_STL::GXTypes;

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX