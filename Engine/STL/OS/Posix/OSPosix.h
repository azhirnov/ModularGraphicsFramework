// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/OS/Base/Common.h"

#ifdef PLATFORM_BASE_POSIX

#if defined( PLATFORM_SDL )
	// disable WinAPI Shells
#else
	// enable WinAPI Shells
#	define PLATFORM_BASE_POSIX_SHELL
#endif


namespace GX_STL
{
namespace posix
{

#	include <time.h>
#	include <dlfcn.h>
#	include <pthread.h>
//#	include <signal.h>
#	include <unistd.h>
#	include <semaphore.h>
//#	include <errno.h>

}	// posix
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX