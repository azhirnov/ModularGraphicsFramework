// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TemplateMath.h"
#include "Engine/STL/Math/BinaryMath.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Power of 2 Value
	//

	template <typename T>
	struct PowOf2Value
	{
		STATIC_ASSERT( CompileTime::IsInteger<T>  and
					   CompileTime::IsUnsigned<T> and
					   CompileTime::IsScalar<T> );
		
	// types
	public:
		typedef PowOf2Value<T>		Self;
		typedef T					Value_t;


	// variables
	private:
		T	_value;


	// methods
	public:
		constexpr PowOf2Value () : _value(0) {}

		constexpr PowOf2Value (const Self &other) : _value(other._value) {}

		constexpr explicit PowOf2Value (T value) : _value(value) {}


		ND_ constexpr T  GetPower ()	const
		{
			return _value;
		}

		ND_ constexpr T  GetValue ()	const
		{
			ASSERT( _value < CompileTime::SizeOf<T>::bits );
			return T(1) << _value;
		}

		ND_ constexpr static Self  From (T value)
		{
			ASSERT( value > 0 and _IsPowOf2( value ) );
			return PowOf2Value( _IntLog2( value ) );
		}

		_GX_DIM_CMP_OPERATORS_SELF( _value );


	private:
		constexpr static bool _IsPowOf2 (const T a)
		{
			return ( a != 0 and ( (a & (a - 1)) == 0 ) );
		}

		constexpr static T  _IntLog2 (const T a)
		{
			return _math_hidden_::_RecursiveBitScanReverse< CompileTime::SizeOf<T>::bits-1 >::Get( a ) - 1;
		}
	};


}	// GXMath
}	// GX_STL
