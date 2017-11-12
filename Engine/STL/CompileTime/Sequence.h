// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "NewTypeInfo.h"
#include "TypeListHelpers.h"
#include "Engine/STL/Types/Noninstancable.h"


namespace GX_STL
{
namespace CompileTime
{

	//
	// Sequence
	//

	template <typename T, T ...Values>
	struct Sequence
	{
	// types
		using Self			= Sequence< T, Values... >;
		using Value_t		= T;
		using ValueList_t	= ValueListFrom< T, Values... >;

		struct _Iterator
		{
			usize	_index;

			constexpr explicit _Iterator (usize i = 0) : _index(i)		{}
			//constexpr  operator usize () const					{ return { Values...}[_index]; }
			constexpr T		operator * () const							{ return *((InitializerList<T>{ Values...}).begin() + _index); }
			_Iterator		operator ++ ()								{ ++_index;  return *this; }
			_Iterator		operator ++ (int)							{ _Iterator res(*this);  ++(*this);  return res; }
			constexpr bool	operator == (const _Iterator &right) const	{ return _index == right._index; }
			constexpr bool	operator != (const _Iterator &right) const	{ return not (*this == right); }
		};


	// constants
		constexpr static usize	Count	= sizeof...(Values);

		template <usize Index>
		constexpr static T		Get		= ValueList_t::Get< Index >::value;


	// methods

		// for 'range based for'
		constexpr _Iterator	begin () const					{ return _Iterator(0); }
		constexpr _Iterator	end () const					{ return _Iterator(Count); }

		constexpr T			operator [] (usize index) const	{ ASSERT(index < Count); return *_Iterator(index); }
	};
	


	//
	// Sequence Range
	//

	namespace _ctime_hidden_
	{
		template <typename T, T FirstVal, T LastVal, bool Stop, T ...Values>
		struct _SequenceRange
		{
			using type	= typename _SequenceRange< T, FirstVal+1, LastVal, /*Stop:*/FirstVal+1 >= LastVal, Values..., FirstVal+1 >::type;
		};

		template <typename T, T FirstVal, T LastVal, T ...Values>
		struct _SequenceRange< T, FirstVal, LastVal, true, Values... >
		{
			using type	= Sequence< T, Values... >;
		};

	}	// _ctime_hidden_


	template <typename T, T First, T Last>
	struct SequenceRange : _ctime_hidden_::_SequenceRange< T, First, Last, /*Stop:*/First >= Last, First >::type
	{
		STATIC_ASSERT( (Last - First) >= T(0) );
		STATIC_ASSERT( (Last - First) != ~T(0) );
	};



	//
	// Index Sequence
	//

	template <usize ...I>
	using IndexSequence				= Sequence< usize, I... >;

	template <usize First, usize Last>
	using IndexSequenceFromRange	= SequenceRange< usize, First, Last >;

	template <usize Count>
	using IndexSequenceFromSize		= IndexSequenceFromRange< 0u, Count-1 >;

	template <typename ...Types>
	using IndexSequenceFor			= IndexSequenceFromSize< sizeof...(Types) >;


}	// CompileTime
}	// GX_STL
