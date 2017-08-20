// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"

namespace GX_STL
{
namespace CompileTime
{
	
	//
	// Order (to fix bug in VS 15.3.0)
	//
	template <typename T>
	using Order = T;



	//
	// Switch Type
	//
	
	namespace _ctime_hidden_
	{
		template <bool Switch, typename IfTrue, typename IfFalse>
		struct _SwitchType {
			typedef IfFalse	type;
		};

		template <typename IfTrue, typename IfFalse>
		struct _SwitchType< true, IfTrue, IfFalse > {
			typedef IfTrue	type;
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
		typedef ValueToType< T, Value >		Self;

		typedef T		type;
		static const T	value = Value;
	};



	//
	// Switch Value
	//

	template <bool Switch, typename IfTrue, typename IfFalse>
	struct SwitchValue {
		typedef typename IfFalse::type	type;
		static const type	value = IfFalse::value;
	};

	template <typename IfTrue, typename IfFalse>
	struct SwitchValue< true, IfTrue, IfFalse > {
		typedef typename IfTrue::type	type;
		static const type	value = IfTrue::value;
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
			typedef T	type;
		};

		template <typename T>
		struct _EnableIf <false, T> {
			typedef void	type;
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
		typedef Pair< FirstType, First, SecondType, Second >	Self;

		typedef FirstType		first_t;
		typedef SecondType		second_t;

		static const first_t	first	= First;
		static const second_t	second	= Second;
	};



	//
	// Is Base Of
	//

	namespace _ctime_hidden_
	{
		template <typename B, typename D>
		struct _IsBaseOf_Host
		{
		  operator B*() const;
		  operator D*();
		};

		template <typename Base, typename Derived, bool IsRef>
		struct _IsBaseOf
		{
		private:
			typedef ubyte	_yes;
			typedef ushort	_no;

			template <typename T> 
			static _yes _check (Derived *, T);
			static _no  _check (Base *, int);

		public:
			static const bool	value = sizeof( _check( _IsBaseOf_Host< Base, Derived >(), int() ) ) == sizeof(_yes);
		};

		template <typename Base, typename Derived>
		struct _IsBaseOf< Base, Derived, true >
		{
			static const bool	value = false;
		};

	}	// _ctime_hidden_
	
	template <typename Base, typename Derived>
	static constexpr bool IsBaseOf	= _ctime_hidden_::_IsBaseOf< Base, Derived,
										TypeTraits::IsLValueReference<Base> or TypeTraits::IsLValueReference<Derived> >::value;



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
		typedef Templ< Types... >	type;
	};



}	// CompileTime
}	// GX_STL
