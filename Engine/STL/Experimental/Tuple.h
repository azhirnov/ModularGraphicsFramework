// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/CompileTime/TypeListHelpers.h"
#include <tuple>

namespace GX_STL
{
namespace GXTypes
{
	
	//
	// Tuple
	//

	template <typename ...Types>
	struct Tuple : public CompileTime::CopyQualifiers< Types... >
	{
	// types
	public:
		typedef Tuple< Tuple >	Self;


	// variables
	public:


	// methods
	public:
		Tuple (GX_DEFCTOR) : first(), second() {}

		Tuple (const Self &other) : first(other.first), second(other.second) {}

		Tuple (Self &&other) : first( RVREF(other.first) ), second( RVREF(other.second) ) {}
		
		Self &	operator =  (Self &&right)		= default;
		Self &	operator =  (const Self &right)	= default;

		bool	operator == (const Self &right) const	{}

		bool	operator != (const Self &right)	const	{ return not (*this == right); }


		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			//SwapValues( left.first,		right.first );
			//SwapValues( left.second,	right.second );
		}
	};

	
	/*template <typename FirstType, typename SecondType>
	struct Hash< Pair< FirstType, SecondType > > :
		private Hash< FirstType >, private Hash< SecondType >
	{
		STATIC_ASSERT(( CompileTime::IsSameTypes< typename Hash< FirstType >::result_t,
													typename Hash< SecondType >::result_t > ));

		typedef Pair< FirstType, SecondType >	key_t;
		typedef Hash< FirstType >::result_t		result_t;

		result_t operator () (const key_t &x) const
		{
			return	(Hash< FirstType >::operator ()( x.first ) << 2) ^
					Hash< SecondType >::operator ()( x.second );
		}
	};*/

}	// GX_STL
}	// GXTypes
