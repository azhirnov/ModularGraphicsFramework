// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Helpers for range based 'for' loop.
*/

#pragma once

#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Algorithms/ArrayUtils.h"
#include "Engine/STL/Containers/IndexedIterator.h"
#include "Engine/STL/CompileTime/FunctionInfo.h"

namespace GX_STL
{
namespace GXTypes
{

	namespace _types_hidden_
	{
		GX_CREATE_MEMBER_DETECTOR( Begin );
		GX_CREATE_MEMBER_DETECTOR( End );
		
		GX_CREATE_MEMBER_DETECTOR( begin );
		GX_CREATE_MEMBER_DETECTOR( end );

		template <typename T>
		static constexpr bool	HasBeginEnd = CompileTime::SwitchType< not IsStaticArray<T>::value,
													CompileTime::DeferredAnd< typename CompileTime::DeferredTemplate< Detect_Begin, T >,
																			  typename CompileTime::DeferredTemplate< Detect_End,   T > >,
													CompileTime::ValueToType< bool, false >
												>::value;
		
		template <typename T>
		static constexpr bool	HasBeginEnd2 = CompileTime::SwitchType< not IsStaticArray<T>::value,
													CompileTime::DeferredAnd< typename CompileTime::DeferredTemplate< Detect_begin, T >,
																			  typename CompileTime::DeferredTemplate< Detect_end,   T > >,
													CompileTime::ValueToType< bool, false >
												>::value;

		template <typename T>
		using HasNotBeginEnd_t	= CompileTime::SwitchType< not HasBeginEnd<T>, int, void >;



		//
		// Reverse Iterator
		//
		struct _ReverseIterator
		{
			template <typename T>
			struct Iter
			{
				T	_ptr;

				explicit Iter (T ptr) : _ptr{ptr} {}

				Iter<T>& operator ++ ()					{ --_ptr;  return *this; }
				
				decltype(auto) operator * ()			{ return *_ptr; }

				bool operator == (Iter<T> right) const	{ return _ptr == right._ptr; }
				bool operator != (Iter<T> right) const	{ return _ptr != right._ptr; }
			};

			template <typename T>
			static auto Make (T ptr) {
				return Iter<T>{ ptr };
			}
		};


		//
		// Reverse Result
		//
		struct _ReverseResult
		{
			template <typename Res>
			struct Internal
			{
				Res		_result;

				Internal (Res &&res) : _result{res} {}

				decltype(auto)	begin ()		{ return _ReverseIterator::Make( _result.end()-1 ); }
				decltype(auto)	end ()			{ return _ReverseIterator::Make( _result.begin()-1 ); }
			};

			template <typename Res>
			static auto Make (Res &&res) {
				return Internal<Res>{ RVREF(res) };
			}
		};


		//
		// Range Reuslt
		//
		template <typename Arr>
		struct _RangeResult
		{
			Arr&	_arr;

			_RangeResult (Arr &arr) : _arr(arr) {}

			decltype(auto)	begin ()		{ return _arr.Begin(); }
			auto			end ()			{ return _arr.End(); }
		};
		
		template <typename Arr>
		struct _RangeResult< const Arr >
		{
			Arr const&	_arr;

			_RangeResult (const Arr &arr) : _arr(arr) {}
			
			decltype(auto)	begin () const	{ return _arr.Begin(); }
			auto			end ()	 const	{ return _arr.End(); }
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

			decltype(auto)	begin ()		{ return IndexedIterator<TypeTraits::RemoveAnyReference<decltype(_arr[0])>>{ _arr, _first }; }
			auto			end ()			{ auto i = IndexedIterator<TypeTraits::RemoveAnyReference<decltype(_arr[0])>>{ _arr, _last };  return ++i; }
		};
		
		template <typename Arr>
		struct _IndexRangeResult< const Arr >
		{
			Arr const&	_arr;
			const usize	_first;
			const usize	_last;

			_IndexRangeResult (const Arr &arr, usize first, usize last) : _arr(arr), _first(first), _last(last) {}
			
			decltype(auto)	begin ()		{ return IndexedIterator<TypeTraits::RemoveAnyReference<decltype(_arr[0])>>{ _arr, _first }; }
			auto			end ()			{ auto i = IndexedIterator<TypeTraits::RemoveAnyReference<decltype(_arr[0])>>{ _arr, _last };  return ++i; }
		};

		template <typename T>
		struct _IndexRangeResult< T* >
		{
			using Iter = IndexedIterator< T >;

			T *			_arr;
			const usize	_first;
			const usize	_last;

			_IndexRangeResult (T *arr, usize first, usize last) : _arr(arr), _first(first), _last(last) {}
			
			decltype(auto)	begin ()		{ return Iter{ _arr, _first }; }
			auto			end ()			{ auto i = Iter{ _arr, _last };  return ++i; }
		};

		template <typename T>
		struct _IndexRangeResult< T const* >
		{
			using Iter = IndexedIterator< const T >;

			T const *	_arr;
			const usize	_first;
			const usize	_last;

			_IndexRangeResult (T const *arr, usize first, usize last) : _arr(arr), _first(first), _last(last) {}
			
			decltype(auto)	begin ()		{ return Iter{ _arr, _first }; }
			auto			end ()			{ auto i = Iter{ _arr, _last };  return ++i; }
		};
		
/*
=================================================
	_Range1
=================================================
*/
		template <typename Arr>
		forceinline decltype(auto) _Range1 (Arr &container)
		{
			return _RangeResult< Arr >( container );
		}

		template <typename Arr>
		forceinline decltype(auto) _Range1v (Arr &container)
		{
			return container;	// already have 'begin' 'end' methods
		}

		template <typename Arr>
		forceinline decltype(auto) _Range1i (Arr &container)
		{
			return _IndexRangeResult< Arr >( container, 0, CountOf( container )-1 );
		}

/*
=================================================
	_Range3
=================================================
*/
		template <typename Arr>
		forceinline decltype(auto) _Range3 (Arr &container, const usize inFirst, const usize inCount)
		{
			const usize	max_count	= CountOf( container );
			const usize	first		= inFirst < max_count ? inFirst : max_count-1;
			const usize	last		= (inCount >= max_count or inCount+first >= max_count) ? max_count : inCount+first;

			return _IndexRangeResult< Arr >( container, first, last-1 );
		}

	}	// _types_hidden_
	
/*
=================================================
	Range
=================================================
*/
	template <typename Arr>
	ND_ forceinline decltype(auto)  Range (Arr &container)
	{
		if_constexpr( _types_hidden_::HasBeginEnd<Arr> ) {
			return _types_hidden_::_Range1( container );
		} else
		if_constexpr( _types_hidden_::HasBeginEnd2<Arr> ) {
			return _types_hidden_::_Range1v( container );
		} else {
			return _types_hidden_::_Range1i( container );
		}
	}

	template <typename Arr>
	ND_ forceinline decltype(auto)  Range (Arr &container, const usize first, const usize count)
	{
		return _types_hidden_::_Range3( container, first, count );
	}
	
/*
=================================================
	RevRange
=================================================
*/
	template <typename Arr>
	ND_ forceinline decltype(auto)  RevRange (Arr &container)
	{
		if_constexpr( _types_hidden_::HasBeginEnd<Arr> ) {
			return _types_hidden_::_ReverseResult::Make( _types_hidden_::_Range1( container ) );
		} else {
		//if_constexpr( _types_hidden_::HasBeginEnd2<Arr> ) {
		//	return _types_hidden_::_Range1v( container );
		//} else {
			return _types_hidden_::_ReverseResult::Make( _types_hidden_::_Range1i( container ) );
		}
	}
	
	template <typename Arr>
	ND_ forceinline decltype(auto)  RevRange (Arr &container, const usize first, const usize count)
	{
		return _types_hidden_::_ReverseResult::Make( _types_hidden_::_Range3( container, first, count ) );
	}

}	// GX_STL
}	// GXTypes
