// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Init.h"

namespace GX_STL
{
namespace GXTypes
{
	
	struct UninitializedType;


	namespace _types_hidden_
	{
		template <typename T, bool IsEnum>
		struct _IsEnumWithUnknown2 {
			static const bool	value = false;
		};

		template <typename T>
		struct _IsEnumWithUnknown2< T, true > {
			static const bool	value = true; //Detect_Unknown<T>::value;
		};

		template <typename T>
		static constexpr bool	_IsEnumWithUnknown = _IsEnumWithUnknown2< T, std::is_enum<T>::value >::value;


		template <typename T, int Index>
		struct _GetDefaultValueForUninitialized2 {};

		template <typename T>
		struct _GetDefaultValueForUninitialized2< T, 0 > {
			static T Get ()		{ return UninitializedType(); }
		};

		template <typename T>
		struct _GetDefaultValueForUninitialized2< T, /*int, float, pointer*/2 > {
			static T Get ()		{ return T(0); }
		};
		
		template <typename T>
		struct _GetDefaultValueForUninitialized2< T, /*enum*/1 > {
			static T Get ()		{ return T::Unknown; }
		};

		template <typename T>
		struct _GetDefaultValueForUninitialized
		{
			static constexpr int GetIndex ()
			{
				return	_IsEnumWithUnknown<T>  ? 1 :
							std::is_floating_point<T>::value or
							std::is_integral<T>::value		 or
							std::is_pointer<T>::value		 or
							std::is_enum<T>::value  ? 2 :
								0;
			}

			static T GetDefault ()
			{
				constexpr int	idx = GetIndex();

				//STATIC_ASSERT( idx != 0, "Not supported for this type!" );

				return _GetDefaultValueForUninitialized2< T, idx >::Get();
			}
		};


	}	// _types_hidden_


	struct UninitializedType
	{
		template <typename T>
		operator T () const
		{
			return _types_hidden_::_GetDefaultValueForUninitialized<T>::GetDefault();
		}
	};


	static const UninitializedType	Uninitialized = UninitializedType();



	//
	// Uninitialized Template
	// used for template constructors without arguments
	//

	template <typename T>
	struct UninitializedT
	{
		constexpr UninitializedT ()
		{}
	};


}	// GXTypes
}	// GX_STL
