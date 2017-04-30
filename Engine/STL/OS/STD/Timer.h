// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Base/Date.h"

#if defined( GX_USE_STD ) and \
	not defined( PLATFORM_SDL )

namespace GX_STL
{
namespace OS
{

	//
	// Perfomance Timer
	//

	struct PerfomanceTimer
	{
	// methods
	public:
		PerfomanceTimer ()
		{}

		// seconds
		TimeD GetTime () const;

		// microseconds
		TimeL GetTimeMicroSec () const;

		template <typename T>
		T Get (const T&) const;
	};
	
	
/*
=================================================
	GetTime
=================================================
*/
	inline TimeD PerfomanceTimer::GetTime () const
	{
		return TimeD::FromTime( GetTimeMicroSec() );
	}
	
/*
=================================================
	GetTimeMicroSec
=================================================
*/
	inline TimeL PerfomanceTimer::GetTimeMicroSec () const
	{
		return TimeL::FromNanoSeconds( std::chrono::high_resolution_clock::now().time_since_epoch().count() );
	}
	
/*
=================================================
	Get
=================================================
*/
	template <>
	inline TimeL  PerfomanceTimer::Get (const TimeL &) const
	{
		return GetTimeMicroSec();
	}
	
/*
=================================================
	Get
=================================================
*/
	template <>
	inline TimeD  PerfomanceTimer::Get (const TimeD &) const
	{
		return GetTime();
	}


}	// OS
}	// GX_STL

#endif	// GX_USE_STD