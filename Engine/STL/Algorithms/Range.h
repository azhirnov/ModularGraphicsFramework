// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Helpers for range based 'for' loop.
*/

#pragma once

#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Algorithms/ArrayUtils.h"

namespace GX_STL
{
namespace GXTypes
{

	namespace _types_hidden_
	{
		GX_CREATE_MEMBER_DETECTOR( Begin );
		GX_CREATE_MEMBER_DETECTOR( End );

		template <typename T>
		static constexpr bool	HasBeginEnd = CompileTime::SwitchType< not IsStaticArray<T>::value,
													CompileTime::DeferredAnd< typename CompileTime::DeferredTemplate< Detect_Begin, T >,
																			  typename CompileTime::DeferredTemplate< Detect_End,   T > >,
													CompileTime::ValueToType< bool, false >
												>::value;

		template <typename T>
		using HasNotBeginEnd_t	= CompileTime::SwitchType< not HasBeginEnd<T>, int, void >;


		//
		// Range Reuslt
		//
		template <typename Arr>
		struct _RangeResult
		{
			Arr&	_arr;

			_RangeResult (Arr &arr) : _arr(arr) {}

			decltype(auto)	begin ()		{ return _arr.Begin(); }
			decltype(auto)	end ()			{ return _arr.End(); }
		};
		
		template <typename Arr>
		struct _RangeResult< const Arr >
		{
			Arr const&	_arr;

			_RangeResult (const Arr &arr) : _arr(arr) {}

			decltype(auto)	begin () const	{ return _arr.Begin(); }
			decltype(auto)	end ()	 const	{ return _arr.End(); }
		};


		//
		// Index Range Reuslt
		//
		template <typename Arr>
		struct _IndexRangeResult
		{
			Arr&		_arr;
			const usize	_first;
			const usize	_last;

			_IndexRangeResult (Arr &arr, usize first, usize last) : _arr(arr), _first(first), _last(last) {}

			decltype(auto)	begin ()		{ return &_arr[ _first ]; }
			decltype(auto)	end ()			{ return &_arr[ _last ]; }
		};
		
		template <typename Arr>
		struct _IndexRangeResult< const Arr >
		{
			Arr const&	_arr;
			const usize	_first;
			const usize	_last;

			_IndexRangeResult (const Arr &arr, usize first, usize last) : _arr(arr), _first(first), _last(last) {}

			decltype(auto)	begin () const	{ return &_arr[ _first ]; }
			decltype(auto)	end ()	 const	{ return &_arr[ _last ]; }
		};

		template <typename T>
		struct _IndexRangeResult< T* >
		{
			T *			_arr;
			const usize	_first;
			const usize	_last;

			_IndexRangeResult (T *arr, usize first, usize last) : _arr(arr), _first(first), _last(last) {}

			decltype(auto)	begin ()		{ return &_arr[ _first ]; }
			decltype(auto)	end ()			{ return &_arr[ _last ]; }
		};

		template <typename T>
		struct _IndexRangeResult< T const* >
		{
			T const *	_arr;
			const usize	_first;
			const usize	_last;

			_IndexRangeResult (T const *arr, usize first, usize last) : _arr(arr), _first(first), _last(last) {}

			decltype(auto)	begin ()		{ return &_arr[ _first ]; }
			decltype(auto)	end ()			{ return &_arr[ _last ]; }
		};

	}	// _types_hidden_
	
/*
=================================================
	Range
=================================================
*/
	template <	typename Arr,
				typename = CompileTime::EnableIf< _types_hidden_::HasBeginEnd<Arr> >
			 >
	CHECKRES forceinline decltype(auto)  Range (Arr &container)
	{
		return _types_hidden_::_RangeResult< Arr >( container );
	}

	template <	typename Arr,
				typename = CompileTime::EnableIf< _types_hidden_::HasBeginEnd<Arr> >
			 >
	CHECKRES forceinline decltype(auto)  Range (const Arr &container)
	{
		return _types_hidden_::_RangeResult< const Arr >( container );
	}
	
/*
=================================================
	Range
=================================================
*/
	template <typename Arr>
	CHECKRES forceinline decltype(auto)  Range (Arr &container, usize first, usize count)
	{
		const usize		max_count = CountOf( container );
		return _types_hidden_::_IndexRangeResult< Arr >( container, GXMath::Min( first, max_count ), GXMath::Max( first + count, max_count ) );
	}
	
	template <typename Arr>
	CHECKRES forceinline decltype(auto)  Range (const Arr &container, usize first, usize count)
	{
		const usize		max_count = CountOf( container );
		return _types_hidden_::_IndexRangeResult< const Arr >( container, GXMath::Min( first, max_count ), GXMath::Max( first + count, max_count ) );
	}
	
/*
=================================================
	Range
=================================================
*
	template <	typename Arr,
				typename = CompileTime::EnableIf< not _types_hidden_::HasBeginEnd<Arr> >
			 >
	CHECKRES forceinline decltype(auto) Range (Arr &container)
	{
		return _types_hidden_::_IndexRangeResult< Arr >( container, 0, CountOf( container ) );
	}

	template <	typename Arr,
				typename = CompileTime::EnableIf< not _types_hidden_::HasBeginEnd<Arr> >
			 >
	CHECKRES forceinline decltype(auto) Range (const Arr &container)
	{
		return _types_hidden_::_IndexRangeResult< const Arr >( container, 0, CountOf( container ) );
	}
	*/

}	// GX_STL
}	// GXTypes
