// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/OS/Base/Common.h"

#ifdef PLATFORM_SDL
#	include "Core/STL/OS/SDL/SDLSyncPrimitives.h"
#	include "Core/STL/OS/SDL/SDLThread.h"
#	include "Core/STL/OS/SDL/SDLLibrary.h"
#	include "Core/STL/OS/SDL/SDLTimer.h"
#	include "Core/STL/OS/SDL/SDLFileSystem.h"
#	include "Core/STL/OS/SDL/SDLPlatformUtils.h"
#	include "Core/STL/OS/SDL/SDLRandDevice.h"
#else

# ifdef GX_USE_STD
#	include "Core/STL/OS/STD/STDThread.h"
#	include "Core/STL/OS/STD/STDTimer.h"
#	include "Core/STL/OS/STD/STDSyncPrimitives.h"
# endif

# ifdef PLATFORM_WINDOWS
#	include "Core/STL/OS/Windows/WinSyncPrimitives.h"
#	include "Core/STL/OS/Windows/WinThread.h"
#	include "Core/STL/OS/Windows/WinLibrary.h"
#	include "Core/STL/OS/Windows/WinTimer.h"
#	include "Core/STL/OS/Windows/WinFileSystem.h"
#	include "Core/STL/OS/Windows/WinPlatformUtils.h"
#	include "Core/STL/OS/Windows/WinRandDevice.h"
# endif

# ifdef PLATFORM_BASE_POSIX
#	include "Core/STL/OS/Posix/PosixSyncPrimitives.h"
#	include "Core/STL/OS/Posix/PosixThread.h"
#	include "Core/STL/OS/Posix/PosixLibrary.h"
#	include "Core/STL/OS/Posix/PosixTimer.h"
#	include "Core/STL/OS/Posix/PosixFileSystem.h"
#	include "Core/STL/OS/Posix/PosixPlatformUtils.h"
#	include "Core/STL/OS/Posix/PosixRandDevice.h"
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
	
}	// OS

namespace GXTypes
{
	using Date			= OS::Date;
	using Mutex			= OS::Mutex;
	using ReadWriteSync	= OS::ReadWriteSync;
	using SyncEvent		= OS::SyncEvent;

}	// GXTypes
}	// GX_STL
