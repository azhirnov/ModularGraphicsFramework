// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/OS/Posix/Timer.h"
#include "Engine/STL/Math/BinaryMath.h"

#ifdef PLATFORM_BASE_POSIX

#include "Engine/STL/OS/Posix/PosixHeader.h"

namespace GX_STL
{
namespace OS
{

# if not defined( PLATFORM_SDL ) and not defined( GX_USE_STD )
/*
=================================================
	GetTime
=================================================
*/
	TimeD PerformanceTimer::GetTime () const
	{
		timeval tv;
		::gettimeofday( OUT &tv, null );
		return TimeD::FromSeconds( tv.tv_sec + tv.tv_usec * 1.0e-6 );
	}
		
/*
=================================================
	GetTimeMicroSec
=================================================
*/
	TimeL PerformanceTimer::GetTimeMicroSec () const
	{
		timeval tv;
		::gettimeofday( OUT &tv, null );
		return TimeL::FromMicroSeconds( ulong(tv.tv_sec) * 1000000 + tv.tv_usec );
	}

# endif	// not PLATFORM_SDL
//-----------------------------------------------------------------------------
	


/*
=================================================
	Now
=================================================
*/
	Date &  Date::Now ()
	{
		struct timeval  tv;
		::gettimeofday( OUT &tv, 0 );

		time_t t = tv.tv_sec;
		const tm * dt = ::localtime( &t );

		//time64_t t = time64();
		//const tm * dt = localtime64( &t );

		_year			= dt->tm_year + 1900;
		_month			= dt->tm_mon;								// 0..11
		_day			= dt->tm_mday-1;							// 1..31 convert to 0..30
		_dayOfWeek		= dt->tm_wday == 0 ? 6 : dt->tm_wday-1;		// 0..6, Sun == 0 convert to mon..sun
		_dayOfYear		= dt->tm_yday;								// 0..365

		_hour			= dt->tm_hour;								// 0..23
		_minute			= dt->tm_min;								// 0..59
		_second			= dt->tm_sec;								// 0..59
		_milliseconds	= tv.tv_usec / 1000;						// 0..1000

		return *this;
	}
//-----------------------------------------------------------------------------


}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX
