// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

//#define NOMINMAX
//#define NOMCX
//#define NOIME
//#define NOSERVICE
//#define WIN32_LEAN_AND_MEAN

#define Rectangle	WinRectangle
#define byte		WinByte

#ifdef COMPILER_MSVC
#	pragma warning (push, 0)
#	include <Windows.h>
#	pragma warning (pop)
#else
#	include <Windows.h>
#endif

#undef DeleteFile
#undef SetCurrentDirectory
#undef GetCurrentDirectory
#undef CreateDirectory
#undef CreateFile
#undef CopyFile
#undef MoveFile
#undef Yield
#undef GetFreeSpace
#undef CreateSemaphore
#undef GetCurrentTime
#undef min
#undef max
#undef CreateEvent
#undef FindWindow

#undef Rectangle
#undef byte
