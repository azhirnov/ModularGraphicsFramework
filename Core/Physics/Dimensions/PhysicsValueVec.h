// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Physics/Dimensions/PhysicsValue.h"

namespace GXPhysics
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
	ND_ inline auto  operator * (const GXMath::Vec<ValueType,I,U> &left, const PhysicsValue<ValueType, Dimensions, ValueScale> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I, U >	ret;
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
	ND_ inline auto  operator * (const PhysicsValue<ValueType, Dimensions, ValueScale> &left, const GXMath::Vec<ValueType,I,U> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I, U >	ret;
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
	ND_ inline auto  operator / (const GXMath::Vec<ValueType,I,U> &left, const PhysicsValue<ValueType, Dimensions, ValueScale> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I, U >	ret;
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
	ND_ inline auto  operator / (const PhysicsValue<ValueType, Dimensions, ValueScale> &left, const GXMath::Vec<ValueType,I,U> &right)
	{
		PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I, U >	ret;
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
	ND_ inline bool Equals (const PhysicsValue<ValueType, Dimensions, ValueScale>& a,
							const PhysicsValue<ValueType, Dimensions, ValueScale>& b)
	{
		return GXMath::Equals( a.ref(), b.ref() );
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
	ND_ inline auto  Equals (const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I, U > &a,
							 const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I, U > &b)
	{
		GXMath::Vec<bool,I,U>	ret;
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
	ND_ inline auto  Equals (const PhysicsValue<ValueType, Dimensions, ValueScale>& a,
							 const PhysicsValue<ValueType, Dimensions, ValueScale>& b,
							 /*Bits*/uint accuracy)
	{
		return GXMath::Equals( a.ref(), b.ref(), accuracy );
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
	ND_ inline auto  Equals (const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &a,
							 const PhysicsValueVec< PhysicsValue<ValueType, Dimensions, ValueScale>, I > &b,
							 /*Bits*/uint accuracy)
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, ret )	ret[i] = Equals( a[i], b[i], accuracy );
		return ret;
	}

}	// GXPhysics


namespace GX_STL
{
namespace CompileTime
{

	template<typename ValueType,
			typename Dimensions,
			typename ValueScale,
			usize I, ulong U
	>
	struct TypeInfo< GXPhysics::PhysicsValueVec<GXPhysics::PhysicsValue<ValueType, Dimensions, ValueScale>, I, U> > {
	private:
		using _value_type_info	= CompileTime::TypeInfo<ValueType>;

	public:
		using type			= GXPhysics::PhysicsValueVec<GXPhysics::PhysicsValue<ValueType, Dimensions, ValueScale>, I, U>;
		using inner_type	= GXPhysics::PhysicsValue<ValueType, Dimensions, ValueScale>;

		template<typename OtherType>
		using CreateWith =  GXPhysics::PhysicsValueVec<GXPhysics::PhysicsValue<OtherType, Dimensions, ValueScale>, I>;

		enum {
			FLAGS = (int) _value_type_info::FLAGS | (int) GX_STL::CompileTime::_ctime_hidden_::VECTOR
		};

		static constexpr type	Max()		{ return type(_value_type_info::Max()); }

		static constexpr type	Min()		{ return type(_value_type_info::Min()); }

		static type				Inf()		{ return type(_value_type_info::Inf()); }

		static type				NaN()		{ return type(_value_type_info::NaN()); }

		static constexpr type	Epsilon()	{ return type(_value_type_info::Epsilon()); }

		static constexpr uint	SignBit()	{ return _value_type_info::SignBit(); }

		static constexpr uint	Count()		{ return I * _value_type_info::Count(); }
	};

}	// CompileTime


namespace GXTypes
{
	
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale,
			  usize I, ulong U
			 >
	struct Hash< GXPhysics::PhysicsValueVec< GXPhysics::PhysicsValue< ValueType, Dimensions, ValueScale >, I, U > >
	{
		ND_ HashResult  operator () (const GXPhysics::PhysicsValueVec< GXPhysics::PhysicsValue< ValueType, Dimensions, ValueScale >, I, U > &x) const noexcept
		{
			return HashOf( x.ref() );
		}
	};
	
}	// GXTypes
}	// GX_STL
