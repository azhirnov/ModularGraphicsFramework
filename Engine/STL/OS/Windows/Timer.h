// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "OSWindows.h"
#include "Engine/STL/OS/Base/Date.h"

#if defined( PLATFORM_WINDOWS ) and \
	not defined( PLATFORM_SDL ) and \
	not defined( GX_USE_STD )

namespace GX_STL
{
namespace OS
{
	

	//
	// Performance Timer
	//

	struct _STL_EXPORT_ PerformanceTimer
	{
	// types
	private:
		typedef HiddenOSTypeFrom< ulong >	Handle_t;	// LARGE_INTEGER


	// variables
	private:
		Handle_t	_liFreq;


	// methods
	public:
		PerformanceTimer ();

		// seconds
		TimeD GetTime () const;

		// microseconds
		TimeL GetTimeMicroSec () const;

		template <typename T>
		T Get (const T& = T()) const;
	};
	

	template <>
	inline TimeL  PerformanceTimer::Get (const TimeL &) const
	{
		return GetTimeMicroSec();
	}
	
	template <>
	inline TimeD  PerformanceTimer::Get (const TimeD &) const
	{
		return GetTime();
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS