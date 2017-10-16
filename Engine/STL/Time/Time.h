// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	
	constexpr TimeL operator "" _nanoSec (unsigned long long value)		{ return TimeL::FromNanoSeconds( value ); }
	constexpr TimeD operator "" _nanoSec (long double value)			{ return TimeD::FromNanoSeconds( (double)value ); }
	
	constexpr TimeL operator "" _microSec (unsigned long long value)		{ return TimeL::FromMicroSeconds( value ); }
	constexpr TimeD operator "" _microSec (long double value)			{ return TimeD::FromMicroSeconds( (double)value ); }

	constexpr TimeL operator "" _miliSec (unsigned long long value)		{ return TimeL::FromMilliSeconds( value ); }
	constexpr TimeD operator "" _miliSec (long double value)			{ return TimeD::FromMilliSeconds( (double)value ); }
	
	constexpr TimeL operator "" _sec (unsigned long long value)			{ return TimeL::FromSeconds( value ); }
	constexpr TimeD operator "" _sec (long double value)				{ return TimeD::FromSeconds( (double)value ); }


}	// GXTypes
}	// GX_STL
