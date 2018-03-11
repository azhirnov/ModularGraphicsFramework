// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"
#include "Engine/Config/STL.Config.h"

#if defined( PLATFORM_BASE_POSIX ) and \
	not defined( PLATFORM_SDL ) and \
	not defined( GX_USE_STD )

#include "Engine/STL/OS/Posix/OSPosix.h"
#include "Engine/STL/OS/Base/Date.h"

namespace GX_STL
{
namespace OS
{

	//
	// Performance Timer
	//
	
	struct PerformanceTimer final : public Noncopyable
	{
	// methods
	public:

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

#endif	// PLATFORM_BASE_POSIX and not PLATFORM_SDL and not GX_USE_STD
