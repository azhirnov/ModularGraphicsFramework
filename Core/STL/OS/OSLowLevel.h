// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/OS/Base/Common.h"

#ifdef PLATFORM_SDL
#	include "Core/STL/OS/SDL/SyncPrimitives.h"
#	include "Core/STL/OS/SDL/Thread.h"
#	include "Core/STL/OS/SDL/Library.h"
#	include "Core/STL/OS/SDL/Timer.h"
#	include "Core/STL/OS/SDL/FileSystem.h"
#	include "Core/STL/OS/SDL/SDLUtils.h"
#	include "Core/STL/OS/SDL/RandDevice.h"
#else

# ifdef GX_USE_STD
#	include "Core/STL/OS/STD/Thread.h"
#	include "Core/STL/OS/STD/Timer.h"
# endif

# ifdef PLATFORM_WINDOWS
#	include "Core/STL/OS/Windows/SyncPrimitives.h"
#	include "Core/STL/OS/Windows/Thread.h"
#	include "Core/STL/OS/Windows/Library.h"
#	include "Core/STL/OS/Windows/Timer.h"
#	include "Core/STL/OS/Windows/FileSystem.h"
#	include "Core/STL/OS/Windows/PlatformUtils.h"
#	include "Core/STL/OS/Windows/RandDevice.h"
# endif

# ifdef PLATFORM_BASE_POSIX
#	include "Core/STL/OS/Posix/SyncPrimitives.h"
#	include "Core/STL/OS/Posix/Thread.h"
#	include "Core/STL/OS/Posix/Library.h"
#	include "Core/STL/OS/Posix/Timer.h"
#	include "Core/STL/OS/Posix/FileSystem.h"
#	include "Core/STL/OS/Posix/PlatformUtils.h"
#	include "Core/STL/OS/Posix/RandDevice.h"
# endif

# ifdef PLATFORM_ANDROID
#	include "Core/STL/OS/Android/OSAndroid.h"
# endif

#endif

#include "Core/STL/OS/Base/BaseFileSystem.h"
#include "Core/STL/OS/Base/Endianes.h"


namespace GX_STL
{
namespace OS
{
	using Mutex		= CriticalSection;
	
}	// OS

namespace GXTypes
{
	using Date		= OS::Date;

}	// GXTypes
}	// GX_STL
