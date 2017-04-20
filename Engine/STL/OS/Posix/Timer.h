// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OSPosix.h"
#include "Engine/STL/OS/Base/Date.h"

#ifdef PLATFORM_BASE_POSIX

namespace GX_STL
{
namespace OS
{

#	ifdef PLATFORM_BASE_POSIX_SHELL

	//
	// Perfomance Timer
	//
	
	struct PerfomanceTimer
	{
		// seconds
		TimeD GetTime () const
		{
			timeval tv;
			gettimeofday( &tv, null );
			return TimeD::FromSeconds( tv.tv_sec + tv.tv_usec * 1.0e-6 );
		}

		// microseconds
		TimeL GetTimeMicroSec () const
		{
			timeval tv;
			gettimeofday( &tv, null );
			return TimeL::FromMicroSeconds( ulong(tv.tv_sec) * 1000000 + tv.tv_usec );
		}

		template <typename T>
		T Get (const T&) const;
	};
	

	template <>
	inline TimeL  PerfomanceTimer::Get (const TimeL &) const
	{
		return GetTimeMicroSec();
	}
	
	template <>
	inline TimeD  PerfomanceTimer::Get (const TimeD &) const
	{
		return GetTime();
	}

#	endif	// PLATFORM_BASE_POSIX_SHELL

	

	// Now
	inline Date & Date::Now ()
	{
		struct timeval  tv;
		gettimeofday( &tv, 0 );

		time_t t = tv.tv_sec;
		struct tm * dt = localtime( & t );

		year			= dt->tm_year + 1900;
		month			= dt->tm_mon;								// 0..11
		day				= dt->tm_mday-1;							// 1..31 convert to 0..30
		dayOfWeek		= dt->tm_wday == 0 ? 6 : dt->tm_wday-1;		// 0..6, Sun == 0 convert to mon..sun
		dayOfYear		= dt->tm_yday;								// 0..365

		hour			= dt->tm_hour;								// 0..23
		minute			= dt->tm_min;								// 0..59
		second			= dt->tm_sec;								// 0..59
		milliseconds	= tv.tv_usec / 1000;						// 0..1000

		return *this;
	}

//-------------------------------------------------------------------

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX