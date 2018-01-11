// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/AllFunc.h"
#include "Engine/STL/Defines/MemberDetector.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	CountOf
=================================================
*/
	template <typename T, usize I>
	constexpr forceinline usize  CountOf (const T (&)[I])
	{
		return I;
	}
	
	template <typename T, T First, T ...Values>
	constexpr forceinline usize  CountOf ()
	{
		return sizeof...(Values) + 1;
	}
	
	template <typename ...Types>
	constexpr forceinline usize  CountOf ()
	{
		return sizeof...(Types);
	}
	
/*
=================================================
	CountOf (Any Container)
=================================================
*/
	namespace _types_hidden_
	{
		GX_CREATE_MEMBER_DETECTOR( Count );
		GX_CREATE_MEMBER_DETECTOR( size );

		template <typename T>			struct IsStaticArray			{ static const bool  value = false; };
		template <typename T>			struct IsStaticArray< T[] >		{ static const bool  value = true; };
		template <typename T, usize I>	struct IsStaticArray< T[I] >	{ static const bool  value = true; };

		template <typename T>
		static constexpr bool	HasCountMethod = CompileTime::SwitchType< not IsStaticArray<T>::value,
														typename CompileTime::DeferredTemplate< Detect_Count, T >,
														typename CompileTime::TypeToType< CompileTime::ValueToType< bool, false >> >::type::value;
		
		template <typename T>
		static constexpr bool	HasStdSizeMethod = CompileTime::SwitchType< not IsStaticArray<T>::value,
														typename CompileTime::DeferredTemplate< Detect_size, T >,
														typename CompileTime::TypeToType< CompileTime::ValueToType< bool, false >> >::type::value;
	}	// _types_hidden_

	template <typename Arr>
	constexpr forceinline usize CountOf (const Arr &arr, CompileTime::EnableIf< _types_hidden_::HasCountMethod<Arr>, int > = 0)
	{
		return arr.Count();
	}

	template <typename Arr>
	constexpr forceinline usize CountOf (const Arr &arr, CompileTime::EnableIf< _types_hidden_::HasStdSizeMethod<Arr>, int > = 0)
	{
		return arr.size();
	}

}	// GXTypes
}	// GX_STL
