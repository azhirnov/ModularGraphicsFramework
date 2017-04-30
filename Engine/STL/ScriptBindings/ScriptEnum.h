// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "ScriptEngine.h"

namespace GX_STL
{
namespace GXScript
{
	/*
		Example:

		#define ENUM_LIST( build ) \
			build( ONE, ) \
			build( TWO, ) \
			build( THREE, =3 )

		GX_SCRIPT_ENUM( Enum, ENUM_LIST, );

		#undef ENUM_LIST
	*/

#	define __GX_BUILD_ENUM_VALUE( _name_, value, ... ) \
		_name_ value,

#	define __GX_BUILD_ENUM_CASE( _name_, ... ) \
		case _name_ :  return TOSTRING( _name_ );

#	define __GX_BUILD_ENUM_COUNT( ... ) \
		+1

#	define __GX_BUILD_ENUM_GET_VALUE( _name_, ... ) \
		_name_,
	
#	define __GX_BUILD_ENUM_GET_NAME( _name_, ... ) \
		TOSTRING( _name_ ),

#	define __GX_BUILD_ENUM_GET_INDEX( _name_, ... ) \
		if ( ++i and value == _name_ )	return i-1;


#	define GX_SCRIPT_ENUM( _enumName_, _enumList_, _enumMethods_ ) \
		struct _enumName_ \
		{ \
		/* types */ \
		public: \
			enum type { \
				_enumList_( __GX_BUILD_ENUM_VALUE ) \
			}; \
			\
			typedef _enumName_									Self; \
			\
			typedef ArrayCRef< type >						values_arr_t; \
			typedef ArrayRef< char const * const >				names_arr_t; \
			typedef CompileTime::NearUInt::FromType< type >		value_t; \
			\
			\
		/* constants */ \
		public: \
			static const usize	_count = _enumList_( __GX_BUILD_ENUM_COUNT ); \
			\
			\
		/* compile-time methods */ \
		private: \
			static const type * _Values () { \
				static const type	_values[] = { _enumList_( __GX_BUILD_ENUM_GET_VALUE ) }; \
				return _values; \
			} \
			\
			static char const * const * _Names () { \
				static char const * const	_names[] = { _enumList_( __GX_BUILD_ENUM_GET_NAME ) }; \
				return _names; \
			} \
			\
		public: \
			static StringCRef Name () { \
				return TOSTRING( _enumName_ ); \
			} \
			\
			static constexpr usize Count () { \
				return _count; \
			} \
			\
			static values_arr_t Values () { \
				return values_arr_t( (type *)_Values(), Count() ); \
			} \
			\
			static names_arr_t ValueNames () { \
				return names_arr_t( _Names(), Count() ); \
			} \
			\
			template <usize Index> \
			static constexpr type Get () \
			{ \
				STATIC_ASSERT( Index < _count ); \
				\
				return _Values()[ Index ]; \
			} \
			\
			template <usize Index> \
			static StringCRef GetString () \
			{ \
				STATIC_ASSERT( Index < _count ); \
				\
				return _ValueNames()[ Index ]; \
			} \
			\
			template <type Value> \
			static usize IndexOf () \
			{ \
				const type value = Value; \
				usize i = 0; \
				\
				_enumList_( __GX_BUILD_ENUM_GET_INDEX ) \
				\
				STATIC_WARNING( "no index for value" ); \
				return -1; \
			} \
			\
			\
		/* runtime methods*/ \
		public: \
			static type Get (usize index) \
			{ \
				if ( index < _count ) \
					return _Values()[ index ]; \
				else { \
					WARNING( "invalid index" ); \
					return type(0); \
				} \
			} \
			\
			static StringCRef GetString (usize index) \
			{ \
				if ( index < _count ) \
					return _Names()[ index ]; \
				else { \
					WARNING( "invalid index" ); \
					return StringCRef(); \
				} \
			} \
			\
			static usize IndexOf (type value) \
			{ \
				usize i = 0; \
				\
				_enumList_( __GX_BUILD_ENUM_GET_INDEX ) \
				\
				WARNING( "no index for value" ); \
				return -1; \
			} \
			\
			static StringCRef ToString (type value) \
			{ \
				switch ( value ) \
				{ \
					_enumList_( __GX_BUILD_ENUM_CASE ) \
				} \
				WARNING( "invalid enum value" ); \
				return StringCRef(); \
			} \
			\
			static type FromString (StringCRef str) \
			{ \
				for (usize i = 0; i < Count(); ++i) { \
					if ( str == ValuesNames()[i] ) \
						return Values()[i]; \
				} \
				WARNING( "string does not match with any value" ); \
				return type(0); \
			} \
			\
			static bool Bind (ScriptEngine &se) \
			{ \
				int	res = se->RegisterEnum( Name().cstr() ); \
				\
				if ( res < 0 and res != AngelScript::asALREADY_REGISTERED ) { \
					AS_CALL( res ); \
					return false; \
				} \
				\
				for (usize i = 0; i < Count(); ++i) { \
					AS_CALL( se->RegisterEnumValue( Name().cstr(), ValuesNames()[i], Values()[i] ) ); \
				} \
				return true; \
			} \
			\
			\
		/* variables */ \
		private: \
			union { \
				type	_typeView; \
				value_t	_value; \
			}; \
			\
			\
		/* methods */ \
		public: \
			_enumName_ () : _typeView(Get<0>()) {} \
			\
			_enumName_ (type value) : _typeView(value) {} \
			\
			template <typename T> \
			explicit \
			_enumName_ (const T& value) : _typeView(type(value)) {} \
			\
			Self & Inc () { \
				_typeView = Get( IndexOf( _typeView )+1 ); \
				return *this; \
			} \
			\
			Self Next () const { \
				return Self(*this).Inc(); \
			} \
			\
			Self & Dec () { \
				_typeView = Get( IndexOf( _typeView )-1 ); \
				return *this; \
			} \
			\
			Self Prev () const { \
				return Self(*this).Dec(); \
			} \
			\
			\
		/* operators */ \
		public: \
			_GX_DIM_ALL_INTEGER_OPERATORS_SELF( _value ) \
			_GX_DIM_ALL_INTEGER_OPERATORS_TYPE( _value, type, ) \
			\
			\
		/* user methods*/ \
		public: \
			_enumMethods_ \
		}


}	// GXScript
}	// GX_STL
