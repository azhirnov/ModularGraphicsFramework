// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/MathTypes.h"
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
		
		template <typename T1, typename T2, typename Result>
		using EnableForInt = CompileTime::EnableIf< IsInt<T1> and IsInt<T2>, Result >;
		
		template <typename T1, typename T2, typename Result>
		using EnableForUInt = CompileTime::EnableIf< IsUInt<T1> and IsUInt<T2>, Result >;
		
/*
=================================================
	_AdditionIsSafe_Signed
=================================================
*/
		template <typename T>
		inline bool _AdditionIsSafe_Signed (const T a, const T b)
		{
			STATIC_ASSERT( CompileTime::IsScalar<T> and IsInt<T> );

			bool	overflow =	((b > 0) and (a > MaxValue<T>() - b))	or
								((b < 0) and (a < MinValue<T>() - b));
			return not overflow;
		}

/*
=================================================
	_SubtractionIsSafe_Signed
=================================================
*/
		template <typename T>
		inline bool  _SubtractionIsSafe_Signed (const T a, const T b)
		{
			STATIC_ASSERT( CompileTime::IsScalar<T> and IsInt<T> );

			bool	overflow =	((b < 0) and (a > MaxValue<T>() + b))	or
								((b > 0) and (a < MinValue<T>() + b));
			return not overflow;
		}

/*
=================================================
	_AdditionIsSafe_Unsigned
=================================================
*/
		template <typename T>
		inline bool  _AdditionIsSafe_Unsigned (const T a, const T b)
		{
			return (a + b) >= (a | b);
		}

/*
=================================================
	_SubtractionIsSafe_Unsigned
=================================================
*/
		template <typename T>
		inline bool  _SubtractionIsSafe_Unsigned (const T a, const T b)
		{
			return a >= b;
		}

	}	// _ovfw_hidden_
//-----------------------------------------------------------------------------


/*
=================================================
	AdditionIsSafe
=================================================
*/
	template <typename T1, typename T2>
	inline _ovfw_hidden_::EnableForInt<T1, T2, bool>  AdditionIsSafe (const T1 a, const T2 b)
	{
		STATIC_ASSERT( CompileTime::IsScalar<T1> and CompileTime::IsScalar<T2> );

		using T = CompileTime::GenTypeStd< T1, T2 >;

		return _ovfw_hidden_::_AdditionIsSafe_Signed( T(a), T(b) );
	}
	
	template <typename T, usize I, ulong U>
	inline _ovfw_hidden_::EnableForInt<T, T, Vec<bool,I>>  AdditionIsSafe (const Vec<T,I,U> a, const Vec<T,I,U> b)
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
	template <typename T1, typename T2>
	inline _ovfw_hidden_::EnableForInt<T1, T2, bool>  SubtractionIsSafe (const T1 a, const T2 b)
	{
		STATIC_ASSERT( CompileTime::IsScalar<T1> and CompileTime::IsScalar<T2> );

		using T = CompileTime::GenTypeStd< T1, T2 >;

		return _ovfw_hidden_::_SubtractionIsSafe_Signed( T(a), T(b) );
	}
	
	template <typename T, usize I, ulong U>
	inline _ovfw_hidden_::EnableForInt<T, T, Vec<bool,I>>  SubtractionIsSafe (const Vec<T,I,U> a, const Vec<T,I,U> b)
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
	template <typename T1, typename T2>
	inline _ovfw_hidden_::EnableForUInt<T1, T2, bool>  AdditionIsSafe (const T1 a, const T2 b)
	{
		STATIC_ASSERT( CompileTime::IsScalar<T1> and CompileTime::IsScalar<T2> );

		using T = CompileTime::GenTypeStd< T1, T2 >;

		return _ovfw_hidden_::_AdditionIsSafe_Unsigned( T(a), T(b) );
	}
	
	template <typename T, usize I, ulong U>
	inline _ovfw_hidden_::EnableForUInt<T, T, Vec<bool,I>>  AdditionIsSafe (const Vec<T,I,U> a, const Vec<T,I,U> b)
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
	template <typename T1, typename T2>
	inline _ovfw_hidden_::EnableForUInt<T1, T2, bool>  SubtractionIsSafe (const T1 a, const T2 b)
	{
		STATIC_ASSERT( CompileTime::IsScalar<T1> and CompileTime::IsScalar<T2> );

		using T = CompileTime::GenTypeStd< T1, T2 >;

		return _ovfw_hidden_::_SubtractionIsSafe_Unsigned( T(a), T(b) );
	}
	
	template <typename T, usize I, ulong U>
	inline _ovfw_hidden_::EnableForUInt<T, T, Vec<bool,I>>  SubtractionIsSafe (const Vec<T,I,U> a, const Vec<T,I,U> b)
	{
		Vec<bool,I>		res;
		FOR( i, res )	res[i] = SubtractionIsSafe( a[i], b[i] );
		return res;
	}


}	// OverflowChecker
}	// GXMath
}	// GX_STL
