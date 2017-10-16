// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		typedef FirstType						first_t;
		typedef SecondType						second_t;
		typedef Pair< FirstType, SecondType >	Self;


	// variables
	public:
		first_t		first;
		second_t	second;


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
			SwapValues( left.first,		right.first );
			SwapValues( left.second,	right.second );
		}
	};
	

	
/*
=================================================
	MakePair
=================================================
*/
	template <typename FirstType, typename SecondType>
	Pair<FirstType, SecondType>  MakePair (const FirstType &first, const SecondType &second)
	{
		return Pair<FirstType, SecondType>( first, second );
	}

	template <typename FirstType, typename SecondType>
	Pair<FirstType, SecondType>  MakePair (FirstType &&first, SecondType &&second)
	{
		return Pair<FirstType, SecondType>( FW<FirstType>( first ), FW<SecondType>( second ) );
	}
	

/*
=================================================
	Hash
=================================================
*/
	template <typename FirstType, typename SecondType>
	struct Hash< Pair< FirstType, SecondType > > :
		private Hash< FirstType >, private Hash< SecondType >
	{
		STATIC_ASSERT(( CompileTime::IsSameTypes< typename Hash< FirstType >::result_t,
													typename Hash< SecondType >::result_t > ));

		typedef Pair< FirstType, SecondType >	key_t;
		typedef Hash< FirstType >::result_t		result_t;

		result_t operator () (const key_t &x) const noexcept
		{
			return	Hash< FirstType >::operator ()( x.first ) +
					Hash< SecondType >::operator ()( x.second );
		}
	};

}	// GX_STL
}	// GXTypes
