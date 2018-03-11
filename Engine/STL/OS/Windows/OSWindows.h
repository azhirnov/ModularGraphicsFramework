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
	
	//
	// Hidden OS Type from same type
	//

	template <typename T, bool WithDestructor = false>
	using HiddenOSTypeFrom = DeferredType< sizeof(T), alignof(T), WithDestructor >;

}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
