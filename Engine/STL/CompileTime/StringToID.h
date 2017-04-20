// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "TemplateMath.h"
#include "Engine/STL/Types/Noninstancable.h"

namespace GX_STL
{
namespace CompileTime
{
	
	//
	// String To ID
	//
	namespace _ctime_hidden_
	{
		struct _StringToID_Helper
		{
			using type = ulong;

		// constants
		private:
			static const uint	_MAX_OFFSET		= sizeof(type) * 8;
			static const uint	_SYMBOL_SIZE	= CompileTime::IntLog2< uint, ('Z' - 'A') > + 1;
			static const uint	_COUNT			= SizeOf<type>::bits / _SYMBOL_SIZE;
			static const uint	_OFFSET			= SizeOf<type>::bits % _SYMBOL_SIZE;
			static const type	_NUMERIC		= (1u << _SYMBOL_SIZE) - 1;
			static const uint	_STR_SIZE		= _COUNT + 1;
			static const type	_SYMBOL_MASK	= _NUMERIC;

			
		// methods
		private:
			forceinline static constexpr type _Hash (const char c, const size_t shift)
			{
				return
					(c >= 'A' and c <= 'Z')	? type(c - 'A' + 1) << shift				:	// map A-Z
					(c >= 'a' and c <= 'z') ? type(c - 'a' + 1) << shift				:	// map a-z
					(c == '.')				? type(c - '.' + 'Z' - 'A' + 2) << shift	:	// map .
					(c == '-')				? type(c - '-' + 'Z' - 'A' + 3) << shift	:	// map -
					(c == '/')				? type(c - '/' + 'Z' - 'A' + 4) << shift	:	// map /
					(c == '*')				? type(c - '*' + 'Z' - 'A' + 5) << shift	:	// map *
					(c == '\0' or c == ' ')	? 0											:
					(throw std::logic_error("unsupported char"));
			}


			forceinline static char _RevHash (const ubyte hash)
			{
				static const ubyte	az_size = ('Z' - 'A' + 1);

				return
					(hash == 0)				? ' '			:
					(hash <  az_size)		? 'A' + hash-1	:
					(hash == az_size + 1)	? '.'			:
					(hash == az_size + 2)	? '-'			:
					(hash == az_size + 3)	? '/'			:
					(hash == az_size + 4)	? '*'			:
					'#';
			}


			forceinline static constexpr type _RecursiveHash (char const * const str, const size_t i, const size_t off, const size_t size)
			{
				return
					(i >= size or str[i] == '\0') ?
						0 :
						(off >= _MAX_OFFSET) ?
							(throw std::logic_error("overflow")) :
							(str[i] >= '0' and str[i] <= '9') ? 
								(_NUMERIC << off) | (type(str[i] - '0') << (off + _SYMBOL_SIZE)) |
									_RecursiveHash( str, i+1, off + _SYMBOL_SIZE*2, size ) :	// TODO: check overflow
								_Hash( str[i], off ) | _RecursiveHash( str, i+1, off + _SYMBOL_SIZE, size );
			}

			
		// methods
		public:
			using StString_t		= GXTypes::StaticString< _STR_SIZE >;

			static const type	MASK = (1u << _OFFSET) - 1;


			forceinline static constexpr type CalcHash (char const * const str, const size_t size, const type id) noexcept
			{
				return size > _COUNT ? type(-1) : (_RecursiveHash( str, 0, _OFFSET, size ) | (id & MASK));
			}


			static StString_t ReverseHash (const type id)
			{
				StString_t	result;
				const type	hash	= (id >> _OFFSET);

				for (usize off = 0;
					 off < _MAX_OFFSET - _OFFSET and result.Length() < result.MaxCapacity();
					 off += _SYMBOL_SIZE)
				{
					const ubyte	sym = (hash >> off) & _SYMBOL_MASK;
					
					if ( sym == _NUMERIC )
					{
						off += _SYMBOL_SIZE;

						result << char( '0' + ((hash >> off) & _SYMBOL_MASK) );
						continue;
					}

					result << _RevHash( sym );
				}

				return result;
			}
		};

	}	// _ctime_hidden_


#	define DECL_STRING_TO_ID( _name_, _id_ ) \
		struct _name_ { \
			using _type_ = ::GX_STL::CompileTime::_ctime_hidden_::_StringToID_Helper::type; \
			\
			enum type : _type_ { \
				_IDMask	= ::GX_STL::CompileTime::_ctime_hidden_::_StringToID_Helper::MASK, \
				_ID		= (_id_ & _IDMask), \
				Unknown	= _type_(-1), \
			}; \
			\
			forceinline static type FromString (::GX_STL::GXTypes::StringCRef str) noexcept { \
				return (type) ::GX_STL::CompileTime::_ctime_hidden_::_StringToID_Helper:: \
								CalcHash( str.cstr(), str.Count(), _type_(_name_::_ID) ); \
			} \
			\
			forceinline static ::GX_STL::CompileTime::_ctime_hidden_::_StringToID_Helper::StString_t \
						ToString (type value) noexcept { \
				return ::GX_STL::CompileTime::_ctime_hidden_::_StringToID_Helper::ReverseHash( _type_(value) ); \
			} \
		}; \
		\
		forceinline constexpr _name_::type operator "" _##_name_ (char const * const str, const size_t size) noexcept \
		{ \
			return (_name_::type) ::GX_STL::CompileTime::_ctime_hidden_::_StringToID_Helper:: \
											CalcHash( str, size, _name_::_type_(_name_::_ID) ); \
		} \

	
	/*
	Example:

		DECL_STRING_TO_ID( MsgID, 0 );

		switch ( MsgID::type(8) )
		{
			case MsgID::Unknown	:	break;	// if operator _MsgID returns error this case make compilation error
			case "update"_MsgID :	break;
			case "draw"_MsgID	:	break;
		}
	*/

}	// CompileTime
}	// GX_STL
