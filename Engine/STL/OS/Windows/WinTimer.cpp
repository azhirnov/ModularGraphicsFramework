// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/OS/Windows/Timer.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/Math/Interpolations.h"

#if defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"

namespace GX_STL
{
namespace OS
{
	
# if not defined( PLATFORM_SDL ) and not defined( GX_USE_STD )

/*
=================================================
	constructor
=================================================
*/
	PerformanceTimer::PerformanceTimer () :
		_liFreq( UninitializedT<LARGE_INTEGER>() )
	{
		QueryPerformanceFrequency( &_liFreq.Get<LARGE_INTEGER>() );
	}
		
/*
=================================================
	GetTime
=================================================
*/
	TimeD PerformanceTimer::GetTime () const
	{
		LARGE_INTEGER	perf_counter;
		QueryPerformanceCounter( &perf_counter );
		return TimeD::FromSeconds( (double)perf_counter.QuadPart / (double)_liFreq.Get<LARGE_INTEGER>().QuadPart );
	}
		
/*
=================================================
	GetTimeMicroSec
=================================================
*/
	TimeL PerformanceTimer::GetTimeMicroSec () const
	{
		LARGE_INTEGER	perf_counter;
		QueryPerformanceCounter( &perf_counter );
		return TimeL::FromMicroSeconds( (perf_counter.QuadPart * 1000000) / _liFreq.Get<LARGE_INTEGER>().QuadPart );
	}

# endif	// not PLATFORM_SDL and not GX_USE_STD
//-----------------------------------------------------------------------------
	


/*
=================================================
	Now
=================================================
*/
	Date & Date::Now ()
	{
		SYSTEMTIME	 time = {};
		GetLocalTime( OUT &time );

		_year		= time.wYear;
		_month		= time.wMonth-1;								// 1..12 convert to 0..11
		_dayOfWeek	= time.wDayOfWeek == 0 ? 6 : time.wDayOfWeek-1;	// 0..6, Sun == 0 convert to mon..sun
		_dayOfMonth	= time.wDay-1;									// 1..31 convert to 0..30
		_dayOfYear	= _CalcDayOfYear( _year, _month, _dayOfMonth );
		
		_hour		= time.wHour;									// 0..23
		_minute		= time.wMinute;									// 0..59
		_second		= time.wSecond;									// 0..59
		_millis		= time.wMilliseconds;							// 0..999

		return *this;
	}
//-----------------------------------------------------------------------------


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS