// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Base/Common.h"

#ifdef PLATFORM_SDL
#	include "Engine/STL/OS/SDL/SyncPrimitives.h"
#	include "Engine/STL/OS/SDL/Thread.h"
#	include "Engine/STL/OS/SDL/Library.h"
#	include "Engine/STL/OS/SDL/Timer.h"
#	include "Engine/STL/OS/SDL/FileSystem.h"
#	include "Engine/STL/OS/SDL/SDLUtils.h"
#	include "Engine/STL/OS/SDL/RandDevice.h"
#else

# ifdef GX_USE_STD
#	include "Engine/STL/OS/STD/Thread.h"
#	include "Engine/STL/OS/STD/Timer.h"
# endif

# ifdef PLATFORM_WINDOWS
#	include "Engine/STL/OS/Windows/SyncPrimitives.h"
#	include "Engine/STL/OS/Windows/Thread.h"
#	include "Engine/STL/OS/Windows/Library.h"
#	include "Engine/STL/OS/Windows/Timer.h"
#	include "Engine/STL/OS/Windows/FileSystem.h"
#	include "Engine/STL/OS/Windows/PlatformUtils.h"
#	include "Engine/STL/OS/Windows/RandDevice.h"
# endif

# ifdef PLATFORM_BASE_POSIX
#	include "Engine/STL/OS/Posix/SyncPrimitives.h"
#	include "Engine/STL/OS/Posix/Thread.h"
#	include "Engine/STL/OS/Posix/Library.h"
#	include "Engine/STL/OS/Posix/Timer.h"
#	include "Engine/STL/OS/Posix/FileSystem.h"
#	include "Engine/STL/OS/Posix/PlatformUtils.h"
#	include "Engine/STL/OS/Posix/RandDevice.h"
# endif

# ifdef PLATFORM_ANDROID
#	include "Engine/STL/OS/Android/OSAndroid.h"
# endif

#endif

#include "Engine/STL/OS/Base/BaseFileSystem.h"
#include "Engine/STL/OS/Base/Endianes.h"


namespace GX_STL
{
namespace OS
{
	typedef CriticalSection		Mutex;
	
}	// OS
}	// GX_STL
