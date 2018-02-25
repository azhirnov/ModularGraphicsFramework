// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TemplateUtils.h"
#include "Engine/STL/CompileTime/TypeTraits.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Base POD Type
	//

	struct PODType
	{};

	namespace _ctime_hidden_
	{
		struct NonPODType
		{};

		template <typename T, bool CheckBase>
		struct _IsPodDeepImpl
		{
			static const bool	value =	IsBaseOf< PODType, T > or std::is_pod< T >::value;
		};
		
		template <typename T>
		struct _IsPodDeepImpl< T, false >
		{
			static const bool	value =	true;
		};

		template <typename T>
		struct _IsPOD
		{
			static const bool	value =	_IsPodDeepImpl< T, not ( TypeTraits::IsPointer< T >					or
																 TypeTraits::IsLValueReference< T >			or
																 TypeTraits::IsMemberVariablePointer< T >	or
																 TypeTraits::IsMemberFunctionPointer< T >	or
																 TypeTraits::IsFunctionPointer< T >			or
																 TypeTraits::IsEnum< T > ) >::value;
		};
	
		template <typename T>
		struct _IsPOD< const T >
		{
			static const bool	value = _IsPOD<T>::value;
		};
	
		template <typename T>
		struct _IsPOD< volatile T >
		{
			static const bool	value = _IsPOD<T>::value;
		};

		template <typename T, usize I>
		struct _IsPOD< T[I] >
		{
			static const bool	value = _IsPOD<T>::value;
		};
		
		template <typename T, usize I>
		struct _IsPOD< T const [I] >
		{
			static const bool	value = _IsPOD<T>::value;
		};
		
		template <typename T, usize I>
		struct _IsPOD< T volatile [I] >
		{
			static const bool	value = _IsPOD<T>::value;
		};
		
		template <typename T, usize I>
		struct _IsPOD< T const volatile [I] >
		{
			static const bool	value = _IsPOD<T>::value;
		};
	
		template <>	struct _IsPOD< PODType >	{ static const bool	value = true; };
		template <>	struct _IsPOD< bool >		{ static const bool	value = true; };
		template <>	struct _IsPOD< char >		{ static const bool	value = true; };
		template <>	struct _IsPOD< wchar >		{ static const bool	value = true; };
		template <>	struct _IsPOD< byte >		{ static const bool	value = true; };
		template <>	struct _IsPOD< ubyte >		{ static const bool	value = true; };
		template <>	struct _IsPOD< short >		{ static const bool	value = true; };
		template <>	struct _IsPOD< ushort >		{ static const bool	value = true; };
		template <>	struct _IsPOD< int >		{ static const bool	value = true; };
		template <>	struct _IsPOD< uint >		{ static const bool	value = true; };
		template <>	struct _IsPOD< ilong >		{ static const bool	value = true; };
		template <>	struct _IsPOD< ulong >		{ static const bool	value = true; };
		template <>	struct _IsPOD< float >		{ static const bool	value = true; };
		template <>	struct _IsPOD< double >		{ static const bool	value = true; };

	}	// _ctime_hidden_


	template <typename T>
	static constexpr bool IsPOD = _ctime_hidden_::_IsPOD<T>::value;


}	// CompileTime
}	// GX_STL
