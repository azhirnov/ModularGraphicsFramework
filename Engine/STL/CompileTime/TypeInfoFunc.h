// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "NewTypeInfo.h"
#include "Engine/STL/Common/AllFunc.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	MinValue
=================================================
*/
	template <typename T>
	constexpr T MinValue (const T&)
	{
		return CompileTime::TypeDescriptor::template GetTypeInfo<T>::Min();
	}
	
/*
=================================================
	MaxValue
=================================================
*/
	template <typename T>
	constexpr T MaxValue (const T&)
	{
		return CompileTime::TypeDescriptor::template GetTypeInfo<T>::Max();
	}
	
/*
=================================================
	Epsilon
=================================================
*/
	template <typename T>
	constexpr T Epsilon ()
	{
		return CompileTime::TypeDescriptor::template GetTypeInfo<T>::Epsilon();
	}
	
/*
=================================================
	Infinity
=================================================
*/
	template <typename T>
	constexpr T  Infinity ()
	{
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		return CompileTime::TypeDescriptor::template GetTypeInfo<T>::Inf();
	}
	
/*
=================================================
	NaN
=================================================
*/
	template <typename T>
	constexpr T  NaN ()
	{
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		return CompileTime::TypeDescriptor::template GetTypeInfo<T>::NaN();
	}
	
/*
=================================================
	IsNaN
=================================================
*/
	template <typename T>
	constexpr bool IsNaN (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		return ( x != x );
	}
	
/*
=================================================
	IsInfinity
=================================================
*/
	template <typename T>
	constexpr bool IsInfinity (const T& x)
	{
		STATIC_ASSERT( CompileTime::IsFloat<T> );
		return ( x == Infinity<T>() or x == -Infinity<T>() );
	}
	
/*
=================================================
	IsFinite
=================================================
*/
	namespace _types_hidden_
	{
		template <typename T, bool IsFloat>
		struct _IsFinite
		{
			static constexpr bool Get (const T& x)
			{
				return std::isfinite( x );
				//return ( x == x and x != Infinity<T>() and x != -Infinity<T>() );
			}
		};

		template <typename T>
		struct _IsFinite< T, false >
		{
			static constexpr bool Get (const T& x)
			{
				return true;
			}
		};

	}	// _types_hidden_

	template <typename T>
	constexpr bool IsFinite (const T& x)
	{
		return _types_hidden_::_IsFinite< T, CompileTime::IsFloat<T> >::Get( x );
	}

}	// GXTypes
}	// GX_STL
