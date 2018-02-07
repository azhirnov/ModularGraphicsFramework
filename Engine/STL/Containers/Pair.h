// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TypeListHelpers.h"

namespace GX_STL
{
namespace GXTypes
{
	
	//
	// Pair
	//

	template <typename FirstType, typename SecondType>
	struct Pair : public CompileTime::CopyQualifiers< FirstType, SecondType >
	{
	// types
	public:
		using First_t	= FirstType;
		using Second_t	= SecondType;
		using Self		= Pair< FirstType, SecondType >;


	// variables
	public:
		First_t		first;
		Second_t	second;


	// methods
	public:
		Pair (GX_DEFCTOR) : first(), second() {}

		template <typename FirstT = FirstType,
				  typename SecondT = SecondType>
		Pair (const FirstT &first, const SecondT &second) : first(first), second(second) {}
		
		template <typename FirstT = FirstType,
				  typename SecondT = SecondType>
		Pair (FirstT &&first, SecondT &&second) : first( FW<FirstT>(first) ), second( FW<SecondT>(second) ) {}

		Pair (Self &&other)			= default;
		Pair (const Self &other)	= default;
		
		Self &	operator =  (Self &&right)		= default;
		Self &	operator =  (const Self &right)	= default;

		bool	operator == (const Self &right) const	{ return first == right.first and second == right.second; }
		bool	operator != (const Self &right)	const	{ return not (*this == right); }
		bool	operator >  (const Self &right) const	{ return first != right.first ? first > right.first : second > right.second; }
		bool	operator <  (const Self &right) const	{ return first != right.first ? first < right.first : second < right.second; }
		bool	operator >= (const Self &right) const	{ return not (*this < right); }
		bool	operator <= (const Self &right) const	{ return not (*this > right); }

		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			GXTypes::SwapValues( left.first,	right.first );
			GXTypes::SwapValues( left.second,	right.second );
		}
	};
	

	
/*
=================================================
	MakePair
=================================================
*/
	template <typename FirstType, typename SecondType>
	CHECKRES Pair<FirstType, SecondType>  MakePair (const FirstType &first, const SecondType &second)
	{
		return Pair<FirstType, SecondType>( first, second );
	}

	template <typename FirstType, typename SecondType>
	CHECKRES Pair<FirstType, SecondType>  MakePair (FirstType &&first, SecondType &&second)
	{
		return Pair<FirstType, SecondType>( FW<FirstType>( first ), FW<SecondType>( second ) );
	}
	

/*
=================================================
	Hash
=================================================
*/
	template <typename FirstType, typename SecondType>
	struct Hash< Pair< FirstType, SecondType > >
	{
		CHECKRES HashResult  operator () (const Pair< FirstType, SecondType > &x) const noexcept
		{
			return HashOf( x.first ) + HashOf( x.second );
		}
	};

}	// GX_STL
}	// GXTypes
