// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OldTypeInfo.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Is<Type>
	//

	template <typename T>
	static constexpr bool IsFloat 		= TypeDescriptor::IsFloat<T>::value;
	
	template <typename T>
	static constexpr bool IsInteger 	= TypeDescriptor::IsInteger<T>::value;

	template <typename T>
	static constexpr bool IsSigned 		= TypeDescriptor::IsSigned<T>::value;

	template <typename T>
	static constexpr bool IsUnsigned 	= TypeDescriptor::IsUnsigned<T>::value;

	template <typename T>
	static constexpr bool IsFixed		= TypeDescriptor::IsFixed<T>::value;

	template <typename T>
	static constexpr bool IsSoftware	= TypeDescriptor::IsSoftware<T>::value;

	template <typename T>
	static constexpr bool IsWrapper		= TypeDescriptor::IsWrapper<T>::value;

	template <typename T>
	static constexpr bool IsVoid		= TypeDescriptor::IsVoid<T>::value;

	template <typename T>
	static constexpr bool IsArithmetic	= TypeDescriptor::IsArithmetic<T>::value;

	template <typename T>
	static constexpr bool IsScalar		= TypeDescriptor::IsScalar<T>::value and IsArithmetic<T>;

	template <typename T>
	static constexpr bool IsVector		= TypeDescriptor::IsVector<T>::value and IsArithmetic<T>;

	template <typename T>
	static constexpr bool IsScalarOrEnum	= IsScalar<T> or TypeTraits::IsEnum<T>;



	//
	// IsSame***
	//
	
	// Strong comparison
	template <typename T1, typename T2>
	static constexpr bool IsSameTypes					= TypeDescriptor::IsSameTypes<T1, T2>::value;


	// Weak comparison
	template <typename T1, typename T2>
	static constexpr bool IsSameTypesWithoutQualifiers	= TypeDescriptor::IsSameTypesWithoutQualifiers<T1, T2>::value;
	

	template <template <typename ...> class Left, template <typename ...> class Right>
	static constexpr bool IsSameTemplates				= TypeDescriptor::IsSameTemplates<Left, Right>::value;
	


	//
	// MustBe <Type>
	//

	template <typename T>
	struct MustBeInteger {
		STATIC_ASSERT( IsInteger<T> );
	};

	template <typename T>
	struct MustBeUnsignedInteger : MustBeInteger<T> {
		STATIC_ASSERT( IsUnsigned<T> );
	};

	template <typename T>
	struct MustBeSignedInteger : MustBeInteger<T> {
		STATIC_ASSERT( IsSigned<T> );
	};

	template <typename T>
	struct MustBeFloat {
		STATIC_ASSERT( IsFloat<T> );
	};

	template <typename T>
	struct MustBePOD {
		STATIC_ASSERT( IsPOD<T> );
	};
	
	template <typename LeftType, typename RightType>
	struct MustBeSameTypes {
		STATIC_ASSERT( (IsSameTypes<LeftType, RightType>) );
	};


}	// CompileTime

// TODO: move out from CompileTime
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
