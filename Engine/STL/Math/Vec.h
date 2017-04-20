// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathFunc.h"
#include "MathTypes.h"
#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Common/Cast.h"
#include "Mathematics.h"

namespace GX_STL
{
namespace GXMath
{

	
#	define VEC_UN_OPERATOR( _op_ ) \
		\
		const Self	operator _op_ () const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = _op_ (*this)[i]; \
			return ret; \
		}


#	define VEC_BIN_OPERATOR( _op_ ) \
		Self &		operator _op_##= (const Self& right) \
		{ \
			FOR( i, *this )	 (*this)[i] _op_##= right[i]; \
			return *this; \
		} \
		const Self	operator _op_  (const Self& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = ( (*this)[i] _op_ right[i] ); \
			return ret; \
		} \
		Self &		operator _op_##= (const T& right) \
		{ \
			FOR( i, *this )	 (*this)[i] _op_##= right; \
			return *this; \
		} \
		const Self	operator _op_  (const T& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = ( (*this)[i] _op_ right ); \
			return ret; \
		} \
		friend const Self operator _op_ (const T& left, const Self& right) \
		{ \
			return Self(left) _op_ right; \
		}

#	define VEC_BIN_OPERATOR2( _op_ ) \
		\
		Self	operator _op_  (const T& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = ( (*this)[i] _op_ right ); \
			return ret; \
		} \
		\
		Self	operator _op_  (const Self& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = ( (*this)[i] _op_ right[i] ); \
			return ret; \
		} \
		\
		friend Self operator _op_ (const T& left, const Self& right) \
		{ \
			return Self(left) _op_ right; \
		}


	
#define I	1
#include "VecI.h"
#undef	I


#define I	2
#include "VecI.h"
#undef	I

	
#define I	3
#include "VecI.h"
#undef	I


#define I	4
#include "VecI.h"
#undef	I

	

	template <typename T>
	inline Vec<T,3> Cross (const Vec<T,3> &left, const Vec<T,3> &right)
	{
		return Vec<T,3>( left.y * right.z - right.y * left.z,
						 left.z * right.x - right.z * left.x,
						 left.x * right.y - right.x * left.y );
	}

	
	template <typename T>
	inline T Cross (const Vec<T,2> &left, const Vec<T,2> &right)
	{
		return left.x * right.y - left.y * right.x;
	}


	template <typename T, usize I>
	inline T  Dot (const Vec<T,I> &left, const Vec<T,I> &right)
	{
		return left.Dot( right );
	}


#	undef VEC_UN_OPERATOR
#	undef VEC_BIN_OPERATOR
#	undef VEC_BIN_OPERATOR2


}	// GXMath

namespace GXTypes
{

	//
	// Hash
	//
	template <typename T, usize I>
	struct Hash< GXMath::Vec<T,I> > :
		private Hash< ArrayCRef<T> >
	{
		typedef GXMath::Vec<T,I>			key_t;
		typedef Hash< ArrayCRef<T> >		base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( ArrayCRef<T>( x.ptr(), x.Count() ) );
		}
	};


	//
	// IsNaN, IsInfinity, IsFinite
	//

	template <typename T, usize I>
	inline GXMath::Vec<bool,I>  IsNaN (const GXMath::Vec<T,I>& x)
	{
		GXMath::Vec<bool,I>	ret;
		FOR( i, ret )	ret[i] = IsNaN( x[i] );
		return ret;
	}

	template <typename T, usize I>
	inline GXMath::Vec<bool,I>  IsInfinity (const GXMath::Vec<T,I>& x)
	{
		GXMath::Vec<bool,I>	ret;
		FOR( i, ret )	ret[i] = IsInfinity( x[i] );
		return ret;
	}

	template <typename T, usize I>
	inline GXMath::Vec<bool,I>  IsFinite (const GXMath::Vec<T,I>& x)
	{
		GXMath::Vec<bool,I>	ret;
		FOR( i, ret )	ret[i] = IsFinite( x[i] );
		return ret;
	}

}	// GXTypes

namespace CompileTime
{

	//
	// TypeInfo
	//
	template <typename T, usize I>
	struct TypeInfo < GXMath::Vec<T,I> >
	{
	private:
		typedef CompileTime::TypeInfo<T>	_value_type_info;

	public:
		typedef GXMath::Vec<T,I>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith =  GXMath::Vec< OtherType, I >;

		enum {
			FLAGS	= (int)_value_type_info::FLAGS | (int)GX_STL::CompileTime::_ctime_hidden_::VECTOR,
		};

		static constexpr type	Max()		{ return type( _value_type_info::Max() ); }
		static constexpr type	Min()		{ return type( _value_type_info::Min() ); }
		static			 type	Inf()		{ return type( _value_type_info::Inf() ); }
		static			 type	NaN()		{ return type( _value_type_info::NaN() ); }
		
		static constexpr type	Epsilon()	{ return type( _value_type_info::Epsilon() ); }
		static constexpr uint	SignBit()	{ return _value_type_info::SignBit(); }
		static constexpr uint	Count()		{ return I * _value_type_info::Count(); }
	};

	
	template <typename T, usize I>
	struct TypeDescriptor::IsFloat< GXMath::Vec<T,I> >			{ static const bool  value = IsFloat<T>::value; };
	
	template <typename T, usize I>
	struct TypeDescriptor::IsInteger< GXMath::Vec<T,I> >		{ static const bool  value = IsInteger<T>::value; };
	
	template <typename T, usize I>
	struct TypeDescriptor::IsSigned< GXMath::Vec<T,I> >			{ static const bool  value = IsSigned<T>::value; };
	
	template <typename T, usize I>
	struct TypeDescriptor::IsUnsigned< GXMath::Vec<T,I> >		{ static const bool  value = IsUnsigned<T>::value; };
	
	template <typename T, usize I>
	struct TypeDescriptor::IsFixed< GXMath::Vec<T,I> >			{ static const bool  value = IsFixed<T>::value; };
	
	template <typename T, usize I>
	struct TypeDescriptor::IsSoftware< GXMath::Vec<T,I> >		{ static const bool  value = IsSoftware<T>::value; };

}	// CompileTime

}	// GX_STL
