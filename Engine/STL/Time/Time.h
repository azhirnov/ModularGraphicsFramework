// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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

#	define T	int
#	include "IntTimeImpl.h"

#	define T	uint
#	include "IntTimeImpl.h"
	
#	define T	ilong
#	include "IntTimeImpl.h"

#	define T	ulong
#	include "IntTimeImpl.h"

#	undef _TIME_IMPL_


	using TimeF		= Time<float>;
	using TimeD		= Time<double>;
	using TimeI		= Time<int>;
	using TimeU		= Time<uint>;
	using TimeL		= Time<ilong>;	// max 106751.99 days
	using TimeUL	= Time<ulong>;	// max 213503.98 days
	
	//ND_ constexpr TimeUL operator "" _nanoSec (unsigned long long value)	{ return TimeUL::FromNanoSeconds( value ); }
	ND_ constexpr TimeL  operator "" _nanoSec (unsigned long long value)	{ return TimeL::FromNanoSeconds( value ); }
	ND_ constexpr TimeD  operator "" _nanoSec (long double value)			{ return TimeD::FromNanoSeconds( (double)value ); }
	
	//ND_ constexpr TimeUL operator "" _microSec (unsigned long long value){ return TimeUL::FromMicroSeconds( value ); }
	ND_ constexpr TimeL  operator "" _microSec (unsigned long long value)	{ return TimeL::FromMicroSeconds( value ); }
	ND_ constexpr TimeD  operator "" _microSec (long double value)			{ return TimeD::FromMicroSeconds( (double)value ); }

	//ND_ constexpr TimeUL operator "" _milliSec (unsigned long long value){ return TimeUL::FromMilliSeconds( value ); }
	ND_ constexpr TimeL  operator "" _milliSec (unsigned long long value)	{ return TimeL::FromMilliSeconds( value ); }
	ND_ constexpr TimeD  operator "" _milliSec (long double value)			{ return TimeD::FromMilliSeconds( (double)value ); }
	
	//ND_ constexpr TimeUL operator "" _sec (unsigned long long value)		{ return TimeUL::FromSeconds( value ); }
	ND_ constexpr TimeL  operator "" _sec (unsigned long long value)		{ return TimeL::FromSeconds( value ); }
	ND_ constexpr TimeD  operator "" _sec (long double value)				{ return TimeD::FromSeconds( (double)value ); }


}	// GXTypes

namespace GXMath
{

/*
=================================================
	Max
=================================================
*/
	template <typename T>
	inline Time<T>  Max (const Time<T> &a, const Time<T> &b)
	{
		if_constexpr( Time<T>::GetSecondsPowerOf10() == 0 )
		{
			return Time<T>::FromSeconds( GXMath::Max( a.Seconds(), b.Seconds() ) );
		}
		else
		if_constexpr( Time<T>::GetSecondsPowerOf10() == 9 )
		{
			return Time<T>::FromNanoSeconds( GXMath::Max( a.NanoSeconds(), b.NanoSeconds() ) );
		}
	}
	
/*
=================================================
	Min
=================================================
*/
	template <typename T>
	inline Time<T>  Min (const Time<T> &a, const Time<T> &b)
	{
		if_constexpr( Time<T>::GetSecondsPowerOf10() == 0 )
		{
			return Time<T>::FromSeconds( GXMath::Min( a.Seconds(), b.Seconds() ) );
		}
		else
		if_constexpr( Time<T>::GetSecondsPowerOf10() == 9 )
		{
			return Time<T>::FromNanoSeconds( GXMath::Min( a.NanoSeconds(), b.NanoSeconds() ) );
		}
	}

}	// GXMath
}	// GX_STL
