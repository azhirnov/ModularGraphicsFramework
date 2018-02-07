// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Helpers for dimensions types.
*/

#pragma once

#include "Engine/STL/Common/Types.h"

// TODO: rename
#define _GX_DIM_OPERATORS_SELF( _op_, _value_ ) \
	\
	forceinline Self&					 operator _op_##= (const Self &right) noexcept					{ _value_ _op_##= right._value_;  return *this; } \
	CHECKRES forceinline constexpr Self  operator _op_    (const Self &right) const noexcept			{ return Self( _value_ _op_ right._value_ ); }
		

#define _GX_DIM_OPERATORS_TYPE( _op_, _value_, _type_, _memberOption_ ) \
	\
	forceinline Self&					 operator _op_##= (_type_ right) noexcept						{ _value_ _op_##= static_cast<const Value_t>( right _memberOption_ );  return *this; } \
	CHECKRES forceinline constexpr Self  operator _op_    (_type_ right) const noexcept					{ return Self( _value_ _op_ static_cast<const Value_t>( right _memberOption_ ) ); } \
	\
	CHECKRES friend forceinline constexpr Self operator _op_ (_type_ left, const Self &right) noexcept	{ return Self( static_cast<const Value_t>( left _memberOption_ ) _op_ right._value_ ); }


#define _GX_DIM_OPERATORS_TEMPLATE( _op_, _value_, _memberOption_ ) \
	\
	template <typename _A_> forceinline Self&					 operator _op_##= (const _A_ &right) noexcept		{ _value_ _op_##= static_cast<const Value_t>( right _memberOption_ );  return *this; } \
	template <typename _A_> CHECKRES forceinline constexpr Self  operator _op_    (const _A_ &right) const noexcept	{ return Self( _value_ _op_ static_cast<const Value_t>( right _memberOption_ ) ); } \
	\
	template <typename _A_> \
	CHECKRES friend forceinline constexpr Self operator _op_ (const _A_ &left, const Self &right) noexcept			{ return Self( static_cast<const Value_t>( left _memberOption_ ) _op_ right._value_ ); }


#define _GX_DIM_CMP_OPERATORS_SELF( _value_ ) \
	\
	CHECKRES forceinline constexpr bool operator == (const Self &right) const noexcept		{ return this->_value_ == right._value_; } \
	CHECKRES forceinline constexpr bool operator != (const Self &right) const noexcept		{ return this->_value_ != right._value_; } \
	CHECKRES forceinline constexpr bool operator >  (const Self &right) const noexcept		{ return this->_value_ >  right._value_; } \
	CHECKRES forceinline constexpr bool operator <  (const Self &right) const noexcept		{ return this->_value_ <  right._value_; } \
	CHECKRES forceinline constexpr bool operator >= (const Self &right) const noexcept		{ return this->_value_ >= right._value_; } \
	CHECKRES forceinline constexpr bool operator <= (const Self &right) const noexcept		{ return this->_value_ <= right._value_; }
	

#define _GX_DIM_CMP_OPERATORS_TYPE( _value_, _type_, _memberOption_ ) \
	\
	CHECKRES forceinline constexpr bool operator == (_type_ right) const noexcept			{ return this->_value_ == static_cast<const Value_t>( right _memberOption_ ); } \
	CHECKRES forceinline constexpr bool operator != (_type_ right) const noexcept			{ return this->_value_ != static_cast<const Value_t>( right _memberOption_ ); } \
	CHECKRES forceinline constexpr bool operator >  (_type_ right) const noexcept			{ return this->_value_ >  static_cast<const Value_t>( right _memberOption_ ); } \
	CHECKRES forceinline constexpr bool operator <  (_type_ right) const noexcept			{ return this->_value_ <  static_cast<const Value_t>( right _memberOption_ ); } \
	CHECKRES forceinline constexpr bool operator >= (_type_ right) const noexcept			{ return this->_value_ >= static_cast<const Value_t>( right _memberOption_ ); } \
	CHECKRES forceinline constexpr bool operator <= (_type_ right) const noexcept			{ return this->_value_ <= static_cast<const Value_t>( right _memberOption_ ); } \
	\
	CHECKRES friend forceinline constexpr bool operator == (_type_ left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) == right._value_; } \
	CHECKRES friend forceinline constexpr bool operator != (_type_ left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) != right._value_; } \
	CHECKRES friend forceinline constexpr bool operator >  (_type_ left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) >  right._value_; } \
	CHECKRES friend forceinline constexpr bool operator <  (_type_ left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) <  right._value_; } \
	CHECKRES friend forceinline constexpr bool operator >= (_type_ left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) >= right._value_; } \
	CHECKRES friend forceinline constexpr bool operator <= (_type_ left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) <= right._value_; }
	

#define _GX_DIM_CMP_OPERATORS_TEMPLATE( _value_, _memberOption_ ) \
	\
	template <typename _A_> CHECKRES forceinline constexpr bool operator == (const _A_ &right) const noexcept	{ return this->_value_ == static_cast<const Value_t>( right _memberOption_ ); } \
	template <typename _A_> CHECKRES forceinline constexpr bool operator != (const _A_ &right) const noexcept	{ return this->_value_ != static_cast<const Value_t>( right _memberOption_ ); } \
	template <typename _A_> CHECKRES forceinline constexpr bool operator >  (const _A_ &right) const noexcept	{ return this->_value_ >  static_cast<const Value_t>( right _memberOption_ ); } \
	template <typename _A_> CHECKRES forceinline constexpr bool operator <  (const _A_ &right) const noexcept	{ return this->_value_ <  static_cast<const Value_t>( right _memberOption_ ); } \
	template <typename _A_> CHECKRES forceinline constexpr bool operator >= (const _A_ &right) const noexcept	{ return this->_value_ >= static_cast<const Value_t>( right _memberOption_ ); } \
	template <typename _A_> CHECKRES forceinline constexpr bool operator <= (const _A_ &right) const noexcept	{ return this->_value_ <= static_cast<const Value_t>( right _memberOption_ ); } \
	\
	template <typename _A_> CHECKRES friend forceinline constexpr bool operator == (const _A_ &left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) == right._value_; } \
	template <typename _A_> CHECKRES friend forceinline constexpr bool operator != (const _A_ &left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) != right._value_; } \
	template <typename _A_> CHECKRES friend forceinline constexpr bool operator >  (const _A_ &left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) >  right._value_; } \
	template <typename _A_> CHECKRES friend forceinline constexpr bool operator <  (const _A_ &left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) <  right._value_; } \
	template <typename _A_> CHECKRES friend forceinline constexpr bool operator >= (const _A_ &left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) >= right._value_; } \
	template <typename _A_> CHECKRES friend forceinline constexpr bool operator <= (const _A_ &left, const Self &right) noexcept	{ return static_cast<const Value_t>( left _memberOption_ ) <= right._value_; }
		

#define _GX_DIM_ALL_INTEGER_OPERATORS_SELF( _value_ ) \
	_GX_DIM_OPERATORS_SELF( +,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( -,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( *,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( /,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( %,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( >>,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( <<,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( |,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( &,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( ^,	_value_ ) \
	_GX_DIM_CMP_OPERATORS_SELF( _value_ ) \
	CHECKRES forceinline constexpr Self	operator ! () const noexcept	{ return Self( not _value_ ); } \
	CHECKRES forceinline constexpr Self	operator ~ () const noexcept 	{ return Self( ~_value_ ); } \
	/*Self	operator - () const				{ return Self( -_value_ ); }  / * not available for unsigned types */ \
	forceinline Self &					operator ++ () noexcept			{ ++_value_;  return *this; } \
	forceinline Self &					operator -- () noexcept			{ --_value_;  return *this; } \
	forceinline const Self				operator ++ (int) noexcept		{ Self	ret(*this);  ++(*this);  return ret; } \
	forceinline const Self				operator -- (int) noexcept		{ Self	ret(*this);  --(*this);  return ret; }
	

#define _GX_DIM_ALL_INTEGER_OPERATORS_TYPE( _value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( +,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( -,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( *,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( /,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( %,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( >>,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( <<,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( |,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( &,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( ^,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_CMP_OPERATORS_TYPE(	_value_, _type_, _memberOption_ ) 


#define _GX_DIM_ALL_INTEGER_OPERATORS_TEMPLATE( _value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( +,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( -,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( *,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( /,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( %,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( >>,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( <<,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( |,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( &,	_value_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( ^,	_value_, _memberOption_ ) \
	_GX_DIM_CMP_OPERATORS_TEMPLATE(	_value_, _memberOption_ ) 


#define _GX_DIM_ALL_FLOAT_OPERATORS_SELF( _value_ ) \
	_GX_DIM_OPERATORS_SELF( +,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( -,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( *,	_value_ ) \
	_GX_DIM_OPERATORS_SELF( /,	_value_ ) \
	_GX_DIM_CMP_OPERATORS_SELF( _value_ ) \
	forceinline constexpr Self	operator - () const noexcept	{ return Self( -_value_ ); } \
	forceinline Self &			operator ++ () noexcept			{ ++_value_;  return *this; } \
	forceinline Self &			operator -- () noexcept			{ --_value_;  return *this; } \
	forceinline const Self		operator ++ (int) noexcept		{ Self	ret(*this);  ++(*this);  return ret; } \
	forceinline const Self		operator -- (int) noexcept		{ Self	ret(*this);  --(*this);  return ret; }
	

#define _GX_DIM_ALL_FLOAT_OPERATORS_TYPE( _value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( +,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( -,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( *,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TYPE( /,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_CMP_OPERATORS_TYPE(	_value_, _type_, _memberOption_ ) 


#define _GX_DIM_ALL_FLOAT_OPERATORS_TEMPLATE( _value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( +,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( -,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( *,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_OPERATORS_TEMPLATE( /,	_value_, _type_, _memberOption_ ) \
	_GX_DIM_CMP_OPERATORS_TEMPLATE(	_value_, _type_, _memberOption_ ) 

