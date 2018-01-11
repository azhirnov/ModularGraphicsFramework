// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Base/Date.h"

#if defined( GX_USE_STD ) and \
	not defined( PLATFORM_SDL )

namespace GX_STL
{
namespace OS
{

	//
	// Performance Timer
	//

	struct PerformanceTimer
	{
	// methods
	public:
		PerformanceTimer ()
		{}

		// seconds
		TimeD GetTime () const;

		// microseconds
		TimeL GetTimeMicroSec () const;

		template <typename T>
		T Get (const T& = T()) const;
	};
	
	
/*
=================================================
	GetTime
=================================================
*/
	inline TimeD PerformanceTimer::GetTime () const
	{
		return TimeD::FromTime( GetTimeMicroSec() );
	}
	
/*
=================================================
	GetTimeMicroSec
=================================================
*/
	inline TimeL PerformanceTimer::GetTimeMicroSec () const
	{
		return TimeL::FromNanoSeconds( std::chrono::high_resolution_clock::now().time_since_epoch().count() );
	}
	
/*
=================================================
	Get
=================================================
*/
	template <>
	inline TimeL  PerformanceTimer::Get (const TimeL &) const
	{
		return GetTimeMicroSec();
	}
	
/*
=================================================
	Get
=================================================
*/
	template <>
	inline TimeD  PerformanceTimer::Get (const TimeD &) const
	{
		return GetTime();
	}


}	// OS
}	// GX_STL

#endif	// GX_USE_STD