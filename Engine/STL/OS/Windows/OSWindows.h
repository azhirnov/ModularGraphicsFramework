// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/OS/Base/Common.h"

#ifdef PLATFORM_WINDOWS

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
