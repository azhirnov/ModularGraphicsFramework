// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Math/MathFunc.h"
#include "Engine/STL/Containers/String.h"

namespace GX_STL
{
namespace GXTypes
{
	
	template <typename T>
	struct Time;
	
#	define _TIME_IMPL_

#	define T	double
#	include "FloatTimeImpl.h"

#	define T	float
#	include "FloatTimeImpl.h"

#	define T	uint
#	include "IntTimeImpl.h"
	
#	define T	ulong
#	include "IntTimeImpl.h"

#	undef _TIME_IMPL_


	typedef Time<float>		TimeF;
	typedef Time<double>	TimeD;
	typedef Time<uint>		TimeU;
	typedef Time<ulong>		TimeL;

}	// GXTypes
}	// GX_STL
