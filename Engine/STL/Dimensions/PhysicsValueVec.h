// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "PhysicsValue.h"

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

namespace GX_STL
{
namespace GXMath
{
	
	template <typename T, usize I>
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
			  usize I
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator * (const Vec<ValueType,I> &left, const PhysicsValue<ValueType, Dimensions, ValueScale> &right)
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
			  usize I
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator * (const PhysicsValue<ValueType, Dimensions, ValueScale> &left, const Vec<ValueType,I> &right)
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
			  usize I
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator / (const Vec<ValueType,I> &left, const PhysicsValue<ValueType, Dimensions, ValueScale> &right)
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
			  usize I
			 >
	inline PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I >
		operator / (const PhysicsValue<ValueType, Dimensions, ValueScale> &left, const Vec<ValueType,I> &right)
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
			  usize I
			 >
	inline Vec<bool,I>  Equals (const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &a,
								 const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &b)
	{
		Vec<bool,I>	ret;
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
			  usize I
			 >
	inline Vec<bool,I>  Equals (const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &a,
								 const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &b,
								 /*Bits*/uint accuracy)
	{
		Vec<bool,I>	ret;
		FOR( i, ret )	ret[i] = Equals( a[i], b[i], accuracy );
		return ret;
	}

}	// GXMath

namespace GXTypes
{
	
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I
			 >
	struct ::GX_STL::CompileTime::TypeInfo < GXMath::PhysicsValueVec< GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >, I > >
	{
	private:
		typedef CompileTime::TypeInfo< ValueType >	_value_type_info;

	public:
		typedef GXMath::PhysicsValueVec< GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >, I >	type;
		typedef GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >								inner_type;
		
		template <typename OtherType>
		using CreateWith =  GXMath::PhysicsValueVec< GXMath::PhysicsValue< OtherType, Dimensions, ValueScale >, I >;

		enum { FLAGS = (int)_value_type_info::FLAGS | (int)GX_STL::CompileTime::_ctime_hidden_::VECTOR };

		static constexpr type	Max()		{ return type( _value_type_info::Max() ); }
		static constexpr type	Min()		{ return type( _value_type_info::Min() ); }
		static			 type	Inf()		{ return type( _value_type_info::Inf() ); }
		static			 type	NaN()		{ return type( _value_type_info::NaN() ); }
		
		static constexpr type	Epsilon()	{ return type( _value_type_info::Epsilon() ); }
		static constexpr uint	SignBit()	{ return _value_type_info::SignBit(); }
		static constexpr uint	Count()		{ return I * _value_type_info::Count(); }
	};

	
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I
			 >
	struct Hash< GXMath::PhysicsValueVec< GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >, I > > :
		private Hash< GXMath::Vec< ValueType, I > >
	{
		typedef GXMath::PhysicsValueVec< GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >, I >	key_t;
		typedef Hash< GXMath::Vec< ValueType, I > >														base_t;
		typedef typename base_t::result_t																result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.ref() );
		}
	};
	
}	// GXTypes
}	// GX_STL

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
