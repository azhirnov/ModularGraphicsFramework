// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "MathFunc.h"
#include "MathTypes.h"
#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Common/Cast.h"
#include "Mathematics.h"
#include "Engine/STL/CompileTime/StringToID.h"

namespace GX_STL
{
namespace GXMath
{

	DECL_STRING_TO_ID( Swizzle, 0 );


	namespace _math_hidden_
	{
		inline constexpr uint _SwizzleCharToIndex (const ulong hash)
		{
			const uint	off		= CompileTime::IntLog2< uint, ('Z' - 'A') > + 1;
			const uint	mask	= (1u << off) - 1;
			const uint	mask2	= (1u << (off*2)) - 1;

			const uint	c = 'A' + (hash & mask) - 1;
			const uint	c0 = mask | (1 << off);
			const uint	c1 = mask | (2 << off);

			return	(c == 'R' or c == 'X' ? 1u :		// X
					 c == 'G' or c == 'Y' ? 2u :		// Y
					 c == 'B' or c == 'Z' ? 3u :		// Z
					 c == 'A' or c == 'W' ? 4u :		// W
					 (hash & mask2) == c0 ? 5u :		// 0
					 (hash & mask2) == c1 ? 6u :		// 1
					 (hash & mask) == 0 ? 0u : UMax);	// error
		}

		inline constexpr uint _ParseSwizzle (const Swizzle::type sw, const uint maxI = 4)
		{
			const auto	val	 = sw >> CompileTime::IntLog2< uint, Swizzle::_IDMask+1 >;
			const uint	off	 = CompileTime::IntLog2< uint, ('Z' - 'A') > + 1;

			const uint	a3 = _SwizzleCharToIndex( val );
			const uint	a2 = _SwizzleCharToIndex( val >> (off * (a3<5?1:2)) );
			const uint	a1 = _SwizzleCharToIndex( val >> (off * ((a2<5?1:2) + (a3<5?1:2))) );
			const uint	a0 = _SwizzleCharToIndex( val >> (off * ((a1<5?1:2) + (a2<5?1:2) + (a3<5?1:2))) );

			//const uint	b0 = a0 << 0;
			//const uint	b1 = a1 << (a0 ? 4 : 0);
			//const uint	b2 = a2 << ((a1 ? 4 : 0) + (a0 ? 4 : 0));
			//const uint	b3 = a3 << ((a2 ? 4 : 0) + (a1 ? 4 : 0) + (a0 ? 4 : 0));
			const uint	b0 = a3 << 0;
			const uint	b1 = a2 << (a3 ? 4 : 0);
			const uint	b2 = a1 << ((a2?4:0) + (a3?4:0));
			const uint	b3 = a0 << ((a1?4:0) + (a2?4:0) + (a3?4:0));

			return	b0 | b1 | b2 | b3 |
					((val >> (off*((a0<5?1:2) + (a1<5?1:2) + (a2<5?1:2) + (a3<5?1:2)))) ? UMax : 0u) |
					(a0 <= maxI or a0 == 5 or a0 == 6 ? 0u : UMax) |
					(a1 <= maxI or a1 == 5 or a1 == 6 ? 0u : UMax) |
					(a2 <= maxI or a2 == 5 or a2 == 6 ? 0u : UMax) |
					(a3 <= maxI or a3 == 5 or a3 == 6 ? 0u : UMax);
		}

		template <uint I>
		static constexpr usize SWLengthImpl = I < 0xF ? 1 : I < 0xFF ? 2 : I < 0xFFF ? 3 : 4;

		template <Swizzle::type SW>
		static constexpr usize SWLength = SWLengthImpl< _ParseSwizzle( SW ) >;

	}	// _math_hidden_

	
#	define VEC_OP_UNARY( _op_ ) \
		\
		constexpr const Self	operator _op_ () const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = _op_ (*this)[i]; \
			return ret; \
		}

#	define VEC_OP_BINARY_( _op_ ) \
		\
		constexpr Self	operator _op_  (const Self& right) const \
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
		constexpr Self	operator _op_  (const _scalar_& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = ( (*this)[i] _op_ right ); \
			return ret; \
		} \
		\
		friend constexpr Self operator _op_ (const _scalar_& left, const Self& right) \
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
