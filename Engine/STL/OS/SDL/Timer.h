// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OS_SDL.h"

#ifdef PLATFORM_SDL

#include "Engine/STL/OS/Base/Date.h"
#include "Engine/STL/OS/Windows/Timer.h"
#include "Engine/STL/OS/Posix/Timer.h"

namespace GX_STL
{
namespace OS
{
	
	//
	// Perfomance Timer
	//

	struct PerfomanceTimer
	{
	private:
		ulong	_freq;

	public:
		PerfomanceTimer ()
		{
			_freq = ::SDL_GetPerformanceFrequency();
		}

		// seconds
		forceinline TimeD GetTime () const
		{
			return TimeD::FromSeconds( double(::SDL_GetPerformanceCounter()) / _freq );
		}

		// microseconds
		forceinline TimeL GetTimeMicroSec () const
		{
			return TimeL().FromTime( GetTime() );
		}

		template <typename T>
		T Get (const T&) const;
	};
	

	template <>
	forceinline TimeL  PerfomanceTimer::Get (const TimeL &) const
	{
		return GetTimeMicroSec();
	}
	
	template <>
	forceinline TimeD  PerfomanceTimer::Get (const TimeD &) const
	{
		return GetTime();
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL