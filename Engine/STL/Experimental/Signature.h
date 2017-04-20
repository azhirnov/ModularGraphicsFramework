// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	Signature contains:
		- symbols	!"#$%&'()*+,-./:;<=>?@[\]^_
		- numbers 0-9
		- letters A-Z
*/

#pragma once

#include "TemplateMath.h"
#include "Engine/STL/Types/Noninstancable.h"

namespace GX_STL
{
namespace CompileTime
{
	
	namespace _ctime_hidden_
	{
		template <char C, typename T, uint Index>
		struct _PackChar
		{
			static const char	MIN_VALUE	= ' ';
			static const char	MAX_VALUE	= '_';
			static const uint	CHAR_BITS	= CompileTime::IntLog2< T, (MAX_VALUE - MIN_VALUE) > + 1;
			static const uint	MASK		= (1u << CHAR_BITS) - 1;

			STATIC_ASSERT( MIN_VALUE < MAX_VALUE );
			STATIC_ASSERT( C >= MIN_VALUE and C <= MAX_VALUE );

			static const T	value = ( ( T(C) - MIN_VALUE ) & MASK ) << (CHAR_BITS * Index);
		};

		template <typename T, T Value, uint Index>
		struct _UnpackChar
		{
			typedef _PackChar< 'A', T, Index >	pack_t;

			static const char	MIN_VALUE	= pack_t::MIN_VALUE;
			static const char	MAX_VALUE	= pack_t::MAX_VALUE;
			static const uint	CHAR_BITS	= pack_t::CHAR_BITS;
			static const uint	MASK		= pack_t::MASK;

			static const char	value = (( Value >> (CHAR_BITS * Index) ) & MASK) + MIN_VALUE;

			STATIC_ASSERT( value >= MIN_VALUE and value <= MAX_VALUE );
		};
		

		//
		// Signature 4 bytes
		//

		struct _Signature4 : public Noninstancable
		{
		private:
			typedef uint	_value_t;

		public:
			enum value_t : _value_t {};
		
			template <char C0, char C1 = ' ', char C2 = ' ', char C3 = ' ', char C4 = ' '>
			struct Pack
			{
				DEBUG_ONLY(	static const char	name[]; )

				static const value_t	value =	value_t(
												_ctime_hidden_::_PackChar< C0, _value_t, 0 >::value |
												_ctime_hidden_::_PackChar< C1, _value_t, 1 >::value |
												_ctime_hidden_::_PackChar< C2, _value_t, 2 >::value |
												_ctime_hidden_::_PackChar< C3, _value_t, 3 >::value |
												_ctime_hidden_::_PackChar< C4, _value_t, 4 >::value );
			};

			template <value_t Value>
			struct Unpack
			{
				static const char	value[];
			};
		};



		//
		// Signature 8 bytes
		//

		struct _Signature8 : public Noninstancable
		{
		private:
			typedef ulong	_value_t;

		public:
			enum value_t : _value_t {};
		
			template <char C0,       char C1 = ' ', char C2 = ' ', char C3 = ' ', char C4 = ' ',
					  char C5 = ' ', char C6 = ' ', char C7 = ' ', char C8 = ' ', char C9 = ' '>
			struct Pack
			{
				DEBUG_ONLY(	static const char	name[]; )

				static const value_t	value =	value_t(
												_ctime_hidden_::_PackChar< C0, _value_t, 0 >::value |
												_ctime_hidden_::_PackChar< C1, _value_t, 1 >::value |
												_ctime_hidden_::_PackChar< C2, _value_t, 2 >::value |
												_ctime_hidden_::_PackChar< C3, _value_t, 3 >::value |
												_ctime_hidden_::_PackChar< C4, _value_t, 4 >::value |
												_ctime_hidden_::_PackChar< C5, _value_t, 5 >::value |
												_ctime_hidden_::_PackChar< C6, _value_t, 6 >::value |
												_ctime_hidden_::_PackChar< C7, _value_t, 7 >::value |
												_ctime_hidden_::_PackChar< C8, _value_t, 8 >::value |
												_ctime_hidden_::_PackChar< C9, _value_t, 9 >::value );
			};

			template <value_t Value>
			struct Unpack
			{
				static const char	value[];
			};
		};


		DEBUG_ONLY(
			template <char C0, char C1, char C2, char C3, char C4>
			const char	_Signature4::Pack< C0, C1, C2, C3, C4 >::name[] = { C0, C1, C2, C3, C4, 0 };

			template <char C0, char C1, char C2, char C3, char C4, char C5, char C6, char C7, char C8, char C9>
			const char	_Signature8::Pack< C0, C1, C2, C3, C4, C5, C6, C7, C8, C9 >::name[] = { C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, 0 };
		)

		template <_Signature4::value_t Value>
		const char _Signature4::Unpack< Value >::value[] = {
			_ctime_hidden_::_UnpackChar< _value_t, Value, 0 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 1 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 2 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 3 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 4 >::value
			0
		};

		template <_Signature8::value_t Value>
		const char _Signature8::Unpack< Value >::value[] = {
			_ctime_hidden_::_UnpackChar< _value_t, Value, 0 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 1 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 2 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 3 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 4 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 5 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 6 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 7 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 8 >::value,
			_ctime_hidden_::_UnpackChar< _value_t, Value, 9 >::value,
			0
		};


		template <typename T>
		struct _SwitchSignature
		{
			typedef typename SwitchType< (IsSameTypes< T, uint >), _Signature4,
						typename SwitchType< (IsSameTypes< T, ulong >), _Signature8, void > >	type;
		};


	}	// _ctime_hidden_



	//
	// Signature
	//

	template <typename T>
	using Signature	= typename _ctime_hidden_::_SwitchSignature< T >::type;


}	// CompileTime
}	// GX_STL
