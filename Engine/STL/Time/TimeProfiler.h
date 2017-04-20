// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Math/MathFunc.h"
#include "Engine/STL/Containers/String.h"
#include "Engine/STL/OS/OSLowLevel.h"

namespace GX_STL
{
namespace GXTypes
{
	
	template <typename T>
	struct TimeProfiler;

#	define _TIME_PROFILER_IMPL_

#	define T	double
#	include "FloatTimeImpl.h"

#	define T	float
#	include "FloatTimeImpl.h"

#	define T	uint
#	include "IntTimeImpl.h"
	
#	define T	ulong
#	include "IntTimeImpl.h"

#	undef _TIME_PROFILER_IMPL_


	typedef TimeProfiler<float>		TimeProfilerF;
	typedef TimeProfiler<double>	TimeProfilerD;
	typedef TimeProfiler<uint>		TimeProfilerU;
	typedef TimeProfiler<ulong>		TimeProfilerL;

}	// GXTypes
}	// GX_STL
