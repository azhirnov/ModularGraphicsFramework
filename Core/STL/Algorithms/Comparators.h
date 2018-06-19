// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/Array.h"
#include "Core/STL/Containers/StaticArray.h"
#include "Core/STL/Algorithms/Sorts.h"
#include "Core/STL/Math/Vec.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Comparator (default)
	//

	template <typename T>
	struct Comparator
	{
	public:
		using Self	= Comparator<T>;

	private:
		T const&	_ref;

	public:
		forceinline explicit Comparator (const T &ref) : _ref(ref) {}

		ND_ forceinline bool operator == (const Self &right) const		{ return _ref == right._ref; }
		ND_ forceinline bool operator >  (const Self &right) const		{ return _ref >  right._ref; }
		ND_ forceinline bool operator <  (const Self &right) const		{ return right._ref >  _ref; }

		ND_ forceinline bool operator != (const Self &right) const		{ return not (*this == right); }
		ND_ forceinline bool operator >= (const Self &right) const		{ return not (*this <  right); }
		ND_ forceinline bool operator <= (const Self &right) const		{ return not (*this >  right); }
	};


	
	//
	// Comparator (vec)
	//

	template <typename T, usize I, ulong U>
	struct Comparator< GXMath::Vec<T,I,U> >
	{
	public:
		using Self	= Comparator< GXMath::Vec<T,I,U> >;

	private:
		GXMath::Vec<T,I,U> const&	_ref;

	public:
		forceinline explicit Comparator (const GXMath::Vec<T,I,U> &ref) : _ref(ref)
		{}

		ND_ forceinline bool operator == (const Self &right) const
		{
			return GXMath::All( _ref == right._ref );
		}

		ND_ forceinline bool operator > (const Self &right) const
		{
			for (usize i = 0; i < I-1; ++i) {
				if ( _ref[i] != right._ref[i] )
					return _ref[i] > right._ref[i];
			}
			return _ref[I-1] > right._ref[I-1];
		}

		ND_ forceinline bool operator <  (const Self &right) const		{ return (right > *this); }
		ND_ forceinline bool operator != (const Self &right) const		{ return not (*this == right); }
		ND_ forceinline bool operator >= (const Self &right) const		{ return not (*this <  right); }
		ND_ forceinline bool operator <= (const Self &right) const		{ return not (*this >  right); }
	};


	
	//
	// Sort Compare
	//

	template <typename T>
	struct TSortCmp< Comparator<T> >
	{
		// sort from smallest to biggest
		forceinline bool operator () (const T& val1, const T& val2)	const
		{
			return Comparator<T>(val1) > Comparator<T>(val2);
		}
	};


}	// GXTypes
}	// GX_STL
