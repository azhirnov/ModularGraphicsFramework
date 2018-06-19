// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#if defined( PLATFORM_WINDOWS ) and \
	not defined( PLATFORM_SDL ) and \
	not defined( GX_USE_STD )

#include "Core/STL/OS/Windows/OSWindows.h"
#include "Core/STL/OS/Base/Date.h"

namespace GX_STL
{
namespace OS
{
	

	//
	// Performance Timer
	//

	struct PerformanceTimer final : public Noncopyable
	{
	// types
	private:
		using Handle_t = DeferredTypeFrom< ulong >;	// LARGE_INTEGER


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

#endif	// PLATFORM_WINDOWS and not PLATFORM_SDL and not GX_USE_STD