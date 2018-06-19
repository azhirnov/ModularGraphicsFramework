// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/MathFunc.h"
#include "Core/STL/Containers/String.h"
#include "Core/STL/OS/OSLowLevel.h"

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
	
#	define T	ilong
#	include "IntTimeImpl.h"

#	define T	ulong
#	include "IntTimeImpl.h"

#	undef _TIME_PROFILER_IMPL_


	using TimeProfilerF	= TimeProfiler<float>;
	using TimeProfilerD	= TimeProfiler<double>;
	using TimeProfilerU	= TimeProfiler<uint>;
	using TimeProfilerL	= TimeProfiler<ilong>;

}	// GXTypes
}	// GX_STL
