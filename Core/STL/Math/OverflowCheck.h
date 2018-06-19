// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/MathTypes.h"
#include "Core/STL/Math/BinaryMath.h"
#include "Core/STL/CompileTime/NewTypeInfo.h"
#include "Core/STL/CompileTime/TemplateUtils.h"

namespace GX_STL
{
namespace GXMath
{
namespace OverflowChecker
{
	namespace _ovfw_hidden_
	{
		template <typename T>
		static constexpr bool	IsInt	= CompileTime::IsInteger<T> and CompileTime::IsSigned<T>;
		
		template <typename T>
		static constexpr bool	IsUInt	= CompileTime::IsInteger<T> and CompileTime::IsUnsigned<T>;

		template <typename T, typename Result>
		using EnableForInt = CompileTime::EnableIf< IsInt<T>, Result >;
		
		template <typename T, typename Result>
		using EnableForUInt = CompileTime::EnableIf< IsUInt<T>, Result >;

	}	// _ovfw_hidden_
	
/*
=================================================
	AdditionIsSafe
=================================================
*/
	template <typename T>
	inline _ovfw_hidden_::EnableForInt<T, bool>  AdditionIsSafe (const T a, const T b)
	{
		bool	overflow =	((b > 0) and (a > MaxValue<T>() - b))	or
							((b < 0) and (a < MinValue<T>() - b));
		return not overflow;
	}

	template <typename T, usize I, ulong U>
	inline _ovfw_hidden_::EnableForInt<T, Vec<bool,I>>  AdditionIsSafe (const Vec<T,I,U> a, const Vec<T,I,U> b)
	{
		Vec<bool,I>		res;
		FOR( i, res )	res[i] = AdditionIsSafe( a[i], b[i] );
		return res;
	}

/*
=================================================
	SubtractionIsSafe
=================================================
*/
	template <typename T>
	inline _ovfw_hidden_::EnableForInt<T, bool>  SubtractionIsSafe (const T a, const T b)
	{
		bool	overflow =	((b < 0) and (a > MaxValue<T>() + b))	or
							((b > 0) and (a < MinValue<T>() + b));
		return not overflow;
	}

	template <typename T, usize I, ulong U>
	inline _ovfw_hidden_::EnableForInt<T, Vec<bool,I>>  SubtractionIsSafe (const Vec<T,I,U> a, const Vec<T,I,U> b)
	{
		Vec<bool,I>		res;
		FOR( i, res )	res[i] = SubtractionIsSafe( a[i], b[i] );
		return res;
	}
	
/*
=================================================
	AdditionIsSafe
=================================================
*/
	template <typename T>
	inline _ovfw_hidden_::EnableForUInt<T, bool>  AdditionIsSafe (const T a, const T b)
	{
		return (a + b) >= (a | b);
	}

/*
=================================================
	SubtractionIsSafe
=================================================
*/
	template <typename T>
	inline _ovfw_hidden_::EnableForUInt<T, bool>  SubtractionIsSafe (const T a, const T b)
	{
		return a >= b;
	}


}	// OverflowChecker
}	// GXMath
}	// GX_STL
