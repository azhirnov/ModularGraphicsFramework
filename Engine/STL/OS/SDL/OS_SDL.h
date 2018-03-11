// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/STL/OS/Base/Common.h"

# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable: 4005)	// macros redefinition
#	pragma warning (disable: 4668)	// '...' is not defined as a preprocessor macro
# endif

#	define SDL_MAIN_HANDLED
#	include "External/SDL2/include/SDL.h"

# ifdef COMPILER_MSVC
#	pragma warning (pop)
# endif


namespace GX_STL
{
namespace OS
{

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
