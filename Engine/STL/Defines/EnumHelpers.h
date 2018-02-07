// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once


// bit operations
#define __GX_ENUM_BIT_OPERATIONS( _type_, _qual_ ) \
	typedef CompileTime::NearUInt::FromType<_type_>		__uint_bit_t; \
	\
	template <typename T>						_qual_ _type_ & operator |=  (_type_& left, const T& right)				{ return left = _type_( __uint_bit_t(left) |  __uint_bit_t(right) ); } \
	template <typename T>						_qual_ _type_ & operator &=  (_type_& left, const T& right)				{ return left = _type_( __uint_bit_t(left) &  __uint_bit_t(right) ); } \
	template <typename T>						_qual_ _type_ & operator ^=  (_type_& left, const T& right)				{ return left = _type_( __uint_bit_t(left) ^  __uint_bit_t(right) ); } \
	template <typename T>						_qual_ _type_ & operator >>= (_type_& left, const T& right)				{ return left = _type_( __uint_bit_t(left) >> __uint_bit_t(right) ); } \
	template <typename T>						_qual_ _type_ & operator <<= (_type_& left, const T& right)				{ return left = _type_( __uint_bit_t(left) << __uint_bit_t(right) ); } \
	\
							CHECKRES constexpr	_qual_ _type_  operator |   (const _type_& left, const _type_& right)	{ return _type_( __uint_bit_t(left) |  __uint_bit_t(right) ); } \
							CHECKRES constexpr	_qual_ _type_  operator &   (const _type_& left, const _type_& right)	{ return _type_( __uint_bit_t(left) &  __uint_bit_t(right) ); } \
							CHECKRES constexpr	_qual_ _type_  operator ^   (const _type_& left, const _type_& right)	{ return _type_( __uint_bit_t(left) ^  __uint_bit_t(right) ); } \
							CHECKRES constexpr	_qual_ _type_  operator >>  (const _type_& left, const _type_& right)	{ return _type_( __uint_bit_t(left) >> __uint_bit_t(right) ); } \
							CHECKRES constexpr	_qual_ _type_  operator <<  (const _type_& left, const _type_& right)	{ return _type_( __uint_bit_t(left) << __uint_bit_t(right) ); } \
	\
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator |   (const _type_& left, const T& right)		{ return _type_( __uint_bit_t(left) |  __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator &   (const _type_& left, const T& right)		{ return _type_( __uint_bit_t(left) &  __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator ^   (const _type_& left, const T& right)		{ return _type_( __uint_bit_t(left) ^  __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator >>  (const _type_& left, const T& right)		{ return _type_( __uint_bit_t(left) >> __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator <<  (const _type_& left, const T& right)		{ return _type_( __uint_bit_t(left) << __uint_bit_t(right) ); } \
	\
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator |   (const T& left, const _type_& right)		{ return _type_( __uint_bit_t(left) |  __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator &   (const T& left, const _type_& right)		{ return _type_( __uint_bit_t(left) &  __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator ^   (const T& left, const _type_& right)		{ return _type_( __uint_bit_t(left) ^  __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator >>  (const T& left, const _type_& right)		{ return _type_( __uint_bit_t(left) >> __uint_bit_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_  operator <<  (const T& left, const _type_& right)		{ return _type_( __uint_bit_t(left) << __uint_bit_t(right) ); } \
	\
							CHECKRES constexpr	_qual_ _type_  operator ~   (const _type_& value)						{ return _type_( ~__uint_bit_t(value) ); } \
							CHECKRES constexpr	_qual_ bool    operator !   (const _type_& value)						{ return not __uint_bit_t( value ); } \


#define GX_ENUM_BIT_OPERATIONS( _type_ )	__GX_ENUM_BIT_OPERATIONS( _type_, forceinline friend )


// add operations
#define __GX_ENUM_ADD_OPERATIONS( _type_, _qual_ ) \
	typedef CompileTime::NearUInt::FromType<_type_>		__uint_math_t; \
	typedef CompileTime::NearInt::FromType<_type_>		__int_math_t; \
	\
												_qual_ _type_ & operator ++ (_type_& value)								{ return value = _type_( __uint_math_t(value)+1 ); } \
												_qual_ _type_ & operator -- (_type_& value)								{ return value = _type_( __uint_math_t(value)-1 ); }	 \
	\
												_qual_ _type_   operator ++ (_type_& value, int)						{ _type_ t(value);  ++value;  return t; } \
												_qual_ _type_   operator -- (_type_& value, int)						{ _type_ t(value);  --value;  return t; } \
	\
	template <typename T>						_qual_ _type_ & operator += (_type_& left, const T& right)				{ return left = _type_( __uint_math_t(left) + __uint_math_t(right) ); } \
	template <typename T>						_qual_ _type_ & operator -= (_type_& left, const T& right)				{ return left = _type_( __uint_math_t(left) - __uint_math_t(right) ); } \
	\
							CHECKRES constexpr	_qual_ _type_   operator +  (const _type_& left, const _type_& right)	{ return _type_( __uint_math_t(left) + __uint_math_t(right) ); } \
							CHECKRES constexpr	_qual_ _type_   operator -  (const _type_& left, const _type_& right)	{ return _type_( __uint_math_t(left) - __uint_math_t(right) ); } \
	\
	template <typename T>	CHECKRES constexpr	_qual_ _type_   operator +  (const _type_& left, const T& right)		{ return _type_( __uint_math_t(left) + __uint_math_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_   operator -  (const _type_& left, const T& right)		{ return _type_( __uint_math_t(left) - __uint_math_t(right) ); } \
	\
	template <typename T>	CHECKRES constexpr	_qual_ _type_   operator +  (const T& left, const _type_& right)		{ return _type_( __uint_math_t(left) + __uint_math_t(right) ); } \
	template <typename T>	CHECKRES constexpr	_qual_ _type_   operator -  (const T& left, const _type_& right)		{ return _type_( __uint_math_t(left) - __uint_math_t(right) ); } \
	\
							CHECKRES constexpr	_qual_ _type_   operator +  (const _type_& value)						{ return _type_( +__uint_math_t(value) ); } \
							CHECKRES constexpr	_qual_ _type_   operator -  (const _type_& value)						{ return _type_( -__int_math_t(value) ); } \


#define GX_ENUM_ADD_OPERATIONS( _type_ )	__GX_ENUM_ADD_OPERATIONS( _type_, forceinline friend )


// enum bitfield
#define GX_ENUM_BITFIELD( _type_ ) \
	using bits = EnumBitfield< _type_ >; \
	CHECKRES friend constexpr bits operator | (type left, type right)	{ return bits().Set( left ).Set( right ); }

