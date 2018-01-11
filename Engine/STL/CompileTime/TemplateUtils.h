// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/CompileTimeTypes.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Switch Type
	//
	
	namespace _ctime_hidden_
	{
		template <bool Switch, typename IfTrue, typename IfFalse>
		struct _SwitchType {
			using type	= IfFalse;
		};

		template <typename IfTrue, typename IfFalse>
		struct _SwitchType< true, IfTrue, IfFalse > {
			using type	= IfTrue;
		};
	}
	
	template <bool Switch, typename IfTrue, typename IfFalse>
	using SwitchType = typename _ctime_hidden_::_SwitchType< Switch, IfTrue, IfFalse >::type;



	//
	// Value To Type in compile time
	//

	template <typename T, T Value>
	struct ValueToType
	{
		using Self	= ValueToType< T, Value >;
		using type	= T;

		static const T	value = Value;
	};



	//
	// Switch Value
	//

	template <bool Switch, typename IfTrue, typename IfFalse>
	struct SwitchValue {
		using type					= typename IfFalse::type;
		static const type	value	= IfFalse::value;
	};

	template <typename IfTrue, typename IfFalse>
	struct SwitchValue< true, IfTrue, IfFalse > {
		using type					= typename IfTrue::type;
		static const type	value	= IfTrue::value;
	};



	//
	// Static Size Of
	//

	template <typename T>
	struct SizeOf
	{
		static const usize	bytes	= sizeof(T);
		static const usize	bits	= bytes << 3;
	};
	
	template <>
	struct SizeOf <void>
	{
		static const usize	bytes	= 0;
		static const usize	bits	= 0;
	};



	//
	// Static Align Of
	//

	template <typename T>
	struct AlignOf
	{
		static const usize	bytes	= alignof(T);
		static const usize	bits	= bytes << 3;
	};

	template <>
	struct AlignOf <void>
	{
		static const usize	bytes	= 0;
		static const usize	bits	= 0;
	};



	//
	// Enable / Disable If
	//
	
	namespace _ctime_hidden_
	{
		template <bool Condition, typename T = void>
		struct _EnableIf {
			using type	= T;
		};

		template <typename T>
		struct _EnableIf <false, T> {
			using type	= void;
		};
	}

	template <bool Condition, typename T = void>
	using EnableIf = typename _ctime_hidden_::_EnableIf< Condition, T >::type;

	template <bool Condition, typename T = void>
	using DisableIf = typename _ctime_hidden_::_EnableIf< not Condition, T >::type;

	

	//
	// Pair
	//

	template <	typename FirstType, FirstType First,
				typename SecondType, SecondType Second
			 >
	struct Pair
	{
		using Self		= Pair< FirstType, First, SecondType, Second >;
		using First_t	= FirstType;
		using Second_t	= SecondType;

		static const First_t	first	= First;
		static const Second_t	second	= Second;
	};


	//
	// Must Be Same Size
	//

	namespace _ctime_hidden_
	{
		// to show sizes in build log
		template <usize LeftSize, usize RightSize>
		struct MustBeSameSizeImpl {
			STATIC_ASSERT( LeftSize == RightSize );
		};
	}

	template <typename LeftType, typename RightType>
	struct MustBeSameSize : _ctime_hidden_::MustBeSameSizeImpl< sizeof(LeftType), sizeof(RightType) >
	{};



	//
	// Deferred Template Type
	//

	/*
		example:

			template <typename T> struct Templ {};

			// switch type without template instantiation
			using D = SwitchType< condition, DeferredTemplate< Templ, T1 >, DeferredTemplate< Templ, T2 > >;

			// instantiate only 1 template
			using T = D::type;
	*/

	template <template <typename ...> class Templ, typename ...Types>
	struct DeferredTemplate
	{
		using type	= Templ< Types... >;
	};


	//
	// Type to Type
	//
	template <typename T>
	struct TypeToType
	{
		using type = T;
	};


	//
	// Offset Of
	//
	/*template <typename A, typename B>
	inline usize constexpr Offsetof (A B::*member)
	{
		constexpr B object {};
		return usize(&(object.*member)) - usize(&object);
	}*/


}	// CompileTime
}	// GX_STL
