// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	For stl compatibility we need iterators :(
*/

#pragma once

#include "Core/STL/Containers/UniBuffer.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Indexed Iterator
	//

	template <typename T>
	struct IndexedIterator
	{
		STATIC_ASSERT( not TypeTraits::IsAnyReference<T> );

	// types
	public:
		using Self	= IndexedIterator<T>;


	// variables
	private:
		UniBuffer<T>	_ref;
		usize			_index	= 0;


	// methods
	public:
		IndexedIterator () {}
		IndexedIterator (IndexedIterator<T> &&) = default;
		IndexedIterator (const IndexedIterator<T> &) = default;

		IndexedIterator& operator = (IndexedIterator<T> &&) = default;
		IndexedIterator& operator = (const IndexedIterator<T> &) = default;

		template <typename Container,
				  typename = CompileTime::DisableIf<CompileTime::IsSameTypes<Self, Container>> >
		explicit IndexedIterator (Container &buf, usize index = 0) : _ref{buf}, _index{index} {}

		template <typename Container,
				  typename = CompileTime::DisableIf<CompileTime::IsSameTypes<Self, Container>> >
		explicit IndexedIterator (const Container &buf, usize index = 0) : _ref{UniBuffer<const T>(buf)}, _index{index} {}

		ND_ T *			operator -> ()					{ return &_ref[_index]; }
		ND_ T const *	operator -> ()	const			{ return &_ref[_index]; }
		
		ND_ T &			operator * ()					{ return _ref[_index]; }
		ND_ T const &	operator * ()	const			{ return _ref[_index]; }

			Self& operator ++ ()						{ ++_index;  return *this; }
			Self& operator -- ()						{ --_index;  return *this; }

			Self  operator ++ (int)						{ Self res{*this};  ++_index;  return res; }
			Self  operator -- (int)						{ Self res{*this};  --_index;  return res; }

			Self& operator += (usize value)				{ _index += value;  return *this; }
			Self& operator -= (usize value)				{ _index -= value;  return *this; }

		ND_ Self  operator +  (usize value) const		{ return Self(*this) += value; }
		ND_ Self  operator -  (usize value) const		{ return Self(*this) -= value; }

		ND_ bool operator == (const Self &right) const	{ return _ref.RawPtr() == right._ref.RawPtr() and _index == right._index; }
		ND_ bool operator != (const Self &right) const	{ return not (*this == right); }

		ND_ bool operator >= (const Self &right) const	{ return not (*this < right); }
		ND_ bool operator <= (const Self &right) const	{ return not (*this > right); }
		
		ND_ bool operator <  (const Self &right) const	{ return (right > *this); }
		ND_ bool operator >  (const Self &right) const
		{
			return	_ref.RawPtr() != right._ref.RawPtr() ?	_ref.RawPtr() > right._ref.RawPtr() :
															_index		  > right._index;
		}
	};


}	// GXTypes
}	// GX_STL
