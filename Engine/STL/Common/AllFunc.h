// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/TypeId.h"
#include "Engine/STL/Math/MathFunc.h"
#include "Engine/STL/CompileTime/PlainOldDataType.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Type Info
	//

	template <typename T>				constexpr T		MinValue (const T& v = T());
	template <typename T>				constexpr T		MaxValue (const T& v = T());
	template <typename T>				constexpr T		Infinity ();
	template <typename T>				constexpr T		NaN ();
	template <typename T>				constexpr T		Epsilon ();

	template <typename T>				constexpr bool	IsNaN (const T& x);
	template <typename T>				constexpr bool	IsInfinity (const T& x);
	template <typename T>				constexpr bool	IsFinite (const T& x);



	//
	// Type Cast
	//

	template <typename R, typename T>	constexpr const R & ReferenceCast (const T & t);
	template <typename R, typename T>	constexpr R & ReferenceCast (T & t);
	
	template <typename R, typename T>	constexpr R Cast (const T& value);

	
	template <typename T, usize I>		constexpr usize	CountOf (const T (&arr)[I]);



	//
	// Enum Compare functions
	//
	template <typename T1, typename T2>
	constexpr bool  EnumEq (const T1& left, const T2& right);

	template <typename T1, typename T2, typename T3>
	constexpr bool  EnumEqMask (const T1& left, const T2& right, const T3& mask);
	
	template <typename T1, typename T2>
	constexpr bool  EnumAnyEq (const T1& left, const T2& right);



	//
	// Memory Operations
	//

	template <typename T, usize I>		inline void ZeroMem (T (&tArr)[I]) noexcept;
	template <typename T>				inline void ZeroMem (T &val) noexcept;
	
	template <typename T0, typename T1>	inline void MemCopy (T0 &dst, const T1 &src) noexcept;

	
}	// GXTypes
}	// GX_STL
