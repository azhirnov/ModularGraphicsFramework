// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Core/STL/OS/SDL/OS_SDL.h"
#include "Core/STL/OS/Base/Date.h"
#include "Core/STL/OS/Windows/WinTimer.h"
#include "Core/STL/OS/Posix/PosixTimer.h"

namespace GX_STL
{
namespace OS
{
	
	//
	// Performance Timer
	//

	struct PerformanceTimer
	{
	private:
		ulong	_freq;

	public:
		PerformanceTimer ()
		{
			_freq = ::SDL_GetPerformanceFrequency();
		}

		// seconds
		ND_ forceinline TimeD  GetTime () const
		{
			return TimeD::FromSeconds( double(::SDL_GetPerformanceCounter()) / _freq );
		}

		// microseconds
		ND_ forceinline TimeL  GetTimeMicroSec () const
		{
			return TimeL().FromTime( GetTime() );
		}

		template <typename T>
		ND_ T  Get (const T& = T()) const;
	};
	

	template <>
	forceinline TimeL  PerformanceTimer::Get (const TimeL &) const
	{
		return GetTimeMicroSec();
	}
	
	template <>
	forceinline TimeD  PerformanceTimer::Get (const TimeD &) const
	{
		return GetTime();
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL