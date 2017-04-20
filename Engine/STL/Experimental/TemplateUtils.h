// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"

namespace GX_STL
{
namespace CompileTime
{


	//
	// And
	//

	template <bool L, typename R>
	struct And {
		static const bool	value = L and R::value;
	};

	template <typename R>
	struct And< false, R > {
		static const bool	value = false;
	};



	//
	// ValueIf
	//

	template <bool Condition, typename T, typename Value>
	struct ValueIf {
		static const T	value = Value::value;
	};
	
	template <typename T, typename Value>
	struct ValueIf< false, T, Value > {
		static const T	value = T(0);
	};

	

	//
	// AddIf
	//
	
	template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
	struct AddIf {
		static const T	value = Left + Right;
	};
	
	template <typename T, T Left, T Right, T Default>
	struct AddIf< false, T, Left, Right, Default > {
		static const T	value = Default;
	};

	

	//
	// SubIf
	//
	
	template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
	struct SubIf {
		static const T	value = Left - Right;
	};
	
	template <typename T, T Left, T Right, T Default>
	struct SubIf< false, T, Left, Right, Default > {
		static const T	value = Default;
	};

	

	//
	// MulIf
	//
	
	template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
	struct MulIf {
		static const T	value = Left * Right;
	};
	
	template <typename T, T Left, T Right, T Default>
	struct MulIf< false, T, Left, Right, Default > {
		static const T	value = Default;
	};

	

	//
	// DivIf
	//
	
	template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
	struct DivIf {
		static const T	value = Left / Right;
	};
	
	template <typename T, T Left, T Right, T Default>
	struct DivIf< false, T, Left, Right, Default > {
		static const T	value = Default;
	};



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
										TypeTraits::IsReference<Base> or TypeTraits::IsReference<Derived> >::value;



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


}	// CompileTime
}	// GX_STL
