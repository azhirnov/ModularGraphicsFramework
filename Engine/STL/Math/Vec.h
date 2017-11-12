// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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

	
#	define VEC_OP_UNARY( _op_ ) \
		\
		const Self	operator _op_ () const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = _op_ (*this)[i]; \
			return ret; \
		}

#	define VEC_OP_BINARY_( _op_ ) \
		\
		Self	operator _op_  (const Self& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = ( (*this)[i] _op_ right[i] ); \
			return ret; \
		}

#	define VEC_OP_BINARY_A( _op_ ) \
		\
		Self &		operator _op_##= (const Self& right) \
		{ \
			FOR( i, *this )	 (*this)[i] _op_##= right[i]; \
			return *this; \
		}
		
#	define VEC_OP_BINARY_SCALAR_( _op_, _scalar_ ) \
		\
		Self	operator _op_  (const _scalar_& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = ( (*this)[i] _op_ right ); \
			return ret; \
		} \
		\
		friend Self operator _op_ (const _scalar_& left, const Self& right) \
		{ \
			return Self(left) _op_ right; \
		}

#	define VEC_OP_BINARY_SCALAR_A( _op_, _scalar_ ) \
		\
		Self &		operator _op_##= (const _scalar_& right) \
		{ \
			FOR( i, *this )	 (*this)[i] _op_##= right; \
			return *this; \
		}

#	define VEC_OP_BINARY( _op_ ) \
		VEC_OP_BINARY_( _op_ ) \
		VEC_OP_BINARY_A( _op_ )

#	define VEC_OP_BINARY_SCALAR( _op_, _scalar_ ) \
		VEC_OP_BINARY_SCALAR_( _op_, _scalar_ ) \
		VEC_OP_BINARY_SCALAR_A( _op_, _scalar_ )

	
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

	

	template <typename T, ulong U>
	inline Vec<T,3,U>  Cross (const Vec<T,3,U> &left, const Vec<T,3,U> &right)
	{
		return Vec<T,3,U>(	left.y * right.z - right.y * left.z,
							left.z * right.x - right.z * left.x,
							left.x * right.y - right.x * left.y );
	}

	
	template <typename T, ulong U>
	inline auto  Cross (const Vec<T,2,U> &left, const Vec<T,2,U> &right)
	{
		return left.x * right.y - left.y * right.x;
	}


	template <typename T, usize I, ulong U>
	inline auto  Dot (const Vec<T,I,U> &left, const Vec<T,I,U> &right)
	{
		return left.Dot( right );
	}

	
#	undef VEC_OP_UNARY
#	undef VEC_OP_BINARY_
#	undef VEC_OP_BINARY_A
#	undef VEC_OP_BINARY_SCALAR_
#	undef VEC_OP_BINARY_SCALAR_A
#	undef VEC_OP_BINARY
#	undef VEC_OP_BINARY_SCALAR

}	// GXMath

namespace GXTypes
{

	//
	// Hash
	//
	template <typename T, usize I, ulong U>
	struct Hash< GXMath::Vec<T,I,U> > :
		private Hash< ArrayCRef<T> >
	{
		typedef GXMath::Vec<T,I,U>			Key_t;
		typedef Hash< ArrayCRef<T> >		Base_t;
		typedef typename Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( ArrayCRef<T>( x.ptr(), x.Count() ) );
		}
	};


	//
	// IsNaN, IsInfinity, IsFinite
	//

	template <typename T, usize I, ulong U>
	inline GXMath::Vec<bool,I,U>  IsNaN (const GXMath::Vec<T,I,U>& x)
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = IsNaN( x[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	inline GXMath::Vec<bool,I,U>  IsInfinity (const GXMath::Vec<T,I,U>& x)
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = IsInfinity( x[i] );
		return ret;
	}

	template <typename T, usize I, ulong U>
	inline GXMath::Vec<bool,I,U>  IsFinite (const GXMath::Vec<T,I,U>& x)
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = IsFinite( x[i] );
		return ret;
	}

}	// GXTypes

namespace CompileTime
{

	//
	// TypeInfo
	//
	template <typename T, usize I, ulong U>
	struct TypeInfo < GXMath::Vec<T,I,U> >
	{
	private:
		typedef CompileTime::TypeInfo<T>	_value_type_info;

	public:
		typedef GXMath::Vec<T,I,U>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith =  GXMath::Vec< OtherType, I, U >;

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

	
	template <typename T, usize I, ulong U>
	struct TypeDescriptor::IsFloat< GXMath::Vec<T,I,U> >		{ static const bool  value = IsFloat<T>::value; };
	
	template <typename T, usize I, ulong U>
	struct TypeDescriptor::IsInteger< GXMath::Vec<T,I,U> >		{ static const bool  value = IsInteger<T>::value; };
	
	template <typename T, usize I, ulong U>
	struct TypeDescriptor::IsSigned< GXMath::Vec<T,I,U> >		{ static const bool  value = IsSigned<T>::value; };
	
	template <typename T, usize I, ulong U>
	struct TypeDescriptor::IsUnsigned< GXMath::Vec<T,I,U> >		{ static const bool  value = IsUnsigned<T>::value; };
	
	template <typename T, usize I, ulong U>
	struct TypeDescriptor::IsFixed< GXMath::Vec<T,I,U> >		{ static const bool  value = IsFixed<T>::value; };
	
	template <typename T, usize I, ulong U>
	struct TypeDescriptor::IsSoftware< GXMath::Vec<T,I,U> >		{ static const bool  value = IsSoftware<T>::value; };

}	// CompileTime

}	// GX_STL
