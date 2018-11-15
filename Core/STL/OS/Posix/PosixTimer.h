// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#if defined( PLATFORM_BASE_POSIX ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Posix/OSPosix.h"
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
	// methods
	public:

		// seconds
		ND_ TimeD  GetTime () const;

		// microseconds
		ND_ TimeL  GetTimeMicroSec () const;

		template <typename T>
		ND_ T  Get (const T& = T()) const;
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

#endif	// PLATFORM_BASE_POSIX and GX_USE_NATIVE_API
