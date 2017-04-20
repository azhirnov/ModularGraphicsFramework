// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Trigonometry.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

#undef MAT_BIN_OPERATOR_SCALAR
#undef MAT_BIN_OPERATOR_MAT
#undef MAT_BIN_OPERATOR

namespace GX_STL
{
namespace GXTypes
{
	
	template <typename T, usize C, usize R>
	struct Hash< GXMath::Matrix<T,C,R> > :
		private Hash< ArrayCRef<T> >
	{
		typedef GXMath::Matrix<T,C,R>		key_t;
		typedef Hash< ArrayCRef<T> >		base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( base_t::key_t( x.ptr(), x.Count() ) );
		}
	};

}	// GXTypes

namespace CompileTime
{

	//
	// TypeInfo
	//
	template <typename T, usize C, usize R>
	struct TypeInfo < GXMath::Matrix<T,C,R> >
	{
	private:
		typedef CompileTime::TypeInfo<T>	_value_type_info;

	public:
		typedef GXMath::Matrix<T,C,R>	type;
		typedef T						inner_type;
		
		template <typename OtherType>
		using CreateWith =  GXMath::Matrix< OtherType, C, R >;

		enum {
			FLAGS	= (int)_value_type_info::FLAGS | (int)GX_STL::CompileTime::_ctime_hidden_::VECTOR,
		};

		static constexpr type	Max()		{ return type( _value_type_info::Max() ); }
		static constexpr type	Min()		{ return type( _value_type_info::Min() ); }
		static			 type	Inf()		{ return type( _value_type_info::Inf() ); }
		static			 type	NaN()		{ return type( _value_type_info::NaN() ); }
		
		static constexpr type	Epsilon()	{ return type( _value_type_info::Epsilon() ); }
		static constexpr uint	SignBit()	{ return _value_type_info::SignBit(); }
		static constexpr uint	Count()		{ return C * R * _value_type_info::Count(); }
	};

}	// CompileTime
}	// GX_STL
