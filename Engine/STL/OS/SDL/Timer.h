// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/STL/OS/SDL/OS_SDL.h"
#include "Engine/STL/OS/Base/Date.h"
#include "Engine/STL/OS/Windows/Timer.h"
#include "Engine/STL/OS/Posix/Timer.h"

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
		T Get (const T& = T()) const;
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