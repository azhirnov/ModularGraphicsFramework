// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "PhysicsValue.h"

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

namespace GX_STL
{
namespace GXMath
{
	
	template <typename T, usize I, ulong U = 0>
	struct PhysicsValueVec;


#define I	1
#include "PhysicsValueVecI.h"
#undef	I


#define I	2
#include "PhysicsValueVecI.h"
#undef	I

	
#define I	3
#include "PhysicsValueVecI.h"
#undef	I


#define I	4
#include "PhysicsValueVecI.h"
#undef	I


	
/*
=================================================
	operator *
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator * (const Vec<ValueType,I,U> &left, const PhysicsValue<ValueType, Dimensions, ValueScale> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >	ret;
		FOR( i, ret )	ret[i] = left[i] * right;
		return ret;
	}
	
/*
=================================================
	operator *
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator * (const PhysicsValue<ValueType, Dimensions, ValueScale> &left, const Vec<ValueType,I,U> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >	ret;
		FOR( i, ret )	ret[i] = left * right[i];
		return ret;
	}
	
/*
=================================================
	operator /
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator / (const Vec<ValueType,I,U> &left, const PhysicsValue<ValueType, Dimensions, ValueScale> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >	ret;
		FOR( i, ret )	ret[i] = left[i] / right;
		return ret;
	}
	
/*
=================================================
	operator /
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator / (const PhysicsValue<ValueType, Dimensions, ValueScale> &left, const Vec<ValueType,I,U> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >	ret;
		FOR( i, ret )	ret[i] = left / right[i];
		return ret;
	}
	
/*
=================================================
	Equals
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale
			 >
	inline bool Equals (const PhysicsValue<ValueType, Dimensions, ValueScale>& a,
						const PhysicsValue<ValueType, Dimensions, ValueScale>& b)
	{
		return Equals( a.ref(), b.ref() );
	}
	
/*
=================================================
	Equals
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	inline Vec<bool,I,U>  Equals (const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &a,
								 const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &b)
	{
		Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = Equals( a[i], b[i] );
		return ret;
	}
	
/*
=================================================
	Equals
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale
			 >
	inline bool Equals (const PhysicsValue<ValueType, Dimensions, ValueScale>& a,
						const PhysicsValue<ValueType, Dimensions, ValueScale>& b,
						/*Bits*/uint accuracy)
	{
		return Equals( a.ref(), b.ref(), accuracy );
	}
	
/*
=================================================
	Equals
=================================================
*/
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	inline Vec<bool,I,U>  Equals (const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &a,
								 const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &b,
								 /*Bits*/uint accuracy)
	{
		Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = Equals( a[i], b[i], accuracy );
		return ret;
	}

}	// GXMath


namespace CompileTime
{

	template<typename ValueType,
			typename Dimensions,
			typename ValueScale,
			usize I, ulong U
	>
	struct TypeInfo<GXMath::PhysicsValueVec<GXMath::PhysicsValue<ValueType, Dimensions, ValueScale>, I, U> > {
	private:
		typedef CompileTime::TypeInfo<ValueType> _value_type_info;

	public:
		typedef GXMath::PhysicsValueVec<GXMath::PhysicsValue<ValueType, Dimensions, ValueScale>, I, U> type;
		typedef GXMath::PhysicsValue<ValueType, Dimensions, ValueScale> inner_type;

		template<typename OtherType>
		using CreateWith =  GXMath::PhysicsValueVec<GXMath::PhysicsValue<OtherType, Dimensions, ValueScale>, I>;

		enum {
			FLAGS = (int) _value_type_info::FLAGS | (int) GX_STL::CompileTime::_ctime_hidden_::VECTOR
		};

		static constexpr type Max() { return type(_value_type_info::Max()); }

		static constexpr type Min() { return type(_value_type_info::Min()); }

		static type Inf() { return type(_value_type_info::Inf()); }

		static type NaN() { return type(_value_type_info::NaN()); }

		static constexpr type Epsilon() { return type(_value_type_info::Epsilon()); }

		static constexpr uint SignBit() { return _value_type_info::SignBit(); }

		static constexpr uint Count() { return I * _value_type_info::Count(); }
	};

}	// CompileTime


namespace GXTypes
{
	
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	struct Hash< GXMath::PhysicsValueVec< GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >, I, U > > :
		private Hash< GXMath::Vec< ValueType, I, U > >
	{
		typedef GXMath::PhysicsValueVec< GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >, I, U >	Key_t;
		typedef Hash< GXMath::Vec< ValueType, I, U > >														Base_t;
		typedef typename Base_t::Result_t																	Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( x.ref() );
		}
	};
	
}	// GXTypes
}	// GX_STL

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
