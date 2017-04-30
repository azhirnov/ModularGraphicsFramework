// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Helpers for dimensions types.
*/

#pragma once

#include "Engine/STL/Common/Types.h"

// TODO: rename
#define _GX_DIM_OPERATORS_SELF( _op_, _value_ ) \
	\
	forceinline Self&			operator _op_##= (const Self &right)		{ _value_ _op_##= right._value_;  return *this; } \
	forceinline constexpr Self  operator _op_    (const Self &right) const	{ return Self( _value_ _op_ right._value_ ); }
		

#define _GX_DIM_OPERATORS_TYPE( _op_, _value_, _type_, _memberOption_ ) \
	\
	forceinline Self&			operator _op_##= (const _type_ &right)						{ _value_ _op_##= Cast<value_t>( right _memberOption_ );  return *this; } \
	forceinline constexpr Self  operator _op_    (const _type_ &right) const				{ return Self( _value_ _op_ Cast<value_t>( right _memberOption_ ) ); } \
	\
	forceinline constexpr friend Self operator _op_ (const _type_ &left, const Self &right)	{ return Self( Cast<value_t>( left _memberOption_ ) _op_ right._value_ ); }


#define _GX_DIM_OPERATORS_TEMPLATE( _op_, _value_, _memberOption_ ) \
	\
	forceinline template <typename _A_> Self&			operator _op_##= (const _A_ &right)			{ _value_ _op_##= Cast<value_t>( right _memberOption_ );  return *this; } \
	forceinline template <typename _A_> constexpr Self  operator _op_    (const _A_ &right) const	{ return Self( _value_ _op_ Cast<value_t>( right _memberOption_ ) ); } \
	\
	forceinline template <typename _A_> \
	forceinline constexpr friend Self operator _op_ (const _A_ &left, const Self &right)			{ return Self( Cast<value_t>( left _memberOption_ ) _op_ right._value_ ); }


#define _GX_DIM_CMP_OPERATORS_SELF( _value_ ) \
	\
	forceinline constexpr bool operator == (const Self &right)	const		{ return this->_value_ == right._value_; } \
	forceinline constexpr bool operator != (const Self &right)	const		{ return this->_value_ != right._value_; } \
	forceinline constexpr bool operator >  (const Self &right)	const		{ return this->_value_ >  right._value_; } \
	forceinline constexpr bool operator <  (const Self &right)	const		{ return this->_value_ <  right._value_; } \
	forceinline constexpr bool operator >= (const Self &right)	const		{ return this->_value_ >= right._value_; } \
	forceinline constexpr bool operator <= (const Self &right)	const		{ return this->_value_ <= right._value_; }
	

#define _GX_DIM_CMP_OPERATORS_TYPE( _value_, _type_, _memberOption_ ) \
	\
	forceinline constexpr bool operator == (const _type_ &right)	const		{ return this->_value_ == Cast<value_t>( right _memberOption_ ); } \
	forceinline constexpr bool operator != (const _type_ &right)	const		{ return this->_value_ != Cast<value_t>( right _memberOption_ ); } \
	forceinline constexpr bool operator >  (const _type_ &right)	const		{ return this->_value_ >  Cast<value_t>( right _memberOption_ ); } \
	forceinline constexpr bool operator <  (const _type_ &right)	const		{ return this->_value_ <  Cast<value_t>( right _memberOption_ ); } \
	forceinline constexpr bool operator >= (const _type_ &right)	const		{ return this->_value_ >= Cast<value_t>( right _memberOption_ ); } \
	forceinline constexpr bool operator <= (const _type_ &right)	const		{ return this->_value_ <= Cast<value_t>( right _memberOption_ ); } \
	\
	forceinline constexpr friend bool operator == (const _type_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) == right._value_; } \
	forceinline constexpr friend bool operator != (const _type_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) != right._value_; } \
	forceinline constexpr friend bool operator >  (const _type_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) >  right._value_; } \
	forceinline constexpr friend bool operator <  (const _type_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) <  right._value_; } \
	forceinline constexpr friend bool operator >= (const _type_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) >= right._value_; } \
	forceinline constexpr friend bool operator <= (const _type_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) <= right._value_; }
	

#define _GX_DIM_CMP_OPERATORS_TEMPLATE( _value_, _memberOption_ ) \
	\
	template <typename _A_> forceinline constexpr bool operator == (const _A_ &right) const	{ return this->_value_ == Cast<value_t>( right _memberOption_ ); } \
	template <typename _A_> forceinline constexpr bool operator != (const _A_ &right) const	{ return this->_value_ != Cast<value_t>( right _memberOption_ ); } \
	template <typename _A_> forceinline constexpr bool operator >  (const _A_ &right) const	{ return this->_value_ >  Cast<value_t>( right _memberOption_ ); } \
	template <typename _A_> forceinline constexpr bool operator <  (const _A_ &right) const	{ return this->_value_ <  Cast<value_t>( right _memberOption_ ); } \
	template <typename _A_> forceinline constexpr bool operator >= (const _A_ &right) const	{ return this->_value_ >= Cast<value_t>( right _memberOption_ ); } \
	template <typename _A_> forceinline constexpr bool operator <= (const _A_ &right) const	{ return this->_value_ <= Cast<value_t>( right _memberOption_ ); } \
	\
	template <typename _A_> forceinline constexpr friend bool operator == (const _A_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) == right._value_; } \
	template <typename _A_> forceinline constexpr friend bool operator != (const _A_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) != right._value_; } \
	template <typename _A_> forceinline constexpr friend bool operator >  (const _A_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) >  right._value_; } \
	template <typename _A_> forceinline constexpr friend bool operator <  (const _A_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) <  right._value_; } \
	template <typename _A_> forceinline constexpr friend bool operator >= (const _A_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) >= right._value_; } \
	template <typename _A_> forceinline constexpr friend bool operator <= (const _A_ &left, const Self &right)	{ return Cast<value_t>( left _memberOption_ ) <= right._value_; }
		

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
	forceinline constexpr Self	operator ! () const		{ return Self( not _value_ ); } \
	forceinline constexpr Self	operator ~ () const		{ return Self( ~_value_ ); } \
	/*Self	operator - () const				{ return Self( -_value_ ); }  /* not available for unsigned types */ \
	forceinline Self &			operator ++ ()			{ ++_value_;  return *this; } \
	forceinline Self &			operator -- ()			{ --_value_;  return *this; } \
	forceinline const Self		operator ++ (int)		{ Self	ret(*this);  ++(*this);  return ret; } \
	forceinline const Self		operator -- (int)		{ Self	ret(*this);  --(*this);  return ret; }
	

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
	forceinline constexpr Self	operator - () const		{ return Self( -_value_ ); } \
	forceinline Self &			operator ++ ()			{ ++_value_;  return *this; } \
	forceinline Self &			operator -- ()			{ --_value_;  return *this; } \
	forceinline const Self		operator ++ (int)		{ Self	ret(*this);  ++(*this);  return ret; } \
	forceinline const Self		operator -- (int)		{ Self	ret(*this);  --(*this);  return ret; }
	

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

