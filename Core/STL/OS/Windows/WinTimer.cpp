// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#if defined( PLATFORM_WINDOWS )

#include "Core/STL/OS/Windows/WinTimer.h"
#include "Core/STL/OS/Base/Date.h"
#include "Core/STL/OS/Windows/WinHeader.h"

namespace GX_STL
{
namespace OS
{
	
# ifdef GX_USE_NATIVE_API

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
		return TimeL::FromMicroSeconds( (perf_counter.QuadPart * 1000'000) / _liFreq.Get<LARGE_INTEGER>().QuadPart );
	}

# endif	// GX_USE_NATIVE_API
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