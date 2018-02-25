// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/OldTypeInfo.h"

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
}	// GX_STL
