// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

//#define NOMINMAX
//#define NOMCX
//#define NOIME
//#define NOSERVICE
//#define WIN32_LEAN_AND_MEAN

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
#undef CopyFile
#undef MoveFile
